.*----------------------------------------------------------
.* WPSAM -- Workplace Shell Administration
.*
.* IPF Help File
.*
.*----------------------------------------------------------

:userdoc.

.*----------------------------------------------------------
.*  'Set/Change/Enable Paáwort' Hilfeseiten
.*      res = PANEL_PWD
.*----------------------------------------------------------
:h1 res=40.Paáwort „ndern und aktivieren 
:i1 id=PWD.Paáwort

:p.Arbeitsoberfl„chen k”nnen ber ein frei w„hlbares Paáwort geschtzt werden.
Das Paáwort darf Leerzeichen enthalten, Groá- und Kleinbuchstaben werden
unterschieden.

:p.Wenn ihr Ger„t mit einem Magnetkartenleser ausgestattet ist, kann die
Eingabe wahlweise ber Tastatur oder ber eine Identifikationskarte erfolgen.

:p.Der Schutz erstreckt sich auf
:ul compact.
:li.™ffnen, 
:li.Starten und 
:li.L”schen der Arbeitsoberfl„che.
:eul.

:p.Unbefugte Zugriffsversuche werden im
:link reftype=fn refid=fnProtokoll.Protokollfeld:elink.
eingetragen.
:fn id=fnProtokoll.
:p.:lines align=center.:hp2.Protokoll:ehp2.:elines.
:p.Das Protokollfeld befindet sich in den :hp1.Einstellungen:ehp1.
unter dem Menpunkt :hp1.Datei:ehp1..
:efn.

.*----------------------------------------------------------
.*  'About' Hilfeseiten
.*      res = PANEL_ABOUT
.*----------------------------------------------------------
:h1 res=10 hide.Produktinformation / Registrierung

:p.Von WPSam erstellte Arbeitsoberfl„chen arbeiten ab einem Alter von 3 Wochen nur
noch unter der registrierten Version des WPSam.
:p.Ist Ihre Version des WPSam noch nicht registriert, so enth„lt diese Seite alle
fr eine
:link reftype=fn refid=fnRegForm.Registrierung:elink.
notwendigen Eingabefelder.

:fn id=fnRegForm.
:p.:lines align=center.:hp2.Bestellformular:ehp2.:elines.
:p.Ein Bestellformular befindet sich in der Datei :hp4.WPSam.inf:ehp4..
:efn.


.*----------------------------------------------------------
.*  'Adjustments' Hilfeseiten
.*      res = PANEL_ADJUSTMENTS
.*----------------------------------------------------------
:h1 res=50.Umgebungsvariable und Extras

:p.:hp2.Umgebungsvariable:ehp2.
:p.Jede Arbeitsoberfl„che bekommt die in der Datei :hp1.Config.sys:ehp1. angegebenen
:link reftype=fn refid=fnEnvVar.Umgebungsvariablen:elink.
bergeben.
Diese k”nnen hier berschrieben, ge„ndert und erg„nzt werden.
Sie werden gltig, sobald die zugeh”rige Arbeitsoberfl„che das n„chste mal gestartet wird.

:fn id=fnEnvVar.
Jede Umgebungsvariable muá in einer eigenen Zeile stehen und von dem ihr folgenden
Zuweisungswert durch ein Gleichheitszeichen getrennt sein.

:p.Der in der Datei :hp1.Config.sys:ehp1. notwendige Befehl :hp1.SET:ehp1.
muá hier weggelassen werden.
:efn.

:p.:hp2.Extras:ehp2.
:p.Mit diesen Einstellungen l„át sich der Zugriff auf die Arbeitsoberfl„che einschr„nken.
Um sie umbenennen, verschieben oder um etwas in sie
hineinziehen zu k”nnen
(:link reftype=fn refid=fnDropping.dropping:elink.)
mssen die jeweiligen Optionen aktiviert sein.
:p.Die Optionen sind nur fr Operationen auf der Arbeitsoberfl„che wirksam. Der
:hp4.move:ehp4. oder :hp4.ren:ehp4. Befehl der Kommandozeile l„át sich hier nicht
unterbinden.

:fn id=fnDropping.
:p.Eine :hp4.WPSam:ehp4.-Arbeitsoberfl„che ist ein Ordner, der das benutzerspezifische Profil und dessen 
zugeh”rige Arbeitsoberfl„chen-Struktur beherbergt.
Durch die Option :hp2.dropping:ehp2. kann die M”glichkeit, etwas in diesen Ordner
zu ziehen oder zu kopieren aktiviert werden.

