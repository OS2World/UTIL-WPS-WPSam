/*
 * 
 * 
 *   Module Name: SamDlg
 * 
 *   OS/2 Workplace Shell Access Manager
 * 
 */


#include <string.h>
#include <stdlib.h>

#include "wpsam.ih"
#include "wpsam.ph"
#include "WPSamPan.h"

extern HMODULE  hmod;
extern HMODULE  hmodMag;        // Handle fr Desko-DLL

/* Modulinterne Funktionen */
APIRET APIENTRY thMagRead (PMAGNET pmagnet);

/******************************************************************************
*****                                                                     *****
*****   Code, der einen somThis-Pointer auf WPSam benutzt                 *****
*****                                                                     *****
******************************************************************************/
MRESULT EXPENTRY InitOS2DlgProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
    {
    static PWINDOWDATA pWindowData;
    static WPSamData  *somThis;

    switch (msg)
        {
        case WM_INITDLG:
            pWindowData =
                (PWINDOWDATA) _wpAllocMem ((WPSam *) mp2, sizeof(*pWindowData), NULL);

            if (pWindowData)
                {
                pWindowData->somSelf = (WPSam *) mp2;
                somThis = WPSamGetData(pWindowData->somSelf);
                }
            else
                {
                DebugE (-1, "InitOS2DlgProc: WM_INITDLG",
                          "Couldn't allocate window words");
                break;
                }

            WinSetWindowPtr (hwnd, QWL_USER, pWindowData);

            WinCheckButton (hwnd, DID_PROT_INMENU, (_ulFlag & ININITOS2MENU) ? 1 : 0 );

            WinSendDlgItemMsg (hwnd, (_ulFlag & OTHERPROTSHELL) ?
                                     DID_PROT_OTHER : DID_PROT_DEFAULT,
                               BM_SETCHECK, MPFROM2SHORT(TRUE, 0), NULL);

            WinSendDlgItemMsg(hwnd, DID_PROT_OTHER_ENTRY, EM_SETTEXTLIMIT,
                              MPFROMSHORT(CCHMAXPATH), 0);
            WinSetDlgItemText(hwnd, DID_PROT_OTHER_ENTRY, _szProtShell);

            if ( !(_ulFlag & ININITOS2MENU) )
               {
               WinEnableControl(hwnd, DID_PROT_DEFAULT,     FALSE);
               WinEnableControl(hwnd, DID_PROT_OTHER,       FALSE);
               WinEnableControl(hwnd, DID_PROT_OTHER_ENTRY, FALSE);
               }

            return (MRESULT) FALSE;

        case WM_COMMAND:
            switch (COMMANDMSG(&msg)->cmd)
                {
                case DID_DEFAULT:
                   DebugE (-1, "InitOS2DlgProc: WM_COMMAND", "DEFAULT");
                   break;

                case DID_HELP:
                   break;
                }
            return 0;

        case WM_CONTROL:
            switch (SHORT1FROMMP(mp1))
            {
                case DID_PROT_DEFAULT:
                   if (SHORT2FROMMP(mp1) == BN_CLICKED) {
                      _ulFlag &= ~OTHERPROTSHELL;
                      _wpSaveDeferred(pWindowData->somSelf);
                   }
                   break;

                case DID_PROT_OTHER:
                   if (SHORT2FROMMP(mp1) == BN_CLICKED) {
                      _ulFlag |= OTHERPROTSHELL;
                      _wpSaveDeferred(pWindowData->somSelf);
                   }
                   break;

                case DID_PROT_INMENU:
                   if (WinSendDlgItemMsg(hwnd, DID_PROT_INMENU, BM_QUERYCHECK, NULL, NULL))
                      {
                      _ulFlag |= ININITOS2MENU;
                      WinEnableControl(hwnd, DID_PROT_DEFAULT,     TRUE);
                      WinEnableControl(hwnd, DID_PROT_OTHER,       TRUE);
                      WinEnableControl(hwnd, DID_PROT_OTHER_ENTRY, TRUE);
                      }

                   else
                      {
                      _ulFlag &= ~ININITOS2MENU;
                      WinEnableControl(hwnd, DID_PROT_DEFAULT,     FALSE);
                      WinEnableControl(hwnd, DID_PROT_OTHER,       FALSE);
                      WinEnableControl(hwnd, DID_PROT_OTHER_ENTRY, FALSE);
                      }

                      _wpSaveDeferred(pWindowData->somSelf);
                   break;
            }
            break;

        case WM_DESTROY:
           WinSendMsg (hwnd, WM_CLOSE, 0L, 0L);
           return 0;

        case WM_CLOSE:
           DebugS (-1, "InitOS2DlgProc: WM_CLOSE");

           {
              ULONG ulLen;

              ulLen = (ULONG) WinQueryDlgItemTextLength (hwnd, DID_PROT_OTHER_ENTRY);
              _wpFreeMem( pWindowData->somSelf, _szProtShell);
              _szProtShell = _wpAllocMem( pWindowData->somSelf, ulLen+1, NULL );
              WinQueryDlgItemText (hwnd, DID_PROT_OTHER_ENTRY,   ulLen+1, _szProtShell);
           }

           _wpSaveDeferred(pWindowData->somSelf);
           _wpFreeMem (pWindowData->somSelf, (UCHAR*) pWindowData);
           break;
        }

    return WinDefDlgProc (hwnd, msg, mp1, mp2);
    }



