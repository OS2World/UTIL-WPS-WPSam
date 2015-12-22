#define INCL_VIO
#define INCL_MOU
#define INCL_DOS
#include <os2.h>
#include <string.h>
#include <stdlib.h>
#include "winman.h"

extern PDIALOG pDialog;
extern ULONG   ulDCount;                   /* max. Fensterzahl */
extern ULONG   ulzOrder;                   /* hîchste z-Order */

extern VIOMODEINFO strucVioMode;
extern HMOU        hMou;

static NOPTRRECT PtrArea;

/* In einem REGION-Feld reprÑsentiert jedes Bit ein Zeichen eines Fensters */
/* Jeder '1' ist ein sichtbares Zeichen zugeordnet                         */
/* Jede '0' gehîrt entweder nicht zum Fenster oder ist verdeckt            */

/*******************************************************************
   Interne Funktion: PrÅft, ob v<=u<=w
   Eingang: u, v, w : Koordinatenwerte
   return:  TRUE : u liegt zwischen v und w
 *******************************************************************/
BOOL IsPart (ULONG u, ULONG v, ULONG w)
    {
    return (v<=u) && (u<=w);
    }

/* Allokiert einen REGION-Puffer */
PBYTE RegAllocate (void)
    {
    PBYTE pb;
    size_t szb;

    szb = ((strucVioMode.col+7)/8) * strucVioMode.row;
    pb = (PBYTE) malloc (szb);
    if (pb != NULL)
        memset (pb, '\0', szb);

    return pb;
    }

/* Deallokiert einen REGION-Puffer */
void RegFree (PBYTE pb)
    {
    free (pb);
    }

/* Setzt ein Bit im REGION-Puffer an der Position (x,y) */
void RegSet (PBYTE pb, ULONG x, ULONG y)
    {
    ULONG ulCX;

    ulCX = (strucVioMode.col+7)>>3;
    *(pb + x/8 + ulCX*y) |= 1<<(x&7);

    return;
    }

/* Lîscht ein Bit im REGION-Puffer an der Position (x,y) */
void RegReset (PBYTE pb, ULONG x, ULONG y)
    {
    ULONG ulCX;

    ulCX = (strucVioMode.col+7)>>3;
    *(pb + x/8 + ulCX*y) &= ~(1<<(x&7));

    return;
    }

/* öbergibt den Zustand des Bits im REGION-Puffer an der Position (x,y) */
BOOL RegTest (PBYTE pb, ULONG x, ULONG y)
    {
    ULONG ulCX;

    ulCX = (strucVioMode.col+7)>>3;

    return (*(pb + x/8 + ulCX*y) & (1<<(x&7))) ? TRUE : FALSE;
    }

/* Setzt die Bits im REGION-Puffer auf die Koordinatenwerte des Fensters */
void RegSetBits (PDIALOG pdlg)
    {
    ULONG x, y;
    SIZL  sizl;

    memset (pdlg->pRegion, '\0', (size_t)(((strucVioMode.col+7)/8) * strucVioMode.row));

    WMGetWindowPoints (pdlg->hwnd, &sizl);

    for (y=sizl.y; y<sizl.y+sizl.cy; y++)
        for (x=sizl.x; x<sizl.x+sizl.cx; x++)
            RegSet (pdlg->pRegion, x, y);

    return;
    }

/* Testet, ob das Fenster wDst an irgendeiner Stelle wSrc Åberlagert */
BOOL RegCorrelateWnd (PDIALOG wSrc, PDIALOG wDst)
    {
    SIZL sDst, sSrc;

    if ((!WMGetWindowPoints (wSrc->hwnd, &sSrc)) ||
        (!WMGetWindowPoints (wDst->hwnd, &sDst)))
        return FALSE;

    if (IsPart (sSrc.x, sDst.x, sDst.x+sDst.cx-1) ||
        IsPart (sSrc.x+sSrc.cx-1, sDst.x, sDst.x+sDst.cx-1) ||
        IsPart (sDst.x, sSrc.x, sSrc.x+sSrc.cx-1) ||
        IsPart (sDst.x+sDst.cx-1, sSrc.x, sSrc.x+sSrc.cx-1))
        if (IsPart (sSrc.y, sDst.y, sDst.y+sDst.cy-1) ||
            IsPart (sSrc.y+sSrc.cy-1, sDst.y, sDst.y+sDst.cy-1) ||
            IsPart (sDst.y, sSrc.y, sSrc.y+sSrc.cy-1) ||
            IsPart (sDst.y+sDst.cy-1, sSrc.y, sSrc.y+sSrc.cy-1))
            return TRUE;

    return FALSE;
    }

