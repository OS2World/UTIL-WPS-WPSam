/*
 *
 *
 *   Module Name: MakeIni
 *
 *   OS/2 Workplace Shell Access Manager
 *
 *   Initialize User desktop
 *
 */

#define CLASSNAME   "thCreate"          // FÅr Debug-Funktionen

#define INCL_DOS
#define INCL_WINWORKPLACE
#define INCL_WINWINDOWMGR

#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "samglob.h"

#define WPC_SAM  ";WPSamF;"
#define WPC_IBM  ";WPDesktop;"

#define MAKEINI  "?:\\OS2\\MAKEINI.EXE"

APIRET APIENTRY thCreate (USER *pUser)
    {
    ULONG          ulBootDrive, ulFlags;
    LONG           lLen;
    PCHAR          szPos;
    STRUC_EAT_DATA arValue[2];
    FILE           *source, *dest;
    BOOL           bDeskFound = FALSE;
    BOOL           bWPSamF;
    static CHAR szResName[CCHMAXPATH], szPrfName[CCHMAXPATH];
    static CHAR szLine[CCHMAXRCLINE], szLine2[CCHMAXRCLINE];

    /* Boot-Laufwerk bestimmen */
    DosQuerySysInfo (QSV_BOOT_DRIVE, QSV_BOOT_DRIVE, &ulBootDrive, sizeof (ULONG));
    pUser->ulErrorMsg = 0;

    /* User Resource und User Profile erstellen, falls nicht vorhanden */
    lLen = strlen (pUser->pszMyPath) + strlen (pUser->pszUserProfile) + 1;
    if (lLen+1 >= CCHMAXPATH)
        {
        pUser->ulErrorMsg = ERR_PATHTOOLONG;
        DosExit (EXIT_THREAD, 0);
        }
    strcpy (szPrfName, pUser->pszMyPath);
    strcat (szPrfName, "\\");
    strcat (szPrfName, pUser->pszUserProfile);

    if (!FileExist (szPrfName))
        {
        /* User resource erzeugen */
        strcpy (szResName, pUser->pszMyPath);
        strcat (szResName, "\\INI.RC");

        DebugE (D_CRT, "User rc-file, source", pUser->pszUserResource);
        DebugE (D_CRT, "User rc-file, target", szResName);

        /* Default User Resource îffnen */
        if ((source = fopen (pUser->pszUserResource, "r")) == NULL)
            {
            pUser->ulErrorMsg = ERR_NOUSERRESOURCE;
            DosExit (EXIT_THREAD, 0);
            }

        /* User Resource îffnen */
        if ((dest = fopen (szResName, "w")) == NULL)
            {
            fclose (source);
            pUser->ulErrorMsg = ERR_NOUSERINICREATED;
            DosExit (EXIT_THREAD, 0);
            }

        /* Objekt-Typ lesen */  // ??? nicht benîtigt
        arValue[0].uscValue = sizeof (szLine2);
        arValue[0].pValue   = szLine2;
        arValue[1].pValue   = NULL;
        EAReadMV (pUser->pszMyPath, EA_TYPENAME, EAT_MVST, arValue);

        arValue[0].uscValue = sizeof (ULONG);
        arValue[0].pValue   = (PBYTE)&ulFlags;
        EARead   (pUser->pszMyPath, EA_FLAGS, arValue);

        DebugE (D_CRT, "Object Type", szLine2);
        DebugULx (D_CRT, "Read EA", "_ulFlag", ulFlags);
// ???  bWPSamF = strcmp (szLine2, EA_TYPEWPSAM) == 0 && !(ulFlags & PRIVATESYS);
        bWPSamF = !(ulFlags & PRIVATESYS);
        bWPSamF = FALSE;        // provisorisch fÅr WPSac ???
        DebugE (D_CRT, "bWPSamF", bWPSamF ? "TRUE" : "FALSE");

        /* PM_InstallObject fÅr <WPDesktop> Ñndern */
        while (fgets (szLine, CCHMAXRCLINE, source) != NULL)
            {
            if (strstr (szLine, "PM_InstallObject") == NULL)
                fputs (szLine, dest);
            else
                {
                szPos = strstr (szLine, "WPDesktop");

                if (szPos == NULL)
                    fputs (szLine, dest);
                else
                    {
                    szPos = strstr (szLine, "\"OBJECTID=");
                    strcpy (szLine2, "  \"PM_InstallObject\" \"");

                    /* lLen enthÑlt den Pufferplatz */
                    lLen = CCHMAXRCLINE     - strlen (szLine2)
                        - strlen (szPos)     - strlen (pUser->pszMyPath)
                        - strlen (pUser->pszDesktopName)
                        - strlen (pUser->pszMyName)
                        - 2 - strlen (WPC_SAM);
                    if (lLen <= 0)
                        {
                        fclose (source);
                        fclose (dest);
                        pUser->ulErrorMsg = ERR_PATHTOOLONG;
                        DosExit (EXIT_THREAD, 0);
                        }

                    /* Zusammenbauen der neuen Zeile in INI-Datei */
                    strcat (szLine2, pUser->pszDesktopName);
                    strcat (szLine2, pUser->pszMyName);
                    strcat (szLine2, bWPSamF ? WPC_SAM : WPC_IBM);
                    strcat (szLine2, pUser->pszMyPath);
                    strcat (szLine2, "\" ");            /* LÑnge: 2 Zeichen */
                    strcat (szLine2, szPos);

                    fputs (szLine2, dest);
                    DebugE (D_CRT, "User Resource File, <WP_DESKTOP>-Registration\n\t", szLine2);
                    bDeskFound = TRUE;
                    }
                }
            }

        fclose (source);
        fclose (dest);

        if (!bDeskFound)
            {
            pUser->ulErrorMsg = ERR_NODESKINRC;
            DosExit (EXIT_THREAD, 0);
            }

        /* User Profile compilieren */
        lLen = CCHMAXRCLINE - strlen (MAKEINI)
            - 1 - 3 - 1 - strlen (szPrfName) - strlen (szResName);
        if (lLen < 3)
            {
            pUser->ulErrorMsg = ERR_PATHTOOLONG;
            DosExit (EXIT_THREAD, 0);
            }

        memset (szLine, '\0', sizeof (szLine));
        strcpy (szLine, MAKEINI);
        szLine[0] = (CHAR) (ulBootDrive - 1) + 'A';
        szPos = szLine + strlen (szLine) + 1;
        strcpy (szPos, "\"");                           /* LÑnge: 1 Zeichen */
        strcat (szPos, szPrfName);
        strcat (szPos, "\" \"");                        /* LÑnge: 3 Zeichen */
        strcat (szPos, szResName);
        strcat (szPos, "\"");                           /* LÑnge: 1 Zeichen */

        DebugE (-1, "User resource file, ExecPgm", szLine);
        DebugE (-1, "User resource file, Argument", szPos);

        DosExecPgm (szLine2,                            /* Object name buffer */
                    sizeof(szLine2),                    /* Length of object name buffer */
                    EXEC_SYNC,                          /* Execution flags */
                    szLine,                             /* Argument string */
                    NULL,                               /* Environment */
                    &pUser->rcSuccess,                  /* Termination codes */
                    szLine);                            /* Program file name */

        DebugULd (-1, "User resource file", "codeTerminate",
                 pUser->rcSuccess.codeTerminate);
        DebugULd (-1, "User resource file", "codeResult",
                 pUser->rcSuccess.codeResult);

// ???  #ifndef DEBUG
//      DosForceDelete (szResName);
//      #endif

        if (pUser->rcSuccess.codeTerminate || pUser->rcSuccess.codeResult)
            {
            pUser->ulErrorMsg = ERR_NOUSERINICREATED;
            DosExit (EXIT_THREAD, 0);
            }

        WinSetObjectData (WinQueryObject (szPrfName),
                          "NOMOVE=YES;NODELETE=YES;NORENAME=YES");
        }

    /* System Profile erstellen, falls nicht vorhanden */
    if (*(pUser->pszSystemResource) != '\0')
        {
        if (strlen (pUser->pszSystemResource) > CCHMAXPATH-1)
            {
            pUser->ulErrorMsg = ERR_PATHTOOLONG;
            DosExit (EXIT_THREAD, 0);
            }
        strcpy (szResName, pUser->pszSystemResource);

        lLen = strlen (pUser->pszMyPath) + strlen (pUser->pszSystemProfile) + 1;
        if (lLen+1 > CCHMAXPATH)
            {
            pUser->ulErrorMsg = ERR_PATHTOOLONG;
            DosExit (EXIT_THREAD, 0);
            }
        strcpy (szPrfName, pUser->pszMyPath);
        strcat (szPrfName, "\\");
        strcat (szPrfName, pUser->pszSystemProfile);

        if (!FileExist (szPrfName))
            {
            if (!FileExist (szResName))
                {
                pUser->ulErrorMsg = ERR_NOSYSTEMRESOURCE;
                DosExit (EXIT_THREAD, 0);
                }

            /* System Profile compilieren */
            lLen = CCHMAXRCLINE - strlen (MAKEINI)
                - 1 - 3 - 1 - strlen (szPrfName) - strlen (szResName);
            if (lLen < 3)
                {
                pUser->ulErrorMsg = ERR_PATHTOOLONG;
                DosExit (EXIT_THREAD, 0);
                }

            memset (szLine, '\0', sizeof (szLine));
            strcpy (szLine, MAKEINI);
            szLine[0] = (CHAR) (ulBootDrive - 1) + 'A';
            szPos = szLine + strlen (szLine) + 1;
            strcpy (szPos, "\"");                        /* LÑnge: 1 Zeichen */
            strcat (szPos, szPrfName);
            strcat (szPos, "\" \"");                     /* LÑnge: 3 Zeichen */
            strcat (szPos, szResName);
            strcat (szPos, "\"");                        /* LÑnge: 1 Zeichen */

            DebugE (-1, "System resource file, ExecPgm", szLine);
            DebugE (-1, "System resource file, Argument", szPos);

            DosExecPgm (szLine2,                /* Object name buffer */
                        sizeof(szLine2),        /* Length of object name buffer */
                        EXEC_SYNC,              /* Execution flags */
                        szLine,                 /* Argument string */
                        NULL,                   /* Environment */
                        &pUser->rcSuccess,      /* Termination codes */
                        szLine);                /* Program file name */

            DebugULd (-1, "System resource file", "codeTerminate",
                    pUser->rcSuccess.codeTerminate);
            DebugULd (-1, "System resource file", "codeResult",
                    pUser->rcSuccess.codeResult);

            if (pUser->rcSuccess.codeTerminate || pUser->rcSuccess.codeResult)
                {
                pUser->ulErrorMsg = ERR_NOSYSTEMINICREATED;
                DosExit (EXIT_THREAD, 0);
                }

            WinSetObjectData (WinQueryObject (szPrfName),
                              "NOMOVE=YES;NODELETE=YES;NORENAME=YES");
            }
        }
    pUser->ulErrorMsg = 0;
    return 0;
    }
