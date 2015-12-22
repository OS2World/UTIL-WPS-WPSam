/* Register classes WPSam and WPSamF */
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
say "Try to Register classes"
say ""

do until SysRegisterObjectClass("WPSam", wpsam)
   beep(1890,100)
   call SysSleep 1
   end

say ""
say 'WPSam class registered'

do until SysRegisterObjectClass("WPSamF", wpsamf)
   beep(2248,100)
   call SysSleep 1
   end

say ""
say 'WPSamF class registered'

beep(3000,100)
beep(2832,100)
beep(3000,100)

exit
