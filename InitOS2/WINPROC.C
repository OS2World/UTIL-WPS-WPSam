#define INCL_VIO
#define INCL_KBD
#define INCL_MOU
#define INCL_DOS
#include <os2.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "winman.h"

/* Definitionen fÅr Fenstersteuerung */
#define HORZ1   'ƒ'
#define VERT1   '≥'
#define OL1     '⁄'
#define OR1     'ø'
#define UL1     '¿'
#define UR1     'Ÿ'
#define OLT     '¥'
#define ORT     '√'

#define TXT_BTN1    "<"
#define TXT_BTN2    ">"

/* Variable fÅr Fenstersteuerung */
CHAR cHORZ1 = HORZ1;
CHAR cVERT1 = VERT1;
CHAR cOL1 = OL1;
CHAR cOR1 = OR1;
CHAR cUL1 = UL1;
CHAR cUR1 = UR1;
CHAR cOLT = OLT;
CHAR cORT = ORT;

extern BOOL flInsMode;                  /* Default: Insert ein */

/* Farblayout */
extern BYTE bNormal;                    /* Attribut fÅr normalen Bildschirm */
extern BYTE bDsktAttr;                  /* Attribut fÅr Bildschirmhintergrund */
extern BYTE bMainAttr;                  /* Attribut fÅr Hauptfenster */
extern BYTE bEntryAttr;                 /* Attribut fÅr Entry-Felder */
extern BYTE bATitleAttr;                /* Attribut fÅr aktiven Titlebar */
extern BYTE bNTitleAttr;                /* Attribut fÅr inaktiven Titlebar */
extern BYTE bMenuAttr;                  /* Attribut fÅr MenÅzeile */
extern BYTE bErrAttr;                   /* Attribut fÅr Fehlerbox */
extern BYTE bMsgAttr;                   /* Attribut fÅr Messageboxen */
extern BYTE bBtnAttr;                   /* Attribut fÅr Button */
extern BYTE bListBox;                   /* Attribut fÅr Listbox */
extern BYTE bSelected;                  /* Attribut fÅr selektierte Zeile */

extern VIOCURSORINFO CursorVisible;     /* Sichtbarer Cursor   */
extern VIOCURSORINFO CursorBigVisible;  /* Block-Cursor        */

/*****************************************************************************
 * Default Dialog-Procedure
 *****************************************************************************/
