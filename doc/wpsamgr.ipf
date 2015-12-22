.*----------------------------------------------------------
.* WPSAM -- Workplace Shell Access Manager
.*
.* IPF Dokumentation
.*
.* Version 3.0á - 28.7.1995
.*----------------------------------------------------------
:userdoc.
:title.WPSam
:docprof toc=123.

.*----------------------------------------------------------
.*  WPSam (Einleitungsseite)
.*----------------------------------------------------------
:h1.WPSam
:lines align=center.
:artwork align=center name='WPSAM.BMP'.

:font facename=Helv size=24x24.
WPSam:font. Version 3.0á

der :font facename=Helv size=24x24.Workplace Shell Access Manager:font.

(C) :artwork align=center name='NB.BMP' runin. Noller &amp. Breining Software, 1993...1995


Compuserve&colon. 100101,3201
Fido-net&colon. 2&colon.246/1406
Telefon&colon. (49)7191-960345

    * * * * * * * * * * * * * * * * * * * * * * * * *
:elines.

.*----------------------------------------------------------
.*  Einleitung
.*----------------------------------------------------------
:h1.Einleitung

:p.:hp4.WPSam:ehp4. ist eine Erweiterung der Workplace Shell von OS/2, die es erm”glicht,
einen Satz verschiedener Arbeitsoberfl„chen auf einem PC zu halten. Zwischen diesen
Arbeitsoberfl„chen kann w„hrend einer Sitzung umgeschaltet werden, ohne den
PC dazwischen neu zu starten. Es mssen dazu nicht einmal gerade laufende Programme
beendet werden.

:p.Die einzelnen Arbeitsoberfl„chen k”nnen durch Passw”rter geschtzt werden, die Paáworteingabe
kann ber einen :link reftype=hd refid=MAGSUPP.Magnetkartenleser:elink. erfolgen.

:p.Fr dieses Programm sind eine Vielzahl von Anwendungen vorstellbar&colon.
:ul.
:li.Mssen auf einem Rechner verschiedene Aufgaben durchgefhrt werden, kann fr jede
einzelne eine eigene Arbeitsoberfl„che eingerichtet werden (z. B. Programmentwicklung
und Textverarbeitung).
:li.Ein PC kann mehreren Benutzern geh”ren, von denen jeder bestimmte Vorlieben fr das
Aussehen "seiner" Arbeitsoberfl„che hat.
:li.Es ist sogar m”glich, eine private Arbeitsoberfl„che auf einem Netzwerk-Server fr
jeden einzelnen Benutzer in diesem Netz zu halten. Jeder dieser Benutzer kann seine
private Shell auf jedem PC dieses Netzwerks starten.
:li.Die eigene "Arbeitsoberfl„che in der Westentasche", d. h. auf Diskette wurde getestet
und funktioniert, ist jedoch intolerabel langsam.
:eul.

.*----------------------------------------------------------
.*  Begriffe
.*----------------------------------------------------------
:h1 id=TERM.Begriffe
:p.Auf den folgenden Seiten werden einige Begriffe definiert, die fr das Verst„ndnis dieser Anleitung notwendig sind.

:h2 id=DT.Arbeitsoberfl„che und Benutzerumgebung
:p.Das mitgelieferte Programm :hp4.InitOS2:ehp4. kann dazu verwendet werden, Sitzungen ohne Workplace-Shell bzw.
sogar ohne Presentation Manager zu starten. In diesem Fall ist die Verwendung des Begriffes :hp1.Arbeitsoberfl„che:ehp1.
nicht mehr angebracht. Daher wird hier synonym das Wort :hp1.Benutzerumgebung:ehp1. verwendet.

:h2 id=PRIVDT.Private Arbeitsoberfl„che
:p.OS/2 sucht seine Informationen zu den Einstellungen einer spezifischen Arbeitsoberfl„che in zwei sogenannten
Profildateien, der Benutzerprofildatei :hp2.OS2.INI:ehp2. und der Systemprofildatei :hp2.OS2SYS.INI:ehp2..
Letztere ist dazu gedacht, alle systemspezifischen Informationen zu halten, wie z. B. die verschiedenen
Layoutpaletten, die Druckertreiber, Fonts usw.

:p.Beim Umschalten verschiedener Arbeitsoberfl„chen verlangt OS/2, daá die Systemprofildatei
beibehalten wird. Lediglich die Benutzerprofildatei wird umgeschaltet.

:p.Eine Arbeitsoberfl„che, die eine eigene Systemprofildatei besitzt, wird daher im folgenden :hp1.private Arbeitsoberfl„che:ehp1.
genannt. Sie kann nur w„hrend des Systemstarts ausgew„hlt werden (falls das Programm :hp4.InitOS2:ehp4. installiert ist).
Deren Systemprofildatei bleibt bis zum n„chsten Systemabschluá aktiv.
Dies hat zur Folge, daá jede mit :hp4.WPSam:ehp4. angelegte Arbeitsoberfl„che zu einer bestimmten
privaten Arbeitsoberfl„che geh”rt.

:h2 id=DEFDT.Default-Arbeitsoberfl„che
:p.Die Arbeitsoberfl„che, die bei der Installation des Systems erstellt wurde, wird im folgenden als :hp1.Default-Arbeitsoberfl„che:ehp1.
bezeichnet. Sie wird von :hp4.WPSam:ehp4. wie eine normale :link reftype=hd refid=PRIVDT.private Arbeitsoberfl„che:elink.
behandelt.
:p.Sie wird aus :hp4.InitOS2.EXE:ehp4. durch die Eingabe von :hp2.ROOT:ehp2. gestartet.

:h2 id=ROOTDT.Stammarbeitsoberfl„che
:p.Die :hp1.Stammarbeitsoberfl„che:ehp1. ist die Arbeitsoberfl„che, die eine zur aktuellen Arbeitsoberfl„che zugeh”rige
Systemprofildatei :hp4.OS2SYS.INI:ehp4. besitzt. Wurde :hp4.InitOS2.EXE:ehp4. nicht installiert, ist dies
die :link reftype=hd refid=DEFDT.Default-Arbeitsoberfl„che:elink..

:p.Nach Installation von :hp4.InitOS2.EXE:ehp4. ist die zugeh”rige
:link reftype=hd refid=PRIVDT.private Arbeitsoberfl„che:elink. die Stammarbeitsoberfl„che.



.*----------------------------------------------------------
.*  Lieferungsumfang
.*----------------------------------------------------------
:h1 id=LUMF.Lieferungsumfang

