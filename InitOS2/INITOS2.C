#define INCL_DOS                // ???
#define INCL_VIO                // ???
#define INCL_DOSFILEMGR
#define INCL_DOSMISC
#define INCL_DOSNLS
#define INCL_DOSPROCESS
#define INCL_DOSERRORS
#include <os2.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "winman.h"
#include "initos2.h"
#include "initdlg.h"

char **margv;
int  margc;

ULONG key[2] = {ULKEY1, ULKEY2};

/* Setup-Variable */
ULONG ulTimeout;
CHAR  szAutouser[CCHMAXUSERNAME];
CHAR  szAdminPwd[CCHMAXPASSWORD];

/* Registrierung */
CHAR  szRegUser[CCHMAXREGUSER];
CHAR  szRegPassword[CCHMAXREGPWD];
ULONG ulRegState;

PCHAR pszRootUserIni, pszRootSystemIni;

PCHAR pMemShared;                   /* Zeiger auf den Shared-Memory-Bereich */

extern TID tidKbd;

/* Kartenleser-UnterstÅtzung */
BOOL    bIsMag = FALSE;             /* TRUE: Kartenleser-Support o.k. */

/* Benutzer-Initialisierung */
USER User;
TID  tid;
CHAR szUserResource[CCHMAXPATH];
CHAR szUserProfile[CCHMAXPATH];
CHAR szSystemResource[CCHMAXPATH];
CHAR szSystemProfile[CCHMAXPATH];

MPARAM EXPENTRY MainDlgProc  (HWND, ULONG, MPARAM, MPARAM);
MPARAM EXPENTRY SetupDlgProc (HWND, ULONG, MPARAM, MPARAM);
MPARAM EXPENTRY PwdDlgProc   (HWND, ULONG, MPARAM, MPARAM);
MPARAM EXPENTRY SDDlgProc    (HWND, ULONG, MPARAM, MPARAM);
MPARAM EXPENTRY MkIniDlgProc (HWND, ULONG, MPARAM, MPARAM);
void   Shutdown (void);
ULONG  SumUsers (PCHAR, PULONG, PCHAR, PULONG);
LONG   Str2Idx (PCHAR, PCHAR, ULONG);
PCHAR  Idx2Str (PCHAR, LONG, ULONG);

void _Optlink thCreate (void *);           // ???

int main (int argc, char *argv[])
    {
    QUMSG  qmsg;

    ULONG  ulScratch;
    USHORT usBytes, usSerial;
    PCHAR  pszTemp;

    STRUC_EAT_DATA arValue[3];

    static COUNTRYCODE strucCountryCode = {0, 0};   /* LÑndercode */
    static COUNTRYINFO strucCountryInfo;            /* lÑnderabhÑngige Daten */
    static ULONG       ulBootDrive;                 /* Boot-Drive */

    /* Boot-Drive bestimmen */
    DosQuerySysInfo (QSV_BOOT_DRIVE, QSV_BOOT_DRIVE, &ulBootDrive,
        sizeof (ulBootDrive));
    DosQueryCtryInfo (sizeof (COUNTRYINFO), &strucCountryCode,
        &strucCountryInfo, &ulScratch);

    /* Shared Memory Bereich fÅr Sammy und WPSam anlegen */
    DosScanEnv (ENV_USER_INI, &pszRootUserIni);
    ulScratch = strlen (pszRootUserIni) + 1;
    DosScanEnv (ENV_SYSTEM_INI, &pszRootSystemIni);
    ulScratch += strlen (pszRootSystemIni) + 1;
    ulScratch += sizeof (SHARE1);

    ulScratch = CCHSHARE_INITOS2;   // ??? solange SAMMY auch nur pfuscht

    if (DosGetNamedSharedMem ((PPVOID) &pMemShared, SHARE_INITOS2,
        PAG_READ | PAG_WRITE))
        {
        if (DosAllocSharedMem((PPVOID) &pMemShared, SHARE_INITOS2,
            ulScratch, PAG_COMMIT | PAG_READ | PAG_WRITE))
            Shutdown ();
        else
            memset (pMemShared, '\0', ulScratch);
        }

    /* Shared Memory initialisieren */
    ((PSHARE1)pMemShared)->ulActive |= ACTIVE_INITOS2;

    pszTemp = pMemShared + sizeof (SHARE1);         /* Register File */
    ((PSHARE1) pMemShared)->pszRegFile = pszTemp;
    strcpy (pszTemp, pszRootSystemIni);

    pszTemp += strlen (pszRootSystemIni) + 1;       /* Root: User Ini */
    ((PSHARE1) pMemShared)->pszRootUserIni = pszTemp;
    strcpy (pszTemp, pszRootUserIni);

    pszTemp += strlen (pszRootUserIni) + 1;         /* Root: System Ini */
    ((PSHARE1) pMemShared)->pszRootSystemIni = pszTemp;
    strcpy (pszTemp, pszRootSystemIni);

    /* Setup lesen */
    usBytes = CCHMAXUSERNAME;
    EAReadASCII (pszRootSystemIni, EA_DEFTIMEOUT, szAutouser, &usBytes);
    if (usBytes != 0)
        {
        ulTimeout = atol (szAutouser);
        usBytes = CCHMAXUSERNAME;
        EAReadASCII (pszRootSystemIni, EA_DEFUSER, szAutouser, &usBytes);
        if (usBytes == 0)
            szAutouser[0] = '\0';
        }

    /* Registrierung prÅfen */
    arValue[0].uscValue = CCHMAXREGUSER;
    arValue[0].pValue   = szRegUser;
    arValue[1].uscValue = CCHMAXREGPWD;
    arValue[1].pValue   = szRegPassword;
    arValue[2].pValue   = NULL;
    EAReadMV (pszRootSystemIni, EA_REGISTERNAME, EAT_MVST, arValue);
    if (arValue[0].usEAType != EAT_ASCII)
        *szRegUser = '\0';
    if (arValue[1].usEAType != EAT_ASCII)
        *szRegPassword = '\0';
    ulRegState =  RegCheck (szRegUser, szRegPassword, &usSerial,
        WPSam_MajorVersion, WPSam_MinorVersion);

    /* Admin-Pa·wort lesen; es steht verschlÅsselt in szAdminPwd */
    usBytes = CCHMAXPASSWORD;
    EAReadASCII (pszRootSystemIni, EA_ADMINPWD, szAdminPwd, &usBytes);

    /* Magnetkartenleser vorbereiten */
    bIsMag  = MCInit ();

    WMInitialize (40);
    WMCreateMsgQueue (50);

    if (argc != 2)
        Shutdown ();
    else
        {
        margc = argc;
        margv = argv;

        WMLoadDlg (HWND_DESKTOP, HWND_DESKTOP, MainDlgProc, dlg, DID_DIALOG, NULL);

        while (WMGetMsg (&qmsg))
            WMDispatchMsg (&qmsg);
        }

    WMDestroyMsgQueue ();
    WMTerminate();

    return 0;
    }

