/* noch nicht implementiert: */
/*  - HWND_BOTTOM/TOP        */
/*  - PresParm, CreateParm   */
/*  - NULL-Pointer fÅr Window-Text */

#define INCL_VIO
#define INCL_KBD
#define INCL_MOU
#define INCL_DOS
#include <os2.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "winman.h"

/* Farblayout */
BYTE bNormal    = 0x07;                 /* Attribut fÅr normalen Bildschirm */
BYTE bDsktAttr  = 0x00;                 /* Attribut fÅr Bildschirmhintergrund */
BYTE bMainAttr  = 0x1E;                 /* Attribut fÅr Hauptfenster */
BYTE bEntryAttr = 0x70;                 /* Attribut fÅr Entry-Felder */
BYTE bATitleAttr= 0x2E;                 /* Attribut fÅr aktiven Titlebar */
BYTE bNTitleAttr= 0x71;                 /* Attribut fÅr inaktiven Titlebar */
BYTE bMenuAttr  = 0x70;                 /* Attribut fÅr MenÅzeile */
BYTE bErrAttr   = 0x4E;                 /* Attribut fÅr Fehlerbox */
BYTE bMsgAttr   = 0x70;                 /* Attribut fÅr Messageboxen */
BYTE bBtnAttr   = 0x70;                 /* Attribut fÅr Button */
BYTE bListBox   = 0x1E;                 /* Attribut fÅr Listbox */
BYTE bSelected  = 0x70;                 /* Attribut fÅr selektierte Zeile */

/* Variable zur Steuerung der Message-Queue */
PQUMSG pQueue = NULL;
PQUMSG pReadMsg;                        /* NÑchste Message lesen */
PQUMSG pWriteMsg;                       /* NÑchste Message schreiben */
ULONG  ulcQueue;                        /* Messages in Queue */
ULONG  ulQSize;                         /* Grî·e der Queue */
HEV    hev;

/* Variable fÅr den Window-Manager */
PDIALOG pDialog = NULL;
ULONG   ulDCount;                   /* max. Fensterzahl */
ULONG   ulzOrder;                   /* hîchste z-Order */

/* maximale Zahl verschiedener Window-Klassen in einer Applikation */
/* Diese Zahl sollte grî·er sein als die Zahl der verschiedenen */
/* Dialog-Elemente DC_* */
#define MAXCLASSES      20

/* Struktur, in der die Window-Procedures der einzelnen */
/* Window-Klassen abgelegt sind */
typedef struct _WNDPROC
    {
    ULONG ulControlType;
    PFNWP fnControl;
    } WNDPROC;

WNDPROC WndProc[MAXCLASSES];
ULONG   ulNumClasses;

/* Struktur fÅr WMBeginEnumWindows, WMGetNextWindow */
typedef struct _WNDENUM
    {
    HWND hwnd;
    ULONG ulzOrder;
    } WNDENUM;
typedef WNDENUM *PWNDENUM;

/* Variable zur Steuerung von VIO und KBD */
#define STACKLEN        16384           /* StacklÑnge fÅr Threads */

#define BACKSP      0x08                /* Backspace */
#define TAB         0x09                /* Tabulator-Zeichen */
#define CR          0x0D                /* Carriage-Return */
#define ESC         0x1B                /* Escape */

TID tidKbd;
TID tidMou;

VIOMODEINFO strucVioMode;
KBDKEYINFO  key;
HMTX        hmtxKbd;
BOOL        flInsMode = TRUE;           /* Default: Insert ein */

BYTE cell[] = {' ', 0};

VIOCURSORINFO CursorOriginal;                           /* Originalcursor      */
VIOCURSORINFO CursorVisible    = {-90, -100, 1,  1};    /* Sichtbarer Cursor   */
VIOCURSORINFO CursorBigVisible = {-0,  -100, 1,  1};    /* Block-Cursor        */
VIOCURSORINFO CursorInvisible  = {-90, -100, 1, -1};    /* unsichtbarer Cursor */

/* Variable fÅr das Mausinterface */
HMOU    hMou = NULLHANDLE;

/* Definitionen fÅr die Timerservices */
typedef struct _TMTHREAD
    {
    HWND   hwnd;
    TID    tid;
    HEV    hSem;
    HTIMER hTimer;
    ULONG  idTimer;
    ULONG  dtTimeout;
    } TMTHREAD;
typedef TMTHREAD *PTMTHREAD;

TMTHREAD arTimers [MAXTIMER];

/* private Funktionsprototypen */
void _Optlink KBDThread (void *);
void _Optlink TMRThread (void *);
void _Optlink MOUThread (void *);

/*******************************************************************
   Bewegt den Cursor in einem Entry-Feld um ein Zeichen nach links
   Es werden Endepositionen und Scroll-VorgÑnge behandelt.
   Eingang: pD          : Zeiger auf Dialog-Struktur des Entry-Feldes
   Ausgang: pD->lScroll : neue Scroll-Position
            pD->lCursor : neue Cursor-Position
   return:  FALSE       : Zeile kann nicht mehr gescrollt werden
 *******************************************************************/
BOOL Left1Char (PDIALOG pD)
    {
    BOOL bRC = TRUE;

    if (pD->lCursor == 0)
        {
        /* Cursor am Zeilenanfang */
        if (pD->lScroll != 0)
            {
            /* Zeile kann noch gescrollt werden */
            pD->lScroll = max (pD->lScroll - pD->cx/3, 0);
            pD->lCursor += pD->cx/3 - 1;
            }
        else
            /* Zeile kann nicht mehr gescrollt werden */
            bRC = FALSE;
        }
    else
        /* Cursor ist noch nicht am Zeilenanfang */
        pD->lCursor -= 1;

    return bRC;
    }

/*******************************************************************
   Bewegt den Cursor in einem Entry-Feld um ein Zeichen nach rechts
   Es werden Endepositionen und Scroll-VorgÑnge behandelt.
   Eingang: pD          : Zeiger auf Dialog-Struktur des Entry-Feldes
   Ausgang: pD->lScroll : neue Scroll-Position
            pD->lCursor : neue Cursor-Position
   return:  FALSE       : Zeile kann nicht mehr gescrollt werden
 *******************************************************************/
BOOL Right1Char (PDIALOG pD)
    {
    BOOL bRC = TRUE;

    if (pD->lCursor>=pD->cx-1)
        {
        /* Cursor ist am Zeilenende */
        if (pD->flStyle & ES_AUTOSCROLL)
            {
            /* Control-Style ist AUTOSCROLL */
            pD->lScroll += pD->cx/3;
            pD->lCursor = max (pD->lCursor - pD->cx/3 + 1, 0);
            }
        else
            /* Control-Style ist nicht AUTOSCROLL */
            bRC = FALSE;
        }
    else
        pD->lCursor += 1;

    return bRC;
    }

/*******************************************************************
   Dient zur Bestimmung der MaximallÑnge der Zeilen in einem
   Message-String. ??? (funktioniert TAB?)
   Eingang: ppString : Zeiger auf Zeiger auf Quellstring
            pulc1    : Zeiger auf aktuelle Zeichenposition fÅr Untersuchung
   Ausgang: pulc1    : Zeiger auf nÑchste Zeichenposition
   return:  TRUE     : CR oder LF erkannt.
 *******************************************************************/
BOOL ParseLine (PCHAR *ppString, PULONG pulc1)
    {
    BOOL bRC = FALSE;

    switch ((*ppString)[*pulc1])
        {
        case ' ':
            (*pulc1)++;
            break;
        case '\t':
            *pulc1 += 8 - (*pulc1%8);
            break;
        case '\n':
            if (*pulc1==1)
                {
                (*ppString)++;  /* Es ging '\r' voraus */
                break;          /*  => ignorieren */
                }
        case '\r':
                (*pulc1)++;
                bRC = TRUE;
                break;
        case '\0':
                break;
        }
    return bRC;
    }

/*******************************************************************
   Untersucht, ob hwndDlg Parent von hwnd ist
   Eingang: hwndDlg : Window-Handle des ersten Fensters
            hwnd    : Window-Handle des zweiten Fensters
   return:  TRUE    : hwndDlg ist Parent von hwnd
 *******************************************************************/