:p.Das gesamte Programmpaket besteht aus 4 Teilen, den Programmen :hp4.InitOS2.EXE:ehp4.
und :hp4.SAMMY.EXE:ehp4., sowie den DLL-Dateien :hp4.WPSAM.DLL:ehp4. bzw. :hp4.WPSAMF.DLL:ehp4..
Der folgende Abschnitt beschreibt in kurzen Worten deren Funktion. Das Verst„ndnis der Funktion
ist fr den Gebrauch von :hp4.WPSam:ehp4. nicht unbedingt erforderlich. Sie k”nnen daher diesen
Abschnitt berspringen und sofort mit der :link reftype=hd refid=INSTALL.Installation:elink.
oder, falls :hp4.WPSam:ehp4. bereits installiert ist, mit der
:link reftype=hd refid=EINREF.Einrichtung eines Benutzers:elink. fortfahren.

:p.:hp7.InitOS2:ehp7.
:p.Dieses Programm liefert einen Anmelde-Dialog fr den Benutzer bereits w„hrend
dem Startprozeá des Betriebssystems. Dies bietet den Vorteil, daá der Rechner
fr alle unberechtigten Benutzer gesperrt ist, vorausgesetzt, alle
:link reftype=hd refid=DT.Benutzerumgebungen:elink.
sind mit einem Paáwort versehen.
Es gibt drei Typen von Benutzerumgebungen&colon.
:ol.
:li.:hp1.OS/2-Gesamtbildschirm&colon.:ehp1. Der Rechner startet nach dem
Anmeldevorgang nur einen OS/2-Gesamtbildschirmmodus. Von hier k”nnen jedoch trotzdem
alle Typen von Programmen gestartet werden, da OS/2 beim Aufruf von PM-Programmen
automatisch den Presentation-Manager startet.
Durch Eingabe von :hp1.PMSHELL:ehp1. kann die Arbeitsoberfl„che auch von Hand gestartet werden.
(Falls gewnscht, l„át sich dies natrlich durch entsprechende Einstellungen unterbinden.)
:lp.Dieser Umgebungstyp bietet den Vorteil einer sehr kurzen Boot-Zeit. Das ist manchmal
ntzlich, wenn man den Rechner "nur mal kurz einschalten" muá, um z. B. eine Diskette zu
kopieren. Auáerdem k”nnen hier Aktionen durchgefhrt werden, fr die man normalerweise
mit einer Installtionsdiskette booten muá, z. B. das Restaurieren einer Sicherung
der Arbeitsoberfl„che.
:li.:hp1.Presentation-Manager&colon.:ehp1. Der Rechner startet den
Presentation-Manager. Es wird nur ein Programm (z. B. eine OS/2-Befehlszeile im Fenster)
ge”ffnet. Dies ist dann von Nutzen, wenn ein Benutzer eines Rechners nur ein ganz
bestimmtes Programm starten darf.
:li.:hp1.Workplace-Shell&colon.:ehp1. Dies ist der Normalfall. Es wird die
gesamte objektorientierte Arbeitsoberfl„che von OS/2 gestartet. Jeder Benutzer kann sich
seine Arbeitsoberfl„che nach eigenen Vorstellungen gestalten und einrichten.
:eol.

:p.:hp7.WPSam:ehp7.
:p.Diese DLL-Datei beinhaltet eine Objekt-Klasse, die einen modifizierten Ordner darstellt,
der eine einzelne Arbeitsoberfl„che repr„sentiert.
Er enth„lt alle privaten Einstellungen dieser Arbeitsoberfl„che, sowie die fr deren Start notwendigen
Dateien und Verzeichnisse. Der Order selbst
kann durch ein Paáwort geschtzt werden. Durch Objekte dieser Klasse werden auch Arbeitsoberfl„chen
angelegt, die nur durch :hp4.InitOS2:ehp4. gestartet werden k”nnen, beispielsweise
die Umgebungstypen 1 und 2 (siehe oben) oder sogenannte
:link reftype=hd refid=PRIVDT.private Arbeitsoberfl„chen:elink..
:hp4.WPSam:ehp4. erm”glicht es, daá eine neue Arbeitsoberfl„che gestartet werden kann,
ohne den Rechner neu zu booten. Dazu muá nur deren Symbol doppelt angeklickt werden. Ist
sie vom Typ 1 oder 2, weigert sich :hp4.WPSam:ehp4., diese Arbeitsoberfl„che zu starten.

:p.:hp7.WPSamF:ehp7.
:p.Diese DLL-Datei ersetzt die Standard-Klasse der Arbeitsoberfl„chen, :hp1.WPDesktop:ehp1. und ist
im wesentlichen mit dieser identisch. Es kommen lediglich zwei Erweiterungen dazu&colon.
:ol compact.
:li.Einen zus„tzlichen Eintrag im Kontextmen, um die Umschaltung zur
:link reftype=hd refid=ROOTDT.Stammarbeitsoberfl„che:elink. zu erm”glichen.
:li.Eine zus„tzliche Einstellseite, um einige Punkte im Kontextmen entfernen zu k”nnen.
:eol.

:p.:hp7.Sammy:ehp7.
:p.Dieses Programm ist fr die Umschaltung der verschiedenen Arbeitsoberfl„chen zust„ndig.
:p.Auáerdem verwaltet es die Umschaltung von Umgebungsvariablen beim Wechsel auf eine
andere Arbeitsoberfl„che.
Beispielsweise verwendet ein Benutzer einen 16-Bit Compiler, ein anderer einen 32-Bit Compiler,
so daá unterschiedliche PATH, INCLUDE und LIB-Variable verwendet werden mssen.

.*----------------------------------------------------------
.*  Installation
.*----------------------------------------------------------
:h1 id=INSTALL.Installation

Die Installation des gesamten Programmpaketes kann sowohl mit dem Installationsprogramm als auch
manuell erfolgen. Es werden aus diesem Grund hier beide Methoden beschrieben.

:h2.Installationsprogramm

Das Installationsprogramm untersttzt
:ul.
:li.die Installation,
:li.den Update sowie
:li.die Deinstallation
:eul.
:p.des gesamten Programmpaketes.

Die Bedienung ist denkbar einfach&colon. einfach die Installationsdiskette einlegen und
:hp2.SAMINST:ehp2. aufrufen. Eine kurze Erl„uterung erscheint im Hauptfenster des
Installationsprogramms, das selbstverst„ndlich auch ber eine Online-Hilfe verfgt.