MRESULT EXPENTRY CreateDlgProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
    {
    static PWINDOWDATA pWindowData;
    static WPSamData  *somThis;

    switch (msg)
        {
        case WM_INITDLG:
            pWindowData =
                (PWINDOWDATA) _wpAllocMem ((WPSam *) mp2, sizeof(*pWindowData), NULL);

            if (pWindowData)
                {
                pWindowData->somSelf = (WPSam *) mp2;
                somThis = WPSamGetData(pWindowData->somSelf);
                }
            else
                {
                DebugE (-1, "CreateDlgProc: WM_INITDLG",
                          "Couldn't allocate window words");
                break;
                }

            WinSetWindowPtr (hwnd, QWL_USER, pWindowData);

            WinSendDlgItemMsg (hwnd, (_ulFlag & PRIVATESYS) ? DID_PRIVATESYS : DID_GLOBALSYS,
                               BM_SETCHECK, MPFROM2SHORT(TRUE, 0), NULL);

            WinSendDlgItemMsg(hwnd, DID_USERRESOURCE, EM_SETTEXTLIMIT,
                              MPFROMSHORT(CCHMAXPATH), 0);
            WinSetDlgItemText(hwnd, DID_USERRESOURCE, _szUserResource);

            WinSendDlgItemMsg(hwnd, DID_USERPROFILE, EM_SETTEXTLIMIT,
                              MPFROMSHORT(CCHMAXPATH), 0);
            WinSetDlgItemText(hwnd, DID_USERPROFILE, _szUserProfile);

            WinSendDlgItemMsg(hwnd, DID_SYSTEMRESOURCE, EM_SETTEXTLIMIT,
                              MPFROMSHORT(CCHMAXPATH), 0);
            WinSetDlgItemText(hwnd, DID_SYSTEMRESOURCE, _szSystemResource);

            WinSendDlgItemMsg(hwnd, DID_SYSTEMPROFILE, EM_SETTEXTLIMIT,
                              MPFROMSHORT(CCHMAXPATH), 0);
            WinSetDlgItemText(hwnd, DID_SYSTEMPROFILE, _szSystemProfile);

            if ( !(_ulFlag & PRIVATESYS) )
               {
               WinEnableControl(hwnd, DID_SYSTEMRESOURCE, FALSE);
               WinEnableControl(hwnd, DID_SYSTEMPROFILE,  FALSE);
               }

            if ( _ProfilesCreated(pWindowData->somSelf) )
                {
                WinSendDlgItemMsg (hwnd, DID_USERPROFILE, EM_SETREADONLY,
                                   MPFROMSHORT (TRUE), 0);
                WinSendDlgItemMsg (hwnd, DID_SYSTEMPROFILE, EM_SETREADONLY,
                                   MPFROMSHORT (TRUE), 0);

                WinEnableControl(hwnd, DID_USERRESOURCE,   FALSE);
                WinEnableControl(hwnd, DID_SYSTEMRESOURCE, FALSE);
                WinEnableControl(hwnd, DID_PRIVATESYS,     FALSE);
                WinEnableControl(hwnd, DID_GLOBALSYS,      FALSE);
                }

            return (MRESULT) FALSE;

        case WM_COMMAND:
            switch (COMMANDMSG(&msg)->cmd)
                {
                case DID_DEFAULT:
                   DebugE (-1, "CreateDlgProc: WM_COMMAND", "DEFAULT");
                   break;

                case DID_HELP:
                   break;
                }
            return 0;

        case WM_CONTROL:
            switch (SHORT1FROMMP(mp1))
            {
                case DID_GLOBALSYS:
                   DebugE (-1, "ProfilesDlgProc: WM_CONTROL", "DID_GLOBALSYS");

                   if (SHORT2FROMMP(mp1) == BN_CLICKED) {
                      _ulFlag &= ~PRIVATESYS;
                      WinEnableControl(hwnd, DID_SYSTEMRESOURCE, FALSE);
                      WinEnableControl(hwnd, DID_SYSTEMPROFILE,  FALSE);
                      _RefreshStyle(pWindowData->somSelf);
                   }
                   break;

                case DID_PRIVATESYS:
                   DebugE (-1, "ProfilesDlgProc: WM_CONTROL", "DID_PRIVATESYS");

                   if (SHORT2FROMMP(mp1) == BN_CLICKED) {
                      _ulFlag |= PRIVATESYS;
                      WinEnableControl(hwnd, DID_SYSTEMRESOURCE, TRUE);
                      WinEnableControl(hwnd, DID_SYSTEMPROFILE,  TRUE);
                      _RefreshStyle(pWindowData->somSelf);
                   }
                   break;
            }
            break;

        case WM_DESTROY:
           WinSendMsg (hwnd, WM_CLOSE, 0L, 0L);
           return 0;

        case WM_CLOSE:
           DebugE (-1, "CreateDlgProc: WM_CLOSE", "");

           {
              ULONG ulLen;

              ulLen = (ULONG) WinQueryDlgItemTextLength (hwnd, DID_USERRESOURCE);
              _wpFreeMem( pWindowData->somSelf, _szUserResource);
              _szUserResource = _wpAllocMem( pWindowData->somSelf, ulLen+1, NULL );
              WinQueryDlgItemText (hwnd, DID_USERRESOURCE,   ulLen+1, _szUserResource);

              ulLen = (ULONG) WinQueryDlgItemTextLength (hwnd, DID_USERPROFILE);
              _wpFreeMem( pWindowData->somSelf, _szUserProfile);
              _szUserProfile = _wpAllocMem( pWindowData->somSelf, ulLen+1, NULL );
              WinQueryDlgItemText (hwnd, DID_USERPROFILE,    ulLen+1, _szUserProfile);

              ulLen = (ULONG) WinQueryDlgItemTextLength (hwnd, DID_SYSTEMRESOURCE);
              _wpFreeMem( pWindowData->somSelf, _szSystemResource);
              _szSystemResource = _wpAllocMem( pWindowData->somSelf, ulLen+1, NULL );
              WinQueryDlgItemText (hwnd, DID_SYSTEMRESOURCE, ulLen+1, _szSystemResource);

              ulLen = (ULONG) WinQueryDlgItemTextLength (hwnd, DID_SYSTEMPROFILE);
              _wpFreeMem( pWindowData->somSelf, _szSystemProfile);
              _szSystemProfile = _wpAllocMem( pWindowData->somSelf, ulLen+1, NULL );
              WinQueryDlgItemText (hwnd, DID_SYSTEMPROFILE,  ulLen+1, _szSystemProfile);
           }

           _RefreshStyle(pWindowData->somSelf);
           _wpFreeMem (pWindowData->somSelf, (UCHAR*) pWindowData);
           break;
        }

    return WinDefDlgProc (hwnd, msg, mp1, mp2);
    }



