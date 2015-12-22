/*
 *
 *   Module Name: Sammy
 *
 *   OS/2 Workplace Shell Access Manager
 *   Umschalten zwischen den Shells
 */



/*
       Beschreibung der Semaphoren:

Sam benutzt zwei Semaphoren: HEV_SAMMY und HEV_PRFRESETLOCK (siehe SamGlob.h)
Der Ablauf beim Wechseln der Profiles ist folgenderma·en:

Sammy wartet, da· HEV_SAMMY zurÅckgesetzt wird.
Sodann bereitet er alles fÅr einen prfReset vor. (Environment, Schlie·en der ArbeitsoberflÑche etc.)
Unmittelbar vor dem eigentlichen prfReset wartet er auf das RÅcksetzen von HEV_PRFRESETLOCK.

HEV_PRFRESETLOCK wird ausschlie·lich von WPSamF manipuliert.
In der _wpOpen-Methode wird (falls es sich um einen aktiven Desktop handelt) die Semaphore gesetzt und in der SaveState-Methode wird sie wieder gelîscht.
Bei einem neuen Desktop wird die SaveState-Methode erst ganz am Schlu· aufgerufen.
D.h. ein prfReset wird verhindert, bis alle Objekte ihre Instanzdaten geschrieben haben.

Ist Sammy nicht installiert, so wird HEV_SAMMY nicht verwendet, sondern die WPSam-eigene Funktion thChangeWPS aufgerufen.
Auch diese Routine wartet auf das RÅcksetzen von HEV_PRFRESETLOCK.

Die Semaphore HEV_SAMMY wird von Sammy verwendet, um zu testen, ob er bereits aufgerufen ist.
Wird er nÑmlich ein zweites mal aufgerufen, so reagiert er folgenderma·en:
Er beauftragt den bereits installierten Sammy, auf den Rootdesktop zurÅckzuschalten.

*/


#define INCL_WIN
#define INCL_PM
#define INCL_DOS

#include <os2.h>
#include <string.h>
#include <stdlib.h>
#include <memory.h>
#include "..\samglob.h"

#define THREADSTACK     0x8000

#define CLASSNAME "SAMMY"

/* Externe Funktionen */
PCHAR           MakeEnv (CHAR *szEnvSource);
BOOL            FileExist (PCHAR szFileName);

/* Modulinterne Funktionen */
APIRET APIENTRY thStartProg (CHAR *szProgName);
APIRET APIENTRY thSwitch (void);
BOOL            ChangeWPS(PCHAR pszUserIni, PCHAR pszSystemIni);
ULONG           ulExecProg (CHAR *szProgName);
APIRET          ScanEnv (PSZ szEnvVarName, PSZ* pszResultPointer, PCHAR szzEnvironment);

PSHARE1 pShareInitOS2 = 0;
HEV     hevSammy = 0;
HEV     hevPrfResetLock = 0;
HMQ     hmq;
HAB     hab;
QMSG    qmsg;
ULONG   ulShellID;
int     intSammyRetCode = 0; // ReturnCode von Sammy kann von bel. Prozedur geschrieben werden

TID     tid1 = 0;
TID     tid2 = 0;
ULONG   ulOldFlag = 0;       // Nach erfolgr. Umschalten steht hier ulFlag vom aktiven Benutzer

