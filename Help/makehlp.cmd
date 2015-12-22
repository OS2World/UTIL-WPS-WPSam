/* Hilfedateien fÅr WPSam erzeugen */
/* Benutzung: MAKEHLP [GR|US]      */
/* Default ist US                  */
arg lang
if \(lang='US' | lang='GR') then
  lang = 'US'
say 'Language is 'lang
'd:\devtools\ipfcpp\ipfcprep wpsam'lang'.scr wpsam'lang'.ipf'
'ipfc wpsam'lang'.ipf'