MRESULT EXPENTRY AdjustDlgProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
    {
    static PWINDOWDATA pWindowData;
    static WPSamData  *somThis;

    switch (msg)
        {
        case WM_INITDLG:
            pWindowData =
                (PWINDOWDATA) _wpAllocMem ((WPSam *) mp2, sizeof(*pWindowData), NULL);

            if (pWindowData)
                {
                pWindowData->somSelf = (WPSam *) mp2;
                somThis = WPSamGetData(pWindowData->somSelf);
                }
            else
                {
                DebugE (-1, "AdjustDlgProc: WM_INITDLG",
                          "Couldn't allocate window words");
                break;
                }

            WinSetWindowPtr (hwnd, QWL_USER, pWindowData);

            WinCheckButton (hwnd, DID_RENENABLE,  (_ulFlag & RENENABLE)  ? 1 : 0 );
            WinCheckButton (hwnd, DID_MOVEENABLE, (_ulFlag & MOVEENABLE) ? 1 : 0 );
            WinCheckButton (hwnd, DID_DROPALLOW,  (_ulFlag & DROPALLOW)  ? 1 : 0 );
            WinCheckButton (hwnd, DID_CLOSEAPPL,  (_ulFlag & CLOSEAPPL)  ? 1 : 0 );

            WinSetDlgItemText (hwnd, DID_ENVIRONMENT, _szEnvironment);

            return (MRESULT) FALSE;

        case WM_COMMAND:
            switch (COMMANDMSG(&msg)->cmd)
                {
                case DID_DEFAULT:
                   DebugE (-1, "AdjustDlgProc: WM_COMMAND", "DEFAULT");
                   break;

                case DID_HELP:
                   break;

                }
            return 0;

        case WM_CONTROL:
            switch (SHORT1FROMMP(mp1))
            {
                case DID_RENENABLE:
                   DebugE (-1, "AdjustDlgProc: WM_CONTROL", "DID_RENENABLE");

                   if (WinSendDlgItemMsg(hwnd, DID_RENENABLE, BM_QUERYCHECK, NULL, NULL))
                      _ulFlag |= RENENABLE;
                   else
                      _ulFlag &= ~RENENABLE;

                   _RefreshStyle(pWindowData->somSelf);
                   break;

                case DID_MOVEENABLE:
                   DebugE (-1, "AdjustDlgProc: WM_CONTROL", "DID_MOVEENABLE");

                   if (WinSendDlgItemMsg(hwnd, DID_MOVEENABLE, BM_QUERYCHECK, NULL, NULL))
                      _ulFlag |= MOVEENABLE;
                   else
                      _ulFlag &= ~MOVEENABLE;

                   _RefreshStyle(pWindowData->somSelf);
                   break;

                case DID_DROPALLOW:
                   DebugE (-1, "AdjustDlgProc: WM_CONTROL", "DID_DROPALLOW");

                   if (WinSendDlgItemMsg(hwnd, DID_DROPALLOW, BM_QUERYCHECK, NULL, NULL))
                      _ulFlag |= DROPALLOW;
                   else
                      _ulFlag &= ~DROPALLOW;

                   _RefreshStyle(pWindowData->somSelf);
                   break;

                case DID_CLOSEAPPL:
                   DebugE (-1, "AdjustDlgProc: WM_CONTROL", "DID_CLOSEAPPL");

                   if (WinSendDlgItemMsg(hwnd, DID_CLOSEAPPL, BM_QUERYCHECK, NULL, NULL))
                      _ulFlag |= CLOSEAPPL;
                   else
                      _ulFlag &= ~CLOSEAPPL;

                   _RefreshStyle(pWindowData->somSelf);
                   break;
            }
            break;

        case WM_DESTROY:
           WinSendMsg (hwnd, WM_CLOSE, 0L, 0L);
           return 0;

        case WM_CLOSE:
           DebugS (-1, "AdjustDlgProc: WM_CLOSE");

           {
              ULONG ulLen;

              ulLen = (ULONG) WinQueryDlgItemTextLength (hwnd, DID_ENVIRONMENT);
              _wpFreeMem( pWindowData->somSelf, _szEnvironment);
              _szEnvironment = _wpAllocMem( pWindowData->somSelf, ulLen+1, NULL );

              WinQueryDlgItemText (hwnd, DID_ENVIRONMENT, ulLen+1, _szEnvironment);
           }
           _RefreshStyle(pWindowData->somSelf);
           _wpFreeMem (pWindowData->somSelf, (UCHAR*) pWindowData);
           break;
        }

    return WinDefDlgProc (hwnd, msg, mp1, mp2);
    }



