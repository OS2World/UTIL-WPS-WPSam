/*******************************************************************
   Definitionen fÅr Fullscreen-Window-Manager
   Version 1.0 - 12.3.1994
 *******************************************************************/

/* Struktur zur Definition eines Dialog-Elementes */
typedef struct _DIALOG
    {
    ULONG   ulControlType;
    ULONG   idControl;
    ULONG   ulzOrder;
    HWND    hwnd;
    HWND    hwndOwner;
    HWND    hwndParent;
    HWND    hwndFocus;
    PCHAR   pszText;
    ULONG   ulcBuffer;          /* Puffergrî·e fÅr pszText in Byte */
    USHORT  x;
    USHORT  y;
    USHORT  cx;
    USHORT  cy;
    BYTE    bCol;
    BYTE    bSelCol;
    ULONG   flVisible;
    ULONG   flStyle;
    ULONG   usResult;           /* 0xAAAA0000 A>0 => Dismiss Window */
    /* REGION-Verwaltung */
    BOOL    bIsValid;               /* TRUE: Fenster ist gÅltig */
    PBYTE   pRegion;                /* Zeiger auf REGION */
    /* nur Eingabefelder: */
    LONG    lScroll;            /* Zahl der nach links gescrollten Zeichen */
    LONG    lCursor;            /* Cursorposition bei Eingabefeldern */
    } DIALOG;
typedef DIALOG *PDIALOG;

/* Definition eines Dialog-Elementes im Resource-File */
typedef struct _DLGTEMPLATE
    {
    ULONG   ulControlType;
    ULONG   idControl;
    PCHAR   pszText;
    USHORT  x;
    USHORT  y;
    USHORT  cx;
    USHORT  cy;
    ULONG   flStyle;
    } DLGTEMPLATE;
typedef DLGTEMPLATE *PDLGTEMPLATE;

/* Dialog-Elemente (DIALOG.ulControlType) */
/* DC_DIALOG wird nur fÅr das Resource-File benîtigt. Intern */
/* erhalten Dialog-Boxen einen Class-Style (ClassId), die    */
/* vom Window-Manager festgelegt wird!                       */
#define DC_DESKTOP      1
#define DC_DIALOG       2
#define DC_BUTTON       3
#define DC_STATIC       4
#define DC_ENTRY        5
#define DC_LISTBOX      6
#define DC_USER         100

/* Dialog-Messages */
#define DM_QUIT         0       /* Terminierung:    mp1: ./.     */
                                /*                  mp2: ./.     */
#define DM_INITDLG      1       /* Initialisierung: mp1: *DIALOG */
                                /*                  mp2: ./.     */
#define DM_PAINT        2       /* Fenster aktualisieren:        */
                                /*                  mp1: ./.     */
                                /*                  mp2: ./.     */
#define DM_DESTROY      3       /* Fenster schlie·en:            */
                                /*                  mp1: ./.     */
                                /*                  mp2: ./.     */
#define DM_CLOSE        4       /* Fenster schlie·en:            */
                                /*                  mp1: ./.     */
                                /*                  mp2: ./.     */
#define DM_CHAR         5       /* Tastaturzeichen: mp1: fsFlags */
                                /*                       ucRepeat*/
                                /*                       ucScan  */
                                /*                  mp2: ucChar  */
                                /*                       ucVirt  */
    /* fsFlags fÅr DM_CHAR: */
    #define KC_NONE         0x0000      /* Reserved */
    #define KC_CHAR         0x0001
    #define KC_VIRTUALKEY   0x0002
    #define KC_SCANCODE     0x0004
    /* ucVirt fÅr DM_CHAR: */
    #define VK_BACKSPACE    0x05
    #define VK_TAB          0x06
    #define VK_BACKTAB      0x07
    #define VK_NEWLINE      0x08
    #define VK_END          0x13
    #define VK_HOME         0x14
    #define VK_LEFT         0x15
    #define VK_UP           0x16
    #define VK_RIGHT        0x17
    #define VK_DOWN         0x18
    #define VK_ESC          0x0F
    #define VK_INSERT       0x1A
    #define VK_DELETE       0x1B
    #define VK_ENTER        0x1E
    #define VK_F1           0x20
    #define VK_F2           0x21
    #define VK_F3           0x22
    #define VK_F4           0x23
    #define VK_F5           0x24
    #define VK_F6           0x25
    #define VK_F7           0x26
    #define VK_F8           0x27
    #define VK_F9           0x28
    #define VK_F10          0x29

