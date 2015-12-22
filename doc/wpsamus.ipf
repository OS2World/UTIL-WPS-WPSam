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
WPSam:font. version 3.0á

the :font facename=Helv size=24x24.Workplace Shell Access Manager:font.

(C) :artwork align=center name='NB.BMP' runin. Noller &amp. Breining Software, 1993...1995


Compuserve&colon. 100101,3201
Fido-net&colon. 2&colon.246/1406
Voice&colon. (+49)7191-960345

    * * * * * * * * * * * * * * * * * * * * * * * * *
:elines.

.*----------------------------------------------------------
.*  Introduction
.*----------------------------------------------------------
:h1.Introduction

:p.:hp4.WPSam:ehp4. is an extension to the Workplace Shell of OS/2 that enables you to maintain
a set of different desktop settings on your PC. You may switch between these
workplace shells during one session (i. e. without restarting your PC).
You even don't have to stop running programs.

:p.The different sets of Workplace Shell settings may be protected by passwords, which may be entered
by means of a :link reftype=hd refid=MAGSUPP.magnetic-stripe unit:elink..
                                                                                        
:p.A lot of applications are imagineable for this program&colon.
:ul.
:li.One can create different desktops for different jobs he has to carry out on a single
workstation.
:li.A PC might belong to different users and each of them has a different taste of a
nice workplace shell.
:li.It is even possible to hold a private workplace shell on a network server for each
user in this network. And each of these users can get his private shell on every
PC in this network.
:eul.

.*----------------------------------------------------------
.*  Terms
.*----------------------------------------------------------
:h1 id=TERM.Terms
:p.On the following pages some terms will be explained that are necessary for the understanding of this document.

:h2 id=DT.Desktop and user environment
:p.The supplemental program :hp4.InitOS2:ehp4. may be used to start sessions without workplace shell and even
without presentation manager. In this case the term :hp1.desktop:ehp1. isn't suitable any more.
In this case the word :hp1.user environment:ehp1. will be used as a synonym.

:h2 id=PRIVDT.Private desktop
:p.OS/2 receives its information about the settings of the workplace shell out of two so called profiles,
the user profile :hp2.OS2.INI:ehp2. and the system profile :hp2.OS2SYS.INI:ehp2..
The latter one is used to keep all system specific information like the layout palettes, the printer drivers or fonts.

:p.If you switch to another desktop OS/2 enforces you not to change the system profile. Only the user profile
may be changed.

:p.A desktop that uses an own system profile will therefore be called :hp1.private desktop:ehp1. in the following sections.
It may only be selected during the system start (if :hp4.InitOS2:ehp4. has been installed).
The system profile will then remain active until shutdown.
Thus it follows, that each desktop created by use of :hp4.WPSam:ehp4. belongs to one defined private desktop.

:h2 id=DEFDT.Default desktop
:p.The desktop that was created during system installation will be called :hp1.default desktop:ehp1. in the following sections.
It is used by :hp4.WPSam:ehp4. like a normal :link reftype=hd refid=PRIVDT.private desktop:elink..
:p.It may be started out of :hp4.InitOS2:ehp4. by entering :hp2.ROOT:ehp2..

:h2 id=ROOTDT.Root desktop
:p.The :hp1.root desktop:ehp1. is the desktop that owns the system profile belonging to the active desktop.
If :hp4.InitOS2:ehp4. was not installed, this will be the :link reftype=hd refid=DEFDT.default desktop:elink..

:p.After installation of :hp4.InitOS2:ehp4. the appertaining :link reftype=hd refid=PRIVDT.private desktop:elink.
will be the root desktop.

.*----------------------------------------------------------
.*  contents of package
.*----------------------------------------------------------
:h1 id=LUMF.Contents of Package

:p.The complete package consists of four parts, the programs :hp4.InitOS2.EXE:ehp4.
and :hp4.SAMMY.EXE:ehp4., as well as a DLL-files :hp4.WPSAM.DLL:ehp4. and :hp4.WPSAMF.DLL:ehp4..
The following section describes the function of these parts in brief. The understanding of the function is
not necessary for the usage of :hp4.WPSam:ehp4.. So you may skip this section and start
with reading section :link reftype=hd refid=INSTALL.Installation:elink. or with
:link reftype=hd refid=EINREF.Setting up a new user:elink..