/*****************************************************************************
 * Systemabschlu·; Unbedingt notwendig fÅr HPFS-Systeme
 *****************************************************************************/
void Shutdown (void)
    {
    /* ??? evtl in CREATEPARMS fÅr den Fall fehlerhafter ParameterÅbergabein main */
    /* einen anderen Textstring Åbergeben ! */
    WMDlgBox (HWND_DESKTOP, HWND_DESKTOP, SDDlgProc, dlg, DID_SHUTDOWN, NULL);
    while (TRUE) ;
    }

/*****************************************************************************
 * Untersucht, ob eine Datei vom Typ WPSAM ist. Dazu wird der Puffer
 * von DosFind* Åbergeben.
 * Eingang: pFileFind: Ausgangspuffer von DosFind* (Level 3 Info)
 * return:  TRUE: Typ ist korrekt
 *****************************************************************************/
BOOL IsType (PFILEFINDBUF3 pffb3)
    {
    BOOL bRC;
    CHAR szPath[CCHMAXPATH];
    STRUC_EAT_DATA arValue[2];

    /* Existiert der Benutzer? */
    DosSearchPath (SEARCH_IGNORENETERRS | SEARCH_ENVIRONMENT, SAMENVVAR,
        pffb3->achName, szPath, CCHMAXPATH);

    arValue[0].uscValue = sizeof (EA_TYPEWPSAM);
    arValue[0].pValue   = EA_TYPENAME;
    arValue[1].pValue   = NULL;
    bRC =  EAReadMV (szPath, EA_TYPENAME, EAT_MVST, arValue);
    bRC &= ((arValue[0].usEAType == EAT_ASCII) & (arValue[0].uscValue > 0));
    if (!bRC)
        return bRC;

    /* Hat der Benutzer den richtigen Typ? */
    bRC = strcmp (arValue[0].pValue, EA_TYPEWPSAM) == 0;

    return bRC;
    }

/*****************************************************************************
 * PrÅft, ob ein Benutzer initialisiert ist und ob er privat oder global ist.
 * Eingang: pszUser: kompletter Pfad incl. Benutzername
 * return:  USTAT_*-Werte
 *****************************************************************************/
ULONG IsInitialized (PCHAR pszUser)
    {
    ULONG          ulRC = 0;                    /* User nicht initialisiert */
    ULONG          ulFlags;
    BOOL           bRC;
    USHORT         usBytes;
    PCHAR          pszProfile;
    CHAR           szPath[CCHMAXPATH];
    CHAR           szPathProfile[CCHMAXPATH];
    FILESTATUS3    fs3;
    STRUC_EAT_DATA arValue[2];

    /* Pfad des WPSam-Verzeichnisses */
    strcpy (szPath, pszUser);

    /* Ist es eine private ArbeitsoberflÑche? */
    arValue[0].uscValue = sizeof (ULONG);
    arValue[0].pValue   = (PBYTE)&ulFlags;
    EARead (szPath, EA_FLAGS, arValue);
    if (ulFlags & PRIVATESYS)
        ulRC = USTAT_PRIVATE;

    /* Ist das Benutzerprofil bereits initialisiert? */
    strcpy (szPathProfile, pszUser);
    strcat (szPathProfile, "\\");
    pszProfile = szPathProfile + strlen (szPathProfile);
    usBytes = CCHMAXPATH - (pszProfile - szPathProfile);
    EAReadASCII (szPath, EA_USERPROFILE, pszProfile, &usBytes);
    bRC = DosQueryPathInfo (szPathProfile, FIL_STANDARD, &fs3, sizeof (fs3)) == 0;
    if (bRC && (fs3.cbFile != 0))
        {
        /* Wurde bei einer privaten ArbeitsoberflÑche auch das Systemprofil initialisiert? */
        if (ulRC & USTAT_PRIVATE)
            {
            usBytes = CCHMAXPATH - (pszProfile - szPathProfile);
            EAReadASCII (szPath, EA_SYSTEMPROFILE, pszProfile, &usBytes);
            bRC = DosQueryPathInfo (szPathProfile, FIL_STANDARD, &fs3, sizeof (fs3)) == 0;
            if (bRC && (fs3.cbFile != 0))
                ulRC |= USTAT_INITIALIZED;
            }
        else
            ulRC |= USTAT_INITIALIZED;
        }

    return ulRC;
    }

/*****************************************************************************
 * Pa·wort lesen: Falls der User *pszUser ein Pa·wort benîtigt, wird es
 * abgefragt.
 * Eingang: pszUser:   Benutzer: vollstÑndiger Pfad
 *          pszPasswd: eingegebenes Pa·wort
 * return:  STAT_* - Werte
 *****************************************************************************/
SHORT ReadPasswd (PCHAR pszUser, PCHAR pszPasswd)
    {
    USHORT usBytes;
    SHORT  sPwd;
    CHAR   bPwd[CCHMAXPASSWORD];

    /* Pa·wort EA lesen */
    usBytes = CCHMAXPASSWORD;
    sPwd = EAReadASCII (pszUser, EA_PWDNAME, bPwd, &usBytes) ?
        STAT_WRONGPASSWD : STAT_NOPASSWORD;
    if (sPwd == STAT_WRONGPASSWD)
        sPwd = (usBytes == 0) ? STAT_NOPASSWORD : STAT_WRONGPASSWD;

    /* Pa·wort existiert; abfragen und vergleichen */
    if (sPwd == STAT_WRONGPASSWD)
        {
        sPwd = (strcmp (bPwd, ChiffreSZ (pszPasswd)) == 0) ?
            STAT_PASSWORDOK : STAT_WRONGPASSWD;
        }

    return sPwd;
    }

