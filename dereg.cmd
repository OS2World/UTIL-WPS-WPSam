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

exit