BOOL IsParent (HWND hwndDlg, HWND hwnd)
    {
    PDIALOG pDlg1, pDlg2;

    pDlg1 = WMGetDlgStruc (hwndDlg);
    pDlg2 = WMGetDlgStruc (hwnd);

    /* rÅckwÑrts die neue Fokuskette aufbauen bis zum Desktop-Fenster */
    if ((pDlg1 != NULL) || (pDlg2 != NULL))
        {
        while (pDlg2->hwnd != HWND_DESKTOP)
            {
            if (pDlg2->hwndParent == pDlg1->hwnd)
                return TRUE;
            pDlg2 = WMGetDlgStruc (pDlg2->hwndParent);
            }
        }

    return FALSE;
    }

/*******************************************************************
   Bestimmt zu einem Dialog-Zeiger den Index ulDialogIndex.
   Eingang: pD    : Zeiger auf Dialog-Struktur
   return:  -1    : Index nicht gefunden
            sonst : Index in pDialog
 *******************************************************************/
ULONG GetIndex (PDIALOG pD)
    {
    ULONG i;

    for (i=0; i<ulDCount; i++)
        if (&pDialog[i] == pD)
            return i;
    return (ULONG)-1;
    }

/*******************************************************************
   Untersucht, ob das Fenster mit dem Index ulDialogIndex sichtbar ist.
   Dazu mÅssen auch alle Parent-Fenster sichtbar sein.
   Eingang: ulDialogIndex : Index des Fensters in pDialog
   return:  TRUE          : Fenster ist sichtbar
 *******************************************************************/
BOOL IsVisible (ULONG ulDialogIndex)
    {
    ULONG i;
    PDIALOG pD;

    if (ulDialogIndex >= ulDCount)
        return FALSE;

    i = ulDialogIndex;
    pD = &pDialog[i];
    while ((pD->hwndParent!=HWND_DESKTOP) && (pD->hwndParent!=0))
        {
        if (pD->flVisible == 0)
            break;
        for (i=0; i<ulDCount; i++)
            if (pDialog[i].hwnd == pD->hwndParent)
                break;
        pD = &pDialog[i];
        }

    return (pD->flVisible == 1);
    }

/*******************************************************************
   Starten des Tastatur-Threads. ErlÑuterung zur Tastatursemaphore
   siehe LinkKeyboard.
   return:  TRUE : Tastaturthread erfolgreich gestartet
 *******************************************************************/
BOOL InitKeyboard (void)
    {
    BOOL rc;

    /* Mutex-Semaphore wird 'unowned' erzeugt          */
    /* Damit erfolgt automatisch ein Link der Tastatur */
    rc = (DosCreateMutexSem (NULL, &hmtxKbd, 0, 0) == 0);

    /* Start des Tastatur-Threads */
    if (rc)
        rc = ((tidKbd = _beginthread (KBDThread, NULL, STACKLEN, NULL)) != -1);

    return rc;
    }

/*******************************************************************
>  Keyboard-Link: Wenn die Tastatur gelinkt ist, wartet der Tastaturthread
>  auf Zeichen von der Tastatur und postet sie in die Applikations-
>  Message-Queue. Wird beispielsweise mit DosExecPgm eine andere Applikation
>  gestartet, mu· mit UnlinkKeyboard dieser Link aufgehoben werden, da sonst
>  die neue Applikation keinen Zugang zur Tastatur bekommt.
>  return:  TRUE : Link erfolgreich
 *******************************************************************/
BOOL LinkKeyboard (void)
    {
    return (DosReleaseMutexSem (hmtxKbd) == 0);
    }

/*******************************************************************
>  Unlink der Tastatur. ErlÑuterung siehe LinkKeyboard.
>  return:  TRUE : Unlink erfolgreich
 *******************************************************************/
APIRET UnlinkKeyboard (ULONG ulTimeout)
    {
    return DosRequestMutexSem (hmtxKbd, ulTimeout);
    }

/*******************************************************************
   Starten des Maus-Threads.
   return:  TRUE : Thread erfolgreich gestartet
 *******************************************************************/
BOOL InitMouse (void)
    {
HFILE DevHandle;
ULONG ParmLengthInOut, DataLengthInOut;
BYTE data[10];
USHORT dpf[5];
APIRET fRet;

FILE *ptr;
USHORT rc, t1, t2, rx[4], sn[4];
ULONG ulAction;
VIOCONFIGINFO vcfg;
ptr = fopen ("c:\\mouse", "w");

    fRet = DosOpen ("POINTER$", &DevHandle, &ulAction, 0, 0, 1, 0x40, NULL);
    fprintf (ptr, "Open POINTER$: %d\n", fRet);

    ParmLengthInOut = 0;
    DataLengthInOut = 8;
    fRet = DosDevIOCtl (DevHandle, 3, 0x72, NULL, 0, &ParmLengthInOut,
        rx, 8, &DataLengthInOut);
    fprintf (ptr, "Ret: %d\n", fRet);
    fprintf (ptr, "Ret: %.4x\n", rx[0]);
    fprintf (ptr, "Sel: %.4x\n", rx[2]);
    fprintf (ptr, "Off: %.4x\n", rx[1]);
    fprintf (ptr, "Dat: %.4x\n", rx[3]);

    fRet = DosOpen ("MOUSE$", &DevHandle, &ulAction, 0, 0, 1, 0x40, NULL);
    fprintf (ptr, "Open MOUSE$: %d\n", fRet);

    dpf[0] = 0;
    sn[0] = 8;
    sn[1] = 16;
    sn[2] = 0;
    sn[3] = 1;
    DataLengthInOut = 1;
    ParmLengthInOut = 8;
    fRet = DosDevIOCtl (DevHandle, 11, 0x41, sn, 8, &ParmLengthInOut,
        dpf, 1, &DataLengthInOut);
    fprintf (ptr, "ScreenControl: %d\n", fRet);

    dpf[0]=rx[1];
    dpf[1]=rx[2];
    dpf[2]=rx[3];
    dpf[3]=0;
    rx[0] = 6;
    rx[1] = 0;
    rx[2] = 2;
    ParmLengthInOut = 8;
    DataLengthInOut = 6;
    fRet = DosDevIOCtl (DevHandle, 7, 0x5A, dpf, 8, &ParmLengthInOut,
        rx, 6, &DataLengthInOut);
    fprintf (ptr, "MOUSE$: %d\n", fRet);

    t2 = 0x100;
    ParmLengthInOut = 2;
    DataLengthInOut = 0;
    fRet = DosDevIOCtl (DevHandle, 7, 0x5C, &t2, 2, &ParmLengthInOut,
        NULL, 0, &DataLengthInOut);
    fprintf (ptr, "IOCTL MOUSE$: Set Status: %d\n", fRet);
    memset (dpf, '\0', 10);
    for (t1 = 0; t1 < 10; t1++)
        {
        ParmLengthInOut = 0;
        DataLengthInOut = 4;
        fRet=DosDevIOCtl (DevHandle, 7, 0x67, NULL, 0, &ParmLengthInOut,
            dpf, 10, &DataLengthInOut);
        fprintf (ptr, "Pos: %.4x %.4x", dpf[0], dpf[1]);

        t2 = 0;
        ParmLengthInOut = 2;
        DataLengthInOut = 10;
        fRet=DosDevIOCtl (DevHandle, 7, 0x63, &t2, 2, &ParmLengthInOut,
            dpf, 10, &DataLengthInOut);
        fprintf (ptr, "-> %.4x, %.4x, %.4x, %.4x, %.4x | RET: %d\n",
            dpf[0], dpf[1], dpf[2], dpf[3], dpf[4], fRet);
        DosBeep (0x25, 1000);
        }

    /* Mausinterface initialisieren */
//  if (rc = MouOpen ("POINTER$", &hMou))    //NULL, &hMou))
//      {
//      fprintf (ptr, "MouOpen: %d\n", rc);
//      hMou = NULLHANDLE;              /* Fehler beim ôffnen der Maus */
//      }
//  DosBeep (300, 200);
//  rc = MouDrawPtr (hMou);
//  fprintf (ptr, "MouDrawPtr: %d\n", rc);
    fclose (ptr);

    /* Start des Maus-Threads */
    if (hMou)
        tidMou = _beginthread (MOUThread, NULL, STACKLEN, NULL);

    return (hMou != NULLHANDLE);
    }