/*****************************************************************************
 * Bestimmt zu einem Benutzernamen den zugehîrigen vollstÑndigen Pfad
 * des Benutzerverzeichnisses. Ist der Benutzername ungÅltig, wird
 * ein DOS-Fehlercode zurÅckgegeben.
 * Eingang: pszUser    : Benutzername
 *          ulcUserPath: Grî·e des Ergebnispuffers in Byte
 * Ausgang: pszUserPath: Ergebnis
 * return:  0          : Ergebnis o.k.
 *          sonst      : DOS-Fehlercode
 *****************************************************************************/
APIRET GetUserPath (PCHAR pszUser, PCHAR pszUserPath, ULONG ulcUserPath)
    {
    PCHAR        pszSamEnv;
    CHAR         szString[CCHMAXEALEN];
    CHAR         szDir[CCHMAXPATH];
    CHAR         szUser[CCHMAXPATH];
    USHORT       usBytes;
    ULONG        ulCount, i;
    APIRET       rc;
    HDIR         hDir;
    FILEFINDBUF3 ffb3;

    /* Environment-Variable fÅr User-Directory holen */
    rc = DosScanEnv (SAMENVVAR, &pszSamEnv);
    if (rc)
        pszSamEnv = NULL;

    while (pszSamEnv != NULL)
        {
        /* Pfad+Wildcard erstellen */
        for (i=0; TRUE; i++)
            {
            if (*pszSamEnv == ';')
                {
                pszSamEnv++;
                break;
                }
            else if (*pszSamEnv == '\0')
                {
                pszSamEnv = NULL;
                break;
                }
            else
                szDir[i] = *pszSamEnv++;
            }
        if (szDir[i-1] != '\\')
            i++;

        /* Der Pfad wird nur ausgewertet, wenn er nicht zu lang ist */
        if (i + sizeof (DIR_ALL)-2 < CCHMAXPATH)
            {
            strcpy (szDir+i-1, DIR_ALL);
            strcpy (szUser, szDir);

            /* Dateisuche */
            ulCount = 1;
            hDir = HDIR_CREATE;
            rc = DosFindFirst (szDir, &hDir, MUST_HAVE_DIRECTORY,
                &ffb3, sizeof (FILEFINDBUF3), &ulCount, FIL_STANDARD);

            while (rc==0)
                {
                if (IsType (&ffb3) && (i+ffb3.cchName < CCHMAXPATH))
                    {
                    strcpy (szUser+i, ffb3.achName);
                    usBytes = CCHMAXEALEN;
                    if (EAReadASCII (szUser, EA_LONGNAME, szString, &usBytes))
                        if (stricmp (szString, pszUser) == 0)
                            {
                            if (strlen (szUser) < ulcUserPath)
                                {
                                rc = 0;
                                strcpy (pszUserPath, szUser);
                                }
                            else
                                rc = ERROR_BUFFER_OVERFLOW;
                            return rc;
                            }
                    }

                ulCount = 1;
                rc = DosFindNext (hDir, &ffb3, sizeof (FILEFINDBUF3), &ulCount);
                }

            DosFindClose (hDir);
            }
        }

    return rc;
    }

/*****************************************************************************
 * Bestimmt alle Benutzer, die in SAMENVVAR liegen
 * Eingang: pszUsers   : Puffer fÅr Benutzer
 *          pulSize    : Zeiger auf Puffergrî·e fÅr Benutzer
 *          pszPwds    : Puffer fÅr Passwîrter
 *          pulPwds    : Zeiger auf Puffergrî·e fÅr Passwîrter
 * Ausgang: pszUsers   : [Benutzer]*
 *          pulSize    : LÑnge der Gesamtdaten (werden notfalls abgeschnitten!)
 *          pszPwds    : [Pa·wort]*
 *          pulPwds    : Zeiger auf Puffergrî·e fÅr Passwîrter
 * return:  Zahl der Benutzer im Puffer
 *****************************************************************************/
ULONG SumUsers (PCHAR pszUsers, PULONG pulSize, PCHAR pszPwds, PULONG pulPwds)
    {
    PCHAR        pszSamEnv;
    CHAR         szString[CCHMAXEALEN];
    CHAR         szDir[CCHMAXPATH];
    CHAR         szUser[CCHMAXPATH];
    USHORT       usBytes;
    ULONG        ulCount, i;
    APIRET       rc;
    HDIR         hDir;
    FILEFINDBUF3 ffb3;
    ULONG        ulcUser = 0;               /* Zahl der Byte fÅr Benutzer-Puffer */
    ULONG        ulcPwds = 0;               /* Zahl der Byte fÅr Pa·wort-Puffer */
    ULONG        ulc     = 0;               /* Zahl der Benutzer im Puffer */

    /* Environment-Variable fÅr User-Directory holen */
    rc = DosScanEnv (SAMENVVAR, &pszSamEnv);
    if (rc)
        pszSamEnv = NULL;

    while (pszSamEnv != NULL)
        {
        /* Pfad+Wildcard erstellen */
        for (i=0; TRUE; i++)
            {
            if (*pszSamEnv == ';')
                {
                pszSamEnv++;
                break;
                }
            else if (*pszSamEnv == '\0')
                {
                pszSamEnv = NULL;
                break;
                }
            else
                szDir[i] = *pszSamEnv++;
            }
        if (szDir[i-1] != '\\')
            i++;

        /* Der Pfad wird nur ausgewertet, wenn er nicht zu lang ist */
        if (i + sizeof (DIR_ALL)-2 < CCHMAXPATH)
            {
            strcpy (szDir+i-1, DIR_ALL);
            strcpy (szUser, szDir);

            /* Dateisuche */
            ulCount = 1;
            hDir = HDIR_CREATE;
            rc = DosFindFirst (szDir, &hDir, MUST_HAVE_DIRECTORY,
                &ffb3, sizeof (FILEFINDBUF3), &ulCount, FIL_STANDARD);

            while (rc==0)
                {
                if (IsType (&ffb3) && (i+ffb3.cchName < CCHMAXPATH))
                    {
                    strcpy (szUser+i, ffb3.achName);
                    /* Benutzername lesen */
                    usBytes = CCHMAXEALEN;
                    if (EAReadASCII (szUser, EA_LONGNAME, szString, &usBytes))
                        {
                        if ((LONG) strlen (szString) < (LONG)*pulSize)
                            {
                            strcpy (pszUsers+ulcUser, szString);
                            *pulSize -= strlen (szString) + 1;
                            }
                        else
                            *pulSize = 0;
                        ulcUser += strlen (szString) + 1;
                        }
                    /* Pa·wort lesen */
                    usBytes = CCHMAXEALEN;
                    if (EAReadASCII (szUser, EA_PWDNAME, szString, &usBytes))
                        {
                        if ((LONG) strlen (szString) < (LONG)*pulPwds)
                            {
                            strcpy (pszPwds+ulcPwds, szString);
                            *pulPwds -= strlen (szString) + 1;
                            ulc++;
                            }
                        else
                            *pulPwds = 0;
                        ulcPwds += strlen (szString) + 1;
                        }
                    }

                ulCount = 1;
                rc = DosFindNext (hDir, &ffb3, sizeof (FILEFINDBUF3), &ulCount);
                }

            DosFindClose (hDir);
            }
        }

    /* GesamtlÑnge zurÅckgeben */
    *pulSize = ulcUser;
    *pulPwds = ulcPwds;

    return ulc;
    }