MPARAM EXPENTRY WMDefDlgProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
    {
    BYTE cell[2];
    USHORT i, usc;
    HWND  hwndChild, hwndNext;
    SIZL sizl;
    PDIALOG pD, pDChild;

    switch (msg)
        {
        case DM_CREATE:
            pD = (PDIALOG) mp1;
            pD->bCol    = bMainAttr;
            pD->bSelCol = bMainAttr;
            pD->ulcBuffer = pD->cx+1;
            pD->pszText = malloc ((size_t) pD->ulcBuffer);
            break;

        case DM_PAINT:
            WMGetWindowPoints (hwnd, &sizl);
            pD = WMGetDlgStruc (hwnd);
            if (pD->ulControlType != DC_DESKTOP)
                {
                /* Bildschirm mit Fensterfarbe fÅllen */
                cell[0] = ' ';
                cell[1] = pD->bCol;
                RegCls (pD, cell, sizl.x+1, sizl.y+1, sizl.cx-2, sizl.cy-2);

                /* Zeichnen des Rahmens */
                cell[1] = bATitleAttr;
                usc = min (sizl.cx-4, strlen (pD->pszText));
                if (usc > 0)
                    {
                    RegWrtCharStrAtt (pD, pD->pszText, usc,
                        sizl.y, sizl.x+(sizl.cx-usc)/2, &cell[1]);
                    cell[0] = cOLT;
                    RegWrtNCell (pD, cell, 1, sizl.y, sizl.x+(sizl.cx-usc)/2-1);
                    cell[0] = cORT;
                    RegWrtNCell (pD, cell, 1, sizl.y, sizl.x+(sizl.cx+usc)/2);
                    cell[0] = cHORZ1;
                    RegWrtNCell (pD, cell, (sizl.cx-usc)/2-2,
                        sizl.y, sizl.x+1);
                    RegWrtNCell (pD, cell, (sizl.cx-usc+1)/2-2, sizl.y,
                        sizl.x+(sizl.cx+usc)/2+1);      /* Anzahl: +1 wg. Rundungsfehler */
                    }
                else
                    {
                    cell[0] = cHORZ1;
                    RegWrtNCell (pD, cell, sizl.cx-1, sizl.y, sizl.x+1);
                    }
                cell[0] = cOL1;
                RegWrtNCell (pD, cell, 1, sizl.y, sizl.x);
                cell[0] = cOR1;
                RegWrtNCell (pD, cell, 1, sizl.y, sizl.x+sizl.cx-1);

                cell[0] = cHORZ1;
                cell[1] = pD->bCol;
                RegWrtNCell (pD, cell, sizl.cx-2, sizl.y+sizl.cy-1, sizl.x+1);
                cell[0] = cVERT1;
                for (i=1; i<sizl.cy-1; i++)
                    {
                    RegWrtNCell (pD, cell, 1, sizl.y+i, sizl.x+sizl.cx-1);
                    RegWrtNCell (pD, cell, 1, sizl.y+i, sizl.x);
                    }
                cell[0] = cUL1;
                RegWrtNCell (pD, cell, 1, sizl.y+sizl.cy-1, sizl.x);
                cell[0] = cUR1;
                RegWrtNCell (pD, cell, 1, sizl.y+sizl.cy-1, sizl.x+sizl.cx-1);
                }

            pD->bIsValid = TRUE;
            break;

        case DM_CHAR:
            /* Ist das Zeichen die ENTER-Taste? */
            pD = WMGetDlgStruc (hwnd);
            if (SHORT1FROMMP (mp1) & KC_VIRTUALKEY)
                {
                if (SHORT2FROMMP (mp2) == VK_TAB)           /* TAB */
                    {
                    WMSetFocus (WMEnumDlgItem (hwnd,
                        WMQueryFocus (HWND_DESKTOP), EDI_NEXTTABITEM));
                    return 0;
                    }
                else if (SHORT2FROMMP (mp2) == VK_BACKTAB)  /* Back TAB */
                    {
                    WMSetFocus (WMEnumDlgItem (hwnd,
                        WMQueryFocus (HWND_DESKTOP), EDI_PREVTABITEM));
                    return 0;
                    }
                else if (SHORT2FROMMP (mp2) == VK_ESC)
                    {
                    WMSendMsg (hwnd, DM_COMMAND, MPFROMSHORT (DID_CANCEL),
                        MPFROMSHORT (CMDSRC_PUSHBUTTON));
                    return 0;
                    }
                else if (SHORT2FROMMP (mp2) == VK_ENTER)    /* Enter */
                    {
                    hwndChild = WMEnumDlgItem (hwnd, NULLHANDLE, EDI_FIRSTTABITEM);
                    hwndNext = hwndChild;
                    do
                        {
                        pDChild = WMGetDlgStruc (hwndNext);
                        if ((pDChild->ulControlType==DC_BUTTON) &&
                            (pDChild->flStyle & BS_DEFAULT))
                            break;
                        hwndNext = WMEnumDlgItem (hwnd, hwndNext, EDI_NEXTTABITEM);
                        } while (hwndNext != hwndChild);
                    if ((pDChild->ulControlType==DC_BUTTON) &&
                        (pDChild->flStyle & BS_DEFAULT))
                        {
                        WMSendMsg (pDChild->hwnd, DM_CHAR, mp1, mp2);
                        return 0;
                        }
                    }
                }

            /* restliche Zeichen werden an den Parent geschickt, */
            /* wenn das aktuelle Fenster keine Dialogbox ist     */
            if ((hwnd != HWND_DESKTOP) &&
                (pD->ulControlType != DC_DIALOG) &&
                (pD->ulControlType < DC_USER))
                WMSendMsg (pD->hwndParent, DM_CHAR, mp1, mp2);
            break;

        case DM_COMMAND:
            WMDismissDlg (hwnd, SHORT1FROMMP (mp1));
            break;

        case DM_FOCUSCHANGE:
            /* Jedes Fenster in der z-Order neu zeichnen */
            WMGetWindowPoints (hwnd, &sizl);
            pD = WMGetDlgStruc (hwnd);
            if (IsVisible (GetIndex (pD)))
                {
                WMRefreshWindows ();
                /* Fenster bekommt Fokus => Cursor setzen */
                if (SHORT1FROMMP (mp2) == TRUE)
                    switch (pD->ulControlType)
                        {
                        case DC_BUTTON:
                            usc = min (sizl.cx-2, strlen (pD->pszText));
                            VioSetCurPos (sizl.y + sizl.cy/2,
                                sizl.x + (sizl.cx-usc)/2, 0);
                            VioSetCurType (&CursorVisible, 0);
                            break;

                        case DC_ENTRY:
                            VioSetCurPos (sizl.y, sizl.x + pD->lCursor, 0);
                            VioSetCurType (flInsMode ?
                                &CursorBigVisible : &CursorVisible, 0);
                            break;
                        }
                }
            break;

        case DM_CLOSE:
            WMDismissDlg (hwnd, DID_CANCEL);
            break;

        case DM_QUIT:
            WMSendMsg (hwnd, DM_DESTROY, 0, 0);
            break;

        case DM_DESTROY:
            pD = WMGetDlgStruc (hwnd);
            free (pD->pszText);
            break;
        }

    return 0;
    }

