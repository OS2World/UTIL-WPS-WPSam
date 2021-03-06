
/*
 * This file was generated by the SOM Compiler.
 * FileName: WPSAMF.ph.
 * Generated using:
 *     SOM Precompiler spc: 1.22
 *     SOM Emitter emitph: 1.10
 */

/*
 * 
 * 
 * 
 *   Module Name: WPSamFolder
 * 
 *   OS/2 Workplace Shell Access Manager
 * 
 *   Container for userdefined desktops
 * 
 */


#ifndef WPSAMF_ph
#define WPSAMF_ph


/*
 * Passthru lines: File: "C.ph", "before"
 */

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


#include "WPSAMF.h"

/*
 * Include any needed private header files
 */

#endif       /* WPSAMF_ph */

/*
 * This file was generated by the SOM Compiler.
 * FileName: WPSAMF.ph.
 * Generated using:
 *     SOM Precompiler spc: 1.22
 *     SOM Emitter emitph: 1.10
 */

#ifndef WPSAMF_mph
#define WPSAMF_mph


/*
 * Include any needed private header files
 */

#endif       /* WPSAMF_ph */