/* PrÅft alle Dialog-Strukturen, ob eine Region von einer neuen Åberlagert wird. */
BOOL RegAddRegion (PDIALOG pd)
    {
    ULONG i, j, x, y;
    SIZL sizl;

    if (!WMGetWindowPoints (pd->hwnd, &sizl))
        return FALSE;

    for (i=1; i<pd->ulzOrder; i++)
        for (j=0; j<ulDCount; j++)
            if (pDialog[j].ulzOrder == i)
                {
                for (y = sizl.y; y<sizl.y+sizl.cy; y++)
                    for (x = sizl.x; x<sizl.x+sizl.cx; x++)
                        RegReset (pDialog[j].pRegion, x, y);
                break;
                }

    return TRUE;
    }

/* PrÅft alle Dialog-Strukturen, ob Teile einer Region sichtbar werden */
BOOL RegRemoveRegion (PDIALOG pd)
    {
    ULONG i, j, x, y;
    SIZL sizl;

    if (!WMGetWindowPoints (pd->hwnd, &sizl))
        return FALSE;

    /* Alle Fenster ab pd->ulzOrder bis zOrder = 0 durchgehen */
    /* In allen Fenstern Åberlagerte Bereiche wieder als      */
    /* sichtbar markieren und in der Region-Kopie lîschen     */
    for (i=pd->ulzOrder-1; i>0; i--)
        for (j=0; j<ulDCount; j++)
            if (pDialog[j].ulzOrder == i)
                {
                if (RegCorrelateWnd (&pDialog[j], pd))
                    for (y = sizl.y; y<sizl.y+sizl.cy; y++)
                        for (x = sizl.x; x<sizl.x+sizl.cx; x++)
                            if (RegTest (pd->pRegion, x, y) &&
                                WMIsPart (&pDialog[j], x, y))
                                {
                                RegReset (pd->pRegion, x, y);
                                RegSet (pDialog[j].pRegion, x, y);
                                pDialog[j].bIsValid = FALSE;
                                }
                break;
                }

    return TRUE;
    }

/* Testet, ob ein Koordinatenwert Teil des Fensters ist */
BOOL WMIsPart (PDIALOG pd, ULONG x, ULONG y)
    {
    SIZL sizl;

    if (!WMGetWindowPoints (pd->hwnd, &sizl))
        return FALSE;

    if ((sizl.x<=x) && (sizl.x+sizl.cx>x) && (sizl.y<=y) && (sizl.y+sizl.cy>y))
        return TRUE;
    else
        return FALSE;
    }

//##################################################################
//  WM Koordinaten-Berechnung
//##################################################################
/*******************************************************************
>  Rechnet die Koordinaten bezogen auf hwndFrom um auf den
>  Koordinatenbereich von hwndTo
>  Eingang: hwndFrom : Fenster fÅr die umzurechnenden Koordinaten
>           hwndTo   : Zielfenster fÅr Koordinatenrechnung
>           pptl     : umzurechnende Koordinaten
>  Ausgang: pptl     : umgerechnete Koordinaten
>  return:  TRUE : Umrechnung erfolgreich
 *******************************************************************/
BOOL WMMapWindowPoint (HWND hwndFrom, HWND hwndTo, PPOINTL pptl)
    {
    ULONG   i;
    POINTL  ptl;
    HWND    hwndSearch;

    /* Existieren die Fenster Åberhaupt? */
    if ((WMGetDlgStruc (hwndFrom) == NULL) || (WMGetDlgStruc (hwndTo) == NULL))
        return FALSE;

    hwndSearch = hwndFrom;
    ptl.x = pptl->x;
    ptl.y = pptl->y;

    if (hwndFrom != hwndTo)
        do
            {
            for (i=0; i<ulDCount; i++)
                if (pDialog[i].hwnd == hwndSearch)
                    {
                    ptl.x += pDialog[i].x;
                    ptl.y += pDialog[i].y;
                    hwndSearch = pDialog[i].hwndParent;
                    break;
                    }
            } while ((hwndSearch != HWND_DESKTOP) && (hwndSearch != hwndTo));

    /* RÅckgabe des Ergebnisses */
    pptl->x = ptl.x;
    pptl->y = ptl.y;

    return TRUE;
    }

/*******************************************************************
>  Fenstergrî·e und -position bestimmen.
>  die sizl-Struktur enthÑlt:
>   - x, y : Fensterecke oben, links
>   - cx,cy: Fenstergrî·e. Aus dem Bildschirm ragende Teile werden
>            nicht mitgerechnet.
>  Eingang: hwnd  : zu bestimmendes Fenster
>  Ausgang: psizl : Ergebnis
>  return:  TRUE  : Bestimmung erfolgreich
 *******************************************************************/
BOOL WMGetWindowPoints (HWND hwnd, PSIZL psizl)
    {
    POINTL  ptl;
    PDIALOG pDlg;

    /* linke obere Ecke bezÅglich Desktop bestimmen */
    ptl.x = ptl.y = 0;
    if (!WMMapWindowPoint (hwnd, HWND_DESKTOP, &ptl))
        return FALSE;

    psizl->x = ptl.x;
    psizl->y = ptl.y;

    if ((pDlg = WMGetDlgStruc (hwnd)) == NULL)
        return FALSE;

    /* Fenstergrî·e cx, cy an die Bildschirmberandung anpassen */
    psizl->cx = (LONG)strucVioMode.col-ptl.x;
    if (psizl->cx > pDlg->cx)
        psizl->cx = pDlg->cx;
    psizl->cy = (LONG)strucVioMode.row-ptl.y;
    if (psizl->cy > pDlg->cy)
        psizl->cy = pDlg->cy;

    return TRUE;
    }