:p.:hp7.InitOS2:ehp7.
:p.This program makes a logon dialog available at boot time. This offers the opportunity,
that the computer is locked for all unauthorized persons provided, all 
:link reftype=hd refid=DT.use environments:elink.
have a password. There are three types of user IDs&colon.
:ol.
:li.:hp1.OS/2 fullscreen:ehp1.&colon. The computer starts an OS/2 fullscreen session
after the logon. From here however all types of programs may be started, because OS/2
automatically starts the presentation manager on invocation of a PM program.
The workplace shell may even be started by entering :hp1.PMSHELL:ehp1. on the command line.
(This functionality may of course be suppressed if needed.)
:lp.This ID-type offers the shortest boot time. This is sometimes useful, if you e.g. want to
switch on your computer for simply copying a diskette.
Besides this, you may perform actions from here for which you normally have to boot from
the installation diskettes, e.g. the restore of a workplace shell backup.
:li.:hp1.Presentation manager:ehp1.&colon. The computer starts the presentation manager.
Only one program (e.g. a commandline in a window) will be opened.
This may be used, if a user must only start one specific program.
:li.:hp1.Workplace shell:ehp1.&colon. This is the normal case. The complete object oriented
workplace shell of OS/2 will be started. Each user may set up its own workplace shell.
:eol.

:p.:hp7.WPSam:ehp7.
:p.This DLL-file defines a new workplace shell class. :hp4.WPSam:ehp4. is a modified folder,
representing a single desktop. It contains all private settings of a specific desktop, as well as
the files and the directory necessary for starting a workplace shell (INI files, desktop directory).
The folder itsself may be protected by a password. You also generate users, that may only be
started using :hp4.InitOS2:ehp4., e.g. the ID-types 1 and 2 (see above) or so called
:link reftype=hd refid=PRIVDT.private users:elink..
:hp4.WPSam:ehp4. makes it possible to start a new desktop without rebooting your computer.
You simply have to double click onto a user symbol. If you click on a type 1 or 2 ID, :hp4.WPSam:ehp4.
refuses the starting of this user desktop.

:p.:hp7.WPSamF:ehp7.
:p.This DLL-file replaces the standard desktop-class :hp1.WPDesktop:ehp1. and is identical to it apart from
some minor extensions&colon.
:ol compact.
:li.An additional entry in the context menu to allow for a switch back to the
:link reftype=hd refid=ROOTDT.root desktop:elink.
:li.An additional settings page to give you the opportunity to remove some context menu items.
:eol.

:p.:hp7.Sammy:ehp7.
:p.This program manages the switching of environment variables when changing the desktop and
so enables the user to have its own environment.
For example one user might use a 16 bit compiler, whereas another uses a 32 bit compiler.
These two user need two different sets of environment variables PATH, INCLUDE and LIB.

.*----------------------------------------------------------
.*  Installation
.*----------------------------------------------------------
:h1 id=INSTALL.Installation

The installation of the whole package may be performed manually as well as by using
the installation program. For that reason both methodes will be described here.

:h2.Installation program

The installation program supports
:ul.
:li.the installation,
:li.the update and
:li.the deinstallation
:eul.
:p.of the whole program package.

The usage of the program is straight forward&colon. simply insert the installation diskette
and start :hp2.SAMINST:ehp2.. A brief explanation will appear in the main window of the
installation program, which of course includes an online help.

:note.The installation program has always to be started out of the
:link reftype=hd refid=DEFDT.default desktop:elink.
Only here additional
:link reftype=hd refid=WPSE3.global desktops:elink.
may be created.

:h2.Manual installation
:p.This kind of installation is intended only for experienced OS/2 users, who know about
the architecture of this operating system. The following information has been added to
give interested users a better insight into this application in order to allow for
modifications of the installation.