MPARAM EXPENTRY DesktopDlgProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
    {
    return WMDefDlgProc (hwnd, msg, mp1, mp2);
    }

/*****************************************************************************
 * Dialog-Procedure fÅr Static Windows
 *****************************************************************************/
MPARAM EXPENTRY StaticDlgProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
    {
    ULONG   i, ulc1, ulc2, ulLen;
    BOOL    bNewLine;
    BYTE    cell[2];
    PCHAR   pString;
    SIZL    sizl;
    PDIALOG pD;

    switch (msg)
        {
        case DM_CREATE:
            pD = (PDIALOG) mp1;
            if (pD->flStyle & DT_WORDBREAK)
                {
                pD->bCol    = bMainAttr;
                pD->bSelCol = bMainAttr;
                pD->ulcBuffer = (pD->cx * pD->cy) + 1;
                pD->pszText = malloc ((size_t) pD->ulcBuffer);
                return 0;
                }
            break;

        case DM_PAINT:
            WMGetWindowPoints (hwnd, &sizl);
            pD = WMGetDlgStruc (hwnd);

            /* Fensterbereich lîschen */
            cell[0] = ' ';
            cell[1] = pD->bCol;
            RegCls (pD, cell, sizl.x, sizl.y, sizl.cx, sizl.cy);

            /* Textstring ausgeben */
            pString = pD->pszText;
            ulLen = strlen (pString);
            if (pD->flStyle & DT_WORDBREAK)
                {
                /* Multiline-Control -> Zeilen ausgeben */
                for (i=0; i<pD->cy; i++)
                    {
                    ulc2 = ulc1 = 0;
                    bNewLine = FALSE;
                    do
                        {
                        ulc1 += strcspn (&pString[ulc1], BREAKCHAR);
                        if (ulc1 > pD->cx)
                            break;
                        ulc2 = ulc1;

                        bNewLine = ParseLine (&pString, &ulc1);
                        } while (ulc1<pD->cx && pString[ulc1]!='\0' && !bNewLine);
                    if ((ulc2 == 0) && (ulc1 != 0))
                        ulc2 = min (ulc1, pD->cx);
                    RegWrtCharStr (pD, pString, ulc2, sizl.y+i, sizl.x);
                    if (ulc1 != ulc2)
                        {
                        ParseLine (&pString, &ulc2);
                        ulc1 = ulc2;
                        }
                    pString += ulc1;
                    }
                }
                else
                    RegWrtCharStr (pD, pString, min (ulLen, pD->cx), sizl.y, sizl.x);

            pD->bIsValid = TRUE;
            return 0;

        case DM_CHAR:
            pD = WMGetDlgStruc (hwnd);
            WMSendMsg (pD->hwndParent, DM_CHAR, mp1, mp2);
            return 0;
        }
    return WMDefDlgProc (hwnd, msg, mp1, mp2);
    }

/*****************************************************************************
 * Dialog-Procedure fÅr Pushbuttons
 *****************************************************************************/
