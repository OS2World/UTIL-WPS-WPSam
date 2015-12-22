.*----------------------------------------------------------
.* WPSAM -- Workplace Shell Administration
.*
.* IPF Help File
.*
.*----------------------------------------------------------

:userdoc.

.*----------------------------------------------------------
.*  Set/Change/Enable password
.*      res = PANEL_PWD
.*----------------------------------------------------------
:h1 res=40.Change and activate password
:i1 id=PWD.Password

:p.A desktop may optionally be protected by an individual password. This password may contain spaces and is
case sensitive.

:p.If your computer is equipped with a magnetic stripe reader, password input may be carried out
by either using the keyboard or an identification card.

:p.The protection covers
:ul compact.
:li.opening,
:li.starting and
:li.deleting a desktop.
:eul.

:p.Unauthorized accesses will be recorded in the
:link reftype=fn refid=fnProtokoll.history:elink..

:fn id=fnProtokoll.
:p.:lines align=center.:hp2.History:ehp2.:elines.
:p.You can find the history field in the :hp1.file settings page:ehp1..
:efn.

.*----------------------------------------------------------
.*  About panel
.*      res = PANEL_ABOUT
.*----------------------------------------------------------
:h1 res=10 hide.Product information / registration

:p.Desktops created by WPSam will discontinue to work after 3 weeks until your copy of WPSam is registered.
:p.If your version of WPSsam is not yet registered, this page contains the necessary entry-fields to unlock
your version of WPSam.
We will send you your personal registration password, if you fill out the
:link reftype=fn refid=fnRegForm.registration form:elink..

:fn id=fnRegForm.
:p.:lines align=center.:hp2.Registration form:ehp2.:elines.
:p.You may find a registration form in the file :hp4.WPSam.inf:ehp4..
:efn.

.*----------------------------------------------------------
.*  Adjustments
.*      res = PANEL_ADJUSTMENTS
.*----------------------------------------------------------
:h1 res=50.Environment variables and styles

:p.:hp2.Environment variables:ehp2.
:p.Every desktop inherits the 
:link reftype=fn refid=fnEnvVar.environment variables:elink.
found in the file :hp1.CONFIG.SYS:ehp1..
These may be modified, substituted or deleted here and get valid as soon as the associated desktop
will be started the next time.

:fn id=fnEnvVar.
Each environment variable must be placed in a separate line and be separated from the following value by an
equals sign '='.
:p.The assignment operator :hp1.SET:ehp1. used in the :hp1.CONFIG.SYS:ehp1. and on a command line must be
omitted here.
:efn.

:p.:hp2.Style:ehp2.
:p.These settings allow to restrict some features of the user object.
To rename or move it and to enable you to drop other objects into it
(:link reftype=fn refid=fnDropping.dropping:elink.),
you have to enable the appertaining check box.
:p.These options take only effect on operations on the desktop.
The :hp4.move:ehp4. and :hp4.ren:ehp4. operation of the command processor may not be prevented here.

:fn id=fnDropping.
:p.A :hp4.WPSam:ehp4. desktop is a folder containing the user specific profile and the appertaining desktop tree.
Activating the :hp2.dropping:ehp2. option, the possibility to move or copy objects into this folder may be enabled.

:p.To move objects between different desktops, first the icon view of the inactive desktop has to be opened
(select :hp1.Open -> icon view:ehp1.).
From there the desktop icon may be opened by a double click.

:p.:p.:hp2.Note:ehp2.
:p.So called :hp2.abstract objects:ehp2., like shadows, printer objects or program objects manage their
instance data in the user profile (:hp1.OS2.INI:ehp1.).
To move such objects between different desktops, both profiles would have to be opened.
This however is not supported by OS/2, so moving abstract objects to another desktop is not possible.
:efn.

:p.:p.:hp2.Close applications:ehp2.
:p.All applications, that were started while this desktop was active, will be closed automatically
when switching to another desktop.
This field gets valid on the next starting of the desktop.

.*----------------------------------------------------------
.*  Resource files
.*      res = PANEL_CREATE
.*----------------------------------------------------------
:h1 res=60.Resource files

:p.On this page you can select the resource files which are to be used to
generate the profiles.

:p.Default values in the entry fields are the resource files that were generated
during the installation of your OS/2 system.

:p.:p.:hp2.Annotation:ehp2.
:p.The system profile may not be changed in a running system.
.br
Therefore user objects with a private system profile can only be started
with help of the program InitOS2 while booting your system.

.*----------------------------------------------------------
.*  'INITOS2' Hilfeseiten
.*      res = PANEL_INITOS2
.*----------------------------------------------------------
:h1 res=70.Init OS/2

:p.:hp2.ProtShell:ehp2.
:p.For desktop objects started via InitOS2 a ProtShell statement different
from the one in the :hp1.CONFIG.SYS:ehp1. file can be defined.

:p.E.g. when entering the OS/2 command processor :hp4.CMD.EXE:ehp4. the user will
not boot the Presentation Manager but only an OS/2 fullscreen session instead.

.*----------------------------------------------------------
.*  Enter password
.*      res = PANEL_PWDIN
.*----------------------------------------------------------
:h1 res=30 hide nosearch.Enter password

:p.This desktop is protected by a password.
.br
Please enter it to continue performing the desired action.

:p. In case your machine is equiped with a magnetic stripe reader,
you may insert your identification card instead.

:p.Unauthorized attempts are recorded in the
:link reftype=fn refid=fnProtokoll.history panel:elink..

.*----------------------------------------------------------
.*  Context menu
.*      res = PANEL_CONTEXTMENU
.*----------------------------------------------------------
:h1 res=80.Context menu

:p.This page is provided by WPSam to remove inappropriate menus out of the
:link reftype=fn refid=fnKontext.contextmenu:elink.
of the desktop.

:fn id=fnKontext.
The :hp4.primary context menu of the desktop:ehp4. is activated by clicking
on the background of the desktop with the right mouse button.
:efn.

:euserdoc.