//##################################################################
//  VioWrt-Emulationen
//##################################################################
void TestStr (PDIALOG pdlg, USHORT usc, USHORT y, USHORT x, PUSHORT pusc1, PUSHORT pusc2)
    {
    USHORT i;

    for (i=0; i<usc; i++)
        if (!RegTest (pdlg->pRegion, x+i, y))
            break;
    *pusc1 = i;
    for (; i<usc; i++)
        if (RegTest (pdlg->pRegion, x+i, y))
            break;
    *pusc2 = i;
    return;
    }

void RegWrtCellStr (PDIALOG pdlg, PCH pch, USHORT usc, USHORT usRow, USHORT usColumn)
    {
    USHORT i, usc1, usc2;

    PtrArea.cRow =  PtrArea.row = usRow;
    PtrArea.cCol = (PtrArea.col = usColumn)+usc-1;
    MouRemovePtr (&PtrArea, hMou);
    i = 0;
    while (i<usc)
        {
        TestStr (pdlg, usc-i, usRow, usColumn+i, &usc1, &usc2);
        if (usc1>0)
            VioWrtCellStr (pch+(i*(1+strucVioMode.attrib)), usc1,
                usRow, usColumn+i, 0);
        i += usc2;
        }
    MouDrawPtr (hMou);
    return;
    }

void RegWrtCharStr (PDIALOG pdlg, PCH pch, USHORT usc, USHORT usRow, USHORT usColumn)
    {
    USHORT i, usc1, usc2;

    PtrArea.cRow =  PtrArea.row = usRow;
    PtrArea.cCol = (PtrArea.col = usColumn)+usc-1;
    MouRemovePtr (&PtrArea, hMou);
    i = 0;
    while (i<usc)
        {
        TestStr (pdlg, usc-i, usRow, usColumn+i, &usc1, &usc2);
        if (usc1>0)
            VioWrtCharStr (pch+i, usc1, usRow, usColumn+i, 0);
        i += usc2;
        }
    MouDrawPtr (hMou);
    return;
    }

void RegWrtCharStrAtt (PDIALOG pdlg, PCH pch, USHORT usc, USHORT usRow, USHORT usColumn, PBYTE pAttr)
    {
    USHORT i, usc1, usc2;

    PtrArea.cRow =  PtrArea.row = usRow;
    PtrArea.cCol = (PtrArea.col = usColumn)+usc-1;
    MouRemovePtr (&PtrArea, hMou);
    i = 0;
    while (i<usc)
        {
        TestStr (pdlg, usc-i, usRow, usColumn+i, &usc1, &usc2);
        if (usc1>0)
            VioWrtCharStrAtt (pch+i, usc1, usRow, usColumn+i, pAttr, 0);
        i += usc2;
        }
    MouDrawPtr (hMou);
    return;
    }

void RegWrtNCell (PDIALOG pdlg, PBYTE pCell, USHORT usc, USHORT usRow, USHORT usColumn)
    {
    USHORT i, usc1, usc2;

    PtrArea.cRow =  PtrArea.row = usRow;
    PtrArea.cCol = (PtrArea.col = usColumn)+usc-1;
    MouRemovePtr (&PtrArea, hMou);
    i = 0;
    while (i<usc)
        {
        TestStr (pdlg, usc-i, usRow, usColumn+i, &usc1, &usc2);
        if (usc1>0)
            VioWrtNCell (pCell, usc1, usRow, usColumn+i, 0);
        i += usc2;
        }
    MouDrawPtr (hMou);
    return;
    }

void RegWrtNChar (PDIALOG pdlg, PCH pch, USHORT usc, USHORT usRow, USHORT usColumn)
    {
    USHORT i, usc1, usc2;

    PtrArea.cRow =  PtrArea.row = usRow;
    PtrArea.cCol = (PtrArea.col = usColumn)+usc-1;
    MouRemovePtr (&PtrArea, hMou);
    i = 0;
    while (i<usc)
        {
        TestStr (pdlg, usc-i, usRow, usColumn+i, &usc1, &usc2);
        if (usc1>0)
            VioWrtNChar (pch, usc1, usRow, usColumn+i, 0);
        i += usc2;
        }
    MouDrawPtr (hMou);
    return;
    }

void RegCls (PDIALOG pdlg, PBYTE pCell, USHORT x, USHORT y, USHORT cx, USHORT cy)
    {
    USHORT i;

    for (i=y; i<y+cy; i++)
        RegWrtNCell (pdlg, pCell, cx, i, x);

    return;
    }