:p.:hp7.InitOS2:ehp7.
:p.This program replaces the :hp1.proteced mode shell:ehp1. of the operating system.
You simply have to change the :hp2.PROTSHELL:ehp2.-statement and to add an environment
variable in the file :hp2.CONFIG.SYS:ehp2..
:p.:hp5.Example&colon.:ehp5.
:parml.
:pt.old statement&colon.
:pd.:cgraphic.PROTSHELL=C&colon.&bsl.OS2&bsl.PMSHELL.EXE:ecgraphic.
:pt.new statements&colon.
:pd.:cgraphic.PROTSHELL=C&colon.&bsl.WPSAM&bsl.INITOS2.EXE C&colon.&bsl.OS2&bsl.PMSHELL.EXE
SET SAM=<userpath>[;<userpath>]:ecgraphic.
:pt.<userpath> stands for a path pointing to a user-directory. The default is
:pd.:cgraphic.C&colon.&bsl.WPSAM&bsl.USER:ecgraphic.
:eparml.

:p.:hp7.Sammy:ehp7.
:p.This program is started instead of the workplace shell. For that purpose the environment
variable :hp2.RUNWORKPLACE:ehp2. has to be changed and the old workplace shell has to be
referenced in a new variable :hp2.SAMWORKPLACE:ehp2..

:p.:hp5.Example&colon.:ehp5.
:parml.
:pt.old statement&colon.
:pd.:cgraphic.SET RUNWORKPLACE=C&colon.&bsl.OS2&bsl.PMSHELL.EXE:ecgraphic.
:pt.new statements&colon.
:pd.:cgraphic.SET RUNWORKPLACE=C&colon.&bsl.WPSAM&bsl.SAMMY.EXE
SET SAMWORKPLACE=C&colon.&bsl.OS2&bsl.PMSHELL.EXE:ecgraphic.
:eparml.

:p.:hp7.WPSam:ehp7. and :hp7.WPSamF:ehp7.
:p.The installation of these DLL-file is somewhat more difficult. You have to register 2
additional workplace shell classes in the
:link reftype=hd refid=DEFDT.default desktop:elink..
They are called :hp2.WPSam:ehp2. and :hp2.WPSamF:ehp2.
(note&colon. the names are case sensitive!). This may be done with REXX using the command
:hp1.SysRegisterObjectClass:ehp1..

:note.If new desktops shall be switched out of any
:link reftype=hd refid=PRIVDT.private desktop:elink.,
these classes have also to be registered there.
This case is not handeled by the installation program and has always to be done manually.

.*----------------------------------------------------------
.*  Setting up a new user
.*----------------------------------------------------------
:h1 id=EINREF.Setting up a new user

.*-- Introduction ------------------------------------------
:hp2.Introduction:ehp2.

:p.Creating new desktops is conforming to the CUA-standard&colon.
simply drag a template icon out of the template folder.

.*-- Creating a new user environment -----------------------------------
:h2.Creating a new user environment
:p.Creating a new desktop is a quite simple job&colon. Open the templates folder
and drag a :hp1.User template:ehp1. to anywhere. The system will then create a new user
desktop. Parts of this generation will be completed during the first start of this
user. Please be patient then. Any further starts will be performed very fast.

:p.Although a lot of people like an untidy desktop, it is a good idea to create
a new folder at least on your 
:link reftype=hd refid=ROOTDT.root desktop:elink.
that will contain all user icons.
The best way is to create a new directory and to create a shadow of this directory
on your workplace shell.

:p.If you have installed :hp4.WPSam:ehp4. by means of the installation program, this folder
will already be created for you. Look for a folder called "User" on your desktop.

