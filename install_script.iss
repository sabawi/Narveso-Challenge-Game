; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{32512152-3FCF-4C4D-91D5-06122B530930}
AppName=Narveso
AppVerName=Narveso 0.94
AppPublisher=Al Sabawi
AppPublisherURL=http://alsabawi.com/
AppSupportURL=http://alsabawi.com/
AppUpdatesURL=http://alsabawi.com/
AppVersion=0.94
AppCopyright=Copyright (C) 1997-2009 Al Sabawi
DefaultDirName={pf}\Narveso
DefaultGroupName=Narveso
LicenseFile=C:\Users\sabawi\programs\MyPrograms\narveso2\license.txt
OutputDir=C:\Users\sabawi\programs\MyPrograms\narveso2\setup
OutputBaseFilename=setup_Narveso0_94
SetupIconFile=C:\Users\sabawi\programs\MyPrograms\narveso2\narveso.ico
Compression=lzma/ultra64
SolidCompression=yes
ArchitecturesAllowed=
Uninstallable=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}";

[Files]
Source: "C:\Users\sabawi\programs\MyPrograms\narveso2\Narveso\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\Narveso"; Filename: "{app}\Narveso.exe";  WorkingDir: "{app}"; Comment: "Narveso Game - By Al Sabawi"
Name: "{group}\Uninstall Narveso"; Filename: "{uninstallexe}" ;  WorkingDir: "{app}"; Comment: "Click here to uninstall game from your system"
Name: "{commondesktop}\Narveso"; Filename: "{app}\Narveso.exe"; WorkingDir: "{app}";  Comment: "Narveso Game - By Al Sabawi"; Tasks: desktopicon
Name: "{commonprograms}\Games\Narveso"; Filename: "{app}\Narveso.exe"; WorkingDir: "{app}";  Comment: "Narveso Game - By Al Sabawi"; Tasks: desktopicon

[Run]
;Filename: "{app}\Narveso.exe"; Description: "{cm:LaunchProgram,Narveso}"; Flags: nowait postinstall skipifsilent
Filename: "{app}\README.htm"; Description: "View the README file"; Flags: postinstall shellexec skipifsilent