:p.Um Objekte zwischen verschiedenen Arbeitsoberfl„chen zu verschieben,
ist zun„chst die Symbolanzeige der nicht aktiven Arbeitsoberfl„che zu ”ffnen (hierzu den Menpunkt
:hp1.”ffnen->Symbolanzeige:ehp1. ausw„hlen) um dann die zugeh”rige
Arbeitsoberfl„che durch Doppelklicken ebenfalls zu ”ffnen.

:p.:p.:hp2.Anmerkung:ehp2.
:p.Sogenannte :hp4.abstrakte Objekte,:ehp4. wie z.B.: Referenzen, Druckerobjekte,
abstrakte Programmobjekte etc. verwalten ihre Instanzdaten im Benutzerprofil (:hp1.OS2.INI:ehp1.).
Um solche Objekte zwischen verschiedenen Arbeitsoberfl„chen zu verschieben, máten
deren Profile gleichzeitig ge”ffnet sein.
:p.Da der Pr„sentation-Manager von OS/2 das gleichzeitige ™ffnen
verschiedener Benutzerprofile nicht untersttzt, ist das Verschieben
abstrakter Objekte zu einer anderen Arbeitsoberfl„che nicht
m”glich!
:efn.

:p.:p.:hp2.Anwendungen schlieáen:ehp2.
:p.Alle gestarteten Anwendungen werden beim Umschalten auf eine 
andere Arbeitsoberfl„che automatisch wieder geschlossen.
:p.Dieses Feld wird beim n„chsten Start der zugeh”rigen Arbeitsoberfl„che wirksam.

.*----------------------------------------------------------
.*  Resource-Dateien
.*      res = PANEL_CREATE
.*----------------------------------------------------------
:h1 res=60.Resource-Dateien

:p.Auf dieser Seite k”nnen die Resource-Dateien angegeben werden, aus denen die
Profile generiert werden.

:p.In der Grundeinstellung sind hier Resource-Dateien eingetragen, die w„hrend
der Betriebssysteminstallation generiert worden sind.

:p.:p.:hp2.Anmerkung:ehp2.
:p.Das Systemprofil wird w„hrend des Bootens geladen und kann im laufenden
Betrieb nicht mehr umgeschaltet werden.
.br
Arbeitsoberfl„chen mit privatem Systemprofil k”nnen daher nur w„hrend dem Bootvorgang
durch das Programm InitOS2 geladen werden.

.*----------------------------------------------------------
.*  'INITOS2' Hilfeseiten
.*      res = PANEL_INITOS2
.*----------------------------------------------------------
:h1 res=70.InitOS/2

:p.:hp2.ProtShell:ehp2.
:p.Fr Benutzer, die w„hrend des Bootvorgangs aus dem InitOS/2-Men gestartet werden,
kann hier eine ProtShell-Anweisung angegeben werden, die von der in der
:hp1.CONFIG.SYS:ehp1.-Datei angegebenen abweicht.

:p.Wird hier beispielsweise der OS/2-Befehlsprozessor :hp1.CMD.EXE:ehp1. eingetragen,
so wird beim Booten nicht der Pr„sentations-Manager, sondern lediglich eine
OS/2-Gesamtbildschirm Sitzung geladen.

.*----------------------------------------------------------
.*  'Paáwort' Hilfeseiten
.*      res = PANEL_PWDIN
.*----------------------------------------------------------
:h1 res=30 hide nosearch.Paáwort

:p.Diese Arbeitsoberfl„che ist durch ein Paáwort geschtzt.
.br
Bitte geben Sie es ein, damit die gewnschte Aktion ausgefhrt werden kann.

:p.Wenn ihr Ger„t mit einem Magnetkartenleser ausgestattet ist, kann die
Eingabe wahlweise ber Tastatur oder ber eine Identifikationskarte erfolgen.

:p.Unbefugte Zugriffsversuche werden im
:link reftype=fn refid=fnProtokoll.
Protokollfeld
:elink.
eingetragen.


.*----------------------------------------------------------
.*  'Context menu' Hilfeseiten
.*      res = PANEL_CONTEXTMENU
.*----------------------------------------------------------
:h1 res=80.Kontext Men

:p.Diese Seite wird durch das Programm WPSam
bereitgestellt, um unliebsame Menpunkte aus dem prim„ren
:link reftype=fn refid=fnKontext.
Kontextmen
:elink.
der Arbeitsoberfl„che zu entfernen.

:fn id=fnKontext.
Das :hp4.prim„re Kontextmen der Arbeitsoberfl„che :ehp4. erreichen Sie durch
Anklicken der Arbeitsoberfl„che mit der rechten Maustaste.
:efn.


:euserdoc.