/*****************************************************************************
 * Bestimmt den Index eines Tokens in der Tokenliste
 * Eingang: szString   : Token
 *          parString  : Tokenliste
 *          ulc        : Zahl der Tokens in der Liste
 * return:  -1   : Token nicht gefunden
 *          sonst: Index des Tokens in der Liste 
 *****************************************************************************/
LONG Str2Idx (PCHAR szString, PCHAR parString, ULONG ulc)
    {
    ULONG i;

    for (i=0; i<ulc; i++)
        {
        if (stricmp (szString, parString) == 0)
            return (LONG)i;
        parString += strlen (parString) + 1;
        }

    return -1;
    }

/*****************************************************************************
 * Sucht einen Token in einer Tokenliste anhand des Index
 * Eingang: parString  : Tokenliste
 *          lIndex     : Tokenindex
 *          ulc        : Zahl der Tokens in der Liste
 * return:  NULL : Index zu gro·
 *          sonst: Zeiger auf Token
 *****************************************************************************/
PCHAR Idx2Str (PCHAR parString, LONG lIndex, ULONG ulc)
    {
    ULONG i;

    if ((ULONG)lIndex > ulc)
        return NULL;

    for (i=0; i<lIndex; i++)
        parString += strlen (parString) + 1;

    return parString;
    }

