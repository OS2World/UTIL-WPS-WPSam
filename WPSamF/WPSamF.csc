/*****************************************************************************
*
*  Module Name: WPSamFolder
*
*  OS/2 Workplace Shell Access Manager
*
*  Container for userdefined desktops
*
******************************************************************************/



#******************************************************************************
#   class definition file for the parent class: WPDesktop
#******************************************************************************

include <WPDesk.sc>



#******************************************************************************
#   Define the new class
#******************************************************************************

class: WPSamF,
       external stem   = WPSamF,
       external prefix = WPSamFolder_,
       classprefix     = WPSamFM_,
       major version   = 1,
       minor version   = 1,
       local;

--
-- CLASS: WPSamF
--
-- CLASS HIERARCHY:
--
--     SOMObject
--       ¿ƒƒ WPObject
--             ¿ƒƒ WPFileSystem
--                   ¿ƒƒ WPFolder
--                         ¿ƒƒ WPDesktop
--                               ¿ƒƒ WPSamF
--



#******************************************************************************
#   Specify the parent class
#******************************************************************************

parent: WPDesktop;



#******************************************************************************
#   Specify the release order of new methods.  This ensures binary
#   compatability if we change the order of our methods in .c and .csc files.
#******************************************************************************

release order:
    RootDesktop,
    clsQueryModuleHandle,
    clsIsShareware,
    clsAbout;


#******************************************************************************
#   Passthru PRIVATE definitions to the .ph file
#******************************************************************************

passthru: C.ph;

   /*
    *   The following structures will be used to store window specific data
    *   and a pointer to the object that created the window/dialog.
    *
    *   They're allocated when the Client window is created.  This
    *   allows us to pass the *somSelf pointer and use it in our
    *   window and dialog procedures (the system only passes this
    *   pointer to methods).
    */
  typedef struct _WINDOWDATA
      {
      WPSamF      *somSelf;       /* pointer to this instance */
      } WINDOWDATA;
  typedef WINDOWDATA *PWINDOWDATA;

endpassthru;   /* C.ph */



#******************************************************************************
#   Passthru IMPLEMENTATION definitions to the .ih file
#******************************************************************************

passthru: C.ih;

#define INCL_WPCLASS
#define INCL_WPFOLDER
#define INCL_WIN
#define INCL_DOS
#define INCL_PM
#include <os2.h>
#include <samglob.h>
#include "wpsamfdlg.h"

#define HELPFILE                "WPSAM.HLP"     /* Name der Hilfedatei */
#define CLASSNAME               "WPSamF"        /* Name der Klasse */

#define USER_INI                "USER_INI"
#define SYSTEM_INI              "SYSTEM_INI"

/* keyvalues fÅr die _wpSaveState-Methode */
#define KEY_ZEITPUNKT1          1       /* wird fÅr SHAREWARE benîtigt */
#define KEY_ZEITPUNKT2          2       /* wird fÅr SHAREWARE benîtigt */
#define KEY_FLAG                3       /* wird fÅr SHAREWARE benîtigt */

/* Flaggen fÅr die ulFlag-Variable */
#define SORTENABLE              0x0001  /* Sort enabled */
#define ARRANGEENABLE           0x0002  /* Arrange enabled */
#define SHUTDOWNENABLE          0x0004  /* Shutdown enabled */
#define LOCKUPENABLE            0x0008  /* Lockup enabled */
#define ORIGDESKENABLE          0x0010  /* Root desktop enabled */

#define THREADSTACK             0x8000

MRESULT EXPENTRY CtxtDlgProc (HWND, ULONG, MPARAM, MPARAM);
void             MessageBox (ULONG, ULONG, ULONG);
APIRET APIENTRY  thChangeWPS (PPRFPROFILE);
ULONG            Hours (void);

CHAR    szBuffer1[256]; /* Puffer fÅr z.B. WinLoadString, WinMessageBox etc. */

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

ULONG   ulZeitpunkt1;   /* FÅr Shareware */
ULONG   ulZeitpunkt2;   /* FÅr Shareware */
ULONG   ulChiffreKey;   /* FÅr Shareware */
ULONG   ulFlag;
/* Variablen die nicht abgespeichert werden */
ULONG   bCheckShare;   /* Wenn TRUE, wird in Open-Methode Shareware ÅberprÅft */



#******************************************************************************
#   Define methods
#******************************************************************************

methods:

BOOL   RootDesktop ();

override wpInitData;
override wpSaveState;
override wpRestoreState;
override wpAddSettingsPages;
override wpFilterPopupMenu;
override wpModifyPopupMenu;
override wpMenuItemSelected;
override wpMenuItemHelpSelected;
override wpOpen;

HMODULE  clsQueryModuleHandle (), class;
BOOL     clsIsShareware (), class;
void     clsAbout (), class;

override wpclsInitData, class;
override wpclsQueryTitle, class;