MRESULT EXPENTRY PwdNewDlgProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
    {
    static PWINDOWDATA pWindowData;
    static WPSamData   *somThis;
    static MAGNET      magnet;
    static CHAR        szReadData[CCHMAXPASSWORD];

    switch (msg)
        {
        case WM_INITDLG:
            pWindowData =
                (PWINDOWDATA) _wpAllocMem ((WPSam *) mp2, sizeof(*pWindowData), NULL);

            if (pWindowData)
                {
                pWindowData->somSelf = (WPSam *) mp2;
                somThis = WPSamGetData(pWindowData->somSelf);
                }
            else
                {
                DebugE (-1, "PwdNewDlgProc: WM_INITDLG",
                          "Couldn't allocate window words");
                break;
                }

            WinSetWindowPtr (hwnd, QWL_USER, pWindowData);

            WinCheckButton (hwnd, DID_PWDENABLE, (_ulFlag & PWDENABLE) ? 1 : 0 );
            WinSendDlgItemMsg(hwnd, DID_PWDOLD, EM_SETTEXTLIMIT,
                      MPFROMSHORT(CCHMAXPASSWORD), 0);
            WinSendDlgItemMsg(hwnd, DID_PWDNEW, EM_SETTEXTLIMIT,
                      MPFROMSHORT(CCHMAXPASSWORD), 0);
            WinSendDlgItemMsg(hwnd, DID_PWDVER, EM_SETTEXTLIMIT,
                      MPFROMSHORT(CCHMAXPASSWORD), 0);

            return (MRESULT) FALSE;

        case WM_ACTIVATE:
            DebugE (-1, "PwdNewDlgProc: WM_ACTIVATE",
                      SHORT1FROMMP(mp1) ? "TRUE" : "FALSE");

            switch (SHORT1FROMMP(mp1))
                {
                case TRUE:
                   {
                   if (MCOpen())
                      {
                      WinShowWindow (WinWindowFromID (hwnd, DID_PWDOLDTXTMITKARTE), TRUE);
                      WinShowWindow (WinWindowFromID (hwnd, DID_PWDNEWTXTMITKARTE), TRUE);
                      WinShowWindow (WinWindowFromID (hwnd, DID_PWDOLDTXTOHNEKARTE), FALSE);
                      WinShowWindow (WinWindowFromID (hwnd, DID_PWDNEWTXTOHNEKARTE), FALSE);
                      }
                   else
                      {
                      WinShowWindow (WinWindowFromID (hwnd, DID_PWDOLDTXTMITKARTE), FALSE);
                      WinShowWindow (WinWindowFromID (hwnd, DID_PWDNEWTXTMITKARTE), FALSE);
                      WinShowWindow (WinWindowFromID (hwnd, DID_PWDOLDTXTOHNEKARTE), TRUE);
                      WinShowWindow (WinWindowFromID (hwnd, DID_PWDNEWTXTOHNEKARTE), TRUE);
                      }
                   break;
                   }

                case FALSE:
                   {
                   MCClose();
                   break;
                   }
                }
            break;

        case WM_COMMAND:
            switch (COMMANDMSG(&msg)->cmd)
                {
                case DID_OK:
                {
                   CHAR        szPwdOld[CCHMAXPASSWORD];
                   CHAR        szPwdNew[CCHMAXPASSWORD];
                   CHAR        szPwdVer[CCHMAXPASSWORD];

                   DebugE (-1, "PwdNewDlgProc: WM_COMMAND", "OK");
                   WinQueryDlgItemText (hwnd, DID_PWDOLD, sizeof(szPwdOld), szPwdOld);
                   WinQueryDlgItemText (hwnd, DID_PWDNEW, sizeof(szPwdNew), szPwdNew);
                   WinQueryDlgItemText (hwnd, DID_PWDVER, sizeof(szPwdVer), szPwdVer);

                   ChiffreSZ(szPwdOld);
                   ChiffreSZ(szPwdNew);
                   ChiffreSZ(szPwdVer);

                   DebugE (-1, "szPwdOld:", szPwdOld);
                   DebugE (-1, "_szPassword:", _szPassword);

                   if ( strcmp(szPwdOld, _szPassword) != 0 )
                   {
                      WinAlarm (HWND_DESKTOP, WA_WARNING);
                      MessageBox (IDS_WRONGPWDOLD, MBTITLE_ERROR, MB_ENTER | MB_ICONHAND);
                      break;
                   }

                   if ( strcmp(szPwdNew, szPwdVer) != 0 )
                   {
                      WinAlarm (HWND_DESKTOP, WA_WARNING);
                      MessageBox (IDS_WRONGPWDVER, MBTITLE_ERROR, MB_ENTER | MB_WARNING);
                      break;
                   }

                   _wpFreeMem( pWindowData->somSelf, _szPassword);
                   _szPassword = _wpAllocMem(pWindowData->somSelf,
                                             strlen(szPwdNew)+1, NULL);
                   strcpy(_szPassword, szPwdNew);

                   if ( _wpSaveImmediate(pWindowData->somSelf) )
                   {
                      WinAlarm (HWND_DESKTOP, WA_NOTE);
                      WinSetDlgItemText (hwnd, DID_PWDOLD, "");
                      WinSetDlgItemText (hwnd, DID_PWDNEW, "");
                      WinSetDlgItemText (hwnd, DID_PWDVER, "");
                   }
                   break;
               }

               case DID_HELP:
                   DebugE (-1, "PwdNewDlgProc: WM_COMMAND", "HELP");
                   break;
            }
            return 0;

        case WM_CONTROL:
            switch (SHORT1FROMMP(mp1))
                {
                case DID_PWDENABLE:
                   DebugE (-1, "PwdNewDlgProc: WM_CONTROL", "DID_PWDENABLE");

                   if (WinSendDlgItemMsg(hwnd, DID_PWDENABLE, BM_QUERYCHECK, NULL, NULL))
                      _ulFlag |= PWDENABLE;

                   else
                      _ulFlag &= ~PWDENABLE;

                   _RefreshStyle(pWindowData->somSelf);
                   break;

                case DID_PWDOLD:
                case DID_PWDNEW:
                case DID_PWDVER:
                   switch (SHORT2FROMMP(mp1))
                      {
                      case EN_SETFOCUS:
                         DebugS (-1, "PwdNewDlgProc: EN_SETFOCUS");

                         magnet.usDID         = SHORT1FROMMP(mp1);
                         magnet.hwnd          = hwnd;
                         magnet.pszReadData   = szReadData;

                         MCStart(&magnet);
                         break;

                      case EN_KILLFOCUS:
                         DebugS (-1, "PwdNewDlgProc: EN_KILLFOCUS");

                         MCAbort();
                         break;
                      }
                }
            return 0;

        case WM_DESTROY:
           WinSendMsg (hwnd, WM_CLOSE, 0L, 0L);
           return 0;

        case WM_CLOSE:
            _wpFreeMem (pWindowData->somSelf, (UCHAR*) pWindowData);
            break;

        case WM_USER:
           WinSetDlgItemText (hwnd, magnet.usDID, magnet.pszReadData);

           WinSetFocus (HWND_DESKTOP,
                        WinEnumDlgItem (hwnd, WinQueryFocus (HWND_DESKTOP),            
                        EDI_NEXTTABITEM));

           return 0;
        }

    return WinDefDlgProc (hwnd, msg, mp1, mp2);
    }


