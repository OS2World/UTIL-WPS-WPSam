/******************************************************************************
*
*  Module Name: WPSam
*
*  OS/2 Workplace Shell Access Manager
*
******************************************************************************/



#******************************************************************************
#   class definition file for the parent class: WPFolder
#******************************************************************************

include <WPFolder.sc>



#******************************************************************************
#   Define the new class
#******************************************************************************

class: WPSam,
       external stem   = wpsam,
       external prefix = wpsam_,
       classprefix     = wpsamM_,
       major version   = 1,
       minor version   = 1,
       local;

--
-- CLASS: WPSam
--
-- CLASS HIERARCHY:
--
--     SOMObject
--       ¿ƒƒ WPObject
--             ¿ƒƒ WPFileSystem
--                   ¿ƒƒ WPFolder
--                         ¿ƒƒ WPSam
--



#******************************************************************************
#   Specify the parent class
#******************************************************************************

parent: WPFolder;



#******************************************************************************
#   Specify the release order of new methods.  This ensures binary
#   compatability if we change the order of our methods in .c and .csc files.
#******************************************************************************

release order:
    CheckPassword,
    RefreshStyle,
    CommonSetup,
    ProfilesCreated,
    MyHistory,
    clsQueryModuleHandle,
    clsIsShareware,
    clsQueryRegFile,
    clsAbout;


#******************************************************************************
#   Passthru PRIVATE definitions to the .ph file
#******************************************************************************

passthru: C.ph;

   /*
    *   The following structures will be used to store window specific data
    *   and a pointer to the object that created the window/dialog.
    */

  typedef struct _WINDOWDATA
      {
      WPSam      *somSelf;        /* pointer to this instance */
      } WINDOWDATA;
  typedef WINDOWDATA *PWINDOWDATA;


    #define GetResourceString(Id, size, sz) \
        WinLoadString(WinQueryAnchorBlock (HWND_DESKTOP), hmod, Id, size, sz);

endpassthru;   /* C.ph */



#******************************************************************************
#   Passthru IMPLEMENTATION definitions to the .ih file
#******************************************************************************

passthru: C.ih;

#define INCL_WIN
#define INCL_DOS
#define INCL_PM
#define INCL_WPCLASS
#define INCL_WPFOLDER

#include <os2.h>
#include <samglob.h>
#include <crypt.h>
#include "wpsamdlg.h"

#define ENV_PROTSHELL_FS        "OS2_SHELL"     /* Env. fÅr Default-FullScreen-Protshell */
#define HELPFILE                "WPSam.HLP"     /* Name der Hilfedatei */
#define CLASSNAME               "WPSam"         /* Name der Klasse */
#define MAGCALLS                "MagCALLs"      /* DLL fÅr DESKO */

/* keyvalues fÅr die _wpSaveState-Methode */
#define KEY_FLAG                1       /* _ulFlag                  */
#define KEY_PASSWORD            2       /* _szPassword              */
#define KEY_WRONGPWDCOUNT       3       /* ZÑhlt falsche Passwîrter */
#define KEY_ENVIRONMENT         4       /* _szEnvironment           */
#define KEY_USERRESOURCE        5       /* _szUserResource          */
#define KEY_SYSTEMRESOURCE      6       /* _szSystemResource        */
#define KEY_USERPROFILE         7       /* IniDateiname             */
#define KEY_SYSTEMPROFILE       8       /* SystemIniDateiname       */
#define KEY_PROTSHELL           9       /* _szProtShell             */

#define THREADSTACK     0x8000

APIRET16 (* APIENTRY16 pfnMagOpen) (PHMAG);
APIRET16 (* APIENTRY16 pfnMagClose) (HMAG);
APIRET16 (* APIENTRY16 pfnMagSetOperationMode) (HMAG, OPMODE);
APIRET16 (* APIENTRY16 pfnMagResetDevice) (HMAG);
APIRET16 (* APIENTRY16 pfnMagAbort) (HMAG);
APIRET16 (* APIENTRY16 pfnMagReadData) (HMAG, USHORT, PBYTE, PUSHORT);
APIRET16 (* APIENTRY16 pfnMagReadConfigStatus) (HMAG, PUSHORT);
APIRET16 GetHMag (PHMAG);

MRESULT EXPENTRY InitOS2DlgProc (HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY CreateDlgProc (HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY AdjustDlgProc (HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY DbgDlgProc (HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY PwdNewDlgProc (HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY PwdDlgProc (HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY ProfilesDlgProc (HWND, ULONG, MPARAM, MPARAM);
APIRET  APIENTRY thCreate (PUSER);
APIRET  APIENTRY thChangeWPS (PPRFPROFILE pprfProfile);
MRESULT EXPENTRY AboutDlgProc (HWND, ULONG, MPARAM, MPARAM);
void             MessageBox (ULONG, ULONG, ULONG);

endpassthru;   /* .ih */



#******************************************************************************
#   Passthru PUBLIC definitions to the .h file
#******************************************************************************

passthru: C.h, after;

endpassthru;   /* C.h */



#******************************************************************************
#   Define instance data
#******************************************************************************

data:

ULONG   ulFlag;
CHAR    *szPassword;
ULONG   ulWrongPwdCount;
CHAR    *szEnvironment;
CHAR    *szUserResource;   /* Name der UserResource-Datei mit Pfad, mit Extension */
CHAR    *szSystemResource; /* Name der SystemResource-Datei mit Pfad, mit Extension */
CHAR    *szUserProfile;    /* Name der UserINI-Datei ohne Pfad, mit Extension */
CHAR    *szSystemProfile;  /* Name der SystemINI-Datei ohne Pfad, mit Extension */
CHAR    *szProtShell;      /* Name der ProtShell, falls anders als in Config.sys */
CHAR    *szRegUser,     class;
CHAR    *szRegPassword, class;


#******************************************************************************
#   Define methods
#******************************************************************************

methods:

BOOL    CheckPassword ();
BOOL    RefreshStyle ();
BOOL    CommonSetup ();
BOOL    ProfilesCreated ();
BOOL    MyHistory(ULONG ulIDS);

override wpInitData;
override wpUnInitData;
override wpSetup;
override wpCopiedFromTemplate;
override wpSaveState;
override wpRestoreState;
override wpOpen;
override wpDelete;
override wpConfirmDelete;
override wpAddSettingsPages;
override wpModifyPopupMenu;
override wpMenuItemSelected;
override wpMenuItemHelpSelected;

HMODULE clsQueryModuleHandle (), class;
BOOL    clsIsShareware (), class;
void    clsAbout (), class;
PCHAR   clsQueryRegFile (), class;

override wpclsInitData, class;
override wpclsUnInitData, class;
override wpclsQueryDefaultView, class;
override wpclsQueryIconData, class;
override wpclsQueryStyle, class;
override wpclsQueryTitle, class;
