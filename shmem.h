/*******************************************************************
   Allgemeine Definitionen fÅr SAM, WPSAM und INITOS2
   OS/2 - System Access Manager Version 2.1 - 9.7.1994
 *******************************************************************/

/* Semaphoren und Shared Memory */
#define SHARE_INITOS2    "\\SHAREMEM\\WPSam.InitOS2" /* Shared-Memory Name   */

/* Flags fÅr die ulFlag-Variable */
#define OTHERPROTSHELL  0x0001          /* Other ProtShell              */
#define PWDENABLE       0x0002          /* Password enabled             */
#define RENENABLE       0x0004          /* Rename enabled               */
#define MOVEENABLE      0x0008          /* Move enabled                 */
#define COPYENABLE      0x0010          /* Copy enabled                 */
#define DELENABLE       0x0020          /* Delete enabled               */
#define DROPALLOW       0x0040          /* Allow drop into me           */
#define PRIVATESYS      0x0080          /* Private system profile       */
#define WPSDISABLED     0x0100          /* User-WPS disabled            */
#define SETUPPASSED     0x0200          /* wpSetup was running at least one time */
#define ININITOS2MENU   0x0400          /* User is visible in Init-OS/2 menu     */
#define CLOSEAPPL       0x0800          /* Close applications when leaving       */

/* Flags fÅr ulActive in SHARE1 */
#define ACTIVE_INITOS2  0x00000001      /* InitOS2 ist aktiv */
#define ACTIVE_SAMMY    0x00000002      /* Sammy ist aktiv */
#define ACTIVE_WPSAM    0x00000004      /* WPSam ist aktiv */
#define ACTIVE_WPSAC    0x00000008      /* WPSac ist aktiv */

/* Struktur fÅr Shared-Memory SHARE_INITOS2                   */
/* Alle Strings sind maximal CCHMAXPATHCOMP lang (incl. '\0') */
typedef struct _SHARE1
    {
    PCHAR   pszRegFile;                 /* Die EAs von RegFile enthalten Reg.String */
    PCHAR   pszRootUserIni;
    PCHAR   pszRootSystemIni;
    ULONG   ulFlag_Root;                /* Flags des Rootdesktops */
    PCHAR   pszUserIni;                 /* Userprofile fÅr prfReset */
    PCHAR   pszSystemIni;               /* Systemprofile fÅr prfReset */
    PCHAR   pszEnvironment;             /* Environment fÅr Shell */
    ULONG   ulFlag;                     /* Flags des startenden Users */
    ULONG   ulActive;                   /* Aktive Programme im System */
    HWND    hwndSammy;
    ULONG   ulMsgLogout;
    } SHARE1;
typedef SHARE1 *PSHARE1;

---------------------------8<-------cut here---------------------------------
SHARE1 ist die Struktur des Shared Memory-Bereiches. Der Name, der 
beim DosAllocSharedMem verwendet werden mu· ist SHARE_INITOS2.

Die meisten EintrÑge in dieser Struktur werden in WPSac nicht verwendet.
Wichtig sind:
    hwndSammy:      Hier steht der Window-Handle des Fensters, an die die
                    Logout-Message gepostet werden soll.
    ulMsgLogout:    Hier steht die Message-Nummer (WM_USER+n), die beim
                    Logout an hwndSammy gepostet werden soll.
    ulActive:       WPSac setzt in ulActive das Flag ACTIVE_WPSAC, wenn
                    die DLL gestartet wird (fÅr Debug-Zwecke).
                    Hier kînnen weitere Flags fÅr eigene Zwecke gesetzt
                    werden.
Alle weiteren EintrÑge werden nur von WPSam verwendet und kînnen hier
zu NULL bzw. 0 gesetzt werden.

MfG
K. Breining