MRESULT EXPENTRY PwdDlgProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
    {
    static PWINDOWDATA pWindowData;
    static HELPINIT    hini;
    static HWND        hwndHelp;
    static WPSamData   *somThis;
    static MAGNET      magnet;
    static CHAR        szReadData[CCHMAXPASSWORD];

    switch (msg)
        {
        case WM_INITDLG:
            DebugE (-1, "PwdDlgProc", "Init");

            pWindowData =
                (PWINDOWDATA) _wpAllocMem ((WPSam *) mp2, sizeof(*pWindowData), NULL);

            if (pWindowData)
                {
                pWindowData->somSelf = (WPSam *) mp2;
                somThis = WPSamGetData(pWindowData->somSelf);
                }
            else
                {
                DebugE (-1, "PwdDlgProc: WM_INITDLG",
                         "Couldn't allocate window words");
                break;
                }

            WinSetWindowPtr (hwnd, QWL_USER, pWindowData);

            WinSendDlgItemMsg(hwnd, DID_PWD, EM_SETTEXTLIMIT,
                      MPFROMSHORT(CCHMAXPASSWORD), 0);

            /*  Hilfe initialisieren */
            hini.cb = sizeof (HELPINIT);
            hini.ulReturnCode = 0;
            hini.pszTutorialName = NULL;
            hini.phtHelpTable = (PHELPTABLE)MAKEULONG (IDH_MAIN, 0xFFFF);
            hini.hmodHelpTableModule = NULLHANDLE;
            hini.hmodAccelActionBarModule = NULLHANDLE;
            hini.idAccelTable = 0;
            hini.idActionBar = 0;
            hini.fShowPanelId = CMIC_HIDE_PANEL_ID;
            hini.pszHelpWindowTitle = "WPSam";   // ???
            hini.pszHelpLibraryName = HELPFILE;
            hwndHelp =
                WinCreateHelpInstance (WinQueryAnchorBlock (HWND_DESKTOP), &hini);
            if (hwndHelp)
                WinAssociateHelpInstance (hwndHelp, hwnd);
            return (MRESULT) FALSE;

        case WM_ACTIVATE:
            DebugE (-1, "PwdDlgProc: WM_ACTIVATE",
                      SHORT1FROMMP(mp1) ? "TRUE" : "FALSE");

            switch (SHORT1FROMMP(mp1))
                {
                case TRUE:
                   {
                   if (MCOpen())
                      {
                      WinShowWindow (WinWindowFromID (hwnd, DID_PWDTXTMITKARTE), TRUE);
                      WinShowWindow (WinWindowFromID (hwnd, DID_PWDTXTOHNEKARTE), FALSE);
                      }
                   else
                      {
                      WinShowWindow (WinWindowFromID (hwnd, DID_PWDTXTMITKARTE), FALSE);
                      WinShowWindow (WinWindowFromID (hwnd, DID_PWDTXTOHNEKARTE), TRUE);
                      }
                   break;
                   }

                case FALSE:
                   {
                   MCClose();
                   break;
                   }
                }
            break;

/*----- Window Message WM_HELP */
        case WM_HELP:
            if (hwndHelp)
                {
                WinSendMsg (hwndHelp, HM_DISPLAY_HELP,
                    MPFROM2SHORT (PANEL_PWDIN, 0), MPFROMSHORT (HM_RESOURCEID));
                return 0;
                }
            break;

        case WM_COMMAND:
            switch (COMMANDMSG(&msg)->cmd)
                {
                case DID_OK:
                    {
                    CHAR szPwd[CCHMAXPASSWORD];

                    WinQueryDlgItemText (hwnd, DID_PWD, sizeof(szPwd), szPwd);
                    DebugE (-1, "PwdDlgProc", szPwd);
                    ChiffreSZ(szPwd);

                    if ( strcmp(szPwd, _szPassword) != 0 )
                        {
                        _MyHistory(pWindowData->somSelf, IDS_WRONGPASSWORD);
                        _ulWrongPwdCount++;
                        _wpSaveDeferred (pWindowData->somSelf);
                        WinAlarm (HWND_DESKTOP, WA_WARNING);
                        MessageBox (IDS_WRONGPWD, MBTITLE_ERROR, MB_ENTER | MB_ICONHAND);

                        WinDismissDlg(hwnd, 0);
                        }
                    else
                        WinDismissDlg(hwnd, 1);

                    break;
                    }

                case DID_CANCEL:
                    DebugE (-1, "PwdDlgProc", "Cancel");
                    WinDismissDlg(hwnd, 0);
                    break;
                }
            return (MRESULT) 0;

        case WM_CONTROL:
            switch (SHORT1FROMMP(mp1))
                {
                case DID_PWD:
                   switch (SHORT2FROMMP(mp1))
                      {
                      case EN_SETFOCUS:
                         DebugS (-1, "PwdDlgProc: EN_SETFOCUS");

                         magnet.usDID         = SHORT1FROMMP(mp1);
                         magnet.hwnd          = hwnd;
                         magnet.pszReadData   = szReadData;

                         MCStart(&magnet);
                         break;

                      case EN_KILLFOCUS:
                         DebugS (-1, "PwdDlgProc: EN_KILLFOCUS");

                         MCAbort();            
                         break;
                      }
                }
            return 0;

        case WM_DESTROY:
            /* Hilfe beenden */
            WinAssociateHelpInstance (NULLHANDLE, hwnd);
            WinDestroyHelpInstance (hwndHelp);
            break;

        case WM_USER:
           WinSetDlgItemText (hwnd, magnet.usDID, magnet.pszReadData);
           WinPostMsg(hwnd, WM_COMMAND, (MPARAM) DID_OK, 0);
           break;
        }
    return WinDefDlgProc (hwnd, msg, mp1, mp2);
    }


