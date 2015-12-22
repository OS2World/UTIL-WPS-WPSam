
/******************************************************************************
 *  Dialogboxen
 ******************************************************************************/
#define IDD_DEBUG               1015
#define IDD_MESSAGEBOX          1016
#define IDD_CONTEXTMENU         1020
#define IDD_WAIT_PRFRESET       1030
#define IDD_PRFRESET            1031
#define DID_HELP                1040
#define DID_DEFAULT             1041

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

/* CtxtDlgProc (IDD_CONTEXTMENU) */
#define DID_SORTENABLE          470
#define DID_ARRANGEENABLE       471
#define DID_SHUTDOWNENABLE      472
#define DID_LOCKUPENABLE        473
#define DID_ORIGDESKENABLE      474

/******************************************************************************
 *  Strings
 ******************************************************************************/
#define IDS_TITLE_WPSAMF        101

/* Warnings */
#define IDS_INFORMATION         110
#define IDS_ALREADYINROOT       125

/* Errors */
#define IDS_SAVESTATEFAILED     130
#define IDS_PRFRESETERROR       133

/* Namen der Tab-Marken */
#define CCHMAXPGNAME            15      /* maximale LÑnge der Namen */
#define IDS_WPSAMF_PG1          162     /* Einstellungen - KontextmenÅ */

/* Die folgenden benutzerspezifischen Popup-MenÅ EintrÑge (ID_xxx) sollten
 * hîher als WPMENUID_USER (0x6500) sein.
 */
#define ID_ORIGDESK         0x6510                  /* grî·er WPMENUID_USER */
#define IDM_ORIGDESK        0x6510                  /* grî·er WPMENUID_USER */
#define PRIMARY_ORIGDESK    OPEN_USER + 10

#define ID_OPENDEBUG        0x6520                  /* grî·er WPMENUID_USER */
#define IDM_OPENDEBUG       0x6520                  /* grî·er WPMENUID_USER */
#define PRIMARY_DEBUG       OPEN_USER + 20

