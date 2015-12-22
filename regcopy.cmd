/* Copy and Register classes WPSam and WPSamF */
echo off

Call RxFuncadd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
Call SysLoadFuncs

call SysDeregisterObjectClass "WPSam";
RetCode = result;
call SysDeregisterObjectClass "WPSamF";
RetCode = RetCode & result;

if RetCode then
  say 'Deregister successfully completed'
else
  say 'Could not deregister classes'

say ""
say "Try to copy DLL-Files"
say ""

call Copy

if rc > 0 then do
   Killem PMShell
   do until rc = 0
      call SysSleep 1
      call Copy
      end
   end

say ""
say "Try to Register classes"
say ""

do until SysRegisterObjectClass("WPSam", wpsam)
   beep(1890,100)
   call SysSleep 1
   end

say ""
say 'WPSam class registered'

do until SysRegisterObjectClass("WPSamF", wpsam)
   beep(2248,100)
   call SysSleep 1
   end

say ""
say 'WPSamF class registered'

beep(3000,100)
beep(2832,100)
beep(3000,100)

say 'Copy help file'
"copy wpsam\wpsam.hlp   c:\os2\help >nul"
"copy wpsamf\wpsamf.hlp c:\os2\help >nul"

exit

/* Unterprogramme */

Copy:
beep(1500,100)
"copy wpsam\WPSam.dll   c:\os2\dll\WPSam.dll  > nul"

if rc=0 then
    "copy wpsamf\WPSamF.dll c:\os2\dll\WPSamF.dll > nul"

return rc