//##################################################################
//  WM Initialisierung und Terminierung
//##################################################################

/*******************************************************************
>  Initialisierung des Window-Managers. Es werden folgende Aktionen
>  durchgefÅhrt:
>  - aktuellen Cursor sichern
>  - Bildschirmhintergrund vorbereiten
>  - Registrieren der Window-Klassen
>  - Tastatur initialisieren
>  Eingang: ulCtlCount : Maximalzahl der Controls+Windows
>  return:  TRUE       : Initialize erfolgreich
 *******************************************************************/
BOOL WMInitialize (ULONG ulCtlCount)
    {
    /* Eine zweite Initialisierung darf nicht durchgefÅhrt werden */
    if (pDialog != NULL)
        return FALSE;

    /* aktuellen Cursor sichern, unsichtbaren Cursor erzeugen */
    VioGetCurType (&CursorOriginal, 0);
    VioSetCurType (&CursorInvisible, 0);

    /* Bildschirmhintergrund vorbereiten */
    cell[1] = bDsktAttr;
    strucVioMode.cb = sizeof (VIOMODEINFO);
    VioGetMode (&strucVioMode, 0);
    VioScrollUp (0, 0, -1, -1, -1, cell, 0);

    /* Allokieren der Window-Struktur */
    DosAllocMem ((PPVOID) &pDialog, ulCtlCount*sizeof (DIALOG),
        PAG_COMMIT | PAG_READ | PAG_WRITE);
    memset (pDialog, 0, ulCtlCount*sizeof (DIALOG));

    /* ulDCount enthÑlt die maximale Zahl der Controls */
    ulDCount = ulCtlCount;
    ulzOrder = 1;

    /* Initialisierung der Window-Klassen */
    ulNumClasses = 0;
    WMRegisterClass (DC_DESKTOP, DesktopDlgProc);   /* Desktop */
    WMRegisterClass (DC_BUTTON,  ButtonDlgProc);    /* Button */
    WMRegisterClass (DC_STATIC,  StaticDlgProc);    /* Static Window */
    WMRegisterClass (DC_ENTRY,   EntryDlgProc);     /* Eingabefeld */

    /* DIALOG-Struktur fÅr Desktop vorbereiten */
    pDialog[0].ulControlType    = DC_DESKTOP;
    pDialog[0].idControl        = 0;
    pDialog[0].ulzOrder         = 1;
    pDialog[0].hwnd             = HWND_DESKTOP;
    pDialog[0].hwndOwner        = 0;
    pDialog[0].hwndParent       = 0;
    pDialog[0].hwndFocus        = 0;
    pDialog[0].pszText          = "";
    pDialog[0].x                = 0;
    pDialog[0].y                = 0;
    pDialog[0].cx               = strucVioMode.col;
    pDialog[0].cy               = strucVioMode.row;
    pDialog[0].bCol             = bDsktAttr;
    pDialog[0].bSelCol          = bDsktAttr;
    pDialog[0].flVisible        = 1;
    pDialog[0].flStyle          = 0;
    pDialog[0].bIsValid         = TRUE;
    pDialog[0].pRegion          = RegAllocate ();

    RegSetBits (&pDialog[0]);

    /* Start des Mouse-Threads */
//  InitMouse ();  // Maus aus

    /* Start des Keyboard-Threads */
    return InitKeyboard ();
    }

/*******************************************************************
>  Window-Manager entfernen. Es werden folgende Aktionen
>  durchgefÅhrt:
>  - Cursor restaurieren
>  - Bildschirmhintergrund lîschen
>  - Tastaturthread stoppen
>  return:  TRUE
 *******************************************************************/
BOOL WMTerminate ()
    {
    ULONG i, ulcWin;

    /* Bildschirmhintergrund vorbereiten */
    cell[1] = bNormal;
    strucVioMode.cb = sizeof (VIOMODEINFO);
    VioGetMode (&strucVioMode, 0);
    VioScrollUp (0, 0, -1, -1, -1, cell, 0);

    /* Cursor restaurieren */
    VioSetCurType (&CursorOriginal, 0);

    /* Window-Struktur freigeben */
    ulcWin = ulDCount;
    ulDCount = 0;                           /* keine neuen Fenster mehr ab hier */
    for (i=0; i<ulcWin; i++)
        if (pDialog[i].hwnd)
            RegFree (pDialog[i].pRegion);   /* alle Regions freigeben */
    DosFreeMem ((PVOID) pDialog);           /* Window-Struktur freigeben */

    /* Tastaturthread entfernen */
    DosKillThread (tidKbd);
    return TRUE;
    }

//##################################################################
//  WM Class Manager
//##################################################################

/*******************************************************************
>  Bestimmt eine neue Klassen-Id fÅr eine neue Instanz. Gehîrt
>  diese bereits zu einer existierenden Klasse, wird deren ID
>  Åbergeben, sonst eine neue registriert.
>  Eingang: pfnDlgProc : Zeiger auf die Dialog-Procedure
>  return:  ulClassId
 *******************************************************************/
ULONG WMGetClass (PFNWP pfnDlgProc)
    {
    ULONG i, ulClassId;

    /* Klasse bereits registriert? */
    for (i=0; i<ulNumClasses; i++)
        if (WndProc[i].fnControl == pfnDlgProc)
            {
            ulClassId = WndProc[i].ulControlType;
            break;
            }

    /* ... nein => registrieren */
    if (i == ulNumClasses)
        {
        ulClassId = WMGetNewClass ();
        if (ulClassId == 0xFFFF)
            return ulClassId;

        WMRegisterClass (ulClassId, pfnDlgProc);
        }

    return ulClassId;
    }

/*******************************************************************
>  Registriert eine neue Klasse
>  Eingang: ulControlType : ControlId (entspr. ClassId)
>           pfnDlgProc    : Dialog-Procedure fÅr diese Klasse
>  return:  TRUE : erfolgreich registriert
 *******************************************************************/
BOOL WMRegisterClass (ULONG ulControlType, PFNWP pfnDlgProc)
    {
    /* Ist noch Platz fÅr eine neue Klasse? */
    if (ulNumClasses == MAXCLASSES)
        return FALSE;

    /* Registrieren der neuen Klasse */
    WndProc[ulNumClasses].ulControlType = ulControlType;
    WndProc[ulNumClasses++].fnControl   = pfnDlgProc;
    return TRUE;
    }

/*******************************************************************
   Bestimmt eine neue Class-Id (max. MAXCLASSES Klassen mîglich) 
   return:  ulClassId : neue Class-Id
            -1        : keine Class-Id mehr frei
 *******************************************************************/
ULONG WMGetNewClass (void)
    {
    ULONG ulNewClass = 0xFFFF;
    ULONG i;

    /* Ist noch Platz fÅr eine neue Klasse? */
    if (ulNumClasses < MAXCLASSES)
        for (i=0, ulNewClass = DC_USER; i<MAXCLASSES; i++)
            ulNewClass = max (ulNewClass, WndProc[i].ulControlType);

    return (ulNewClass==0xFFFF) ? 0xFFFF : ++ulNewClass;
    }

//##################################################################
//  WM Create/Destroy Windows
//##################################################################

/*******************************************************************
>  Erzeugt ein Dialogfenster der Klasse ulControlType
>  Eingang: hwndParent       : Window Handle des Parent Windows
>           ulControlType    : Control-Type (= ClassId)
>           pszName          : Window-Text
>           flStyle          : Window-Flags
>           x, y, cx, cy     : Window-Koordinaten bzgl. Parent
>           hwndOwner        : Window Handle des Owner Windows
>           hwndInsertBehind : z-Position
>           id               : Window ID
>  return:  HWND             : Window Handle des neuen Windows
 *******************************************************************/