:note.Das Installationsprogramm muá immer aus der
:link reftype=hd refid=DEFDT.Default-Arbeitsoberfl„che:elink. aufgerufen werden!
Nur in dieser k”nnen dann weitere
:link reftype=hd refid=WPSE3.globale Arbeitsoberfl„chen:elink. angelegt werden.

:h2.Manuelle Installation
:p.Diese Art der Installation ist nur geeignet fr fortgeschrittene OS/2-Benutzer, die den
prinzipiellen Aufbau des Betriebssystems kennen. Die folgende Information wurde nur
hinzugefgt, um interessierten Anwendern ein besseres Verst„ndnis der
Applikation zu geben und eventuell Modifikationen der Installation zu erlauben.

:p.:hp7.InitOS2:ehp7.
:p.Dieses Programm ersetzt die :hp1.Protected Mode Shell:ehp1. des Betriebssystemes.
Aus diesem Grund muá in der Datei :hp2.CONFIG.SYS:ehp2. die :hp2.PROTSHELL:ehp2.-Anweisung
ge„ndert und eine Umgebungsvariable eingefgt werden.
:p.:hp5.Beispiel&colon.:ehp5.
:parml.
:pt.alte Anweisung&colon.
:pd.:cgraphic.PROTSHELL=C&colon.&bsl.OS2&bsl.PMSHELL.EXE:ecgraphic.
:pt.neue Anweisungen&colon.
:pd.:cgraphic.PROTSHELL=C&colon.&bsl.WPSAM&bsl.INITOS2.EXE C&colon.&bsl.OS2&bsl.PMSHELL.EXE
- - -  g e s t r i c h e n  - - -
SET SAM=<desktoppath>[;<desktoppath>]:ecgraphic.
:pt.<desktoppath> bezeichnet hier den Pfad auf die Verzeichnisse mit den Arbeitsoberfl„chen, defaultm„áig also
:pd.:cgraphic.C&colon.&bsl.WPSAM&bsl.BENUTZER:ecgraphic.
:eparml.

:p.:hp7.Sammy:ehp7.
:p.Dieses Programm wird anstelle der Workplace-Shell gestartet. Dazu muá die Umgebungsvariable
:hp2.RUNWORKPLACE:ehp2. ge„ndert werden und die alte Workplace-Shell in einer neuen Variable
:hp2.SAMWORKPLACE:ehp2. vermerkt werden.

:p.:hp5.Beispiel&colon.:ehp5.
:parml.
:pt.alte Anweisung&colon.
:pd.:cgraphic.SET RUNWORKPLACE=C&colon.&bsl.OS2&bsl.PMSHELL.EXE:ecgraphic.
:pt.neue Anweisungen&colon.
:pd.:cgraphic.SET RUNWORKPLACE=C&colon.&bsl.WPSAM&bsl.SAMMY.EXE
SET SAMWORKPLACE=C&colon.&bsl.OS2&bsl.PMSHELL.EXE:ecgraphic.
:eparml.

:p.:hp7.WPSam:ehp7. und :hp7.WPSamF:ehp7.
:p.Die Installation dieser DLL-Dateien ist etwas aufwendiger. Dazu mssen 2 neue Klassen in der
Workplace-Shell (und zwar in der
:link reftype=hd refid=DEFDT.Default-Arbeitsoberfl„che:elink.)
registriert werden. Diese heiáen :hp2.WPSam:ehp2. und :hp2.WPSamF:ehp2.
(Schreibweise beachten!). Dies geschieht in REXX mit dem Befehl :hp1.SysRegisterObjectClass:ehp1..

:note.Sollen aus einer beliebigen
:link reftype=hd refid=PRIVDT.privaten Arbeitsoberfl„che:elink.
weitere Arbeitsoberfl„chen gestartet werden k”nnen, so sind diese zwei Klassen auch dort zu registrieren.
Diese Vorgehensweise wird vom Installationsprogramm nicht untersttzt und muá durch ein REXX-Script
erzeugt werden.

.*----------------------------------------------------------
.*  Einrichten einer neuen Arbeitsoberfl„che
.*----------------------------------------------------------
:h1 id=EINREF.Einrichten einer neuen Arbeitsoberfl„che

.*-- Einfhrung ------------------------------------------
:hp2.Einfhrung:ehp2.

:p.Das Einrichten einer neuen Arbeitsoberfl„che richtet sich nach der Vorgehensweise des CUA-Standards&colon.
es muá nur eine Schablone aus dem Schablonenordner gezogen werden.

.*-- Erstellen einer neuen Benutzerumgebung -----------------------------------
:h2.Erstellen einer neuen Benutzerumgebung
:p.Das Erstellen einer neuen Arbeitsoberfl„che ist sehr einfach&colon. Es muá nur der
Schablonen-Ordner ge”ffnet werden und eine :hp1.WPSam-Schablone:ehp1. irgendwohin gezogen
werden. Das System generiert dann eine neue Arbeitsoberfl„che. Teile dieser
Generierung werden erst beim ersten ™ffnen der neuen Arbeitsoberfl„che durchgefhrt. Bitte
haben Sie dabei etwas Geduld. Ab dem zweiten Mal wird dann sehr flink umgeschaltet.

:p.Obwohl einige Benutzer m”glicherweise eine unordentliche Arbeitsoberfl„che bevorzugen,
macht es Sinn, wenigstens auf der
:link reftype=hd refid=ROOTDT.Stammarbeitsoberfl„che:elink. einen Ordner anzulegen, der
alle Benutzersymbole enthalten soll. Der beste Weg dazu ist es, ein
Unterverzeichnis anzulegen und eine Referenz dieses Verzeichnisses auf der
Arbeitsoberfl„che zu erstellen.

:p.Wenn Sie :hp4.WPSam:ehp4. mit dem Installationsprogramm installiert haben, dann ist dieser
Ordner bereits angelegt. Suchen Sie auf Ihrer Arbeitsoberfl„che den Ordner mit dem
Namen "Benutzer".