#define DM_COMMAND      6       /* Control event:   mp1: usCmd   */
                                /*                  mp2: usSrc   */
#define DM_FOCUSCHANGE  7       /* Change Focus:    mp1: hwndFoc */
                                /*                  mp2: usFlag  */
#define DM_CREATE       8       /* Erzeugen der Dialogbox:       */
                                /*                  mp1: *DIALOG */
                                /*                  mp2: ./.     */
#define DM_TIMER        9       /* Timer:           mp1: idTimer */
                                /*                  mp2: ./.     */
#define DM_CONTROL      10      /* Control:         mp1: IdDlg   */
                                /*                       Notify  */
                                /*                  mp2: hwnd    */
   /* Entry Field notification messages */
   #define EN_SETFOCUS      0x0001
   #define EN_KILLFOCUS     0x0002
   #define EN_CHANGE        0x0004

#define EM_SETTEXTLIMIT 20      /* TextlÑnge Eingabefelder:      */
                                /*                  mp1: usLen   */
                                /*                  mp2: ./.     */

#define DM_USER         1024    /* Benutzerdefinierte Message */

/* Dialog-Styles */
#define BS_DEFAULT      1       /* Default-Button */
#define ES_READONLY     1       /* Eingabefeld: Readonly */
#define ES_AUTOSCROLL   2       /* Eingabefeld: Scrollen */
#define ES_UNREADABLE   4       /* Eingabefeld: unlesbar */

typedef struct _SIZL
    {
    ULONG   x;
    ULONG   y;
    ULONG   cx;
    ULONG   cy;
    } SIZL;
typedef SIZL *PSIZL;

/* Message Queue */
typedef struct _QUMSG
    {
    HWND    hwnd;
    ULONG   msg;
    MPARAM  mp1;
    MPARAM  mp2;
    } QUMSG;
typedef QUMSG *PQUMSG;

/* maximale Zahl von Timern */
#define MAXTIMER 5

/* Definitionen fÅr Enumerate-Funktionen */
#ifndef HENUM
typedef ULONG HENUM;
#endif

#define EDI_FIRSTTABITEM    0
#define EDI_LASTTABITEM     1
#define EDI_NEXTTABITEM     2
#define EDI_PREVTABITEM     3

/* Trennzeichen fÅr Message-Strings */
#define BREAKCHAR " \t\r\n"

/* Funktionsprototypen */
BOOL LinkKeyboard (void);
APIRET UnlinkKeyboard (ULONG);
BOOL WMInitialize (ULONG);
BOOL WMTerminate (void);
ULONG WMGetClass (PFNWP);
BOOL WMRegisterClass (ULONG, PFNWP);
ULONG WMGetNewClass (void);
HWND WMCreateWindow (HWND, ULONG, PCHAR, ULONG, ULONG, ULONG, ULONG, ULONG,
                     HWND, HWND, ULONG, PVOID, PVOID);
