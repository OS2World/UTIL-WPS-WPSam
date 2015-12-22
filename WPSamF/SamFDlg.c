/*
 * 
 * 
 *   Module Name: SamFDlg
 * 
 *   OS/2 Workplace Shell Access Manager
 * 
 */


#include <string.h>
#include <stdlib.h>

#include "wpsamf.ih"
#include "wpsamf.ph"

extern HMODULE  hmod;



/**************************  ORDINARY CODE SECTION  ***************************
*****                                                                     *****
*****                  Any non-method code should go here.                *****
*****                                                                     *****
******************************************************************************/
#undef SOM_CurrentClass



MRESULT EXPENTRY CtxtDlgProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
    {
    static PWINDOWDATA pWindowData;
    static WPSamFData  *somThis;

    switch (msg)
        {
        case WM_INITDLG:
            pWindowData =
                (PWINDOWDATA) _wpAllocMem ((WPSamF *) mp2, sizeof(*pWindowData), NULL);

            if (pWindowData)
                {
                pWindowData->somSelf = (WPSamF *) mp2;
                somThis = WPSamFGetData(pWindowData->somSelf);
                }
            else
                {
                DebugE (-1, "AdjustDlgProc: WM_INITDLG",
                          "Couldn't allocate window words");
                break;
                }

            WinSetWindowPtr (hwnd, QWL_USER, pWindowData);

            WinCheckButton (hwnd, DID_SORTENABLE,     (_ulFlag & SORTENABLE)     ? 1 : 0);
            WinCheckButton (hwnd, DID_ARRANGEENABLE,  (_ulFlag & ARRANGEENABLE)  ? 1 : 0);
            WinCheckButton (hwnd, DID_SHUTDOWNENABLE, (_ulFlag & SHUTDOWNENABLE) ? 1 : 0);
            WinCheckButton (hwnd, DID_LOCKUPENABLE,   (_ulFlag & LOCKUPENABLE)   ? 1 : 0);
            WinCheckButton (hwnd, DID_ORIGDESKENABLE, (_ulFlag & ORIGDESKENABLE) ? 1 : 0);

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
                case DID_SORTENABLE:
                   if (WinSendDlgItemMsg(hwnd, DID_SORTENABLE, BM_QUERYCHECK, NULL, NULL))
                      _ulFlag |= SORTENABLE;

                   else
                      _ulFlag &= ~SORTENABLE;

                   _wpSaveDeferred(pWindowData->somSelf);
                   break;

                case DID_ARRANGEENABLE:
                   if (WinSendDlgItemMsg(hwnd, DID_ARRANGEENABLE, BM_QUERYCHECK, NULL, NULL))
                      _ulFlag |= ARRANGEENABLE;

                   else
                      _ulFlag &= ~ARRANGEENABLE;

                   _wpSaveDeferred(pWindowData->somSelf);
                   break;

                case DID_SHUTDOWNENABLE:
                   if (WinSendDlgItemMsg(hwnd, DID_SHUTDOWNENABLE, BM_QUERYCHECK, NULL, NULL))
                      _ulFlag |= SHUTDOWNENABLE;

                   else
                      _ulFlag &= ~SHUTDOWNENABLE;

                   _wpSaveDeferred(pWindowData->somSelf);
                   break;

                case DID_LOCKUPENABLE:
                   if (WinSendDlgItemMsg(hwnd, DID_LOCKUPENABLE, BM_QUERYCHECK, NULL, NULL))
                      _ulFlag |= LOCKUPENABLE;

                   else
                      _ulFlag &= ~LOCKUPENABLE;

                   _wpSaveDeferred(pWindowData->somSelf);
                   break;

                case DID_ORIGDESKENABLE:
                   if (WinSendDlgItemMsg(hwnd, DID_ORIGDESKENABLE, BM_QUERYCHECK, NULL, NULL))
                      _ulFlag |= ORIGDESKENABLE;

                   else
                      _ulFlag &= ~ORIGDESKENABLE;

                   _wpSaveDeferred(pWindowData->somSelf);
                   break;

            }

            return 0;

        }

    return WinDefDlgProc (hwnd, msg, mp1, mp2);
    }