HWND WMCreateWindow (HWND hwndParent, ULONG ulControlType, PCHAR pszName,
                     ULONG flStyle, ULONG x, ULONG y, ULONG cx, ULONG cy,
                     HWND hwndOwner, HWND hwndInsertBehind, ULONG id,
                     PVOID pRes1, PVOID pRes2)
    {
    ULONG i;
    PDIALOG pDlg;
    BOOL bOwnerExists;
    BOOL bParentExists;

    bOwnerExists  = (hwndOwner == HWND_DESKTOP)  ? TRUE : FALSE;
    bParentExists = (hwndParent == HWND_DESKTOP) ? TRUE : FALSE;

    /* leeren Eintrag in pDialog fÅr neues Fenster suchen */
    for (pDlg=NULL, i=0; i<ulDCount; i++)
        {
        if ((pDialog[i].hwnd == 0) && (pDlg == NULL))
            pDlg = &pDialog[i];
        if (pDialog[i].hwnd == hwndOwner)
            bOwnerExists = TRUE;
        if (pDialog[i].hwnd == hwndParent)
            bParentExists = TRUE;
        }

    /* Fehlerausstieg bei:            */
    /* - Resourcen erschîpft          */
    /* - Parent/Owner existiert nicht */
    /* - Klasse nicht registriert     */
    if ((pDlg == NULL) || !bOwnerExists || !bParentExists || (ulControlType == 0))
        return 0;

    /* Klasse bereits registriert? */
    for (i=0; i<ulNumClasses; i++)
        if (WndProc[i].ulControlType == ulControlType)
            break;
    if (i==ulNumClasses)
        return 0;

    pDlg->ulControlType    = ulControlType;
    pDlg->idControl        = id;
    pDlg->ulzOrder         = ++ulzOrder;
    pDlg->hwnd             = (HWND) pDlg;
    pDlg->hwndOwner        = hwndOwner;
    pDlg->hwndParent       = hwndParent;
    pDlg->hwndFocus        = 0;
    pDlg->pszText          = NULL;
    pDlg->x                = x;
    pDlg->y                = y;
    pDlg->cx               = cx;
    pDlg->cy               = cy;
    pDlg->flVisible        = 1;
    pDlg->flStyle          = flStyle;
    pDlg->usResult         = 0;
    pDlg->bIsValid         = FALSE;
    pDlg->pRegion          = RegAllocate ();

    RegSetBits (pDlg);
    RegAddRegion (pDlg);

    WMSendMsg (pDlg->hwnd, DM_CREATE, (MPARAM) pDlg, 0);
    WMSetWindowText (pDlg->hwnd, pszName);
    WMSetFocus (pDlg->hwnd);

    return (HWND) pDlg;
    }

/*******************************************************************
>  LÑdt ein Dialogfenster aus den Resourcen
>  Eingang: hwndParent   : Window Handle des Parent Windows
>           hwndOwner    : Window Handle des Owner Windows
>           pfnDlgProc   : Window-Procedure
>           pdlg         : Zeiger auf Dialog-Struktur
>           idControl    : Control Id
>           pCreateParms : Create Parameters
>  return:  HWND         : Window Handle des neuen Windows
 *******************************************************************/
HWND WMLoadDlg (HWND hwndParent, HWND hwndOwner, PFNWP pfnDlgProc,
                PDLGTEMPLATE pdlg, ULONG idControl, PVOID pCreateParms)
    {
    HWND  hwndDlg;
    ULONG ulClassId;

    /* neue Klasse registrieren, falls noch nicht geschehen */
    ulClassId = WMGetClass (pfnDlgProc);
    if (ulClassId == 0xFFFF)
        return 0;

    /* Dialogboxdefinition in Resource-Datei suchen */
    do
        {
        if ((pdlg->ulControlType == DC_DIALOG) &&
            (pdlg->idControl == idControl))
            break;
        pdlg++;
        } while (pdlg->idControl != 0);

    /* Definition nicht gefunden */
    if (pdlg->idControl == 0)
        return 0;

    /* Dialogbox erzeugen */
    hwndDlg = WMCreateWindow (hwndParent, ulClassId, pdlg->pszText,
        pdlg->flStyle, pdlg->x, pdlg->y, pdlg->cx, pdlg->cy,
        hwndOwner, HWND_TOP, idControl, NULL, NULL);

    if (hwndDlg == 0)
        return 0;

    /* Dialogelemente plazieren */
    while ((++pdlg)->ulControlType != DC_DIALOG)
        WMCreateWindow (hwndDlg, pdlg->ulControlType, pdlg->pszText,
            pdlg->flStyle, pdlg->x, pdlg->y, pdlg->cx, pdlg->cy,
            hwndDlg, HWND_TOP, pdlg->idControl, NULL, NULL);

    WMSendMsg (hwndDlg, DM_INITDLG, (MPARAM) WMGetDlgStruc (hwndDlg), 0);

    return hwndDlg;
    }

/*******************************************************************
>  Startet ein Dialogfenster
>  Eingang: hwndDlg : Window Handle des Dialog-Fensters
>  return:  ULONG   : RÅckgabewert von WMDismissDlg
 *******************************************************************/
ULONG WMProcessDlg (HWND hwndDlg)
    {
    QUMSG   qmsg;
    PDIALOG pDlg;

    if ((pDlg = WMGetDlgStruc (hwndDlg)) == NULL)
        return 0;

    while (WMWaitMsg ())
        {
        /* WMDismissDlg aus derselben Dialogbox */
        if (pDlg->usResult > 0xFFFF)
            break;
        /* DM_QUIT-Message von Owner */
        WMPeekMsg (&qmsg, PM_NOREMOVE);
        if (qmsg.msg == DM_QUIT)
            {
            pDlg->usResult = 0;
            break;
            }
        else
            {
            WMGetMsg (&qmsg);
            /* Systemmodale Box: keine CHAR-Messages nach oben geben */
            if ((qmsg.msg != DM_CHAR) || (qmsg.hwnd == hwndDlg) ||
                IsParent (hwndDlg, qmsg.hwnd))
                WMDispatchMsg (&qmsg);
            }
        }

    return pDlg->usResult & 0xFFFF;
    }

/*******************************************************************
>  Beendet ein Dialogfenster
>  Eingang: hwndDlg  : Window Handle des Dialog-Fensters
>           usResult : RÅckgabewert fÅr WMProcessDlg
>  return:  TRUE     : Fenster erfolreich beendet
 *******************************************************************/
BOOL WMDismissDlg (HWND hwndDlg, ULONG usResult)
    {
    PDIALOG pDlg;

    if ((pDlg = WMGetDlgStruc (hwndDlg)) == NULL)
        return FALSE;

    pDlg->usResult = 0x10000 + (usResult & 0xFFFF);
    DosPostEventSem (hev);
    return TRUE;
    }

/*******************************************************************
>  Zerstîrt ein Dialogfenster:
>   - Der Parent erhÑlt den Fokus
>   - Das Fenster wird unsichtbar gemacht
>   - Alle Client-Fenster werden ebenfalls gelîscht
>   - Alle Child-Fenster erhalten einen neuen Parent
>   - Die z-Order wird korrigiert
>   - Die Dialogstruktur wird gelîscht
>  Eingang: hwnd  : Window Handle des Dialog-Fensters
>  return:  TRUE  : Fenster erfolreich zerstîrt
 *******************************************************************/