MPARAM EXPENTRY MainDlgProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
    {
    ULONG  ulUserStat, ulAction;
    USHORT usBytes, usLen;
    SHORT  sPwd;
    PCHAR  pszText, pszShareMem;
    PCHAR  pszEnv, pszTempEnv, pszTempEnvLine;
    HWND   hwndDlg;
    HFILE  hFile, hFileOld, hFileDup;
    FILEFINDBUF ffb3;
    RESULTCODES ReturnCodes;                    /* Ergebnis von DosExecPgm */
    static ULONG ulTimer;
    static BYTE cell[] = {' ', 0};
    static CHAR szObjName[CCHMAXPATH];          /* Object Name buffer fÅr DosExecPgm */
    static CHAR szUser[CCHMAXUSERNAME];         /* Benutzername (LONGNAME) */
    static CHAR szPasswd[CCHMAXPASSWORD];       /* Benutzerkennwort */
    static CHAR szUserPath[CCHMAXPATH];         /* Pfad fÅr User-ArbeitsoberflÑche */
    static CHAR szProtshell[CCHMAXPATH];        /* PROTSHELL-Anweisung */
    static CHAR szProtshellPath[CCHMAXPATH+1];  /* verifizierte PROTSHELL (2x'\0'!) */
    static MAGNET strucMagnet;                  /* Struktur fÅr Kartenleser */
    static PCHAR pUsers, pPwds;                 /* Variable fÅr Benutzerliste */
    static ULONG ulUsers;                       /* -"- */

    switch (msg)
        {
        case DM_INITDLG:
            WMSendDlgItemMsg (hwnd, DID_USER, EM_SETTEXTLIMIT,
                MPFROMSHORT (CCHMAXUSERNAME-1), 0);
            WMSendDlgItemMsg (hwnd, DID_PWD, EM_SETTEXTLIMIT,
                MPFROMSHORT (CCHMAXPASSWORD-1), 0);
            WMSetFocus (WMWindowFromId (hwnd, DID_USER));
            ulTimer = ulTimeout;
            if ((ulTimer != 0xFFFF) && (ulTimer != 0) &&
                (szAutouser[0] != '\0'))
                WMStartTimer (hwnd, ID_TIMER, 1000);

            /* Variable fÅr Benutzerliste initialisieren */
            pUsers = pPwds = NULL;
            ulUsers = 0;
            WMSendMsg (hwnd, DM_GETUSERS, 0, 0);

            /* KartenleserunterstÅtzung starten */
            strucMagnet.hwnd = hwnd;
            strucMagnet.pszReadData = malloc (CCHMAXPASSWORD*sizeof (CHAR));
            bIsMag &= MCOpen ();
            bIsMag &= MCStart (&strucMagnet);
            if (bIsMag)
                WMSetDlgItemText (hwnd, DID_TXTPWD, TXT_PWDMAG);
            if ((ulTimeout == 0) && (szAutouser[0] != 0))
                WMSendMsg (hwnd, DM_TIMER, 0, 0);
            break;

        case DM_TIMER:
            sprintf (szObjName, TXT_TIMEOUT, ulTimer ? --ulTimer : ulTimer);
            WMSetDlgItemText (hwnd, DID_TO, szObjName);
            if (ulTimer == 0)
                {
                WMSetDlgItemText (hwnd, DID_USER, szAutouser);
                WMStopTimer (hwnd, ID_TIMER);
                WMDestroyWindow (WMWindowFromId (hwnd, DID_TO));
                WMSendMsg (hwnd, DM_COMMAND,
                    MPFROMSHORT (DID_OK),
                    MPFROMSHORT (CMDSRC_OTHER));
                }
            return 0;

        case DM_MAGDATA:
            {
            LONG  lIndex;
            PCHAR pszUser;

            WMSetDlgItemText (hwnd, DID_PWD, strucMagnet.pszReadData);
            WMQueryDlgItemText (hwnd, DID_USER, CCHMAXUSERNAME, szUser);
            if (*szUser == '\0')
                {
                ChiffreSZ (strucMagnet.pszReadData);
                lIndex  = Str2Idx (strucMagnet.pszReadData, pPwds, ulUsers);
                pszUser = Idx2Str (pUsers, lIndex, ulUsers);
                if (pszUser == NULL)
                    WMMessageBox (HWND_DESKTOP, hwnd, TXT_USERNOTFOUND,
                        TXT_ERROR, -1, MB_OK);
                else
                    {
                    WMSetDlgItemText (hwnd, DID_USER, pszUser);
                    WMSendMsg (hwnd, DM_COMMAND,
                        MPFROMSHORT (DID_OK),
                        MPFROMSHORT (CMDSRC_OTHER));
                    }
                }
            else
                WMSendMsg (hwnd, DM_COMMAND,
                    MPFROMSHORT (DID_OK),
                    MPFROMSHORT (CMDSRC_PUSHBUTTON));
            return 0;
            }

        /* Mit dieser Message werden alle Benutzer auf dem System in    */
        /* einer Liste angelegt. pUsers ist die Liste aller Benutzer,   */
        /* pPwds enthÑlt alle ihre Passwîrter (gleiche Reihenfolge).    */
        /* ulUsers enthÑlt die Benutzerzahl.                            */
        case DM_GETUSERS:
            {
            ULONG ulcUsers, ulcPwds;            /* Puffergrî·en in Byte */

            if (pUsers != NULL)
                {
                free (pUsers);
                free (pPwds);
                }
            ulcUsers = ulcPwds = 0;
            SumUsers (NULL, &ulcUsers, NULL, &ulcPwds);
            pUsers = malloc (sizeof (CHAR)*ulcUsers);
            pPwds  = malloc (sizeof (CHAR)*ulcPwds);
            ulUsers = SumUsers (pUsers, &ulcUsers, pPwds, &ulcPwds);
            return 0;
            }

        case DM_CHAR:
            if (SHORT1FROMMP (mp1) & KC_VIRTUALKEY)
                {
                switch (SHORT2FROMMP (mp2))
                    {
                    case VK_F2:
                        WMSendMsg (hwnd, DM_COMMAND,
                            MPFROMSHORT (DID_STRTDEF),
                            MPFROMSHORT (CMDSRC_OTHER));
                        return 0;

                    case VK_F9:
                        WMSendMsg (hwnd, DM_COMMAND,
                            MPFROMSHORT (DID_SETUP),
                            MPFROMSHORT (CMDSRC_OTHER));
                        return 0;

                    case VK_F10:
                        WMSendMsg (hwnd, DM_COMMAND,
                            MPFROMSHORT (DID_SHUTDOWN),
                            MPFROMSHORT (CMDSRC_OTHER));
                        return 0;
                    }
                }
            break;

        case DM_CONTROL:
            if ((ulTimer > 0) && (SHORT1FROMMP (mp1) == DID_USER))
                {
                WMStopTimer (hwnd, ID_TIMER);
                ulTimer = 0;
                WMDestroyWindow (WMWindowFromId (hwnd, DID_TO));
                    }
            return 0;

        case DM_COMMAND:
            if (SHORT1FROMMP (mp1) == DID_SETUP)
                {
                if (szAdminPwd[0] != '\0')
                    {
                    if (WMDlgBox (hwnd, hwnd, PwdDlgProc, dlg, DID_GETPWD, NULL) !=
                        DID_OK)
                        {
                        WMMessageBox (HWND_DESKTOP, hwnd, TXT_WRONGPASSWD,
                            TXT_ERROR, -1, MB_OK);
                        return 0;
                        }
                    }
                WMDlgBox (hwnd, hwnd, SetupDlgProc, dlg, DID_DLGSET, NULL);
                return 0;
                }
            else if (SHORT1FROMMP (mp1) == DID_SHUTDOWN)
                {
                Shutdown ();
                return 0;
                }
            else if (SHORT1FROMMP (mp1) == DID_STRTDEF)
                {
                if (ulTimer > 0)
                    {
                    WMStopTimer (hwnd, ID_TIMER);
                    ulTimer = 0;
                    WMDestroyWindow (WMWindowFromId (hwnd, DID_TO));
                    }
                WMSetDlgItemText (hwnd, DID_USER, szAutouser);
                WMSendMsg (hwnd, DM_COMMAND,
                    MPFROMSHORT (DID_OK),
                    MPFROMSHORT (CMDSRC_OTHER));
                return 0;
                }
            else if (SHORT1FROMMP (mp1) == DID_CANCEL)
                return 0;
            else if (SHORT1FROMMP (mp1) == DID_OK)
                {
                /* Es wurde OK gedrÅckt. Es wird Benutzername und Pa·wort gelesen, */
                /* der Benutzer gesucht und das Pa·wort verifiziert.               */
                WMQueryDlgItemText (hwnd, DID_USER, CCHMAXUSERNAME, szUser);
                WMQueryDlgItemText (hwnd, DID_PWD, CCHMAXPASSWORD, szPasswd);
                WMSetDlgItemText (hwnd, DID_PWD, "");
                if (szUser[0] == '\0')
                    sPwd = STAT_NOUSERFOUND;
                else
                    {
                    if (stricmp (szUser, ROOTSHELL) == 0)
                        sPwd = STAT_ROOTUSER;
                    else
                        {
                        if (GetUserPath (szUser, szUserPath, CCHMAXPATH))
                            sPwd = STAT_USERNOTFOUND;
                        else
                            {
                            /* Falls die DM_COMMAND-Quelle der Timer ist, */
                            /* darf kein Pa·wort abgefragt werden.        */
                            if (SHORT1FROMMP (mp2) == CMDSRC_OTHER)
                                sPwd = STAT_NOPASSWORD;
                            else
                                sPwd = ReadPasswd (szUserPath, szPasswd);
                            }
                        }
                    }

                /* Im Fehlerfall den korrekten Fehlertext laden */
                switch (sPwd)
                    {
                    case STAT_NOUSERFOUND:
                        pszText = TXT_NOUSER;
                        break;
                    case STAT_USERNOTFOUND:
                        pszText = TXT_USERNOTFOUND;
                        break;
                    case STAT_WRONGPASSWD:
                        pszText = TXT_WRONGPASSWD;
                        break;
                    }
                if ((sPwd == STAT_NOPASSWORD) || (sPwd == STAT_PASSWORDOK))
                    {
                    /* Protshell des Benutzers bestimmen */
                    usBytes = CCHMAXPATH;
                    if (!EAReadASCII (szUserPath, EA_PROTSHELL,
                        szProtshell, &usBytes))
                        {
                        sPwd   = STAT_VERIFYFAILED;
                        pszText = TXT_USERDESTROYED;
                        }
                    else
                        {
                        /* User benutzt Default-Protshell */
                        if ((*szProtshell == '\0') && (margv[1] != NULL))
                            strcpy (szProtshell, margv[1]);

                        /* Shell verifizieren: Wurde kein Pfad ange- */
                        /* geben, wird die PATH-Variable durchsucht  */
                        if (DosQueryPathInfo (szProtshell, FIL_STANDARD,
                                              &ffb3, sizeof (FILESTATUS3)))
                            {
                            if (DosSearchPath (SEARCH_IGNORENETERRS | SEARCH_ENVIRONMENT,
                                               "PATH", szProtshell,
                                               szProtshellPath, CCHMAXPATH))
                                {
                                sPwd   = STAT_VERIFYFAILED;
                                pszText = TXT_SHELLNOTFOUND;
                                }
                            }
                        else
                            strcpy (szProtshellPath, szProtshell);
                        }
                    }
                else if (sPwd == STAT_ROOTUSER)
                    {
                    strcpy (szProtshell, margv[1]);
                    strcpy (szProtshellPath , margv[1]);
                    }

                /* Fehlerausgabe und Protokollierung */
                switch (sPwd)
                    {
                    case STAT_VERIFYFAILED:
                    case STAT_NOUSERFOUND:
                    case STAT_USERNOTFOUND:
                        WMMessageBox (HWND_DESKTOP, hwnd, pszText, TXT_ERROR,
                            -1, MB_OK);
                        return 0;

                    case STAT_WRONGPASSWD:
                        WMMessageBox (HWND_DESKTOP, hwnd, pszText, TXT_ERROR,
                            -1, MB_OK);
                        History (szUserPath, APPNAME, TXT_WRONGPASSWORD);
                        return 0;
                    }

                /* Der Benutzer wurde gefunden, Pa·wort ist ok.       */
                /* Zuerst wird geprÅft, ob die INI-Dateien existieren */
                if (sPwd == STAT_ROOTUSER)
                    ulUserStat = USTAT_INITIALIZED;
                else
                    ulUserStat = IsInitialized (szUserPath);

                if (!(ulUserStat & USTAT_INITIALIZED))
                    {
                    /* Benutzer noch nicht initialisiert => nachholen */
                    User.pszMyName         = szUser;
                    User.pszMyPath         = szUserPath;
                    User.pszUserProfile    = szUserProfile;
                    User.pszUserResource   = szUserResource;
                    User.pszSystemProfile  = szSystemProfile;
                    User.pszSystemResource = szSystemResource;
                    User.pszDesktopName    = TXT_DESKTOP;

                    usBytes = CCHMAXPATH;
                    EAReadASCII (szUserPath, EA_USERPROFILE,
                        szUserProfile, &usBytes);
                    usBytes = CCHMAXPATH;
                    EAReadASCII (szUserPath, EA_USERRESOURCE,
                        szUserResource, &usBytes);
                    usBytes = CCHMAXPATH;
                    EAReadASCII (szUserPath, EA_SYSTEMPROFILE,
                        szSystemProfile, &usBytes);
                    usBytes = CCHMAXPATH;
                    EAReadASCII (szUserPath, EA_SYSTEMRESOURCE,
                        szSystemResource, &usBytes);

                    /* stdout umleiten nach NUL */
                    DosOpen ("NUL", &hFile, &ulAction, 0, FILE_NORMAL,
                        OPEN_ACTION_OPEN_IF_EXISTS,
                        OPEN_SHARE_DENYNONE | OPEN_ACCESS_WRITEONLY,
                        NULL);
                    hFileOld = 1;           /* stdout umleiten */
                    hFileDup = -1;          /* aktuellen Handle kopieren */
                    DosDupHandle (hFileOld, &hFileDup);
                    DosDupHandle (hFile, &hFileOld);
                    DosClose (hFile);

                    DosCreateThread (&tid, (PFNTHREAD) thCreate,
                                     (ULONG) &User, 0, 16384);
                    hwndDlg = WMLoadDlg (HWND_DESKTOP, HWND_DESKTOP, 
                        WMDefDlgProc, dlg, DID_MAKEINI, NULL);
                    DosWaitThread (&tid, DCWW_WAIT);
                    WMDestroyWindow (hwndDlg);

                    /* stdout-Umleitung rÅckgÑngig machen */
                    hFileOld = 1;           /* alter stdout-Handle */
                    DosDupHandle (hFileDup, &hFileOld);
                    DosClose (hFileDup);

                    switch (User.ulErrorMsg)
                        {
                        case ERR_NOUSERRESOURCE:
                            break;
                        case ERR_NOSYSTEMRESOURCE:
                            break;
                        case ERR_NOUSERINICREATED:
                            break;
                        case ERR_NOSYSTEMINICREATED:
                            break;
                        case ERR_NODESKINRC:
                            break;
                        case ERR_PATHTOOLONG:
                            break;
                        }

                    History (szUserPath, APPNAME, TXT_SETUP);
                    }
                else
                    History (szUserPath, APPNAME, TXT_PRFRESET);

                /* Argumentstring doppelt terminieren */
                szProtshellPath[strlen (szProtshellPath) + 1] = '\0';

                /* Shared Memory Bereich und Environment fertig machen */
                pszShareMem = ((PSHARE1) pMemShared)->pszRootSystemIni;
                pszShareMem += strlen (pszShareMem) + 1;
                ((PSHARE1) pMemShared)->pszUserIni = pszShareMem;
                pszShareMem += CCHMAXPATHCOMP;
                ((PSHARE1) pMemShared)->pszSystemIni = pszShareMem;
                pszShareMem += CCHMAXPATHCOMP;
                ((PSHARE1) pMemShared)->pszEnvironment = pszShareMem;

                if (ulUserStat & USTAT_PRIVATE)
                    {
                    /* privater Benutzer */
                    if (ulRegState != REGCHECK_OK)
                        {
                        WMMessageBox (HWND_DESKTOP, hwnd, TXT_REGISTER, TXT_ERROR,
                            -1, MB_OK);
                        return 0;
                        }
                    ((PSHARE1) pMemShared)->pszRootUserIni[0]   = '\0';
                    ((PSHARE1) pMemShared)->pszRootSystemIni[0] = '\0';
                    ((PSHARE1) pMemShared)->pszUserIni[0]       = '\0';
                    ((PSHARE1) pMemShared)->pszSystemIni[0]     = '\0';
                    ((PSHARE1) pMemShared)->pszEnvironment[0]   = '\0';
                    usBytes = 0;
                    EAReadASCII (szUserPath, EA_ENVNAME, NULL, &usBytes);
                    usLen = usBytes + 2*CCHMAXPATH + 1 +    /* ??? */
                            sizeof ("\r\nUSER_INI=") +
                            sizeof ("\r\nSYSTEM_INI=");
                    DosAllocMem ((PPVOID) &pszTempEnv, usLen,
                        PAG_COMMIT | PAG_READ | PAG_WRITE);
                    EAReadASCII (szUserPath, EA_ENVNAME, pszTempEnv, &usLen);
                    strcat (pszTempEnv, "\r\nUSER_INI=");
                    strcat (pszTempEnv, szUserPath);
                    strcat (pszTempEnv, "\\");
                    pszTempEnvLine = pszTempEnv + strlen (pszTempEnv);
                    usBytes = usLen - strlen (pszTempEnv);
                    EAReadASCII (szUserPath, EA_USERPROFILE, pszTempEnvLine, &usBytes);
                    strcat (pszTempEnvLine, "\r\nSYSTEM_INI=");
                    strcat (pszTempEnvLine, szUserPath);
                    strcat (pszTempEnvLine, "\\");
                    pszTempEnvLine = pszTempEnv + strlen (pszTempEnv);
                    usBytes = usLen - strlen (pszTempEnv);
                    EAReadASCII (szUserPath, EA_SYSTEMPROFILE, pszTempEnvLine, &usBytes);
                    pszEnv = MakeEnv (pszTempEnv);
                    DosFreeMem (pszTempEnv);
                    }
                else
                    {
                    /* globaler Benutzer + Root */
                    /* USERPROFILE */
                    pszShareMem = ((PSHARE1) pMemShared)->pszUserIni;
                    if (sPwd == STAT_ROOTUSER)
                        usBytes = 0;
                    else
                        {
                        usBytes = CCHMAXPATHCOMP;
                        strcpy (pszShareMem, szUserPath);
                        strcat (pszShareMem, "\\");
                        EAReadASCII (szUserPath, EA_USERPROFILE,
                            pszShareMem+strlen (pszShareMem), &usBytes);
                        }
                    if (usBytes == 0)
                        pszShareMem[0] = '\0';

                    /* SYSTEMPROFILE */
                    pszShareMem = ((PSHARE1) pMemShared)->pszSystemIni;
                    if (sPwd == STAT_ROOTUSER)
                        usBytes = 0;
                    else
                        {
                        usBytes = CCHMAXPATHCOMP;
                        strcpy (pszShareMem, szUserPath);
                        strcat (pszShareMem, "\\");
                        EAReadASCII (szUserPath, EA_SYSTEMPROFILE,
                            pszShareMem+strlen (pszShareMem), &usBytes);
                        }
                    if (usBytes == 0)
                        pszShareMem[0] = '\0';

                    /* ENVIRONMENT */
                    pszShareMem = ((PSHARE1) pMemShared)->pszEnvironment;
                    if (sPwd == STAT_ROOTUSER)
                        usBytes = 0;
                    else
                        {
                        usBytes = CCHSHARE_INITOS2 - (pszShareMem-pMemShared);
                        EAReadASCII (szUserPath, EA_ENVNAME, pszShareMem, &usBytes);
                        ((PSHARE1) pMemShared)->pszEnvironment = pszShareMem;
                        }
                    if (usBytes == 0)
                        pszShareMem[0] = '\0';

                    pszEnv = NULL;
                    }

                /* Tastaturthread und Kartenleser anhalten */
                UnlinkKeyboard (SEM_INDEFINITE_WAIT);
                bIsMag = FALSE;
                MCAbort ();
                MCClose ();
                MCFree ();

                /* Benutzerliste lîschen */
                if (pUsers != NULL)
                    {
                    free (pUsers);
                    free (pPwds);
                    pUsers = pPwds = NULL;
                    ulUsers = 0;
                    }

                /* Bildschirm lîschen */
                VioScrollUp (0, 0, -1, -1, -1, cell, 0);
                VioSetCurPos (24, 0, 0);

                /* Pfad auf Root des Bootdrive setzen */
                DosExecPgm (szObjName,              /* Object name buffer */
                            sizeof (szObjName),     /* Length of object name buffer */
                            EXEC_SYNC,              /* Execution flags */
                            szProtshellPath,        /* Argument string */
                            pszEnv,                 /* Environment */
                            &ReturnCodes,           /* Termination codes */
                            szProtshellPath);       /* Program file name */

                /* Tastatur und Kartenleser wieder starten */
                LinkKeyboard ();
                bIsMag  = MCInit ();
                bIsMag &= MCOpen ();
                bIsMag &= MCStart (&strucMagnet);

                /* Benutzerliste wieder anlegen */
                WMSendMsg (hwnd, DM_GETUSERS, 0, 0);

                WMInvalidateAll ();
                WMSetFocus (HWND_DESKTOP);
                return 0;
                }
            break;
        }

    return WMDefDlgProc (hwnd, msg, mp1, mp2);
    }

