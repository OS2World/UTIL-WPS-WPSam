/***************************************************************************\
 *
 * PROGRAMMNAME:
 * -------------
 *
 * VERSION: 1.0
 * --------
 *
 * MODULNAME: DEBUG
 * ----------
 *
 * BESCHREIBUNG:
 * -------------
 *   Bereitstellung der Debugfunktionen
 *
 * HINWEISE:
 * ---------
 *   1. myReplacementForSOMOutChar ersetzt die Standard SOMOutCharRoutine
 *      des SOM-Debug-Interfaces. Es gibt einzelne Zeichen Åber PMPRINTF aus
 *   2. DgbDlgProc ist die Dialog-Prozedur zur Einstellung der
 *      SOM-Debug-Variablen und der ulDebugMask-Maske
 *
 * FUNKTIONEN:
 * -----------
 * myReplacementForSOMOutChar ()
 * DbgDlgProc ()
 *
 *  Ver.    Date      Comment
 *  ----    --------  -------
 *  1.00    10-23-94  First release
 *
 *  Copyright (C) Noller & Breining Software 1995
 *
\******************************************************************************/
#define INCL_WIN
#define INCL_DOS
#define INCL_GPITRANSFORMS

#include <OS2.H>
#include <stdlib.h>


#include "wpsam.ih"

#ifdef DEBUG
ULONG ulDebugMask = -1;
CHAR  szDebug[256];
#endif /* DEBUG */

/* Define your replacement routine */
int SOMLINK myReplacementForSOMOutChar (char c)
    {
#ifdef DEBUG
    pmprintf ("%c", c);
#endif /* DEBUG */

    return 1;
    }

#ifdef DEBUG
MRESULT EXPENTRY DbgDlgProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
    {
    static ULONG ulResult;
    CHAR szText[9];

    switch (msg)
        {
        case WM_INITDLG:
            ulResult = 0;
            WinSendDlgItemMsg (hwnd, DID_TL_0 + SOM_TraceLevel, BM_SETCHECK,
                MPFROMSHORT (1), 0);
            WinSendDlgItemMsg (hwnd, DID_WL_0 + SOM_WarnLevel, BM_SETCHECK,
                MPFROMSHORT (1), 0);
            WinSendDlgItemMsg (hwnd, DID_AL_0 + SOM_AssertLevel, BM_SETCHECK,
                MPFROMSHORT (1), 0);
            WinSendDlgItemMsg (hwnd, DID_DM, EM_SETTEXTLIMIT, MPFROMSHORT (8), MPVOID);
            WinSetDlgItemText (hwnd, DID_DM, _itoa (ulDebugMask, szText, 16));
            WinSendDlgItemMsg (hwnd, DID_DT, MLM_INSERT, MPFROMP (D_TEXT_MASK), MPVOID);
            break;

        case WM_CONTROL:
            switch (LONGFROMMP (mp1))
                {
                case DID_TL_0 + (BN_CLICKED<<16):
                case DID_TL_1 + (BN_CLICKED<<16):
                case DID_TL_2 + (BN_CLICKED<<16):
                    ulResult &= ~(3 << SHFT_TRACE);
                    ulResult += (SHORT1FROMMP (mp1) - DID_TL_0) << SHFT_TRACE;
                    break;

                case DID_WL_0 + (BN_CLICKED<<16):
                case DID_WL_1 + (BN_CLICKED<<16):
                case DID_WL_2 + (BN_CLICKED<<16):
                    ulResult &= ~(3 << SHFT_WARN);
                    ulResult += (SHORT1FROMMP (mp1) - DID_WL_0) << SHFT_WARN;
                    break;
                case DID_AL_0 + (BN_CLICKED<<16):
                case DID_AL_1 + (BN_CLICKED<<16):
                case DID_AL_2 + (BN_CLICKED<<16):
                    ulResult &= ~(3 << SHFT_ASSERT);
                    ulResult += (SHORT1FROMMP (mp1) - DID_AL_0) << SHFT_ASSERT;
                    break;

                case DID_DM + (EN_KILLFOCUS<<16):
                    WinQueryDlgItemText (hwnd, DID_DM, 9, szText);
                    ulDebugMask = strtoul (szText, NULL, 16);
                    break;
                }
            return 0;

        case WM_CLOSE:
            WinDismissDlg (hwnd, ulResult);
            return 0;
        }

    return WinDefDlgProc (hwnd, msg, mp1, mp2);
    }
#endif /* DEBUG */