BOOL WMDestroyWindow (HWND hwnd)
    {
    ULONG   i;
    PDIALOG pDlg, pDlgOwner, pDlgParent;

    if ((pDlg = WMGetDlgStruc (hwnd)) == NULL)
        return FALSE;

    WMSetFocus (pDlg->hwndParent);

    /* Zuerst das Fenster unsichtbar machen */
    WMShowWindow (hwnd, FALSE);

    /* Weitere Fenster in der Owner-Hierarchie lîschen */
    for (i=0; i<ulDCount; i++)
        if (pDialog[i].hwndOwner == pDlg->hwnd)
            WMDestroyWindow (pDialog[i].hwnd);

    /* Alle Parent-EintrÑge der Childs korrigieren */
    for (i=0; i<ulDCount; i++)
        if (pDialog[i].hwndParent == pDlg->hwnd)
            pDialog[i].hwndParent = pDlg->hwndParent;

    pDlgOwner  = WMGetDlgStruc (pDlg->hwndOwner);
    pDlgParent = WMGetDlgStruc (pDlg->hwndParent);
    if (pDlgParent->hwndFocus == pDlg->hwnd)
        {
        pDlgParent->hwndFocus =
            WMEnumDlgItem (pDlgParent->hwnd, pDlg->hwnd, EDI_NEXTTABITEM);
        if (pDlgParent->hwndFocus == pDlg->hwnd)
            pDlgParent->hwndFocus = 0;
        }

    /* z-Order wieder korrigieren */
    for (i=0; i<ulDCount; i++)
        if (pDialog[i].ulzOrder > pDlg->ulzOrder)
            pDialog[i].ulzOrder--;

    /* REGION-Puffer freigeben */
    RegRemoveRegion (pDlg);
    RegFree (pDlg->pRegion);

    /* Dialog-Eintrag freigeben */
    ulzOrder--;
    memset (pDlg, 0, sizeof (DIALOG));

    WMSetFocus ((pDlgOwner->hwndFocus==0) ?
        pDlgOwner->hwnd : pDlgOwner->hwndFocus);

    return TRUE;
    }

/*******************************************************************
>  Startet eine Dialogbox. Der Aufruf entspricht der Sequenz
>   - WMLoadDlg
>   - WMProcessDlg
>   - WMDestroyWindow
>  Eingang: hwndParent   : Window Handle des Parent Windows
>           hwndOwner    : Window Handle des Owner Windows
>           pfnDlgProc   : Dialog Prozedur
>           pdlg         : Zeiger auf Resourcen
>           idControl    : Control ID
>           pCreateParms : Create Parameter
>  return:  ULONG        : RÅckgabewert von WMProcessDlg
 *******************************************************************/
ULONG WMDlgBox (HWND hwndParent, HWND hwndOwner, PFNWP pfnDlgProc,
                PDLGTEMPLATE pdlg, ULONG idControl, PVOID pCreateParms)
    {
    HWND  hwnd;
    ULONG usResult;

    hwnd = WMLoadDlg (hwndParent, hwndOwner, pfnDlgProc,
        pdlg, idControl, pCreateParms);
    if (hwnd == 0)
        return FALSE;

    usResult = WMProcessDlg (hwnd);
    WMDestroyWindow (hwnd);

    return usResult & 0xFFFF;
    }

PFNWP WMGetDlgProc (ULONG ulControlType)
    {
    ULONG i;

    /* Window-Procedure der Klasse suchen */
    for (i=0; i<ulNumClasses; i++)
        if (WndProc[i].ulControlType == ulControlType)
            return WndProc[i].fnControl;

    /* Klasse wurde nicht registriert => NULL zurÅckgeben */
    return NULL;
    }

PDIALOG WMGetDlgStruc (HWND hwnd)
    {
    ULONG i;

    if (hwnd)
        for (i=0; i<ulDCount; i++)
            if (pDialog[i].hwnd == hwnd)
                return &pDialog[i];

    return NULL;
    }

void WMInvalidateAll (void)
    {
    ULONG i;

    for (i=0; i<ulDCount; i++)
        if (pDialog[i].hwnd)
            pDialog[i].bIsValid = FALSE;
    return;
    }

HWND WMWindowFromId (HWND hwnd, ULONG id)
    {
    ULONG i;

    for (i=0; i<ulDCount; i++)
        if ((pDialog[i].idControl == id) && (pDialog[i].hwndParent == hwnd))
            return pDialog[i].hwnd;

    return 0;
    }

void WMRefreshWindow (PDIALOG pD)
    {
    if (!pD->bIsValid)
        WMSendMsg (pD->hwnd, DM_PAINT, 0L, 0L);

    return;
    }

void WMRefreshWindows (void)
    {
    ULONG i;

    for (i=0; i<ulDCount; i++)
        if (pDialog[i].hwnd != 0)
            WMRefreshWindow (&pDialog[i]);

    return;
    }

BOOL WMShowWindow (HWND hwnd, BOOL bNewVis)
    {
    PDIALOG pDlg;

    if (pDlg->flVisible == bNewVis)
        return TRUE;

    if ((pDlg = WMGetDlgStruc (hwnd)) == NULL)
        return FALSE;

    pDlg->flVisible = bNewVis ? 1 : 0;
    pDlg->bIsValid  = FALSE;
    WMRefreshWindows ();

    return TRUE;
    }

BOOL WMSetFocus (HWND hwnd)
    {
    PDIALOG pDlgNew, pDlgOld, pDlg1, pDlg2;
    BOOL    bRet;

    /* Bestimmen des aktuellen Fokus-Fensters */
    pDlgOld = WMGetDlgStruc (WMQueryFocus (HWND_DESKTOP));

    /* rÅckwÑrts die neue Fokuskette aufbauen bis zum Desktop-Fenster */
    pDlg2 = pDlgNew = WMGetDlgStruc (hwnd);
    if (pDlgNew != NULL)
        {
        while (pDlg2->hwnd != HWND_DESKTOP)
            {
            pDlg1 = WMGetDlgStruc (pDlg2->hwndParent);
            pDlg1->hwndFocus = pDlg2->hwnd;
            pDlg2 = pDlg1;
            }

        WMSendMsg (pDlgOld->hwnd, DM_FOCUSCHANGE,
            MPFROMLONG (hwnd), (MPARAM) FALSE);
        WMSendMsg (pDlgNew->hwnd, DM_FOCUSCHANGE,
            MPFROMLONG (hwnd), (MPARAM) TRUE);
        bRet = TRUE;
        }
    else
        bRet = FALSE;

    return bRet;
    }

HWND WMQueryFocus (HWND hwnd)
    {
    PDIALOG pDlg;

    /* Als Desktop-window handle wird nur HWND_DESKTOP akzeptiert */
    if (hwnd != HWND_DESKTOP)
        return 0;

    /* Window chain nach dem Fokusfenster durchsuchen */
    do
        {
        pDlg = WMGetDlgStruc (hwnd);
        hwnd = pDlg->hwndFocus;
        } while (hwnd != 0);

    return pDlg->hwnd;
    }

HENUM WMBeginEnumWindows (HWND hwndParent)
    {
    ULONG ulcChild, i;
    PWNDENUM pWndEnum;
    PULONG   pTemp;

    /* Existiert das Åbergebene Parent-Window? */
    if (WMGetDlgStruc (hwndParent) == NULL)
        return 0;

    /* Zahl der Child-Windows */
    for (ulcChild=i=0; i<ulDCount; i++)
        if (pDialog[i].hwndParent == hwndParent)
            ulcChild++;

    /* Allokieren von Speicher fÅr alle Childs                    */
    /* Die pWndEnum zeigt auf eine Struktur mit folgendem Aufbau: */
    /* - ULONG   Zahl der Child-Windows                           */
    /* - ULONG   zuletzt Åbergebene z-Order                       */
    /* - WNDENUM Struktur; wiederholt fÅr jedes Fenster           */
    DosAllocMem ((PPVOID) &pTemp, ulcChild*sizeof (WNDENUM)+sizeof (ULONG),
        PAG_COMMIT | PAG_READ | PAG_WRITE);
    *pTemp     = ulcChild;
    *(pTemp+1) = 0;
    pWndEnum   = (PWNDENUM) (pTemp+2);

    /* AusfÅllen der Struktur */
    for (i=0; i<ulDCount; i++)
        if (pDialog[i].hwndParent == hwndParent)
            {
            pWndEnum->hwnd     = pDialog[i].hwnd;
            pWndEnum->ulzOrder = pDialog[i].ulzOrder;
            pWndEnum++;
            }

    /* Der Åbergebene Handle ist der Zeiger auf die WNDENUM-Struktur */
    return (HENUM) pTemp;
    }