:p.:lm margin=5.
:font facename=Helv size=14x10.
:hp2.Fr den ungebten Benutzer&colon.:ehp2.
:ol compact.
:li.Den Schablonen-Ordner ”ffnen.
:li.Den Laufwerks-Ordner ”ffnen.
:li.Das Laufwerk ”ffnen, auf dem das Verzeichnis angelegt werden soll.
:li.Eine Ordner-Schablone vom Schablonen-Ordner auf das Laufwerk in das Unterverzeichnis
ziehen, in dem das neue Verzeichnis entstehen soll.
:li.Auf den Namen des neuen Verzeichnisses mit der rechten Maustaste klicken, w„hrend
die :hp1.Alt:ehp1. Taste festgehalten wird.
:li.Den Namen des Verzeichnisses von :hp1.Ordner:ehp1. in einen beliebigen Namen
umbenennen (z. B: Benutzer).
:li.Auf das neue Verzeichnis mit der rechten Maustaste klicken (d. h. das Kontextmen
des Objektes ”ffnen).
:li.:hp1.Referenz erstellen:ehp1. ausw„hlen.
:li.Aus der nun erscheinenden Liste :hp1.Arbeitsoberfl„che:ehp1. ausw„hlen.
:eol.
:font.
:lm margin=1.

.*-- Einstellen einer neuen Benutzerumgebung -------------------------------
:h2.Einstellen einer neuen Benutzerumgebung
:p.Nach dem ™ffnen des Einstellungen-Notizbuches eines Arbeitsoberfl„chensymbols erkennt
man die Seiten eines normalen :hp1.Ordners:ehp1.. Zus„tzlich sind zwei weitere
Indexzungen dazugekommen, :hp4.WPSam:ehp4. und :hp4.InitOS2:ehp4..
:hp4.WPSam:ehp4. seinerseits besteht aus insgesamt drei Seiten.

.*-- WPSam Einstellungen Seite 1 --------------------------------
:h2 scroll=none.WPSam Einstellungen Seite 1
:link reftype=hd res=001 auto split group=10
  vpx=left vpy=top vpcx=35% vpcy=100%
  scroll=vertical titlebar=none.
:link reftype=hd res=002 auto split group=11
  vpx=right vpy=top vpcx=65% vpcy=100%
  scroll=both titlebar=none.

.*   WPSam Einstellungen Seite 1: Secondary 1
:h2 res=001 group=10 hide.t2
:p.Das Bild zeigt die Einstellungsseite fr das Paáwort im Einstellungs-Notizbuch
einer Arbeitsoberfl„che. Hier kann ein Paáwort eingegeben, ge„ndert bzw. aktiviert werden.
:p.Wenn der Auswahlknopf :hp1.Paáwort aktivieren:ehp1. gedrckt wird, kann die
Arbeitsoberfl„che nur gestartet, gel”scht bzw. ge„ndert werden, wenn das Paáwort
bekannt ist.
:p.Das Paáwort kann nur ge„ndert werden, wenn das alte bekannt ist, unabh„ngig vom Zustand
des :hp1.Paáwort aktivieren:ehp1. Knopfes.
:p.Wird vom System das Vorhandensein eines :link reftype=hd refid=MAGSUPP.Magnetkartenlesers:elink. erkannt,
erscheinen ber den Eingabefeldern andere Texte.
Der Hinweis :hp1.Altes/Neues Paáwort oder Magnetkarte:ehp1. deutet an, daá auáer einer
manuellen Paáworteingabe auch das Durchziehen einer Magnetkarte durch den
Kartenleser erlaubt ist.

.*   WPSam Einstellungen Seite 1: Secondary 2
:h2 res=002 group=11 hide.t1
:artwork align=left name='USRPG1GR.BMP'.
:p.WPSam Notizbuchseite 1 der Arbeitsoberfl„cheneinstellungen.

.*-- WPSam Einstellungen Seite 2 --------------------------------
:h2 scroll=none.WPSam Einstellungen Seite 2
:link reftype=hd res=003 auto split group=10
  vpx=left vpy=top vpcx=35% vpcy=100%
  scroll=vertical titlebar=none.
:link reftype=hd res=004 auto split group=11
  vpx=right vpy=top vpcx=65% vpcy=100%
  scroll=both titlebar=none.

.*   WPSam Einstellungen Seite 2: Secondary 1
:h2 res=003 group=10 hide.t2
:p.Das Bild zeigt die zweite Einstellungsseite  im Einstellungs-Notizbuch einer Arbeitsoberfl„che.
Diese Seite umfaát folgende Features&colon.
:ul.
:li.:hp2.Umbenennen:ehp2.&colon. Wenn aktiviert, kann der Name der Arbeitsoberfl„che ge„ndert werden.
:li.:hp2.Verschieben:ehp2.&colon. Wenn aktiviert, kann das Symbol in einen anderen
Ordner verschoben werden.
:li.:hp2.Dropping:ehp2.&colon. Wenn aktiviert, k”nnen andere Symbole auf die
Arbeitsoberfl„che fallen gelassen werden.
Das heiát, daá andere Benutzer Symbole hinzufgen k”nnen, ohne das
Paáwort zu kennen! Um Miáverst„ndnissen vorzubeugen&colon. Das :hp4.WPSam:ehp4.-Symbol ist streng
genommen ein modifizierter Ordner. Wenn man ein Symbol hinzufgt, erscheint dieses nur
in diesem Ordner (man sieht es beispielsweise, wenn man die Symbolanzeige dieses Ordners ”ffnet).
Das Symbol wird :hp8.NICHT:ehp8. auf der zugeh”rigen Arbeitsoberfl„che erscheinen!
:eul.

:p.Die Umgebungsvariable, die auf dieser Seite eingegeben werden k”nnen, werden von
:link reftype=hd refid=LUMF.InitOS2:elink. und
:link reftype=hd refid=LUMF.Sammy:elink. ausgewertet.
Sie mssen in der Form :hp2.VARIABLE=INHALT:ehp2. eingegeben werden.
Umgebungsvariable, die in der Datei CONFIG.SYS standardm„áig eingetragen
sind, k”nnen gel”scht werden, indem ein leerer Inhalt angegeben wird, also&colon.
:hp2.VARIABLE=:ehp2..

.*   WPSam Einstellungen Seite 2: Secondary 2
:h2 res=004 group=11 hide.t1
:artwork align=left name='USRPG2GR.BMP'.
:p.WPSam Notizbuchseite 2 der Arbeitsoberfl„cheneinstellungen.

.*-- WPSam Einstellungen Seite 3 --------------------------------
:h2 id=WPSE3 scroll=none.WPSam Einstellungen Seite 3
:link reftype=hd res=005 auto split group=10
  vpx=left vpy=top vpcx=35% vpcy=100%
  scroll=vertical titlebar=none.
:link reftype=hd res=006 auto split group=11
  vpx=right vpy=top vpcx=65% vpcy=100%
  scroll=both titlebar=none.

