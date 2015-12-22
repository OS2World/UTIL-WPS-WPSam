/*******************************************************************
   Definitionen fÅr INITOS2
   OS/2 - Workplace Shell Access Manager Version 2.1 - 20.7.1991
 *******************************************************************/

#include <samglob.h>

#define WPSam_MajorVersion  1           /* VersionsprÅfung in INITOS2: Major */
#define WPSam_MinorVersion  1           /* VersionsprÅfung in INITOS2: Minor */

#define APPNAME     "InitOS2"           /* Applikationsname */
#define SAMENVVAR   "SAM"
#define ROOTSHELL   "Root"

#define DIR_ALL     "\\*"

/* Timer 1 - Definitionen */
#define ID_TIMER        1               /* Timer Id */
#define DEFTIMEOUT      10000           /* Default-Timeout in ms */
#define CCHMAXNUM       4               /* LÑnge der Timeoutzeit (incl. '\0') */

/* Definitionen fÅr erweiterte Attribute */
#define CCHMAXEALEN     256             /* Maximale LÑnge eines EAT_ASCII-EAs */
#define EA_DEFUSER      "INITOS2.DEFUSER"
#define EA_DEFTIMEOUT   "INITOS2.DEFTIMEOUT"
#define EA_ADMINPWD     "INITOS2.PASSWD"

/* Benutzerstatus */
#define STAT_PASSWORDOK     0           /* Pa·wort ok. */
#define STAT_NOPASSWORD     1           /* kein Pa·wort benîtigt */
#define STAT_ROOTUSER       2           /* Default-Benutzer starten */
#define STAT_VERIFYFAILED   3           /* Problem beim Laden der Parameter */
#define STAT_NOUSERFOUND    4           /* Kein Benutzer gefunden */
#define STAT_USERNOTFOUND   5           /* Benutzer nicht gefunden */
#define STAT_WRONGPASSWD    6           /* Pa·wort falsch */

#define USTAT_INITIALIZED   0x0001      /* Benutzer bereits initialisiert */
#define USTAT_PRIVATE       0x0002      /* privater Benutzer */

/* Benutzer-Messages */
#define DM_MAGDATA  DM_USER             /* Es wurden Daten einer Karte gelesen */
#define DM_GETUSERS DM_USER+1           /* Benutzerliste erzeugen */

