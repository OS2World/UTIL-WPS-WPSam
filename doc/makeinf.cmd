/* Online-Hilfe fÅr WPSam erzeugen */
/* Benutzung: MAKEINF [GR|US]      */
/* Default ist US                  */
arg lang
if \(lang='US' | lang='GR') then
  lang = 'US'
say 'Language is 'lang
if (lang='GR') then
  args = '/L=DEU /COUNTR=049'
else
  args = ' '
'ipfc /inf /codepage=437 'args' wpsam'lang