.*   WPSam Einstellungen Seite 3: Secondary 1
:h2 res=005 group=10 hide.t2
:p.Wenn eine Arbeitsoberfl„che das erste Mal gestartet wird, wird zumindest eine neue Benutzerprofildatei
:hp2.OS2.INI:ehp2. generiert. Die Generierung einer Systemprofildatei :hp2.OS2SYS.INI:ehp2.
h„ngt von der :hp1.Global/:link reftype=hd refid=PRIVDT.Privat:elink.:ehp1. Einstellung ab.
Die Quellen, die fr diesen Compiliervorgang genutzt werden, sind die zwei Dateien
:hp1.INI.RC:ehp1. und :hp1.INISYS.RC:ehp1. im OS2-Verzeichnis. Diese k”nnen direkt verwendet
werden oder den eigenen Bedrfnissen angepaát werden.
Wenn jemanden beispielsweise das :hp1.WPPower:ehp1.-Symbol st”rt, weil er keinen PC mit Power-Management
besitzt, kann er die entsprechende Zeile aus der RC-Datei entfernen.
Wer sich etwas auskennt, kann auch das Aussehen der Windows-Arbeitsoberfl„che simulieren,
indem er sich eine neue RC-Datei aus :hp1.INI.RC:ehp1. und :hp1.WIN_30.RC:ehp1. erzeugt.
Es empfiehlt sich jedoch, nie die Originaldateien, sondern immer Kopien davon zu modifizieren!

:caution.Solche nderungen sollte jedoch wirklich nur jemand machen, der genau weiá, was er tut!
Im Fall einer fehlerhaften .RC-Datei wird die gewnschte Arbeitsoberfl„che nicht gestartet oder der
Rcksprung zur Stammarbeitsoberfl„che funktioniert nicht mehr! Sollte dieser Fall wirklich
einmal eintreten, ist der Rechner neu zu starten und die fehlerhafte Arbeitsoberfl„che zu l”schen!
:ecaution.
:p.Wenn die Generierung einer Arbeitsoberfl„che abgeschlossen ist (nach der ersten Aktivierung),
k”nnen die Einstellungen dieser Seite nicht mehr ge„ndert werden!

:p.In manchen F„llen kann es von Interesse sein, eine Arbeitsoberfl„che auf verschiedenen
Maschinen (z. B. in einem Netzwerk) zu verwenden. Hier st”át man mit dem Konzept zweier
Profildateien auf ein Problem, da diese zwei Dateien Informationen enthalten, die zusammengeh”ren.
Angenommen, es wird auf einem Rechner nur ein System-Profil fr mehrere Arbeitsoberfl„chen verwendet.
Wenn eine dieser Arbeitsoberfl„chen nun ber ein Netzwerk auf einem anderen Rechner mit einem
anderen System-Profil verwendet wird, kann dies zu einem Systemfehler fhren.

:p.Um dies zu vermeiden, kann hier die Verwendung eines privaten System-Profiles
ausgew„hlt werden. Da das Umschalten zwischen verschiedenen System-Profilen in einem laufenden
System von OS/2 nicht vorgesehen ist, k”nnen Arbeitsoberfl„chen mit privaten
System-Profilen nur von :link reftype=hd refid=LUMF.InitOS2:elink. aus gestartet werden.

.*   WPSam Einstellungen Seite 3: Secondary 2
:h2 res=006 group=11 hide.t1
:artwork align=left name='USRPG3GR.BMP'.
:p.WPSam Notizbuchseite 3 der Arbeitsoberfl„cheneinstellungen.


.*-- Init OS2 Einstellungen --------------------------------
:h2 scroll=none.Init OS2 Einstellungen
:link reftype=hd res=007 auto split group=10
  vpx=left vpy=top vpcx=35% vpcy=100%
  scroll=vertical titlebar=none.
:link reftype=hd res=008 auto split group=11
  vpx=right vpy=top vpcx=65% vpcy=100%
  scroll=both titlebar=none.

.*   Init OS2 Einstellungen: Secondary 1
:h2 res=007 group=10 hide.t2
:p.Diese Seite beinhaltet die Einstellm”glichkeiten der Arbeitsoberfl„che fr das
Programm :link reftype=hd refid=LUMF.InitOS2:elink.. Hier wird beispielsweise der Typ der zu
startenden Arbeitsoberfl„che festgelegt.

:p.Der Default der ProtShell ist das Programm, das bei :hp4.InitOS2:ehp4. als Argument in
der Datei CONFIG.SYS steht. Dies ist normalerweise PMSHELL.EXE.

:p.Wird das Eingabefeld aktiviert, kann hier eine andere Shell eingegeben werden.
Normalerweise wird hier CMD.EXE vorgegeben. Dies bedeutet, daá bei Start der Benutzer-ID
von :hp4.InitOS2:ehp4. aus nur ein OS/2-Gesamtbildschirm ge”ffnet wird.

:p.Fr den Start einer PM-Oberfl„che ohne Workplace-Shell ist hier ebenfalls
:hp1.Default:ehp1. zu aktivieren. Zus„tzlich muá in Einstellungsseite 2 von WPSam
eine Umgebungsvariable :hp1.SAMWORKPLACE=x&colon.&bsl.OS2&bsl.CMD.EXE:ehp1. eingegeben werden.

:p.Eine Arbeitsoberfl„che kann von :hp4.InitOS2:ehp4. aus nur gestartet werden, wenn der Auswahlknopf
:hp1.Benutzer in 'Init OS/2' Men bernehmen:ehp1. aktiviert ist.

.*   Init OS2 Einstellungen: Secondary 2
:h2 res=008 group=11 hide.t1
:artwork align=left name='USRPG4GR.BMP'.
:p.Init OS2 Notizbuchseite der Arbeitsoberfl„cheneinstellungen.

.*----------------------------------------------------------
.*  Benutzung von InitOS2
.*----------------------------------------------------------
:h1.Benutzung von InitOS2
:p.:hp2.InitOS2:ehp2. erlaubt es, einen Rechner vor unbefugter Benutzung zu schtzen.
Voraussetzung dafr ist, daá dieser ber ein BIOS verfgt, bei dem die
Boot-Reihenfolge C: A: eingestellt und die BIOS-Einstellungen per Paáwort
gesichert werden k”nnen.