MPARAM EXPENTRY ButtonDlgProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
    {
    USHORT usc, usx;
    BYTE cell[2];
    SIZL sizl;
    PDIALOG pD;

    switch (msg)
        {
        case DM_PAINT:
            /* Button zeichnen */
            WMGetWindowPoints (hwnd, &sizl);
            pD = WMGetDlgStruc (hwnd);
            pD->bCol    = bBtnAttr;
            pD->bSelCol = bBtnAttr;

            /* Fensterbereich lîschen */
            cell[0] = ' ';
            cell[1] = (pD->flStyle&BS_DEFAULT) ? pD->bSelCol : pD->bCol;
            RegCls (pD, cell, sizl.x, sizl.y, sizl.cx, sizl.cy);

            usc = min (sizl.cx-2, strlen (pD->pszText));
            usx = sizl.x + (sizl.cx-2-usc)/2;
            RegWrtNChar (pD, TXT_BTN1, 1, sizl.y + sizl.cy/2, usx++);
            RegWrtCharStr (pD, pD->pszText, usc, sizl.y + sizl.cy/2, usx);
            usx += usc;
            RegWrtNChar (pD, TXT_BTN2, 1, sizl.y + sizl.cy/2, usx);

            /* Cursor setzen, falls Fokus */
            if (WMQueryFocus (HWND_DESKTOP) == hwnd)
                {
                VioSetCurPos (sizl.y + sizl.cy/2, sizl.x + (sizl.cx-usc)/2, 0);
                VioSetCurType (&CursorVisible, 0);
                }

            pD->bIsValid = TRUE;
            return 0;

        case DM_CHAR:
            /* Ist das Zeichen die ENTER-Taste? */
            pD = WMGetDlgStruc (hwnd);
            if (SHORT1FROMMP (mp1) & KC_VIRTUALKEY)
                if (SHORT2FROMMP (mp2) == VK_ENTER)
                    {
                    WMSendMsg (pD->hwndOwner, DM_COMMAND,
                        MPFROMLONG (pD->idControl),
                        (MPARAM) CMDSRC_PUSHBUTTON);
                    return 0;
                    }

            /* restliche Zeichen werden an den Parent geschickt */
            WMSendMsg (pD->hwndOwner, DM_CHAR, mp1, mp2);
            return 0;

        case DM_QUIT:
            WMSendMsg (hwnd, DM_DESTROY, 0, 0);
            return 0;

        case DM_DESTROY:
            free (pD->pszText);
            return 0;
        }

    return WMDefDlgProc (hwnd, msg, mp1, mp2);
    }

/*****************************************************************************
 * Dialog-Procedure fÅr Eingabefelder
 *****************************************************************************/