HWND WMLoadDlg (HWND, HWND, PFNWP, PDLGTEMPLATE, ULONG, PVOID);
ULONG WMProcessDlg (HWND);
BOOL WMDismissDlg (HWND, ULONG);
BOOL WMDestroyWindow (HWND);
ULONG WMDlgBox (HWND, HWND, PFNWP, PDLGTEMPLATE, ULONG, PVOID);
PFNWP WMGetDlgProc (ULONG);
PDIALOG WMGetDlgStruc (HWND);
HWND WMWindowFromId (HWND, ULONG);
void WMInvalidateAll (void);
void WMRefreshWindow (PDIALOG);
void WMRefreshWindows (void);
BOOL WMShowWindow (HWND, BOOL);
BOOL WMSetFocus (HWND);
HWND WMQueryFocus (HWND);
HENUM WMBeginEnumWindows (HWND);
HWND WMGetNextWindow (HENUM);
BOOL WMEndEnumWindows (HENUM);
HWND WMEnumDlgItem (HWND, HWND, ULONG);
BOOL WMCreateMsgQueue (ULONG);
BOOL WMDestroyMsgQueue (void);
BOOL WMPostMsg (HWND, ULONG, MPARAM, MPARAM);
MPARAM WMSendMsg (HWND, ULONG, MPARAM, MPARAM);
ULONG WMQueryDlgItemText (HWND, ULONG, LONG, PCHAR);
BOOL WMSetWindowText (HWND, PCHAR);
BOOL WMGetMsg (PQUMSG);
BOOL WMWaitMsg (void);
BOOL WMPeekMsg (PQUMSG, ULONG);
MPARAM WMDispatchMsg (PQUMSG);
USHORT WMMessageBox (HWND, HWND, PSZ, PSZ, USHORT, ULONG);
BOOL WMStartTimer (HWND, ULONG, ULONG);
BOOL WMStopTimer (HWND, ULONG);
PCHAR SaveBits (USHORT, USHORT, USHORT, USHORT);
void RestoreBits (USHORT, USHORT, USHORT, USHORT, PCHAR);
MPARAM EXPENTRY WMDefDlgProc (HWND, ULONG, MPARAM, MPARAM);
MPARAM EXPENTRY DesktopDlgProc (HWND, ULONG, MPARAM, MPARAM);
MPARAM EXPENTRY StaticDlgProc (HWND, ULONG, MPARAM, MPARAM);
MPARAM EXPENTRY ButtonDlgProc (HWND, ULONG, MPARAM, MPARAM);
MPARAM EXPENTRY EntryDlgProc (HWND, ULONG, MPARAM, MPARAM);

BOOL IsVisible (ULONG);
ULONG GetIndex (PDIALOG);
BOOL ParseLine (PCHAR *, PULONG);
BOOL Left1Char (PDIALOG);
BOOL Right1Char (PDIALOG);

PBYTE RegAllocate (void);
void  RegFree (PBYTE);
void  RegSet (PBYTE, ULONG, ULONG);
void  RegReset (PBYTE, ULONG, ULONG);
BOOL  RegTest (PBYTE, ULONG, ULONG);
void  RegSetBits (PDIALOG);
BOOL  RegCorrelateWnd (PDIALOG, PDIALOG);
BOOL  RegAddRegion (PDIALOG);
BOOL  RegRemoveRegion (PDIALOG);
BOOL  WMIsPart (PDIALOG, ULONG, ULONG);
BOOL  WMMapWindowPoint (HWND, HWND, PPOINTL);
BOOL  WMGetWindowPoints (HWND, PSIZL);
void  RegWrtCellStr (PDIALOG, PCH, USHORT, USHORT, USHORT);
void  RegWrtCharStr (PDIALOG, PCH, USHORT, USHORT, USHORT);
void  RegWrtCharStrAtt (PDIALOG, PCH, USHORT, USHORT, USHORT, PBYTE);
void  RegWrtNCell (PDIALOG, PBYTE, USHORT, USHORT, USHORT);
void  RegWrtNChar (PDIALOG, PCH, USHORT, USHORT, USHORT);
void  RegCls (PDIALOG, PBYTE, USHORT, USHORT, USHORT, USHORT);

/* Funktionsmakros */
#define WMSendDlgItemMsg(hwnd, Id, msg, mp1, mp2) \
    ((MPARAM)WMSendMsg(WMWindowFromId (hwnd, Id), msg, mp1, mp2))

#define WMSetDlgItemText(hwnd, Id, sz) \
    ((BOOL)WMSetWindowText(WMWindowFromId (hwnd, Id), sz))