:p.Auáerdem bietet :hp2.InitOS2:ehp2. die M”glichkeit, Sitzungen ohne
Workplace Shell bzw. ohne Presentation-Manager zu starten.
Durch entsprechende Modifikation der Umgebungsvariablen fr solche
Benutzerumgebungen kann sogar erreicht werden, daá man aus einer solchen
Sitzung den PM auch nicht nachtr„glich starten kann.
Diese M”glichkeiten sind z. B. wichtig, wenn an bestimmten Einsatzorten
vermieden werden soll, daá der Benutzer des Rechners ein anderes als das
vorgesehene Programm starten kann.

.*-- Init OS2 Hauptfenster --------------------------------
:h2 scroll=none.InitOS/2 Hauptfenster
:link reftype=hd res=009 auto split group=10
  vpx=left vpy=top vpcx=52% vpcy=100%
  scroll=horizontal titlebar=none.
:link reftype=hd res=010 auto split group=11
  vpx=right vpy=top vpcx=48% vpcy=100%
  scroll=vertical titlebar=none.

.*   InitOS/2 Hauptfenster: Secondary 1
:h2 res=009 group=10 hide.t1
:artwork align=left name='INITMAIN.BMP'.
:p.InitOS/2 Hauptfenster

.*   InitOS/2 Hauptfenster: Secondary 2
:h2 res=010 group=11 hide.t2
:p.Das nebenstehende Bild zeigt das Fenster, mit dem sich :hp4.InitOS2:ehp4. nach
dem Startvorgang des Rechners meldet. Die Benutzung ist sehr einfach.
Um eine Benutzer-ID zu starten, ist lediglich in die zwei Eingabefelder
der Benutzername und gegebenenfalls das Paáwort einzugeben. Nach Drcken
der Eingabetaste (oder des OK-Knopfes) wird der Benutzer gestartet.
:p.Die "Navigation" durch diese Dialogbox wurde an den CUA-Standard angelehnt.
So erreicht man das n„chste Eingabefeld oder einen Knopf mit der Tabulatortaste.
:p.Folgende Kn”pfe sind vorhanden&colon.
:ul.
:li.OK&colon. Starten der Benutzerumgebung. Dies ist der Default-Knopf und wird auch mit
der Return-Taste erreicht, wenn sich der Cursor auf einem der Eingabefelder
befindet.
:li.Default starten&colon. :hp2.InitOS2:ehp2. l„át es zu, eine
Default-Benutzerumgebung zu definieren.
Diese kann nach einer einstellbaren Zeit
automatisch gestartet werden, wenn der Benutzer keine Eingaben vornimmt.
Sie kann auch durch Drcken dieses Knopfes oder der
F2-Taste sofort gestartet werden.
:li.Setup&colon. Dieser Knopf startet eine Dialogbox, die fr den Systemverwalter
vorgesehen ist und die deshalb mit einem Paáwort gesichert werden kann.
Setup kann auch durch Drcken von F9 gestartet werden.
:li.Systemabschluá&colon. Dieser Knopf (auch ber die F10-Taste erreichbar) bereitet
den Rechner zum Ausschalten vor.
:eul.
:p.Wird vom System das Vorhandensein eines :link reftype=hd refid=MAGSUPP.Magnetkartenlesers:elink.
 erkannt, wird der Text :hp1.Paáwort:ehp1. abge„ndert in :hp1.Paáwort oder Magnetkarte:ehp1..
In diesem Fall kann statt der manuellen Paáworteingabe auch eine Magnetkarte durch
den Leser gezogen werden.
Wurde kein Benutzer angegeben, wird automatisch nach dem ersten passenden gesucht und dieser gestartet.

.*-- Init OS2 Setup --------------------------------
:h2 scroll=none.InitOS/2 Setup
:link reftype=hd res=011 auto split group=10
  vpx=left vpy=top vpcx=40% vpcy=100%
  scroll=horizontal titlebar=none.
:link reftype=hd res=012 auto split group=11
  vpx=right vpy=top vpcx=60% vpcy=100%
  scroll=vertical titlebar=none.

.*   InitOS/2 Setup: Secondary 1
:h2 res=011 group=10 hide.t1
:p.:artwork align=left name='INITPWD.BMP'.
:p.InitOS/2 Administrator-Paáwort

:p.:artwork align=left name='INITSET.BMP'.
:p.InitOS/2 Setup

.*   InitOS/2 Setup: Secondary 2
:h2 res=012 group=11 hide.t2
:p.Wird im Hauptfenster :hp1.Setup:ehp1. gedrckt, erscheint das Paáwortfeld
fr das Systemverwalter-Paáwort (falls eines existiert). Nach korrekter Eingabe wird die
Setup-Dialogbox gestartet.
:p.Das Setup-Fenster enth„lt folgende Einstellm”glichkeiten&colon.
:ul.
:li.:hp1.Default-Benutzer&colon.:ehp1. Hier wird die Arbeitsoberfl„che eingestellt,
die automatisch gestartet wird, wenn innerhalb der Timeout-Zeit keine Taste
gedrckt wird.
:li.:hp1.Timeout&colon.:ehp1. Nach dieser Zeit wird die
Default-Benutzerumgebung gestartet, wenn keine Taste gedrckt wird. Ist eines der Felder
:hp1.Default-Benutzer:ehp1. bzw. :hp1.Timeout:ehp1. leer, ist
dieses Feature deaktiviert.
:li.:hp1.Administrator-Paáwort / Wiederholung&colon.:ehp1. Hier kann ein
Systemverwalter-Paáwort eingegeben werden. Um Schreibfehler zu erkennen,
muá dieses zwei mal erfolgen.
:li.:hp1.OK&colon.:ehp1. Hiermit wird die Dialogbox beendet. Die
:link reftype=hd refid=DEFDT.Default-Arbeitsoberfl„che:elink.
und das Administrator-Paáwort werden gesichert. Dieser Knopf ist der Default-Knopf und kann
daher mit der Eingabetaste erreicht werden.
:li.:hp1.Paáwort l”schen&colon.:ehp1. Wurde in den zwei Paáwort-Feldern kein
Paáwort vor dem Beenden der Dialogbox eingegeben, beh„lt :hp4.InitOS2:ehp4. das alte Paáwort.
Dieses l„át sich durch Drcken dieser Taste l”schen.
:li.:hp1.Abbruch&colon.:ehp1. Nach Drcken dieser Taste wird die Dialogbox geschlossen, ohne die
eingestellten Werte zu bernehmen. Diese Taste kann auch durch Drcken von
Escape erreicht werden.
:eul.

.*----------------------------------------------------------
.*  Benutzung von WPSam
.*----------------------------------------------------------
:h1.Benutzung von WPSam
:p.Die Benutzung des Programmes ist sehr einfach; die Bedienung entspricht
voll dem CUA 91-Standard.

