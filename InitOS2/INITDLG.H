#define DID_DIALOG      10
#define DID_USER        11
#define DID_PWD         12
#define DID_SETUP       13
#define DID_DLGSET      14
#define DID_AUTOUSER    15
#define DID_TIMEOUT     16
#define DID_ADMINPWD    17
#define DID_PWDREPT     18
#define DID_TO          19
#define DID_GETPWD      20
#define DID_SHUTDOWN    21
#define DID_SD          22
#define DID_STRTDEF     23
#define DID_DELPWD      24
#define DID_MAKEINI     25
#define DID_TXTPWD      26

DLGTEMPLATE dlg[] =
    {
    {DC_DIALOG, DID_DIALOG, TITLE,    0,  0, 80, 25, 0},
    {DC_STATIC, -1, "InitOS/2 V. 1.1",
                                     4,   2, 15,  1, 0},
    {DC_STATIC, -1, "'ROOT' eingeben fÅr Standard-ArbeitsoberflÑche",
                                     10,  6, 46,  1, 0},
    {DC_STATIC, DID_TO, "",          61,  2, 17,  1, 0},
    {DC_STATIC, -1, "Benutzer:",     10,  9, 60,  1, 0},
    {DC_ENTRY,  DID_USER, "",        10, 10, 60,  1, ES_AUTOSCROLL},
    {DC_STATIC, DID_TXTPWD, "Pa·wort:",
                                     10, 12, 60,  1, 0},
    {DC_ENTRY,  DID_PWD, "",         10, 13, 60,  1, ES_UNREADABLE | ES_AUTOSCROLL},
    {DC_BUTTON, DID_OK, "OK",        10, 20,  6,  1, BS_DEFAULT},
    {DC_BUTTON, DID_STRTDEF, "Default starten",
                                     19, 20, 19,  1, 0},
    {DC_BUTTON, DID_SETUP, "Setup",  40, 20,  9,  1, 0},
    {DC_BUTTON, DID_SHUTDOWN, "Systemabschlu·",
                                     52, 20, 18,  1, 0},
    {DC_STATIC, -1,
    "Enter=<OK>    F2=<Default starten>    F9=<Setup>    F10=<Systemabschlu·>",
                                      4, 23, 73,  1, 0},

    {DC_DIALOG, DID_DLGSET, "Setup", 10,  5, 60, 15, 0},
    {DC_STATIC, -1, "Default-Benutzer",
                                      2,  3, 40,  1, 0},
    {DC_ENTRY,  DID_AUTOUSER, "",     2,  4, 40,  1, ES_AUTOSCROLL},
    {DC_STATIC, -1, "Timeout:",       2,  6,  9,  1, 0},
    {DC_ENTRY,  DID_TIMEOUT, "",     12,  6,  4,  1, 0},
    {DC_STATIC, -1, "Administrator Pa·wort:",
                                      2,  8, 23,  1, 0},
    {DC_ENTRY,  DID_ADMINPWD, "",    25,  8, 33,  1, ES_UNREADABLE | ES_AUTOSCROLL},
    {DC_STATIC, -1, "Wiederholung:",
                                      2, 10, 44,  1, 0},
    {DC_ENTRY,  DID_PWDREPT, "",     25, 10, 33,  1, ES_UNREADABLE | ES_AUTOSCROLL},
    {DC_BUTTON, DID_OK, "OK",         2, 12,  6,  1, BS_DEFAULT},
    {DC_BUTTON, DID_DELPWD, "Pa·wort lîschen",
                                     17, 12, 20,  1, 0},
    {DC_BUTTON, DID_CANCEL, "Abbruch",
                                     47, 12, 11,  1, 0},

    {DC_DIALOG, DID_GETPWD, "Pa·wort",
                                     20,  8, 40,  6, 0},
    {DC_ENTRY,  DID_PWD, "",          2,  2, 36,  1, ES_UNREADABLE | ES_AUTOSCROLL},
    {DC_BUTTON, DID_OK, "OK",         5,  4,  6,  1, BS_DEFAULT},

    {DC_DIALOG, DID_SHUTDOWN, "Systemabschlu·",
                                      0,  0, 80, 25, 0},
    {DC_STATIC, DID_SD, "System wird angehalten...",
                                     20, 11, 40,  1, 0},

    {DC_DIALOG, DID_MAKEINI, "",     20, 10, 40,  5, 0},
    {DC_STATIC, -1, "Benutzer-Initialisierung...",
                                      4,  2, 34,  1, 0},

    {DC_DIALOG, 0, "",                0,  0,  0,  0, 0}        /* Terminierung */
    };

#define TXT_ERROR           "Fehler"
#define TXT_NOUSER          "kein Benutzer angegeben!"
#define TXT_USERNOTFOUND    "Benutzer nicht gefunden!"
#define TXT_WRONGPASSWD     "falsches Pa·wort angegeben!"
#define TXT_USERDESTROYED   "Benutzer mîglicherweise fehlerhaft!"
#define TXT_SHELLNOTFOUND \
"Die Shell, die in der PMSHELL-Anweisung angegeben ist, konnte konnte nicht gefunden werden"

#define TXT_SETUP           "Initialisierung"
#define TXT_WRONGPASSWORD   "Falsches Pa·wort"
#define TXT_PRFRESET        "Profil umgeschaltet"
#define TXT_DIFFPWD         "Passwîrter stimmen nicht Åberein. Noch einmal versuchen!"
#define TXT_TIMEOUT         "Timeout in %d s"

#define TXT_SHUTDOWN        "Systemabschlu· beendet..."
#define TXT_REGISTER        "Programm nicht registriert, Benutzer wird nicht gestartet"

#define TXT_DELPWD          "Administrator-Pa·wort gelîscht!"
#define TXT_PWDMAG          "Pa·wort oder Magnetkarte:"

/* Texte fÅr thCreate */

#define TXT_NOUSERRESOURCE      "Benutzer-Resource nicht gefunden!"
#define TXT_NOSYSTEMRESOURCE    "System-Resource nicht gefunden!"
#define TXT_NOUSERINICREATED    "Benutzerprofil konnte nicht erzeugt werden!"
#define TXT_NOSYSTEMINICREATED  "Systemprofil konnte nicht erzeugt werden!"
#define TXT_NODESKINRC          "Fehlende oder falsche Definition der ArbeitsoberflÑche in Resource-Datei!"
#define TXT_PATHTOOLONG         "Der Pfadname des Benutzerverzeichnisses ist zu lang!"
#define TXT_DESKTOP             "ArbeitsoberflÑche_"