void _Optlink thMCRead (void *pmagnet)
{
   while (MCRead ((PMAGNET) pmagnet))
      {
      DebugE (-1, "thMagRead", ((PMAGNET)pmagnet)->pszReadData);
      WinPostMsg(((PMAGNET)pmagnet)->hwnd, WM_USER, 0, 0);
      }

      return;
}



/******************************************************************************
*****                                                                     *****
*****   Code, der einen somThis-Pointer auf M_WPSam benutzt               *****
*****                                                                     *****
******************************************************************************/
#undef SOM_CurrentClass
#define SOM_CurrentClass SOMMeta

MRESULT EXPENTRY AboutDlgProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
    {
    USHORT usSerial;
    static HELPINIT    hini;
    static HWND        hwndHelp;
    static ULONG       ulRegState;
    static PWINDOWDATA pWindowData;
    static M_WPSamData *somThis;

    switch (msg)
        {
        case WM_INITDLG:
            DosAllocMem ((PPVOID)&pWindowData, sizeof (*pWindowData),
                         PAG_COMMIT | PAG_READ | PAG_WRITE);

            if (pWindowData)
                {
                pWindowData->somSelf = (M_WPSam *) mp2;
                somThis = M_WPSamGetData(pWindowData->somSelf);
                }
            else
                {
                DebugE (-1, "CreateDlgProc: WM_INITDLG",
                          "Couldn't allocate window words");
                break;
                }

            WinSetWindowPtr (hwnd, QWL_USER, pWindowData);

            WinSendDlgItemMsg (hwnd, DID_USERNAME, EM_SETTEXTLIMIT,
                MPFROMSHORT (CCHMAXREGUSER), 0);
            WinSendDlgItemMsg (hwnd, DID_PASSWORD, EM_SETTEXTLIMIT,
                MPFROMSHORT (CCHMAXREGPWD), 0);

            ulRegState =  RegCheck (_szRegUser, _szRegPassword, &usSerial,
                          WPSam_MajorVersion, WPSam_MinorVersion);
            if (ulRegState != REGCHECK_OK)
                {
                /* Version ist noch nicht registriert */
                WinShowWindow (WinWindowFromID (hwnd, DID_PWDTXT),   TRUE);
                WinShowWindow (WinWindowFromID (hwnd, DID_PASSWORD), TRUE);
                }
            else
                {
                /* Version ist bereits registriert */
                WinSendDlgItemMsg (hwnd, DID_USERNAME, EM_SETREADONLY,
                    MPFROMSHORT (TRUE), 0);
                WinSetDlgItemText (hwnd, DID_USERNAME, _szRegUser);
                }

            /*  Hilfe initialisieren */
            hini.cb = sizeof (HELPINIT);
            hini.ulReturnCode = 0;
            hini.pszTutorialName = NULL;
            hini.phtHelpTable = (PHELPTABLE)MAKEULONG (IDH_MAIN, 0xFFFF);
            hini.hmodHelpTableModule = NULLHANDLE;
            hini.hmodAccelActionBarModule = NULLHANDLE;
            hini.idAccelTable = 0;
            hini.idActionBar = 0;
            hini.fShowPanelId = CMIC_HIDE_PANEL_ID;
            hini.pszHelpWindowTitle = "WPSam";   // ???
            hini.pszHelpLibraryName = HELPFILE;
            hwndHelp =
                WinCreateHelpInstance (WinQueryAnchorBlock (HWND_DESKTOP), &hini);
            if (hwndHelp)
                WinAssociateHelpInstance (hwndHelp, hwnd);
            return (MRESULT) FALSE;

/*----- Window Message WM_HELP */
        case WM_HELP:
            if (hwndHelp)
                {
                WinSendMsg (hwndHelp, HM_DISPLAY_HELP,
                    MPFROM2SHORT (PANEL_ABOUT, 0), MPFROMSHORT (HM_RESOURCEID));
                return 0;
                }
            break;

        case WM_DESTROY:
            /* Hilfe beenden */
            WinAssociateHelpInstance (NULLHANDLE, hwnd);
            WinDestroyHelpInstance (hwndHelp);
            break;

        case WM_CLOSE:
            if (ulRegState != REGCHECK_OK)
                {
                WinQueryDlgItemText (hwnd, DID_USERNAME, CCHMAXREGUSER, _szRegUser);
                WinQueryDlgItemText (hwnd, DID_PASSWORD, CCHMAXREGPWD, _szRegPassword);
                switch (RegCheck (_szRegUser, _szRegPassword, &usSerial,
                    WPSam_MajorVersion, WPSam_MinorVersion))
                    {
                    /* Registrier-Kennwort falsch eingegeben */
                    case REGCHECK_FAILED:
                        WinAlarm (HWND_DESKTOP, WA_NOTE);
                        MessageBox (IDS_WRONGPWD, 0, MB_CANCEL | MB_ERROR);
                        *_szRegUser     = '\0';
                        *_szRegPassword = '\0';
                        break;

                    /* Registrier-Kennwort abgelaufen */
                    case REGCHECK_WV:
                        WinAlarm (HWND_DESKTOP, WA_NOTE);
                        MessageBox (IDS_WRONGVER,IDS_INFORMATION, MB_CANCEL | MB_INFORMATION);
                        *_szRegUser     = '\0';
                        *_szRegPassword = '\0';
                        break;
                    }
                }
            DosFreeMem (pWindowData);
            break;
        }

    return WinDefDlgProc (hwnd, msg, mp1, mp2);
    }