HWND WMGetNextWindow (HENUM henum)
    {
    ULONG    i;
    PWNDENUM pWndEnum;

    /* Zeiger auf die WNDENUM-Struktur */
    pWndEnum = (PWNDENUM) ((PULONG) henum + 2);

    /* Suche des nÑchsten Fensters in der z-Order */
    for (i=0; i<*(PULONG) henum; i++)
        if (*((PULONG) henum + 1) == pWndEnum[i].ulzOrder - 1)
            {
            *((PULONG) henum + 1) += 1;
            return pWndEnum[i].hwnd;
            }

    /* alle Fenster wurden bereits Åbergeben */
    return (HWND) 0;
    }

BOOL WMEndEnumWindows (HENUM henum)
    {
    return (DosFreeMem ((PVOID) henum)) ? FALSE : TRUE;
    }

HWND WMEnumDlgItem (HWND hwndDlg, HWND hwnd, ULONG ulCode)
    {
    ULONG   ulzMin, ulzMax, ulzRet, i, ulTried;
    BOOL    bIsJumpable;
    PDIALOG pDlg, pDlgParent;

    /* Existiert das Åbergebene Dialog-Window? */
    if ((pDlgParent = WMGetDlgStruc (hwndDlg)) == NULL)
        return 0;

    if ((ulCode != EDI_FIRSTTABITEM) && (ulCode != EDI_LASTTABITEM))
        {
        /* Existiert das Åbergebene Control-Window? */
        if ((pDlg = WMGetDlgStruc (hwnd)) == NULL)
            return 0;

        /* Ist hwnd wirklich ein Kind von hwndDlg? */
        if (pDlg->hwndParent != HWND_DESKTOP)
            {
            pDlgParent = pDlg;
            do
                {
                pDlgParent = WMGetDlgStruc (pDlgParent->hwndParent);
                if (pDlgParent->hwnd == HWND_DESKTOP)
                    return 0;
                } while (pDlgParent->hwnd != hwndDlg);
            }
        }

    /* Bestimmen der maximalen/minimalen z-Order im Dialog-Fenster */
    for (ulzMax=i=0, ulzMin=(ULONG) -1; i<ulDCount; i++)
        if (pDialog[i].hwndParent == pDlgParent->hwnd)
            {
            ulzMax = max (ulzMax, pDialog[i].ulzOrder);
            ulzMin = min (ulzMin, pDialog[i].ulzOrder);
            }

//  if ((ulCode != EDI_FIRSTTABITEM) && (ulCode != EDI_LASTTABITEM)) ???
//      ulzStart = pDlg->ulzOrder;
//  else
//      ulzStart = ulzMin;

    ulTried = 0;                /* Zahl der bereits durchsuchten IDs */
    do
        {
        ulTried++;
        switch (ulCode)
            {
            case EDI_PREVTABITEM:
                ulzRet = pDlg->ulzOrder;
                ulzRet = (--ulzRet < ulzMin) ? ulzMax : ulzRet;
                break;

            case EDI_NEXTTABITEM:
                ulzRet = pDlg->ulzOrder;
                ulzRet = (++ulzRet > ulzMax) ? ulzMin : ulzRet;
                break;

            case EDI_FIRSTTABITEM:
                ulzRet = ulzMin;
                ulCode = EDI_NEXTTABITEM;
                break;

            case EDI_LASTTABITEM:
                ulzRet = ulzMax;
                ulCode = EDI_PREVTABITEM;
                break;

            default:                        /* falscher Typ-Code */
                return 0;
            }

        for (i=0; i<ulDCount; i++)
            if (pDialog[i].ulzOrder == ulzRet)
                break;

        pDlg = &pDialog[i];

        switch (pDialog[i].ulControlType)
            {
            case DC_BUTTON:
            case DC_ENTRY:
            case DC_LISTBOX:
                bIsJumpable = TRUE;
                break;
            default:
                bIsJumpable = FALSE;
            }

        if (ulTried > ulzMax-ulzMin)
            return bIsJumpable ? pDialog[i].hwnd : 0;
        } while (bIsJumpable == FALSE);

    return pDialog[i].hwnd;
    }

BOOL WMCreateMsgQueue (ULONG lQueueSize)
    {
    /* Eine zweite Queue darf nicht angelegt werden */
    if (pQueue != NULL)
        return FALSE;

    DosAllocMem ((PPVOID) &pQueue, lQueueSize * sizeof (QUMSG),
        PAG_COMMIT | PAG_READ | PAG_WRITE);

    /* pFirstMsg ist ein Zeiger auf den ersten Queue-Eintrag */
    /* pNextMsg zeigt auf den nÑchsten freien Queue-Eintrag */
    /* Wenn pFirstMsg==pNextMsg, ist die Queue leer */
    pReadMsg  = pQueue;
    pWriteMsg = pQueue;

    /* ulQSize enthÑlt die Zahl der Queue-EintrÑge */
    ulQSize = lQueueSize;

    /* ulcQueue enthÑlt die aktuelle Zahl der Messages */
    ulcQueue = 0;

    /* hev ist eine Semaphore, die gesetzt wird, */
    /* wenn die Queue leer ist */
    DosCreateEventSem (NULL, &hev, 0, (ULONG) FALSE);

    return TRUE;
    }

BOOL WMDestroyMsgQueue ()
    {
    ULONG ulPostCount;

    /* Semaphore lîschen */
    do
        {
        DosResetEventSem (hev, &ulPostCount);
        } while (DosCloseEventSem (hev) != 0);

    /* Queue lîschen */
    ulQSize = 0;            /* ab hier funktionieren keine Posts mehr */
    DosFreeMem ((PVOID) pQueue);
    pQueue = NULL;

    return TRUE;
    }

