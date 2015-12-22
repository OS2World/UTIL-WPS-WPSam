
/******************************************************************************
 *  Bitmaps
 ******************************************************************************/
#define ID_ICON                 300   /* wird fÅr die RegisterNow DlgBox benîtigt */

/******************************************************************************
 *  Dialogboxen
 ******************************************************************************/
#define IDD_ABOUT               1010
#define IDD_PWDIN               1011
#define IDD_PWD                 1012
#define IDD_ADJUSTMENTS         1013
#define IDD_CREATE              1014
#define IDD_DEBUG               1015
#define IDD_MESSAGEBOX          1016
#define IDD_INITOS2             1017

/***** gemeinsame IDs *****/
#define IDD_WAIT_PRFRESET       1030
#define IDD_PRFRESET            1031
#define DID_HELP                1040
#define DID_DEFAULT             1041

/* AboutDlgProc (IDD_ABOUT) */
#define DID_USERNAME            400
#define DID_PASSWORD            401
#define DID_PWDTXT              402

/* PwdDlgProc (IDD_PWDIN) */
#define DID_PWD                 410
#define DID_PWDTXTMITKARTE      411
#define DID_PWDTXTOHNEKARTE     412

/* PwdNewDlgProc (IDD_PWD) */
#define DID_PWDENABLE           420
#define DID_PWDOLD              421
#define DID_PWDNEW              422
#define DID_PWDVER              423
#define DID_PWDNEWTXTMITKARTE  424
#define DID_PWDNEWTXTOHNEKARTE 425
#define DID_PWDOLDTXTMITKARTE  426
#define DID_PWDOLDTXTOHNEKARTE 427

/* AdjustDlgProc (IDD_ADJUSTMENTS) */
#define DID_RENENABLE           430
#define DID_MOVEENABLE          431
#define DID_DROPALLOW           432
#define DID_ENVIRONMENT         433
#define DID_CLOSEAPPL           434

/* CreateDlgProc (IDD_CREATE) */
#define DID_USERRESOURCE        440
#define DID_USERPROFILE         441
#define DID_SYSTEMRESOURCE      442
#define DID_SYSTEMPROFILE       443
#define DID_GLOBALSYS           444
#define DID_PRIVATESYS          445

/* DbgDlgProc (IDD_DEBUG) */
#define DID_TL_0                450
#define DID_TL_1                451
#define DID_TL_2                452
#define DID_WL_0                453
#define DID_WL_1                454
#define DID_WL_2                455
#define DID_AL_0                456
#define DID_AL_1                457
#define DID_AL_2                458
#define DID_DM                  459
#define DID_DT                  460

/* InitOS2DlgProc (IDD_INITOS2) */
#define DID_PROT_DEFAULT        460
#define DID_PROT_OTHER          461
#define DID_PROT_OTHER_ENTRY    462
#define DID_PROT_INMENU         463

/******************************************************************************
 *  Strings
 ******************************************************************************/
#define IDS_TITLE_WPSAM         100

/* Warnings */
#define IDS_INFORMATION         110
#define IDS_DEF_PASSWORD        111
#define IDS_DESKTOP             112
#define IDS_WRONGPWDOLD         113
#define IDS_WRONGPWDVER         114
#define IDS_WRONGPWD            115
#define IDS_WRONGPWDS           116
#define IDS_WRONGVER            117
#define IDS_ALREADYINUSER       118
#define IDS_NOUSERRESOURCE      119
#define IDS_NOSYSTEMRESOURCE    120
#define IDS_NOUSERINICREATED    121
#define IDS_NOSYSTEMINICREATED  122
#define IDS_NODESKINRC          123
#define IDS_USERWITHSYSTEMINI   124

/* Errors */
#define IDS_SAVESTATEFAILED     130
#define IDS_RESTORESTATEFAILED  131
#define IDS_REALNAMEERROR       132
#define IDS_PRFRESETERROR       133
#define IDS_SAVEREGPWD          134
#define IDS_PATHTOOLONG         135

/* Default-Profiles */
#define IDS_USERPROFILE         140
#define IDS_SYSTEMPROFILE       141
#define IDS_USERRESOURCE        142
#define IDS_SYSTEMRESOURCE      143

/* MyHistory */
#define IDS_SETUP               150
#define IDS_WRONGPASSWORD       151
#define IDS_PRFRESET            152

/* Namen der Tab-Marken */
#define CCHMAXPGNAME            15      /* maximale LÑnge der Namen */
#define IDS_WPSAM_PG1           160     /* Einstellungen - Seite 1 */
#define IDS_WPSAM_INIT          161     /* Einstellungen - Init OS/2 */

/* Environment */
#define ENVNUM                  5       /* Anzahl der Variablen         */
#define IDS_ENV1                200     /* Variablenname                */
#define IDS_DEF_ENV1            201     /* Variablen Defaultwert        */
#define IDS_ENV2                202
#define IDS_DEF_ENV2            203     /* Alle Werte mÅssen direkt     */
#define IDS_ENV3                204     /* aufeinanderfolgen            */
#define IDS_DEF_ENV3            205
#define IDS_ENV4                206
#define IDS_DEF_ENV4            207
#define IDS_ENV5                208
#define IDS_DEF_ENV5            209

/* Die folgenden benutzerspezifischen Popup-MenÅ EintrÑge (ID_xxx) sollten
 * hîher als WPMENUID_USER (0x6500) sein.
 */
#define ID_SHELL            0x6501
#define IDM_OPENWP          0x6501
#define OPEN_WP             OPEN_USER + 1

#define ID_ABOUT            0x6502
#define IDM_ABOUT           0x6502

#define ID_OPENDEBUG        0x6520
#define IDM_OPENDEBUG       0x6520
#define PRIMARY_DEBUG       OPEN_USER + 20