int main (int argc, char *argv[])
   {
   APIRET       rc;
   PCHAR        pcEnv;
   PRFPROFILE   prfProfile;

#ifdef DEBUG
   ulDebugMask = 0xFFFFFFFF;
#endif /* DEBUG */

   hab = WinInitialize(0);
   hmq = WinCreateMsgQueue(hab, 0);
   DebugS (1, "PM Interface initialized");

   /* Shared Memory organisieren */
   if (rc = DosGetNamedSharedMem ((PPVOID) &pShareInitOS2,
                                  SHARE_INITOS2,
                                  PAG_READ | PAG_WRITE))
      {
      if (rc = DosAllocSharedMem( (PPVOID) &pShareInitOS2,    // Pointer to shared mem
                                  SHARE_INITOS2,              // Name
                                  CCHSHARE_INITOS2,           // Size of shared mem
                                  PAG_COMMIT | PAG_READ | PAG_WRITE)) // Flags
         return(1);
      else
         {
         /* Shared Memory initialisieren */

         memset (pShareInitOS2, '\0', CCHSHARE_INITOS2);

         pShareInitOS2->pszRegFile       = (PCHAR) pShareInitOS2 +
                                          sizeof(*pShareInitOS2);
         strcpy (pShareInitOS2->pszRegFile,
                 DosScanEnv (ENV_SYSTEM_INI, &pcEnv) ? "" : pcEnv);
         pShareInitOS2->pszRootUserIni   = pShareInitOS2->pszRegFile +
                                          strlen(pShareInitOS2->pszRegFile) + 1;
         pShareInitOS2->pszRootSystemIni = pShareInitOS2->pszRootUserIni + 1;
         pShareInitOS2->pszUserIni       = pShareInitOS2->pszRootSystemIni + 1;
         pShareInitOS2->pszSystemIni     = pShareInitOS2->pszUserIni   + CCHMAXPATH;
         pShareInitOS2->pszEnvironment   = pShareInitOS2->pszSystemIni + CCHMAXPATH;
         }
      }
   DebugS (1, "Shared Memory initialized");

   /* Semaphoren organisieren */
   rc = DosOpenEventSem (HEV_SAMMY, &hevSammy);

   if( rc )
      rc = DosCreateEventSem( HEV_SAMMY,   // Name
                              &hevSammy,   // Pointer to sem
                              0,           // Not used with named sems
                              FALSE);      // Initial state (FALSE = SET)

   else        /* Sammy ist bereits installiert */
      {
      pShareInitOS2->pszEnvironment[0] = '\0';
      pShareInitOS2->pszEnvironment[1] = '\0';
      pShareInitOS2->pszSystemIni[0]   = '\0';
      pShareInitOS2->pszUserIni[0]     = '\0';
      DosPostEventSem(hevSammy);
      goto Exit;
      }

   if( rc )
      {
      intSammyRetCode = rc;
      goto Exit;
      }

   rc = DosOpenEventSem (HEV_PRFRESETLOCK, &hevPrfResetLock);

   if( rc )
      rc = DosCreateEventSem( HEV_PRFRESETLOCK, // Name
                              &hevPrfResetLock, // Pointer to sem
                              0,                // Not used with named sems
                              TRUE);            // Initial state (TRUE = POSTED)

   if( rc )
      {
      intSammyRetCode = rc;
      goto Exit;
      }
   DebugS (1, "Semaphores initialized");

   ChangeWPS(pShareInitOS2->pszUserIni, pShareInitOS2->pszSystemIni);

   /* Hintergrundloop starten, das Shell mit aktueller Env. startet */

   DosCreateThread (&tid1,
                    (PFNTHREAD) thStartProg,
                    (ULONG) ((argc > 1) ? argv[1] : ""),
                    0,
                    THREADSTACK);
   DebugS (1, "Background loop started");

   /* Hintergrundloop starten, das jeweils nach Lîschen einer Semaphore */
   /* einen prfReset initiiert                      */
   DosCreateThread (&tid2,
                    (PFNTHREAD) thSwitch,
                    (ULONG) 0,
                    0,
                    THREADSTACK);

   while (WinGetMsg (hab, &qmsg, 0, 0, 0))
      WinDispatchMsg (hab, &qmsg);

   if (intSammyRetCode)
      {
      DosKillThread (tid1);
      DosKillThread (tid2);

      WinSetObjectData(WinQueryObject("<WP_DESKTOP>"), "WORKAREA=NO");
      WinPostMsg(WinQueryWindow(HWND_DESKTOP, QW_BOTTOM), WM_CLOSE, 0, 0);

      WinAlarm (HWND_DESKTOP, WA_ERROR);

      prfProfile.pszSysName  = (DosScanEnv (ENV_SYSTEM_INI, &pcEnv) ? "" : pcEnv);
      prfProfile.pszUserName = (DosScanEnv (ENV_USER_INI, &pcEnv) ? "" : pcEnv);

      prfProfile.cchUserName = strlen(prfProfile.pszUserName);
      prfProfile.cchSysName  = strlen(prfProfile.pszSysName);

      DosSleep (1000);
      DosKillProcess( DKP_PROCESSTREE, ulShellID );

      if ( !PrfReset(hab, &prfProfile))
         WinSetObjectData(WinQueryObject("<WP_DESKTOP>"), "OPEN=ICON;WORKAREA=YES");
      }
Exit:
   WinDestroyMsgQueue(hmq);
   WinTerminate(hab);

   DebugS (1, "Application terminated");

   return intSammyRetCode;
   }



