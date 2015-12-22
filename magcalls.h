/***************************************************************************
 *
 * FILENAME: MAGCALLS.H
 * ---------           
 *
 * VERSION: 1.2
 * --------
 *
 * BESCHREIBUNG:
 * -------------
 *   C-Include-Datei fÅr das Interface zwischen MAGCALLS.DLL und
 *   dem Anwenderprogramm. Wird fÅr Anwendungsentwicklung
 *   benîtigt.
 *
 * COMPILE REQUIREMENTS:
 * ---------------------
 *   Microsoft C 5.1 oder C 6.0 bzw. IBM C Set++ for OS/2
 *   OS/2 Linker LINK.EXE bzw. LINK386.EXE
 *
 * BENôTIGTE FILES:
 * ----------------
 *
 * BENôTIGTE BIBLIOTHEKEN:
 * -----------------------
 *   MAGCALLS.LIB - Import-Library
 *
 *  Copyright (C) 1993 Noller & Breining Software 1993, 1994
 *
 ******************************************************************************/

/* Definitionen fÅr Thunking bei 32-bit Applikationen */
#ifndef __32BIT__
#define APIRET16    USHORT
#define APIENTRY16  APIENTRY
#endif

/***** allgemeine Definitionen *****/
#define MODE_RETURN_ALL 0
#define MODE_RETURN_ONE 1

typedef SHANDLE HMAG;
typedef HMAG *PHMAG;

typedef struct _TRACKPARM
    {
    BYTE bParity;
    BYTE bCharDef;
    BYTE bPSOMChar;
    BYTE bASOMChar;
    BYTE bPEOMChar;
    BYTE bAEOMChar;
    BYTE bFormCtl;
    BYTE bZeroNum;
    } TRACKPARM;
typedef struct _TRACKPARMS
    {
    TRACKPARM trk1prm;
    TRACKPARM trk2prm;
    TRACKPARM trk3prm;
    } TRACKPARMS;
typedef TRACKPARMS *PTRACKPARMS;

typedef USHORT OPMODE;

#define SLEEP_INTERVAL 100          /* Zeit in ms fÅr Warteintervall auf Daten */

/* Konfigurations-Status */
#define CFG_PARMERR     0x8000      /* Parameterfehler in CONFIG.SYS */
#define CFG_NOHARDW     0x4000      /* keine Hardware gefunden */
#define CFG_PONERRS     0x2000      /* Power-on Errors */
#define CFG_DDERROR     0x1000      /* Device-Driver nicht geladen */
#define CFG_READT1      0x0040      /* Track 1 kann gelesen werden */
#define CFG_READT2      0x0020      /* Track 2 kann gelesen werden */
#define CFG_READT3      0x0010      /* Track 3 kann gelesen werden */
#define CFG_ENCT1       0x0004      /* Track 1 kann geschrieben werden */
#define CFG_ENCT2       0x0002      /* Track 2 kann geschrieben werden */
#define CFG_ENCT3       0x0001      /* Track 3 kann geschrieben werden */

/* ReadTracks Wort in MagReadData */
#define RDTRK_TRACK1    0x04            /* Der Funktionsaufruf liest Track 1 */
#define RDTRK_TRACK2    0x02            /* Der Funktionsaufruf liest Track 2 */
#define RDTRK_TRACK3    0x01            /* Der Funktionsaufruf liest Track 3 */

/* READ DATA Format */
typedef struct _READDATA
    {
    BYTE LVal;                          /* Record length */
    BYTE SVal;                          /* Status field */
    BYTE bBuffer;                       /* Data Area */
    } READDATA;
typedef READDATA *PREADDATA;

/* ERRORSTAT Struktur */
typedef struct _ERRORSTAT
    {
    BYTE bcrTrk1;                       /* Track-1 read error count */
    BYTE bcrTrk2;                       /* Track-2 read error count */
    BYTE bcrTrk3;                       /* Track-3 read error count */
    BYTE bceTrk1;                       /* Track-1 encode error count */
    BYTE bceTrk2;                       /* Track-2 encode error count */
    BYTE bceTrk3;                       /* Track-3 encode error count */
    } ERRORSTAT;
typedef ERRORSTAT *PERRORSTAT;

#define SVAL_BLANK      0x80            /* Track ist leer, kein SOM gefunden */
#define SVAL_READERR    0x40            /* Lesefehler aufgetreten */
#define SVAL_TRACK1     0x04            /* Daten von Track 1 */
#define SVAL_TRACK2     0x02            /* Daten von Track 2 */
#define SVAL_TRACK3     0x01            /* Daten von Track 3 */

/* Die folgenden Zeilen fÅr H2INC auskommentieren */
#if (RDTRK_TRACK1!=SVAL_TRACK1)
    (RDTRK_TRACK2!=SVAL_TRACK2)
    (RDTRK_TRACK3!=SVAL_TRACK3)
#   error RDTRK_value does not match SVAL_value
#endif

APIRET16 APIENTRY16 MagOpen (PHMAG);
APIRET16 APIENTRY16 MagClose (HMAG);
APIRET16 APIENTRY16 MagLoadDevParms (HMAG, PTRACKPARMS);
APIRET16 APIENTRY16 MagSetOperationMode (HMAG, OPMODE);
APIRET16 APIENTRY16 MagEncodeData (HMAG, USHORT, PBYTE);
APIRET16 APIENTRY16 MagResetDevice (HMAG);
APIRET16 APIENTRY16 MagAbort (HMAG);
APIRET16 APIENTRY16 MagReadDevParms (HMAG, PTRACKPARMS, PUSHORT);
APIRET16 APIENTRY16 MagReadErrorStats (HMAG, PERRORSTAT, PUSHORT);
APIRET16 APIENTRY16 MagReadConfigStatus (HMAG, PUSHORT);
APIRET16 APIENTRY16 MagReadData (HMAG, USHORT, PBYTE, PUSHORT);

/* Fehlercodes */
#define MAGERR_OK           0       /* Kein Fehler */
#define MAGERR_OPENERR      110     /* Fehler beim OPEN */  // OPEN
#define MAGERR_NOTATTCH     1538    /* keine Hardware */    // ENCODE,RESET,READ
#define MAGERR_HARDERR      1539    /* Hardwarefehler */    // ENCODE,READ
#define MAGERR_HINVALID     1540    /* ungÅltiges Handle */ // CLOSE,LOAD,MODE,ENCODE,RESET,ABORT,RMODE,ERROR,CONFIG,READ
#define MAGERR_NOTAVL       1541    /* nicht verfÅgbar */   // LOAD,MODE,ENCODE,RESET,READ
#define MAGERR_RDTRACK      1542    /* ungÅltiger Track */  // READ
#define MAGERR_WRTRACK      1543    /* ungÅltiger Track */  // ENCODE
#define MAGERR_ERRENC       1544    /* Datenfehler */       // ENCODE
#define MAGERR_T1PINV       1545    /* Trk1 Prm ungÅltig */ // LOAD
#define MAGERR_T2PINV       1546    /* Trk2 Prm ungÅltig */ // LOAD
#define MAGERR_T3PINV       1547    /* Trk3 Prm ungÅltig */ // LOAD
#define MAGERR_BUFFSIZ      1548    /* Puffer zu klein */   // ENCODE,RMODE,ERROR,READ
#define MAGERR_ERRTEST      1549    /* Selbsttestfehler */  // RESET
#define MAGERR_ENDEDAPP     1550    /* Ende durch Appl. */  // ENCODE,READ
#define MAGERR_ENDEDDLL     1551    /* Ende durch DLL */    // OPEN,ENCODE,RESET,ABORT,READ