:p.:lm margin=5.
:font facename=Helv size=14x10.
:hp2.For the unexperienced user&colon.:ehp2.
:ol compact.
:li.Open the templates folder.
:li.Open the drives folder.
:li.Open the drive, where you want to put the directory.
:li.Drag a :hp1.folder:ehp1. template to the location, where you want to
get the new directory.
:li.Click to the new created folder with the right mouse button while pressing
the :hp1.Alt:ehp1. key.
:li.Change the name of the new directory from :hp1.Folder:ehp1. to anything else
(e. g. :hp1.Users:ehp1.).
:li.Click to the new directory with the right mouse button (i. e. open the
object's context menu).
:li.Click to :hp1.Create shadow:ehp1.
:li.Select :hp1.desktop:ehp1. from the list.
:eol.
:font.
:lm margin=1.

.*-- Setting up the new user -------------------------------
:h2.Setting up the new user
:p.If you open the settings notebook of a user icon you will see the tab marks of
a normal :hp1.folder:ehp1.. Additonally, there will be two new tab marks,
:hp4.WPSam:ehp4. and :hp4.Init OS/2:ehp4..
:hp4.WPSam:ehp4. itsself consists of three pages.

.*-- Password settings page --------------------------------
:h2 scroll=none.WPSam settings page 1
:link reftype=hd res=001 auto split group=10
  vpx=left vpy=top vpcx=35% vpcy=100%
  scroll=vertical titlebar=none.
:link reftype=hd res=002 auto split group=11
  vpx=right vpy=top vpcx=65% vpcy=100%
  scroll=both titlebar=none.

.*   WPSam settings page 1: Secondary 1
:h2 res=001 group=10 hide.t2
:p.The picture shows the password settings page in the desktop settings notebook.
Here you may add, change or delete the password of the user desktop.
:p.If you activate :hp1.Enable password:ehp1. with the check button control, the user desktop
may only be started, deleted or changed if one knows the password.
:p.The password can only be changed, if the old one is known, regardless of the state of
the :hp1.Enable password:ehp1. button.
:p.If the system detects a :link reftype=hd refid=MAGSUPP.magnetic-stripe unit:elink., the textes
above the entry fields are modified.
:hp1.Old/New password or magnetic card:ehp1. indicates, that besides a manual input of the password
the use of a magnetic card is allowed.

.*   WPSam settings page 1: Secondary 2
:h2 res=002 group=11 hide.t1
:artwork align=left name='USRPG1US.BMP'.
:p.WPSam page 1 in the desktop settings notebook.

.*-- WPSam settings page 2 --------------------------------
:h2 scroll=none.WPSam settings page 2
:link reftype=hd res=003 auto split group=10
  vpx=left vpy=top vpcx=35% vpcy=100%
  scroll=vertical titlebar=none.
:link reftype=hd res=004 auto split group=11
  vpx=right vpy=top vpcx=65% vpcy=100%
  scroll=both titlebar=none.

.*   WPSam settings page 2: Secondary 1
:h2 res=003 group=10 hide.t2
:p.The picture shows settings page 2 in the desktop settings notebook.
This page accesses the following features&colon.
:ul.
:li.:hp2.Enable renaming:ehp2.&colon. If activated, the user name may be changed.
:li.:hp2.Enable moving:ehp2.&colon. If activated, a user icon may be moved to another
location.
:li.:hp2.Allow dropping:ehp2.&colon. If activated, other icons may be dropped onto the user desktop icon.
This also means, that other users may add icons to your desktop even if they don't know your
password! One note to prevent misunderstandings&colon. A user icon strictly speaken is
a modified folder. If you move an icon onto it, this icon will simply appear in
this user-folder (you may see it, if you open the 'icon view' of it). The icon will
:hp8.NOT:ehp8. appear on your user desktop!
:eul.

:p.The environment variables, that may be entered on this page, will be evaluated by
:link reftype=hd refid=LUMF.InitOS2:elink. and
:link reftype=hd refid=LUMF.Sammy:elink..
They have to be entered in the form :hp2.VARIABLE=CONTENTS:ehp2..
Environment variables, that are part of the file CONFIG.SYS, may be deleted here 
for a specific user by entering an empty contents, e. g.&colon.
:hp2.VARIABLE=:ehp2..

.*   WPSam settings page 2: Secondary 2
:h2 res=004 group=11 hide.t1
:artwork align=left name='USRPG2US.BMP'.
:p.WPSam settings page 2 in the desktop settings notebook.

.*-- WPSam settings page 3 --------------------------------
:h2 id=WPSE3 scroll=none.WPSam settings page 3
:link reftype=hd res=005 auto split group=10
  vpx=left vpy=top vpcx=35% vpcy=100%
  scroll=vertical titlebar=none.
:link reftype=hd res=006 auto split group=11
  vpx=right vpy=top vpcx=65% vpcy=100%
  scroll=both titlebar=none.

.*   WPSam settings page 3: Secondary 1
:h2 res=005 group=10 hide.t2
:p.If your user desktop is started for the first time, at least a new OS2.INI is generated.
The generation of a new OS2SYS.INI depends on the :hp1.global/:link reftype=hd refid=PRIVDT.private:elink.:ehp1.
setting (see later). The source used for this compilation process are the files :hp1.INI.RC:ehp1. and
:hp1.INISYS.RC:ehp1. in your OS2-directory. These files might be used directly or changed to
better suit the own requirements.
If you e.g. don' t have a PC with power-management and don't want to see the
dead power-management icon, you may remove the appropriate line in the RC-file.
If you are an experienced OS/2 user, you may also simulate the look and feel of the
Windows-desktop by creating a new RC-file out of :hp1.INI.RC:ehp1. and :hp1.WIN_30.RC:ehp1..
We recommend however not to change the original files but to create new ones!

:caution.Only experienced users should make such changes! In case of an erroneous .RC-file
your workplace-shell will not be started or the switching back to the root-desktop will not work!
If this should be the case some time, you have to reboot your computer and delete the
destroyed user object!
:ecaution.

:p.If a user-desktop creation process has already be completed (i. e. you have already
activated it for at least one time), you cannot change these settings again!

:p.In some cases, it might be interesting to use a user desktop on different machines (e. g. in
a network). But here the system of two different profiles implies a problem,
because it is necessary that the user- and system-profiles are able to keep track of each other.
Suggest you use only one system-profile on a single computer. If you now keep your desktop
on a network drive and access it from different machines with different system-profiles, this
might cause a system fault.

:p.To avoid this, you may choose to use a private system-profile.
In this case, each user gets his own system-profile and thus it is possible to use the
resulting combination of system- and user-profiles on different machines.
However OS/2 does not support the switching of the system profile in a running system.
This is the reason why such user desktops may only started by
:link reftype=hd refid=LUMF.InitOS2:elink..

.*   WPSam settings page 3: Secondary 2
:h2 res=006 group=11 hide.t1
:artwork align=left name='USRPG3US.BMP'.
:p.WPSam settings page 3 in the desktop settings notebook.

.*-- InitOS2 settings -------------------------------------
:h2 scroll=none.InitOS2 settings
:link reftype=hd res=007 auto split group=10
  vpx=left vpy=top vpcx=35% vpcy=100%
  scroll=vertical titlebar=none.
:link reftype=hd res=008 auto split group=11
  vpx=right vpy=top vpcx=65% vpcy=100%
  scroll=both titlebar=none.

.*   Init OS2 settings: Secondary 1
:h2 res=007 group=10 hide.t2
:p.This page contains the settings of a desktop for the program
:link reftype=hd refid=LUMF.InitOS2:elink.. For example the type of a session is
entered here.

:p.The default of the ProtShell is the program, named as an argument of :hp4.InitOS2:ehp4.
in the file CONFIG.SYS. This will be PMSHELL.EXE in most cases.

:p.If the entry field is activated, another shell may be entered. Normally CMD.EXE
will be offered as a default. This means, that :hp4.InitOS2:ehp4. simply starts an OS/2
fullscreen session, if such a user ID is opened.

:p.If you want to start the presentation manager without the workplace shell, you
also have to activate :hp1.Default:ehp1. here. Additonally you have to add an environment
variable :hp1.SAMWORKPLACE=x&colon.&bsl.OS2&bsl.CMD.EXE:ehp1. in the WPSam settings page 2.

:p.A user may be started by means of :hp4.InitOS2:ehp4. only if the check box
:hp1.Add user to 'Init OS/2' menu:ehp1. is activated.

.*   Init OS2 settings: Secondary 2
:h2 res=008 group=11 hide.t1
:artwork align=left name='USRPG4US.BMP'.
:p.InitOS2 settings page in the desktop settings notebook.

.*----------------------------------------------------------
.*  Usage of InitOS2
.*----------------------------------------------------------
:h1.Usage of InitOS2
:p.:hp2.InitOS2:ehp2. is used to prevent a computer from unauthorized usage.
As a prerequisite it is necessary, that the BIOS of the PC allows to set the boot order
C: A: and to set a BIOS password.

:p.Additionally :hp2.InitOS2:ehp2. offers the possibility, to start non-workplace shell
and non-presentation manager sessions.

.*-- Init OS2 main window --------------------------------
:h2 scroll=none.InitOS/2 main window
:link reftype=hd res=009 auto split group=10
  vpx=left vpy=top vpcx=52% vpcy=100%
  scroll=horizontal titlebar=none.
:link reftype=hd res=010 auto split group=11
  vpx=right vpy=top vpcx=48% vpcy=100%
  scroll=vertical titlebar=none.

.*   InitOS/2 main window: Secondary 1
:h2 res=009 group=10 hide.t1
:artwork align=left name='USINMAIN.BMP'.
:p.InitOS/2 main window

.*   InitOS/2 main window: Secondary 2
:h2 res=010 group=11 hide.t2
:p.The picture shows the window you will see after startup of the computer.
Usage is very simple. To start a user-ID, you simply have to type in user name
and password (if necessary) into the two entry fields.
After pressing Enter (or the OK button) the user shell will be started.
:p."Navigation" inside this dialog box is conforming to the CUA standard.
So you reach the next entry field or button with the tabulator key.
:p.These are the buttons&colon.
:ul.
:li.OK&colon. Starting a user shell. This is the default button and may thus be reached
with the enter key, if the cursor is on an entry field.
:li.Start default&colon. :hp2.InitOS2:ehp2. allows you to define a default user.
This desktop may be started automatically after an adjustable time, if the operator doesn't make
any input.
This default desktop may also be started by pressing this button or the F2 key.
:li.Setup&colon. This button starts a dialog box, which is intended for use by a system
administrator. It may therefore be protected by a password.
The shortcut key of Setup is F9.
:li.Shutdown&colon. This button (may also be reached via F10 key) prepares the system for
shutdown.
:eul.
:p.If the system detects the presence of a :link reftype=hd refid=MAGSUPP.magnetic-stripe unit:elink.,
the text :hp1.Password:ehp1. is will be changed to :hp1.Password or magnetic card:ehp1..
In this case, the password may be entered by means of a magnetic card.
If no user has been entered before, the system will automatically search the User ID and
start it.

.*-- Init OS2 setup --------------------------------
:h2 scroll=none.InitOS/2 Setup
:link reftype=hd res=011 auto split group=10
  vpx=left vpy=top vpcx=40% vpcy=100%
  scroll=horizontal titlebar=none.
:link reftype=hd res=012 auto split group=11
  vpx=right vpy=top vpcx=60% vpcy=100%
  scroll=vertical titlebar=none.

.*   InitOS/2 setup window: Secondary 1
:h2 res=011 group=10 hide.t1
:p.:artwork align=left name='USINPWD.BMP'.
:p.InitOS/2 Administrator password

:p.:artwork align=left name='USINSET.BMP'.
:p.InitOS/2 Setup

.*   InitOS/2 setup window: Secondary 2
:h2 res=012 group=11 hide.t2
:p.If :hp1.Setup:ehp1. is pressed in the main window, a dialog box is shown, where
the system administrator password has to be entered.
After a correct answer to the system, the setup dialog box will be started.
:p.The setup window contains the following selections&colon.
:ul.
:li.:hp1.Default user&colon.:ehp1. Here a user name may be entered. Its desktop is automatically
started, if during a timeout time no key is pressed.
:li.:hp1.Timeout&colon.:ehp1. If this time elapses without any key being pressed, the
default desktop will be started. If one of the fields :hp1.Default user:ehp1. or :hp1.Timeout:ehp1.
is empty, this feature is deactivated.
:li.:hp1.Administrator password / Verification&colon.:ehp1.
Here a system administrator password may be entered. This has to be done twice in order
to detect wrong spelling.
:li.:hp1.OK&colon.:ehp1. The dialog box will be closed with this button. The 
:link reftype=hd refid=DEFDT.default desktop:elink.
and the administrator password will be saved. This button is the default button and thus may
be reached with the Enter key.
:li.:hp1.Delete password&colon.:ehp1. If no password has been entered in the two password fields,
InitOS2 will keep the old one. It may be deleted with this button.
:li.:hp1.Cancel&colon.:ehp1. After pressing this button the dialog box will be closed without
saving the changes. This button may also be reached by pressing the escape key.
:eul.

.*----------------------------------------------------------
.*  Usage of WPSam
.*----------------------------------------------------------
:h1.Usage of WPSam
:p.The usage of WPSam is quite simple; it is conforming to the CUA91 standard.

:p.The concept took also pattern from the concept of the workplace shell.
So each desktop is represented by an icon. This icon is an object of class
:link reftype=hd refid=LUMF.WPSam:elink.,
which is derived from a normal folder. You may recognize this by the names of the
settings pages. There were merely added some additional pages necessary for the function
of the program.
If such a "desktop folder" is opened as icon view, you will see some files in it that are
important for the operation of a workplace shell.
A :link reftype=hd refid=WPSE3.global desktops:elink. needs the file :hp2.OS2.INI:ehp2. 
as well as the workplace shell folder :hp1.desktop_user:ehp1..

:p.These files will be generated automatically, if the user is started for the first time.
They are also needed for users of :link reftype=hd refid=LUMF.type 2 (presentation manager
user):elink.. The reason is, that in principal you may start the workplace shell out of
a normal user ID.
If these files are not yet generated, the user ID is call :hp1.uninitialized:ehp1..

:h2.Starting a user desktop
:p.Simply double click a desktop icon. If it is not password protected,
it is started immediately. If it is the first invocation of a new desktop, the
appropriate :hp1.INI:ehp1.-files will be generated.

:p.:hp1.WPSam:ehp1. doesn't check the type of a desktop, but :hp6.always:ehp6. starts
the workplace shell. Other user environment types can only be started with :hp1.InitOS2:ehp1..

:p.If the desktop icon is protected by a password, you are first asked for the password.
Invocation of the desktop is rejected on a wrong entered password (of course!).
Every successful and unsuccessful invocation of a desktop is recorded in the
users history data field.

:h2.Returning to the root desktop
:p.Open the desktop context menu by clicking to an empty desktop area with the right mouse button
and choose the menu entry :hp1.Root desktop:ehp1. (see the following figure).

:artwork align=center name='DTCTXUS.BMP'.

After saving the actual window positons on the actual desktop, the
root desktop is opened.

:h2.Viewing the history data
:p.Each succussful and unsuccessful invocation of a user ID is recorded automatically with
a time stamp. You may view the last 100 proceedings. This is done by opening the
:hp1.settings:ehp1. of a user icon. If you select :hp1.File:ehp1. there, you see
the history field on page 3.

.*----------------------------------------------------------
.*  magnetic-stripe support
.*----------------------------------------------------------
:h1 id=MAGSUPP.Magnetic-stripe support
:p.Passwords may either be entered manually or by magnetic cards.
As a prerequisite, the DLL-file :hp1.MAGCALLS.DLL:ehp1. has to be present.
This file usually is supplied with the OS/2 support of the magnetic-strip unit.
We already tested the systems of IBM (4717) and DESKO (MSL4717-x).
Suitable cards for the purpose of entering passwords are all cards that use track 2.
You might use special cards offered by DESKO, but every card customary in trade is sufficient, e. g.
an EC-card or a credit-card.
If you use other cards, be sure, that the contents of track 2 are never changed!
If you know the contents of track 2, the password may also be entered manually. If you
use credit cards, this will not be practicable, however, because the password is very long.

:p.InitOS2 is able, to detect a user belonging to a password, that has been entered by means of
a magnetic card.
If the same card is used for different users, however, always the (alphabetical) first user will be started.

.*----------------------------------------------------------
.*  Planned extensions
.*----------------------------------------------------------
:h1.Planned extensions
:p.There are still lots of ideas for extensions.
Above all :hp4.InitOS2:ehp4. is a very new program, that lacks of some useful
properties. Here are some examples&colon.
:ul compact.
:li.The window manager of InitOS2 will be extended by some controls. 
So a listbox shall simplify the selection of users with long names.
Additionally the colors of the windows will be adjustable.
:li.The parser of environment variables will be extended, so that it can read variables
of type %var%.
:li.An interactive program shall enable the user to generate .RC-files.
:eul.

:p.If you are also missing any features, please don't hesitate to inform us about.

.*----------------------------------------------------------
.*  Registration
.*----------------------------------------------------------
:h1.Registration

:p.:p.:hp4.WPSam:ehp4. is no shareware! Nevertheless we will give interested persons
the chance to test the software. This is the reason, why we distribute
:hp4.WPSam:ehp4. as a complete product.
Unregistered users are able to test the application. You may create as many desktops as
you want, but they only work for 21 days. They will work again after registration of the
product.

:p.If you register :hp4.WPSam:ehp4., you may use the actual and the following version of
:hp4.WPSam:ehp4. with the password provided by us. This enables a quick update
via Internet or Fido-net.

:p.If you don't know a dealer, who can supply you with WPSam, you may order it directly
at us.
On receipt of a proof, we offer students and pensioners very cheap licenses.
Licenses for companies and public utilities are also differentiated.
Please ask by mail, e-mail or fax.

.*   Registration
:h2.Registration
:p.Select :hp1.About:ehp1. in the context menu of a user symbol or the
template in the template folder. If the version of WPSam has not yet been registered
on the computer, you are asked for a name and registration password (see figure).

:artwork align=center name='REGUS.BMP'.

If these two lines have been entered correctly, in future invocations of this window
only the registration name will be shown.

:p.The name has to be entered correctly, because the password is derived from it.
Even spaces have to be entered exactly. The name however isn't case sensitive.
You will get the password after paying the registration fee.

:p.The password is quite long (14 characters), because it contains the name, serial number
and version number.



.*   Registration form
:h2.Registration form

:lm margin=5.
:lines.
                                        ___________________, ___-____-19__




Noller &amp. Breining Software
Tannenstrasse 18

D-71554 Weissach im Tal



:hp2.Order:ehp2.

Herewith I order the actual version of WPSam. I know, that this program
is offered without any kind of warranty.:elines.
:p.The wished type of license is&colon.:dl compact tsize=5.
:dt.:artwork runin name='sq.bmp'.
:dd.student / pensioner (US$ 15.--).
:dt.:artwork runin name='sq.bmp'.
:dd.personal license (US$ 40.--).
:edl.
:p.Please send the software on:dl compact tsize=5.
:dt.:artwork runin name='sq.bmp'.
:dd.3«" diskette or
:dt.:artwork runin name='sq.bmp'.
:dd.5¬" diskette.
:edl.
:p.Payment shall be done&colon.:dl compact tsize=5.
:dt.:artwork runin name='sq.bmp'.
:dd.directly or
:dt.:artwork runin name='sq.bmp'.
:dd.on account.
:edl.
:p.Personal licenses for students have to be payed directly (because of
the low amount). Please note, that we cannot accept any cheques from outside
of Germany.
:p.Please send to the following address&colon.
:cgraphic.:font facename=Courier size=10x10.
name, first name&colon.          ___________________________________________

street, No.&colon.               ___________________________________________

Town&colon.                      ___________________________________________

ZIP, country&colon.              ___________________________________________
:ecgraphic.

:p.:font facename=Courier size=10x10.__________________________________:font.
:p.(Signature, date)                                                  V 3.0
:lm margin=1.


.*----------------------------------------------------------
.*  Versions
.*----------------------------------------------------------
:h1.Versions
:cgraphic.:hp2.
   3.0  Support of SOM 2 Runtime.

   2.1  Addition of magnetic-stripe support.

   2.0  Release of SAMMY and INITOS2. The switching mechanism has been
        changed.

   1.2  Availability of german version. Removing of "20 second pausing".
        First release of InitOS2.
 
   1.2á User desktops save their window-positions. Reorganization
        of extended attributes. Add registration feature.

   1.1  Addition of Workplace Shell support for OS/2 2.x

   1.0  First public version of SAM for OS/2 1.x.
:ehp2.:ecgraphic.
:euserdoc.