MPARAM EXPENTRY SetupDlgProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
    {
    CHAR szText[CCHMAXUSERNAME];
    CHAR szPwd1[CCHMAXPASSWORD];
    CHAR szPwd2[CCHMAXPASSWORD];

    switch (msg)
        {
        case DM_INITDLG:
            WMSendDlgItemMsg (hwnd, DID_AUTOUSER, EM_SETTEXTLIMIT,
                MPFROMSHORT (CCHMAXUSERNAME-1), 0);
            WMSendDlgItemMsg (hwnd, DID_TIMEOUT, EM_SETTEXTLIMIT,
                MPFROMSHORT (CCHMAXNUM), 0);
            WMSendDlgItemMsg (hwnd, DID_ADMINPWD, EM_SETTEXTLIMIT,
                MPFROMSHORT (CCHMAXPASSWORD), 0);
            WMSendDlgItemMsg (hwnd, DID_PWDREPT, EM_SETTEXTLIMIT,
                MPFROMSHORT (CCHMAXPASSWORD), 0);

            if (szAutouser[0] != '\0')
                {
                WMSetDlgItemText (hwnd, DID_AUTOUSER, szAutouser);
                if (ulTimeout != 0xFFFF)
                    _itoa (ulTimeout, szText, 10);
                else
                    *szText = '\0';
                WMSetDlgItemText (hwnd, DID_TIMEOUT, szText);
                }

            WMSetFocus (WMWindowFromId (hwnd, DID_AUTOUSER));
            return 0;

        case DM_COMMAND:
            switch (SHORT1FROMMP (mp1))
                {
                case DID_OK:
                    WMQueryDlgItemText (hwnd, DID_AUTOUSER, CCHMAXUSERNAME, szAutouser);
                    WMQueryDlgItemText (hwnd, DID_TIMEOUT,  CCHMAXNUM,      szText);
                    if (*szText == '\0')
                        ulTimeout = 0xFFFF;
                    else
                        ulTimeout = atoi (szText);
                    if (szAutouser[0] == '\0')
                        {
                        ulTimeout = 0;
                        szAutouser[0] = '\0';
                        }
                    WMQueryDlgItemText (hwnd, DID_ADMINPWD, CCHMAXPASSWORD, szPwd1);
                    WMQueryDlgItemText (hwnd, DID_PWDREPT,  CCHMAXPASSWORD, szPwd2);
                    if (strcmp (szPwd1, szPwd2) != 0)
                        {
                        WMMessageBox (HWND_DESKTOP, hwnd, TXT_DIFFPWD, TXT_ERROR,
                            -1, MB_OK);
                        WMSetDlgItemText (hwnd, DID_ADMINPWD, "");
                        WMSetDlgItemText (hwnd, DID_PWDREPT,  "");
                        return 0;
                        }
                    if (szPwd1[0] != '\0')
                    strcpy (szAdminPwd, ChiffreSZ (szPwd1));
                    WMSendMsg (hwnd, DM_CLOSE, 0, 0);
                    break;

                case DID_DELPWD:
                    WMSetDlgItemText (hwnd, DID_ADMINPWD, "");
                    WMSetDlgItemText (hwnd, DID_PWDREPT,  "");
                    strcpy (szAdminPwd, ChiffreSZ (""));
                    WMMessageBox (HWND_DESKTOP, hwnd, TXT_DELPWD, NULL,
                        -1, MB_OK);
                    return 0;
                }
            break;

        case DM_CLOSE:
            EAWriteASCII (pszRootSystemIni, EA_DEFUSER, szAutouser);
            EAWriteASCII (pszRootSystemIni, EA_DEFTIMEOUT,
                _itoa (ulTimeout, szText, 10));
            EAWriteASCII (pszRootSystemIni, EA_ADMINPWD, szAdminPwd);
            WMDismissDlg (hwnd, DID_OK);
            return 0;
        }

    return WMDefDlgProc (hwnd, msg, mp1, mp2);
    }

