/*
 * 
 * 
 *   Module Name: SamNonM1
 * 
 *   OS/2 Workplace Shell Access Manager
 * 
 *   Container for userdefined desktops
 * 
 */


#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "WPSamF.ih"
#include "WPSamF.ph"

extern HMODULE hmod;
extern HMODULE hmodMag;    // Handle f�r Desko-DLL

/**************************  ORDINARY CODE SECTION  ***************************
*****                                                                     *****
*****                  Any non-method code should go here.                *****
*****                                                                     *****
******************************************************************************/
#undef SOM_CurrentClass

void MessageBox (ULONG idText, ULONG idTitle, ULONG flStyle)
    {
    CHAR  szText[CCHMAXMSG];
    CHAR  szTitle[CCHMAXMSG];
    PCHAR pszTitle;

   /* idTitle = 0             => kein Titel     */
   /* idTitle = MBTITLE_ERROR => Titel "Fehler" */
   /* idTitle   sonst         => Text ID        */

    WinLoadString (WinQueryAnchorBlock (HWND_DESKTOP), hmod,
        idText, CCHMAXMSG, szText);

    switch (idTitle)
        {
        case MBTITLE_ERROR:
            pszTitle = NULL;
            break;
        case 0:
            pszTitle = "";
            break;
        default:
            WinLoadString (WinQueryAnchorBlock (HWND_DESKTOP), hmod,
                idTitle, CCHMAXMSG, szTitle);
            pszTitle = szTitle;
        }

    WinMessageBox (HWND_DESKTOP, HWND_DESKTOP, szText,
        pszTitle, IDD_MESSAGEBOX, flStyle);

    return;
    }


ULONG Hours()
{
   ULONG ulTime[2];
/*
 * Der Wert von  erh�ht sich alle 1.13778 Stunden um den Wert 1
 * d. h. pro Tag um den Wert 21.0937.
 */

   DosQuerySysInfo(QSV_TIME_LOW, QSV_TIME_HIGH, ulTime, 8);

   return ( (ulTime[0] >> 12) | (ulTime[1] << 20) );
}