MPARAM EXPENTRY EntryDlgProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
    {
    BYTE cell[2];
    SIZL sizl;
    PCHAR pString;
    PDIALOG pD;

    switch (msg)
        {
        case DM_CREATE:
            pD = (PDIALOG) mp1;
            pD->lScroll = 0;
            pD->lCursor = 0;
            break;

        case DM_PAINT:
            /* Button zeichnen */
            WMGetWindowPoints (hwnd, &sizl);
            pD = WMGetDlgStruc (hwnd);
            pD->bCol    = bEntryAttr;
            pD->bSelCol = bEntryAttr;
            cell[0] = ' ';
            cell[1] = pD->bCol;
            RegWrtNCell (pD, cell, pD->cx, sizl.y, sizl.x);
            if (pD->flStyle & ES_UNREADABLE)
                RegWrtNChar (pD, "*",
                    min (strlen (pD->pszText) - pD->lScroll, pD->cx),
                    sizl.y, sizl.x);
            else
                RegWrtCharStr (pD, pD->pszText + pD->lScroll,
                    min (strlen (pD->pszText) - pD->lScroll, pD->cx),
                    sizl.y, sizl.x);

            /* Cursor setzen, falls Fokus */
            if (WMQueryFocus (HWND_DESKTOP) == hwnd)
                {
                VioSetCurPos (sizl.y, sizl.x + pD->lCursor, 0);
                VioSetCurType (flInsMode ? &CursorBigVisible : &CursorVisible, 0);
                }

            pD->bIsValid = TRUE;
            return 0;

        case DM_CHAR:
            pD = WMGetDlgStruc (hwnd);
            if (!(pD->flStyle & ES_READONLY))
                {
                if (SHORT1FROMMP (mp1) & KC_VIRTUALKEY)
                    {
                    /* Ist das Zeichen eine Cursortaste? */
                    switch (SHORT2FROMMP (mp2))
                        {
                        case VK_LEFT:
                            if (Left1Char (pD))
                                WMSendMsg (hwnd, DM_PAINT, 0, 0);
                            return 0;
                        case VK_RIGHT:
                            if (pD->lCursor<strlen (pD->pszText)-pD->lScroll)
                                {
                                /* Cursor ist noch nicht am Zeilenende */
                                if (Right1Char (pD))
                                    WMSendMsg (hwnd, DM_PAINT, 0, 0);
                                }
                            return 0;
                        case VK_BACKSPACE:
                            if (Left1Char (pD))
                                {
                                pString = pD->pszText+pD->lScroll+pD->lCursor;
                                memmove (pString, pString+1, strlen (pString+1) + 1);
                                WMSendMsg (hwnd, DM_PAINT, 0, 0);
                                }
                            return 0;
                        case VK_HOME:
                            pD->lCursor = 0;
                            pD->lScroll = 0;
                            WMSendMsg (hwnd, DM_PAINT, 0, 0);
                            return 0;
                        case VK_END:
                            pD->lCursor = 2*pD->cx/3;
                            if (pD->lCursor > strlen (pD->pszText))
                                pD->lCursor = strlen (pD->pszText);
                            pD->lScroll = strlen (pD->pszText) - pD->lCursor;
                            WMSendMsg (hwnd, DM_PAINT, 0, 0);
                            return 0;
                        case VK_DELETE:
                            pString = pD->pszText+pD->lScroll+pD->lCursor;
                            memmove (pString, pString+1, strlen (pString+1) + 1);
                            WMSendMsg (hwnd, DM_PAINT, 0, 0);
                            return 0;
                        case VK_INSERT:
                            flInsMode = !flInsMode;
                            WMSendMsg (hwnd, DM_PAINT, 0, 0);
                            return 0;
                        }
                    }
                else if (SHORT1FROMMP (mp1) & KC_CHAR)
                    {
                    if ((strlen (pD->pszText) < pD->ulcBuffer-1) ||
                        !flInsMode && (pD->lScroll+pD->lCursor<pD->ulcBuffer-1))
                        {
                        if (Right1Char (pD))
                            {
                            pString = pD->pszText+pD->lScroll+pD->lCursor;
                            if (flInsMode)
                                memmove (pString, pString-1, strlen (pString-1) + 1);
                            *(pString-1) = (CHAR) SHORT1FROMMP (mp2);
                            WMSendMsg (hwnd, DM_PAINT, 0, 0);

                            /* Owner von énderung verstÑndigen */
                            WMSendMsg (pD->hwndOwner, DM_CONTROL,
                                MPFROM2SHORT (pD->idControl, EN_CHANGE),
                                (MPARAM) pD->hwnd);
                            }
                        }
                    else
                        /* Fehler: Puffer voll */
                        DosBeep (1500, 100);
                    return 0;
                    }
                }

            /* restliche Zeichen werden an den Parent geschickt */
            WMSendMsg (pD->hwndParent, DM_CHAR, mp1, mp2);
            return 0;

        case EM_SETTEXTLIMIT:
            pD = WMGetDlgStruc (hwnd);
            pD->ulcBuffer = SHORT1FROMMP (mp1) + 1;
            pString = malloc ((size_t) pD->ulcBuffer);
            strncpy (pString, pD->pszText, pD->ulcBuffer);
            pString[pD->ulcBuffer] = '\0';
            pD->pszText = pString;
            pD->lScroll = 0;
            pD->lCursor = 0;
            return 0;

        case DM_QUIT:
            WMSendMsg (hwnd, DM_DESTROY, 0, 0);
            return 0;

        case DM_FOCUSCHANGE:
            pD = WMGetDlgStruc (hwnd);
            WMSendMsg (pD->hwndOwner,
                DM_CONTROL,
                MPFROM2SHORT (pD->idControl, (BOOL)(SHORT1FROMMP (mp2)) ?
                    EN_SETFOCUS : EN_KILLFOCUS),
                (MPARAM) pD->hwnd);
            break;

        case DM_DESTROY:
            free (pD->pszText);
            return 0;
        }

    return WMDefDlgProc (hwnd, msg, mp1, mp2);
    }