MPARAM EXPENTRY PwdDlgProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
    {
    CHAR szText[CCHMAXPASSWORD];

    switch (msg)
        {
        case DM_INITDLG:
            WMSendDlgItemMsg (hwnd, DID_PWD, EM_SETTEXTLIMIT,
                MPFROMSHORT (CCHMAXPASSWORD), 0);
            WMSetFocus (WMWindowFromId (hwnd, DID_PWD));
            return 0;

        case DM_COMMAND:
            if ((SHORT1FROMMP (mp1) == DID_OK) &&
                (SHORT1FROMMP (mp1) == CMDSRC_PUSHBUTTON))
                {
                WMQueryDlgItemText (hwnd, DID_PWD, CCHMAXPASSWORD, szText);
                if (strcmp (szAdminPwd, ChiffreSZ (szText)) == 0)
                    WMDismissDlg (hwnd, DID_OK);
                else
                    WMSendMsg (hwnd, DM_CLOSE, 0, 0);
                return 0;
                }
        }

    return WMDefDlgProc (hwnd, msg, mp1, mp2);
    }

MPARAM EXPENTRY SDDlgProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
    {

    switch (msg)
        {
        case DM_INITDLG:
            {
            VIOCURSORINFO CursorInvisible =
                {-90, -100, 1, -1};                     /* unsichtbarer Cursor */

            VioSetCurType (&CursorInvisible, 0);
            DosShutdown (0L);
            WMSetDlgItemText (hwnd, DID_SD, TXT_SHUTDOWN);
            while (TRUE) ;
            break;
            }
        }
    return WMDefDlgProc (hwnd, msg, mp1, mp2);
    }

MPARAM EXPENTRY MkIniDlgProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
    {
    return WMDefDlgProc (hwnd, msg, mp1, mp2);
    }

void _Optlink thMCRead (void *arg)
    {

    while (MCRead ((PMAGNET) arg))
        WMPostMsg(((PMAGNET)arg)->hwnd, DM_MAGDATA, 0, 0);

    return;
    }