APIRET APIENTRY thSwitch (void)
   {
   BOOL  bSuccess;
   ULONG ul;

   do {
      DosResetEventSem (hevSammy, &ul);
      DosWaitEventSem (hevSammy, SEM_INDEFINITE_WAIT);
      bSuccess = ChangeWPS(pShareInitOS2->pszUserIni, pShareInitOS2->pszSystemIni);
      DebugULd (1, "thSwitch", "bSuccess", bSuccess);
      }
   while( bSuccess );

   return 0;
   }


APIRET APIENTRY thStartProg (CHAR *szProgName)
   {
   RESULTCODES  rcProgResult;
   ULONG        ul;

   while( ulShellID = ulExecProg( szProgName ) )
      {
      DosWaitChild( DCWA_PROCESSTREE,                // action code
                    DCWW_WAIT,                       // wait option
                    &rcProgResult,                   // return codes
                    &ul,                             // return process ID
                    ulShellID);                      // process ID
      DebugS (1, "DosWaitChild returned");
      }

   return 0;
   }



ULONG ulExecProg (CHAR *szProgName)
   {
   ULONG        ulBootDrive;
   ULONG        ulAppType;
   CHAR         szCmdName[CCHMAXPATH];
   CHAR         szLine[CCHMAXPATH];
   PSZ          pszScanEnv;
   RESULTCODES  rcTermination;
   APIRET       rcFailure = 0;
   PCHAR        pszEnvironment;

/*
 *  Diese Routine versucht eine Shell zu starten.
 */

   pszEnvironment = MakeEnv (pShareInitOS2->pszEnvironment);

   if ( !(rcFailure = ScanEnv(ENV_SAMWORKPLACE, &pszScanEnv, pszEnvironment)) )
      {
      rcFailure = DosQueryAppType(pszScanEnv, &ulAppType);

      if ( !rcFailure )
         if ( ( (ulAppType & 7) == FAPPTYP_NOTSPEC) ||
              ( (ulAppType & 7) == FAPPTYP_WINDOWAPI) )
            rcFailure = DosExecPgm (szLine,           /* Object name buffer */
                                 sizeof(szLine),      /* Length of object name buffer */
                                 EXEC_ASYNCRESULT,    /* Execution flags */
                                 "",                  /* Argument string */
                                 pszEnvironment,       /* Environment */
                                 &rcTermination,      /* Termination codes */
                                 pszScanEnv);         /* Program file name */
         else
            rcFailure = 1;
      }

   if (rcFailure)
      {
      WinAlarm (HWND_DESKTOP, WA_ERROR);
      DebugS (1, "DosExecPgm <1> failed");
      }

   if (rcFailure)
      {
      rcFailure = DosExecPgm (szLine,              /* Object name buffer */
                              sizeof(szLine),      /* Length of object name buffer */
                              EXEC_ASYNCRESULT,    /* Execution flags */
                              "",                  /* Argument string */
                              pszEnvironment,       /* Environment */
                              &rcTermination,      /* Termination codes */
                              szProgName);         /* Program file name */
      }

   if (rcFailure)
      {
      if (!(rcFailure = DosScanEnv (ENV_SAMWORKPLACE, &pszScanEnv)))
         rcFailure = DosExecPgm (szLine,        /* Object name buffer */
                           sizeof(szLine),      /* Length of object name buffer */
                           EXEC_ASYNCRESULT,    /* Execution flags */
                           "",                  /* Argument string */
                           pszEnvironment,       /* Environment */
                           &rcTermination,      /* Termination codes */
                           pszScanEnv);         /* Program file name */
      }

   if (rcFailure)
      {
      DebugS (1, "DosExecPgm <2> failed");
      DosQuerySysInfo (QSV_BOOT_DRIVE, QSV_BOOT_DRIVE, &ulBootDrive, sizeof (ULONG));

      memset (szCmdName, '\0', sizeof(szCmdName));
      szCmdName[0] = (CHAR) (ulBootDrive - 1) + 'A';
      strcpy (szCmdName+1, ":\\OS2\\PMSHELL.EXE");

      rcFailure = DosExecPgm (szLine,           /* Object name buffer */
                           sizeof(szLine),      /* Length of object name buffer */
                           EXEC_ASYNCRESULT,    /* Execution flags */
                           "",                  /* Argument string */
                           pszEnvironment,       /* Environment */
                           &rcTermination,      /* Termination codes */
                           szCmdName);          /* Program file name */
      }

/* Falls Shell nicht gestartet werden konnte, wird cmd.exe aufgerufen */

   if (rcFailure)
      {
      DebugS (1, "DosExecPgm <3> failed");
      if (!(rcFailure = DosScanEnv ("COMSPEC", &pszScanEnv)))
         rcFailure = DosExecPgm (szLine,        /* Object name buffer */
                           sizeof(szLine),      /* Length of object name buffer */
                           EXEC_ASYNCRESULT,    /* Execution flags */
                           "/K",                /* Argument string */
                           pszEnvironment,       /* Environment */
                           &rcTermination,      /* Termination codes */
                           pszScanEnv);         /* Program file name */
      }

   if (rcFailure)
      {
      DebugS (1, "DosExecPgm <4> failed");
      DosQuerySysInfo (QSV_BOOT_DRIVE, QSV_BOOT_DRIVE, &ulBootDrive, sizeof (ULONG));

      memset (szCmdName, '\0', sizeof(szCmdName));
      szCmdName[0] = (CHAR) (ulBootDrive - 1) + 'A';
      strcpy (szCmdName+1, ":\\OS2\\CMD.EXE");

      rcFailure = DosExecPgm (szLine,        /* Object name buffer */
                        sizeof(szLine),      /* Length of object name buffer */
                        EXEC_ASYNCRESULT,    /* Execution flags */
                        "/K",                /* Argument string */
                        pszEnvironment,       /* Environment */
                        &rcTermination,      /* Termination codes */
                        szCmdName);          /* Program file name */
      }

   DosFreeMem(pszEnvironment);

   if (rcFailure)
      {
      intSammyRetCode = rcFailure;
      WinPostQueueMsg (hmq, WM_CLOSE, 0L, 0L);
      DebugS (1, "DosExecPgm <5> failed");
      }

   return (rcFailure ? 0 : rcTermination.codeTerminate);
   }