:p.Das Konzept wurde ebenfalls an den brigen Aufbau der Workplace Shell
angelehnt. So wird jede Arbeitsoberfl„che durch ein Symbol repr„sentiert. Dieses
Symbol stellt ein Objekt der Klasse
:link reftype=hd refid=LUMF.WPSam:elink. dar, das von einem normalen
Ordner abgeleitet wurde. Man erkennt dies daran, daá die Einstellungen eines
solchen Objektes dieselben Seiten wie die eines Ordners enthalten. Es wurden
lediglich einige fr die Funktion des Programmes notwendige Seiten hinzugefgt.
Wird eine solche "Benutzer-Arbeitsoberfl„che" als Symbolanzeige ge”ffnet, erkennt man
darin einige Dateien, die fr den Betrieb einer Arbeitsoberfl„che wichtig sind.
Fr :link reftype=hd refid=WPSE3.globale Arbeitsoberfl„chen:elink.
 ist dies die Benutzerprofildatei :hp2.OS2.INI:ehp2. sowie der
Arbeitsoberfl„chen-Ordner :hp1.Arbeitsoberfl„che_Benutzer:ehp1..

:p.Diese Dateien werden automatisch generiert, wenn die Arbeitsoberfl„che das erste Mal
gestartet wird. Sie werden auch fr Benutzerumgebungen vom
:link reftype=hd refid=LUMF.Typ 1 (OS/2-Gesamtbildschirm-Benutzer):elink. bzw.
fr :link reftype=hd refid=LUMF.Typ 2 (Presentation-Manager-Benutzer):elink.
ben”tigt. Der Grund ist der, daá man prinzipiell durch Aufruf von PMSHELL
jederzeit die Workplace Shell aus einer solchen Benutzerumgebung starten kann.
Sind diese Dateien noch nicht vorhanden, gilt die Benutzerumgebung als noch
:hp1.nicht initialisiert:ehp1..

:h2.Starten einer Benutzer-Arbeitsoberfl„che
:p.Zum Start einer Arbeitsoberfl„che einfach doppelt auf das
Benutzersymbol klicken. Wenn dieses nicht
durch ein Paáwort geschtzt ist, wird die Arbeitsoberfl„che sofort gestartet.
Wenn es sich um den ersten Start der Arbeitsoberfl„che handelt, werden zuerst die
erforderlichen :hp1.INI:ehp1.-Dateien generiert.

:p.:hp1.WPSam:ehp1. prft nicht den Typ einer Benutzerumgebung, sondern startet
:hp6.immer:ehp6. die Workplace Shell. Andere Typen k”nnen nur mit :hp1.InitOS2:ehp1.
gestartet werden.

:p.Ist eine Arbeitsoberfl„che durch ein Paáwort gesichert, erscheint ein
Dialogfenster, in dem nach dem Paáwort gefragt wird. Der Start wird
(natrlich!) verweigert, wenn das falsche Paáwort eingegeben wurde. Der Fehlversuch wird
auáerdem protokolliert.

:h2.Rckkehr zur Stammarbeitsoberfl„che
:p.Fr die Rckkehr in die
:link reftype=hd refid=ROOTDT.Stammarbeitsoberfl„che:elink. muá das Kontextmen der
Arbeitsoberfl„che (durch Klicken mit der rechten Maustaste
auf eine leere Fl„che der Arbeitsoberfl„che) ge”ffnet und der Menpunkt
:hp1.Stammarbeitsoberfl„che:ehp1. ausgew„hlt werden (siehe das folgende Bild).

:artwork align=center name='DTCTXGR.BMP'.

Nach einer Sicherung der aktuellen Arbeitsoberfl„che wird diese geschlossen und
auf die Stammarbeitsoberfl„che gewechselt.

:h2.Einsehen des Protokolles
:p.Jeder erfolgreiche oder nicht erfolgreiche Start einer Benutzerumgebung wird
automatisch protokolliert. Die letzten 100 Vorg„nge k”nnen so jederzeit nachvollzogen
werden. Dies geschieht durch ™ffnen der :hp1.Einstellungen:ehp1. des WPSam-Symbols.
Wird dort der Index :hp1.Datei:ehp1. ausgew„hlt, erscheint auf Seite 3
das Protokoll-Feld.

.*----------------------------------------------------------
.*  Magnetkartenuntersttzung
.*----------------------------------------------------------
:h1 id=MAGSUPP.Magnetkartenuntersttzung
:p.Die Paáworteingaben des Systems k”nnen alternativ ber einen Magnetkartenleser
erfolgen. Voraussetzung ist das Vorhandensein einer DLL-Datei :hp1.MAGCALLS.DLL:ehp1..
Diese geh”rt zum Lieferumfang der OS/2-Untersttzung der Kartenleser.
Getestet wurden die Ger„te der Firmen IBM (4717) und DESKO (MSL4717-x).
Geeignet sind alle Magnetkarten, bei denen die Spur 2 beschrieben ist.
Hierfr k”nnen spezielle Karten der Firma DESKO verwendet werden, es reicht jedoch
jede handelsbliche Karte, wie EC-Karte oder eine Kreditkarte.
Werden andere Karten (wie Kantinenkarten o. „.) eingesetzt, ist sicherzustellen, daá der Inhalt
von Spur 2 nie ver„ndert wird.
Ist der Inhalt der Spur 2 bekannt, kann die Paáworteingabe auch von Hand durch Eingabe
der Daten dieser Spur erfolgen.
Bei Kreditkarten ist dies jedoch nicht praktikabel, da das Paáwort sehr lang ist.

:p.:hp4.InitOS2:ehp4. ist in der Lage, eine Benutzerumgebung anhand des Paáwortes zu erkennen, das ber eine
Magnetkarte eingegeben wurde.
Wird dieselbe Karte jedoch fr mehrere Benutzerumgebungen verwendet, wird immer die (alphabetisch) erste
gestartet.

.*----------------------------------------------------------
.*  Geplante Erweiterungen
.*----------------------------------------------------------
:h1.Geplante Erweiterungen
:p.Es gibt noch viele Ideen fr Erweiterungen, die in die n„chsten Versionen von
:hp4.WPSam:ehp4. einflieáen werden. Vor allem :hp4.InitOS2:ehp4. ist ein sehr
neues Programm, so daá hier sicher noch einige wnschenswerte Eigenschaften fehlen.
Hier einige Beispiele&colon.
:ul compact.
:li.Der Window-Manager von InitOS2 soll um einige Controls erweitert werden.
So soll eine Listbox dazu dienen, die Auswahl von Benutzernumgebungen (vor allem mit langen Namen)
zu vereinfachen.
Auáerdem sollen die Farben einstellbar gemacht werden.
:li.Der Parser der Umgebungsvariablen soll so erweitert werden, daá er %var%-Variable
versteht.
:li.Ein interaktives Programm soll die Generierung von .RC-Files erm”glichen.
:eul.

