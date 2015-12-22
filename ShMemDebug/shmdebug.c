/***************************************************************************
 *
 * PROGRAM NAME: SHMDEBUG.C
 * -------------
 *
 * REVISION LEVEL: 1.0
 * ---------------
 *
 * WHAT THIS PROGRAM DOES:
 * -----------------------
 *  Anzeigen der Werte im WPSam-Shared-Memory
 *
 * ROUTINES:
 * ---------
 *
 * COMPILE REQUIREMENTS:
 * ---------------------
 *  IBM C/C++ Set/2 Compiler Version 2.0
 *  IBM OS/2 2.1 Programmers Toolkit
 *
 * REQUIRED FILES:
 * ---------------
 *
 * REQUIRED LIBRARIES:
 * -------------------
 *  OS2386.LIB    -   OS/2 32-Bit import library
 *  CUTIL.LIB     -   Hilfsroutinen
 *
 * CHANGE LOG:
 * -----------
 * 
 *  Ver.    Date      Comment
 *  ----    --------  -------
 *
 *  Copyright (C) 1995 Noller & Breining Software
 *
 ******************************************************************************/
#define INCL_WIN
#define INCL_PM
#define INCL_DOS
#define INCL_DOSSESMGR      /* Session Manager values */
#define INCL_DOSQUEUES      /* Queue values */
#define INCL_DOSERRORS

#include <os2.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include "..\samglob.h"

/* Globale Variablen */
PSHARE1  pShareMem = 0;

int main (void)
    {

    if (!(DosGetNamedSharedMem ((PPVOID) &pShareMem,
                              SHARE_INITOS2,
                              PAG_READ | PAG_WRITE)))
        {
        printf ("Contents of shared memory:\n");
        printf ("pszRegFile:       %s\n", pShareMem->pszRegFile);
        printf ("pszRootUserIni:   %s\n", pShareMem->pszRootUserIni);
        printf ("pszRootSystemIni: %s\n", pShareMem->pszRootSystemIni);
        printf ("ulFlag_Root:      %X\n", pShareMem->ulFlag_Root);
        printf ("pszUserIni:       %s\n", pShareMem->pszUserIni);
        printf ("pszSystemIni:     %s\n", pShareMem->pszSystemIni);
        printf ("pszEnvironment:   %s\n", pShareMem->pszEnvironment);
        printf ("ulFlag:           %X\n", pShareMem->ulFlag);
        printf ("ulActive:         %X\n", pShareMem->ulActive);
        printf ("hwndSammy:        %X\n", pShareMem->hwndSammy);
        printf ("ulMsgLogout:      %X\n", pShareMem->ulMsgLogout);
        }

    else
        printf ("Shared memory not found!\n");

    return 0;
    }

