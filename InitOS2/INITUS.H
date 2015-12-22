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
    {DC_STATIC, -1, "Enter 'ROOT' for root desktop",
                                     10,  6, 46,  1, 0},
    {DC_STATIC, DID_TO, "",          61,  2, 17,  1, 0},
    {DC_STATIC, -1, "User:",         10,  9, 60,  1, 0},
    {DC_ENTRY,  DID_USER, "",        10, 10, 60,  1, ES_AUTOSCROLL},
    {DC_STATIC, DID_TXTPWD, "Password:",
                                     10, 12, 60,  1, 0},
    {DC_ENTRY,  DID_PWD, "",         10, 13, 60,  1, ES_UNREADABLE | ES_AUTOSCROLL},
    {DC_BUTTON, DID_OK, "OK",        10, 20,  6,  1, BS_DEFAULT},
    {DC_BUTTON, DID_STRTDEF, "Start Default",
                                     19, 20, 19,  1, 0},
    {DC_BUTTON, DID_SETUP, "Setup",  40, 20,  9,  1, 0},
    {DC_BUTTON, DID_SHUTDOWN, "Shutdown",
                                     52, 20, 18,  1, 0},
    {DC_STATIC, -1,
    "Enter=<OK>    F2=<Start Default>    F9=<Setup>    F10=<Shutdown>",
                                      4, 23, 73,  1, 0},

    {DC_DIALOG, DID_DLGSET, "Setup", 10,  5, 60, 15, 0},
    {DC_STATIC, -1, "Default user",
                                      2,  3, 40,  1, 0},
    {DC_ENTRY,  DID_AUTOUSER, "",     2,  4, 40,  1, ES_AUTOSCROLL},
    {DC_STATIC, -1, "Timeout:",       2,  6,  9,  1, 0},
    {DC_ENTRY,  DID_TIMEOUT, "",     12,  6,  4,  1, 0},
    {DC_STATIC, -1, "Administrator password:",
                                      2,  8, 23,  1, 0},
    {DC_ENTRY,  DID_ADMINPWD, "",    25,  8, 33,  1, ES_UNREADABLE | ES_AUTOSCROLL},
    {DC_STATIC, -1, "Verification:",
                                      2, 10, 44,  1, 0},
    {DC_ENTRY,  DID_PWDREPT, "",     25, 10, 33,  1, ES_UNREADABLE | ES_AUTOSCROLL},
    {DC_BUTTON, DID_OK, "OK",         2, 12,  6,  1, BS_DEFAULT},
    {DC_BUTTON, DID_DELPWD, "Delete password",
                                     17, 12, 20,  1, 0},
    {DC_BUTTON, DID_CANCEL, "Cancel",
                                     47, 12, 11,  1, 0},

    {DC_DIALOG, DID_GETPWD, "Password",
                                     20,  8, 40,  6, 0},
    {DC_ENTRY,  DID_PWD, "",          2,  2, 36,  1, ES_UNREADABLE | ES_AUTOSCROLL},
    {DC_BUTTON, DID_OK, "OK",         5,  4,  6,  1, BS_DEFAULT},

    {DC_DIALOG, DID_SHUTDOWN, "Shutdown",
                                      0,  0, 80, 25, 0},
    {DC_STATIC, DID_SD, "System termination in process...",
                                     20, 11, 40,  1, 0},

    {DC_DIALOG, DID_MAKEINI, "",     20, 10, 40,  5, 0},
    {DC_STATIC, -1, "User initialization in process...",
                                      3,  2, 34,  1, 0},

    {DC_DIALOG, 0, "",                0,  0,  0,  0, 0}        /* Terminierung */
    };

#define TXT_ERROR                   "Error"
#define TXT_NOUSER                  "No user entered!"
#define TXT_USERNOTFOUND            "User not found!"
#define TXT_WRONGPASSWD             "wrong password entered!"
#define TXT_USERDESTROYED           "User probably destroyed!"
#define TXT_SHELLNOTFOUND \
"The shell, pointed to by the PMSHELL statement, could not be found"

#define TXT_SETUP                   "Initialization"
#define TXT_WRONGPASSWORD           "Wrong password"
#define TXT_PRFRESET                "Profile switched"
#define TXT_DIFFPWD                 "Passwords are not corresponding. Please try again"
#define TXT_TIMEOUT                 "Timeout in %d s"

#define TXT_SHUTDOWN                "System halted..."
#define TXT_REGISTER                "Program not registered, User will not be started"

#define TXT_DELPWD                  "Administrator password deleted"
#define TXT_PWDMAG                  "Password or magnetic card"

/* Texte fÅr thCreate */
#define TXT_NOUSERRESOURCE      "User resource not found!"
#define TXT_NOSYSTEMRESOURCE    "System resource not found!"
#define TXT_NOUSERINICREATED    "The userprofile could not be created!"
#define TXT_NOSYSTEMINICREATED  "The systemprofile could not be created!"
#define TXT_NODESKINRC          "The Desktop definition in the resourcefile is missing or wrong!"
#define TXT_PATHTOOLONG         "The pathname of the user directory is too long!"
#define TXT_DESKTOP             "Desktop_"