:p.Falls Sie ebenfalls irgendwelche Features vermissen, z”gern Sie bitte nicht,
diese uns mitzuteilen.

.*----------------------------------------------------------
.*  Registrierung
.*----------------------------------------------------------
:h1.Registrierung

:p.:p.:hp4.WPSam:ehp4. ist keine Shareware! Trotzdem wollen wir interessierten Anwendern
die M”glichkeit geben, die Software zu testen. Aus diesem Grund wird :hp4.WPSam:ehp4.
als vollst„ndiges Produkt verteilt. Er umfaát fr nicht
registrierte Benutzer jedoch nur die Berechtigung zum Testen der Applikation.
Es k”nnen beliebig viele Arbeitsoberfl„chen angelegt werden, diese arbeiten aber
nur 21 Tage lang. Wenn Ihnen das Programm gef„llt, mssen Sie diese
Arbeitsoberfl„chen nicht l”schen. Nach der Registrierung von :hp4.WPSam:ehp4.
funktionieren sie wieder.

:p.Wenn Sie das Programm :hp4.WPSam:ehp4. registrieren, k”nnen Sie mit dem von uns
gelieferten Paáwort die aktuelle, sowie die nachfolgende Version von :hp4.WPSam:ehp4.
verwenden.
Dies erlaubt einen schnellen Update ber Internet und Fido-net.

:p.Wenn Sie keinen H„ndler kennen, der :hp4.WPSam:ehp4. vorr„tig hat, dann k”nnen Sie
Einzellizenzen auch direkt bei uns bestellen. Dabei bieten wir Schlern,
Studenten und Rentnern, gegen entsprechenden Nachweis und ausschlieálich per
Vorkasse, eine besonders gnstige Lizenz an. Fr Firmen und ”ffentliche
Einrichtungen gelten gnstige Staffelpreise. Bitte fragen Sie
schriftlich oder per FAX an.

.*   Registriervorgang
:h2.Registriervorgang

:p.Den Punkt :hp1.Produktinformation:ehp1. im Kontextmen eines
Benutzersymbols oder der Schablone im Schablonenordner ausw„hlen.
Wenn die Version auf dem Rechner noch nicht registriert ist, wird in
dem erscheinenden Fenster nach einem Namen und einem Registrierpaáwort gefragt
(siehe Bild).

:artwork align=center name='REGGR.BMP'.

Wenn diese zwei Zeilen korrekt eingegeben wurden, erscheint in Zukunft in diesem
Fenster nur noch der Name, fr den das Programm registriert wurde.

:p.Der Name muá korrekt eingegeben werden, da das Paáwort daraus abgeleitet wurde.
Auch die Leerzeichen mssen exakt eingegeben werden. Groá- und Kleinschreibung
des Names ist jedoch ohne Belang. Das Paáwort erhalten Sie von uns nach Bezahlung
der Registriergebhr.

:p.Das Paáwort ist ziemlich lang (14 Zeichen), weil es den Namen, die Seriennummer
und die Versionsnummer enth„lt.

.*   Bestellformular
:h2.Bestellformular

:lm margin=5.
:lines.
                                        ___________________, den ___.____.____




Noller &amp. Breining Software
Tannenstrasse 18

D-71554 Weissach im Tal



:hp2.Bestellung:ehp2.

Ich bestelle hiermit die aktuelle Version von WPSam. Ich weiá, daá
dieses Programm ohne jede Art von Garantie herausgegeben wird.:elines.
:p.Der gewnschte Lizenztyp ist&colon.:dl compact tsize=5.
:dt.:artwork runin name='sq.bmp'.
:dd.Schler/Student/Rentner (DM 20.--).
:dt.:artwork runin name='sq.bmp'.
:dd.pers”nliche Lizenz (DM 60.--).
:edl.
:p.Bitte senden Sie mir die Software auf:dl compact tsize=5.
:dt.:artwork runin name='sq.bmp'.
:dd.3«" Diskette oder
:dt.:artwork runin name='sq.bmp'.
:dd.5¬" Diskette zu.
:edl.
:p.Die Bezahlung des Preises kann &colon.:dl compact tsize=5.
:dt.:artwork runin name='sq.bmp'.
:dd.per Vorkasse oder
:dt.:artwork runin name='sq.bmp'.
:dd.gegen Rechnung erfolgen.
:edl.
:p.Bei pers”nlichen Lizenzen fr Schler und Studenten mssen wir aber
(aufgrund des geringen Betrages) auf Vorkasse per Scheck oder Geldschein
bestehen. Wir akzeptieren in diesem Fall keine ausl„ndischen Schecks.
:p.Bitte senden Sie sie an folgende Adresse&colon.
:cgraphic.:font facename=Courier size=10x10.
Name, Vorname&colon.             ___________________________________________

Straáe, Nr.&colon.               ___________________________________________

PLZ, Stadt&colon.                ___________________________________________

Land&colon.                      ___________________________________________
:ecgraphic.

:p.:font facename=Courier size=10x10.__________________________________:font.
:p.(Unterschrift, Datum)                                               V 3.0
:lm margin=1.

.*----------------------------------------------------------
.*  Versionen
.*----------------------------------------------------------
:h1.Versionen
:cgraphic.:hp2.
   3.0  Untersttzung der SOM 2 Runtime.

   2.1  Untersttzung von Magnetkartenlesern.

   2.0  Auslieferung von SAMMY und INITOS2. Die Umschaltung auf andere
        Benutzer wurde abge„ndert.

   1.2  Deutsche Version verfgbar. Entfernen der "20 Sekunden-Pause".
        Erster Release von InitOS2 zusammen mit WPSam.

   1.2á Benutzer-Arbeitsoberfl„chen sichern ihre Fensterpositonen.
        Umorganisation der erweiterten Attribute. Registrierf„higkeit.

   1.1  Workplace Shell Untersttzung fr OS/2 2.x

   1.0  Erste ”ffentliche Version von SAM fr OS/2 1.x.
:ehp2.:ecgraphic.
:euserdoc.