BOOL WMPostMsg (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
    {
    /* Ist die Message-Queue voll? */
    if (ulcQueue >= ulQSize)
        return FALSE;               /* Queue voll => Messages zurÅckweisen */

    /* Message in die Queue einfÅgen */
    pWriteMsg->hwnd = hwnd;
    pWriteMsg->msg  = msg;
    pWriteMsg->mp1  = mp1;
    pWriteMsg->mp2  = mp2;

    /* Schreibzeiger erhîhen */
    DosEnterCritSec ();
    ulcQueue++;
    pWriteMsg++;
    if (pWriteMsg-pQueue >= ulQSize)
        pWriteMsg = pQueue;
    DosPostEventSem (hev);
    DosExitCritSec ();

    return TRUE;
    }

/* Vorsicht: funktioniert nur fÅr Fenster innerhalb desselben Threads! */
MPARAM WMSendMsg (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
    {
    PFNWP   pfnwp;
    PDIALOG pDlg;

    if ((pDlg = WMGetDlgStruc (hwnd)) == NULL)
        return FALSE;

    pfnwp = WMGetDlgProc (pDlg->ulControlType);
    return (pfnwp) (hwnd, msg, mp1, mp2);
    }

BOOL WMSetWindowText (HWND hwnd, PCHAR pBuffer)
    {
    PDIALOG pDlg;

    if ((pDlg = WMGetDlgStruc (hwnd)) == NULL)
        return FALSE;

    if (pBuffer == NULL)
        memset (pDlg->pszText, '\0', pDlg->ulcBuffer);
    else
        strncpy (pDlg->pszText, pBuffer, pDlg->ulcBuffer);
    pDlg->pszText[pDlg->ulcBuffer] = '\0';

    /* Wichtig fÅr Entry-Felder */
    pDlg->lScroll = pDlg->lCursor = 0;

    WMSendMsg (pDlg->hwnd, DM_PAINT, 0L, 0L);
    return TRUE;
    }

ULONG WMQueryDlgItemText (HWND hwndDlg, ULONG Id, LONG lSize, PCHAR pBuffer)
    {
    PDIALOG pDlg;
    ULONG lRet;

    if ((pDlg = WMGetDlgStruc (WMWindowFromId (hwndDlg, Id))) == NULL)
        return 0;

    lRet = min (lSize-1, strlen (pDlg->pszText));
    memcpy (pBuffer, pDlg->pszText, lRet+1);

    return lRet;
    }

BOOL WMGetMsg (PQUMSG pqmsgmsg)
    {
    BOOL bRC;
    ULONG ulPostCount;

    DosWaitEventSem (hev, SEM_INDEFINITE_WAIT);

    /* DosWaitEventSem wartet auf die erste Message. Sollte trotzdem */
    /* keine Message in der Queue sein, liegt ein Programmfehler vor */
    /* Die Applikation wird terminiert (DM_QUIT vorgetÑuscht)        */
    if (ulcQueue == 0)
        return FALSE;

    /* Message umkopieren */
    memcpy (pqmsgmsg, pReadMsg++, sizeof (QUMSG));

    /* Queue-Eintrag wieder freigeben */
    if (pReadMsg-pQueue >= ulQSize)
        pReadMsg = pQueue;
    DosEnterCritSec ();
    if (--ulcQueue == 0)
        DosResetEventSem (hev, &ulPostCount);
    DosExitCritSec ();

    bRC = (pqmsgmsg->msg != DM_QUIT);

    return bRC;
    }

BOOL WMWaitMsg ()
    {
    ULONG ulPostCount;

    DosWaitEventSem (hev, SEM_INDEFINITE_WAIT);
    if (ulcQueue == 0)
        DosResetEventSem (hev, &ulPostCount);

    /* DosWaitEventSem wartet auf die erste Message. Sollte trotzdem */
    /* keine Message in der Queue sein, liegt ein Fehler vor         */
    return (ulcQueue == 0) ? FALSE : TRUE;
    }

BOOL WMPeekMsg (PQUMSG pqmsgmsg, ULONG flOptions)
    {
    ULONG ulPostCount;

    if (ulcQueue == 0)
        return FALSE;               /* keine Message verfÅgbar */

    /* Message umkopieren */
    memcpy (pqmsgmsg, pReadMsg, sizeof (QUMSG));

    /* Queue-Eintrag wieder freigeben, falls flOptions & PM_REMOVE */
    if (flOptions & PM_REMOVE)
        {
        pReadMsg++;
        if (pReadMsg-pQueue >= ulQSize)
            pReadMsg = pQueue;
        DosEnterCritSec ();
        if (--ulcQueue == 0)
            DosResetEventSem (hev, &ulPostCount);
        DosExitCritSec ();
        }

    return TRUE;
    }

/* Vorsicht: funktioniert nur fÅr Fenster innerhalb desselben Threads! */
MPARAM WMDispatchMsg (PQUMSG pqmsgmsg)
    {
    PDIALOG pDlg;
    PFNWP   pfnwp;

    if ((pDlg = WMGetDlgStruc (pqmsgmsg->hwnd)) == NULL)
        return 0;

    pfnwp = WMGetDlgProc (pDlg->ulControlType);
    return (pfnwp) (pqmsgmsg->hwnd, pqmsgmsg->msg, pqmsgmsg->mp1, pqmsgmsg->mp2);
    }

USHORT WMMessageBox (HWND hwndParent, HWND hwndOwner, PSZ pszText, PSZ pszTitle,
                     USHORT usWindow, ULONG flStyle)
    {
    ULONG i, ulc1, ulc2, ulResult;
    BOOL  bNewLine;
    PCHAR pString;
    HWND  hwndDlg;
    ULONG ulClassId;

    /* Zeilenzahl bestimmen; Box sollte ca. 1/2 Zeile lang sein */
    pString = pszText;

    /* Schleife fÅr Zeilenzahl: Bildschirmhîhe-2xRand-Buttons */
    for (i=1; i<=strucVioMode.row-3; i++)
        {
        ulc2 = ulc1 = 0;
        bNewLine = FALSE;
        do
            {
            ulc1 += strcspn (&pString[ulc1], BREAKCHAR);
            if (ulc1 <= strucVioMode.col/2)
                ulc2 = ulc1;

            bNewLine = ParseLine (&pString, &ulc1);

            } while (ulc1<strucVioMode.col/2 && pString[ulc1]!='\0' && !bNewLine);
        if ((ulc2 == 0) && (ulc1 != 0))
            ulc2 = min (ulc1, strucVioMode.col/2);
        if (ulc1 != ulc2)
            {
            ParseLine (&pString, &ulc2);
            ulc1 = ulc2;
            }
        if (pString[ulc1] == '\0')
            break;
        pString += ulc1;
        }

    /* neue Klasse registrieren, falls nîtig */
    ulClassId = WMGetClass (WMDefDlgProc);
    if (ulClassId == 0xFFFF)
        return 0;

    hwndDlg = WMCreateWindow (hwndParent, ulClassId, pszTitle, 0,
        strucVioMode.col/4, (strucVioMode.row-i)/2, strucVioMode.col/2+2, i+3,
        hwndOwner, HWND_TOP, usWindow, 0, 0);

    WMCreateWindow (hwndDlg, DC_STATIC, pszText, DT_WORDBREAK,
        1, 1, strucVioMode.col/2, i, hwndDlg, HWND_TOP, -1, 0, 0);
    if (flStyle == MB_OK)
        WMCreateWindow (hwndDlg, DC_BUTTON, "OK", 0,
            1, i+1, 6, 1, hwndDlg, HWND_TOP, DID_OK, 0, 0);

    ulResult = WMProcessDlg (hwndDlg);
    WMDestroyWindow (hwndDlg);

    return ulResult & 0xFFFF;
    }

/*****************************************************************************
 * Timer-Services
 *****************************************************************************/
BOOL WMStartTimer (HWND hwnd, ULONG idTimer, ULONG dtTimeout)
    {
    ULONG i;

    /* Ist es bereits ein laufender Timer?      */
    /* Dazu mu· idTimer und hwnd Åbereinstimmen */
    for (i=0; i<MAXTIMER; i++)
        if ((arTimers[i].idTimer == idTimer) && (arTimers[i].hwnd == hwnd))
            {
            /* Timer informieren, da· sich Timeout geÑndert hat */
            arTimers[i].dtTimeout = dtTimeout;
            DosPostEventSem (arTimers[i].hSem);
            return TRUE;
            }

    /* Ist noch Platz fÅr einen weiteren Timer? */
    for (i=0; i<MAXTIMER; i++)
        if (arTimers[i].idTimer == 0)
            break;

    /* Wenn i<MAXTIMER, ist noch Platz fÅr weiteren Timer */
    if (i == MAXTIMER)
        return FALSE;

    /* Timer starten */
    DosCreateEventSem (NULL, &(arTimers[i].hSem), DC_SEM_SHARED, FALSE);
    arTimers[i].hwnd      = hwnd;
    arTimers[i].idTimer   = idTimer;
    arTimers[i].dtTimeout = dtTimeout;
    arTimers[i].tid = _beginthread (TMRThread, NULL, STACKLEN, &arTimers[i]);

    if (arTimers[i].tid == -1)
        {
        DosCloseEventSem (arTimers[i].hSem);
        arTimers[i].dtTimeout = 0;
        arTimers[i].idTimer = 0;
        return FALSE;
        }
    else
        return TRUE;
    }

BOOL WMStopTimer (HWND hwnd, ULONG idTimer)
    {
    ULONG i;

    /* Timer suchen */
    for (i=0; i<MAXTIMER; i++)
        if ((arTimers[i].idTimer == idTimer) && (arTimers[i].hwnd == hwnd))
            {
            arTimers[i].hwnd = 0;
            arTimers[i].idTimer = 0;
            arTimers[i].dtTimeout = 0;
            DosPostEventSem (arTimers[i].hSem);
            return TRUE;
            }

    return FALSE;
    }

/*****************************************************************************
 * Zeichen in Bildschirmbereich sichern
 * Eingang: x,y:   linke, untere Ecke
 *          cx,cy: Bereichsgrî·e
 * return:  Zeiger auf Sicherung
 *****************************************************************************/
PCHAR SaveBits (USHORT x, USHORT y, USHORT cx, USHORT cy)
    {
    PCHAR pSaveBits;
    USHORT usc, i;

    pSaveBits = malloc (cx*cy*2);
    for (i=0; i<cy; i++)
        {
        usc = cx*2;
        VioReadCellStr (pSaveBits+i*cx*2, &usc, strucVioMode.row-y-i-1, x, 0);
        }
    return pSaveBits;
    }

/*****************************************************************************
 * RÅckschreiben einer Textsicherung in den Bildschirmbereich
 * Eingang: x,y:       linke, untere Ecke
 *          cx,cy:     Bereichsgrî·e
 *          pSaveBits: Zeiger auf Sicherung
 *****************************************************************************/
void RestoreBits (USHORT x, USHORT y, USHORT cx, USHORT cy, PCHAR pSaveBits)
    {
    USHORT i;

    for (i=0; i<cy; i++)
        VioWrtCellStr (pSaveBits+i*cx*2, cx*2, strucVioMode.row-y-i-1, x, 0);

    free (pSaveBits);
    return;
    }

/*****************************************************************************
 * Tastaturthread: Sendet jedes Zeichen aus der Tastatur 
 *****************************************************************************/
void _Optlink KBDThread (void *arg)
    {
    MPARAM mp1, mp2;
    USHORT usVirt;

    while (TRUE)
        {
        DosRequestMutexSem (hmtxKbd, SEM_INDEFINITE_WAIT);
        KbdCharIn (&key, 1, 0);
        DosReleaseMutexSem (hmtxKbd);
        if (key.fbStatus == 0)
            DosSleep (5);
        else
            {
            if (key.fbStatus & 2)
                {
                /* Scancode ist extended key */
                /* 1. Behandlung des Shift-Zustandes */
                if ((key.fsState & 3) && ((key.fsState & 12) == 0))
                    {
                    /* Shift gedrÅckt, nicht ALT oder CTRL */
                    switch (key.chScan)
                        {
                        case 0x0F:
                            usVirt = VK_BACKTAB;
                            break;
                        default:
                            usVirt = 0;
                        }
                    }
                else if ((key.fsState & 15) == 0)
                    {
                    /* Shift, ALT, CTRL nicht gedrÅckt */
                    switch (key.chChar + (key.chScan<<8))
                        {
                        case 0x3B00:
                            usVirt = VK_F1;
                            break;
                        case 0x3C00:
                            usVirt = VK_F2;
                            break;
                        case 0x3D00:
                            usVirt = VK_F3;
                            break;
                        case 0x3E00:
                            usVirt = VK_F4;
                            break;
                        case 0x3F00:
                            usVirt = VK_F5;
                            break;
                        case 0x4000:
                            usVirt = VK_F6;
                            break;
                        case 0x4100:
                            usVirt = VK_F7;
                            break;
                        case 0x4200:
                            usVirt = VK_F8;
                            break;
                        case 0x4300:
                            usVirt = VK_F9;
                            break;
                        case 0x4400:
                            usVirt = VK_F10;
                            break;
                        case 0x47E0:
                        case 0x4700:
                            usVirt = VK_HOME;
                            break;
                        case 0x4BE0:
                        case 0x4B00:
                            usVirt = VK_LEFT;
                            break;
                        case 0x4DE0:
                        case 0x4D00:
                            usVirt = VK_RIGHT;
                            break;
                        case 0x48E0:
                        case 0x4800:
                            usVirt = VK_UP;
                            break;
                        case 0x4FE0:
                        case 0x4F00:
                            usVirt = VK_END;
                            break;
                        case 0x50E0:
                        case 0x5000:
                            usVirt = VK_DOWN;
                            break;
                        case 0x52E0:
                        case 0x5200:
                            usVirt = VK_INSERT;
                            break;
                        case 0x53E0:
                        case 0x5300:
                            usVirt = VK_DELETE;
                            break;
                        case 0xE00D:
                            usVirt = VK_ENTER;
                            break;
                        default:
                            usVirt = 0;
                        }
                    }
                mp1 = MPFROMSH2CH (KC_SCANCODE | (usVirt ? KC_VIRTUALKEY : 0),
                    1, key.chScan);
                mp2 = MPFROM2SHORT (key.chChar, usVirt);
                }
            else
                {
                /* Scancode ist normales Zeichen */
                /* 1. Behandlung des Shift-Zustandes */
                if ((key.fsState & 3) && ((key.fsState & 12) == 0))
                    {
                    /* Shift gedrÅckt, nicht ALT oder CTRL */
                    switch (key.chChar)
                        {
                        case CR:
                            usVirt = VK_NEWLINE;
                            break;
                        default:
                            usVirt = 0;
                        }
                    }
                else if ((key.fsState & 15) == 0)
                    {
                    /* Shift, ALT, CTRL nicht gedrÅckt */
                    switch (key.chChar)
                        {
                        case BACKSP:
                            usVirt = VK_BACKSPACE;
                            break;
                        case TAB:
                            usVirt = VK_TAB;
                            break;
                        case CR:
                            usVirt = VK_ENTER;
                            break;
                        case ESC:
                            usVirt = VK_ESC;
                            break;
                        default:
                            usVirt = 0;
                        }
                    }
                mp1 = MPFROMSH2CH (KC_CHAR | KC_SCANCODE | (usVirt ? KC_VIRTUALKEY : 0),
                    1, key.chScan);
                mp2 = MPFROM2SHORT (key.chChar,  usVirt);
                }
            WMPostMsg (WMQueryFocus (HWND_DESKTOP), DM_CHAR, mp1, mp2);
            }
        }

    return;
    }

/*****************************************************************************
 * Mausthread: Sendet Messages bei Maus-Events
 *****************************************************************************/
void _Optlink MOUThread (void *arg)
    {
    MPARAM mp1, mp2;
    USHORT usEventMask = MOUSE_BN1_DOWN | MOUSE_MOTION_WITH_BN1_DOWN |
                         MOUSE_BN2_DOWN | MOUSE_MOTION_WITH_BN2_DOWN |
                         MOUSE_BN3_DOWN | MOUSE_MOTION_WITH_BN3_DOWN |
                         MOUSE_MOTION;
    USHORT usDevStatus = MOU_DRAW | MOU_PELS;
    USHORT usReadType  = MOU_WAIT;
    MOUEVENTINFO MouEvent;

    MouSetDevStatus (&usDevStatus, hMou);
    MouSetEventMask (&usEventMask, hMou);

    while (TRUE)
        {
        MouReadEventQue (&MouEvent, &usReadType, hMou);
 /*fs, time, row, col */
        }

    return;
    }

/*****************************************************************************
 * Timerthread
 *****************************************************************************/
void _Optlink TMRThread (PVOID pTimer)
    {
    ULONG  ulTime = 0;
    ULONG  ulPostCount;

    ((PTMTHREAD)pTimer)->hTimer = 0;
    do
        {
        if (ulTime != ((PTMTHREAD)pTimer)->dtTimeout)
            {
            ulTime = ((PTMTHREAD)pTimer)->dtTimeout;
            if (((PTMTHREAD)pTimer)->hTimer != 0)
                DosStopTimer (((PTMTHREAD)pTimer)->hTimer);
            DosStartTimer (ulTime, (HSEM *) ((PTMTHREAD)pTimer)->hSem, &(((PTMTHREAD)pTimer)->hTimer));
            }

        DosWaitEventSem (((PTMTHREAD)pTimer)->hSem, -1);           /* warten ohne Timeout */
        DosResetEventSem (((PTMTHREAD)pTimer)->hSem, &ulPostCount);
        for (;ulPostCount; ulPostCount--)
            WMPostMsg (((PTMTHREAD)pTimer)->hwnd, DM_TIMER, (MPARAM) ((PTMTHREAD)pTimer)->idTimer, 0);
        } while (((PTMTHREAD)pTimer)->dtTimeout != 0);

    DosStopTimer (((PTMTHREAD)pTimer)->hTimer);
    DosCloseEventSem (((PTMTHREAD)pTimer)->hSem);
    return;
    }