BOOL ChangeWPS(PCHAR pszUserIni, PCHAR pszSystemIni)
   {
   PCHAR        pcEnv;
   PRFPROFILE   prfProfile;
   BOOL         bSuccess;

   if (*pszSystemIni == '\0')
      prfProfile.pszSysName = pShareInitOS2->pszRootSystemIni;
   else
      prfProfile.pszSysName = pszSystemIni;

   if (*prfProfile.pszSysName == '\0')
      prfProfile.pszSysName = (DosScanEnv (ENV_SYSTEM_INI, &pcEnv) ? "" : pcEnv);

   if (*pszUserIni == '\0')
      {
      prfProfile.pszUserName = pShareInitOS2->pszRootUserIni;
      pShareInitOS2->ulFlag  = pShareInitOS2->ulFlag_Root; // ulFlag fÅr Rootdesktop
      }
   else
      prfProfile.pszUserName = pszUserIni;

   if (*prfProfile.pszUserName == '\0')
      prfProfile.pszUserName = (DosScanEnv (ENV_USER_INI, &pcEnv) ? "" : pcEnv);

   prfProfile.cchUserName = strlen(prfProfile.pszUserName);
   prfProfile.cchSysName  = strlen(prfProfile.pszSysName);

   if (ulOldFlag & CLOSEAPPL)
      WinSetObjectData(WinQueryObject("<WP_DESKTOP>"), "WORKAREA=YES");
   else
      WinSetObjectData(WinQueryObject("<WP_DESKTOP>"), "WORKAREA=NO");

   WinPostMsg(WinQueryWindow(HWND_DESKTOP, QW_BOTTOM), WM_CLOSE, 0, 0);
   DebugS (1, "HWND_DESKTOP closed");

   if( hevPrfResetLock )
      {
      DosWaitEventSem (hevPrfResetLock, SEM_INDEFINITE_WAIT);
      DosSleep(1000);
      DebugS (1, "hevPrfResetLock released");
      }
   else
      DosSleep(20000);

   bSuccess = PrfReset(hab, &prfProfile);
   DebugULd (1, "PrfReset", "return", bSuccess);

/*
 *  Beim Umschalten auf den Root, egal ob von Sammy oder von WPSamF ausgelîst,
 *  wird die Shell vom PrfReset nicht geschlossen.
 *  Solange die Ursache nicht bekannt ist, bleibt nichts anderes Åbrig,
 *  als an dieser Stelle symptomatisch vorzugehen und die Shell abzuschie·en.
 */
   if (*pszUserIni == '\0')
      {
      DosSuspendThread (tid1);
      DosKillProcess( DKP_PROCESSTREE, ulShellID );   // sog. Budnik'scher Arschtritt
      DebugS (1, "Shell killed");
      DosResumeThread (tid1);
      }

   if( !bSuccess )
      {
      WinSetObjectData(WinQueryObject("<WP_DESKTOP>"), "OPEN=ICON;WORKAREA=YES");
      DebugS (1, "Desktop set to <WP_DESKTOP>");
      intSammyRetCode = 250;
      WinPostQueueMsg (hmq, WM_QUIT, 0L, 0L);
      }
   else
      ulOldFlag = pShareInitOS2->ulFlag;

   return( bSuccess );
   }


APIRET ScanEnv (PSZ szEnvVarName, PSZ* pszResultPointer, PCHAR szzEnvironment)
/*
 *  Diese Funktion entspricht der DosScanEnv-Funktion. Es mu· jedoch ein Zeiger
 *  auf eine Environment-Struktur Åbergeben werden.
 *  Zwischen Variable und Wert braucht kein Gleichheitszeichen zu stehen.
 */
   {
   CHAR *szEnv;

   for (szEnv = szzEnvironment; *szEnv != '\0'; szEnv += strlen(szEnv) + 1)
      if (!strnicmp(szEnv, szEnvVarName, strlen(szEnvVarName)))
         {
         *pszResultPointer = szEnv + strlen(szEnvVarName);

         while ((**pszResultPointer == ' ')  || (**pszResultPointer == '='))
            (*pszResultPointer)++;

         if (*pszResultPointer != (szEnv + strlen(szEnvVarName))) // Mindestens ein Trennzeichen?
            return (0);
         }

   return (203);
   }
