/* Deinstallation of old versions of WPSam */
echo off

Call RxFuncadd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
Call SysLoadFuncs

/* Abfrage: WPSam-Pfad */
boot:
say 'Bitte Pfad von WPSAM.DLL eingeben: '
pull drive
path=drive'\WPSAM*.DLL'

rc = SysFileTree(path, search, 'F', '*****', '*----')
if search.0=0 then
  do
  say ''
  say 'Datei nicht gefunden! Pfad m�glicherweise falsch.'
  signal boot
  end
say search.0
say search.1

say '����������������������������������������Ŀ'
say '� Deregister old Workplace shell classes �'
say '������������������������������������������'
call SysDeregisterObjectClass "WPSam";
if result then
  say 'Class WPSam deregistered'
else
  say 'No class WPSam found'
call SysDeregisterObjectClass "WPSamF";
if result then
  say 'Class WPSamF deregisterd'
else
  say 'No class WPSamF found'

say '������������������������������Ŀ'
say '� Kill PMSHELL to free modules �'
say '��������������������������������'
Killem PMShell

say ''
say '������������������Ŀ'
say '� Delete DLL-files �'
say '��������������������'
'del' path ' > nul'

beep(3000,100)
beep(2832,100)
beep(3000,100)

exit

