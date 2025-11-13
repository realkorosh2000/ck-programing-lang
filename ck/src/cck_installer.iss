;----------------------------------------
; CK/CKS Installer Script v1.1 (Windows 7 32-bit)
; Using separate icon files for CK, CKS, and shortcut
;----------------------------------------

[Setup]
AppName=CK Language
AppVersion=1.1
AppPublisher=CK Inc.
DefaultDirName={pf}\CCK
DefaultGroupName=CK Language
OutputDir=.
OutputBaseFilename=CK_Installer_1.1
AllowNoIcons=yes
DisableProgramGroupPage=no
Compression=lzma
SolidCompression=yes
ArchitecturesInstallIn64BitMode=x86

[Files]
; Main executable
Source: "C:\users\korosh2000\desktop\C++\ck\cck.exe"; DestDir: "{app}"; Flags: ignoreversion

; Icon files
Source: "C:\users\korosh2000\desktop\C++\ck\ck_icon.ico"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\users\korosh2000\desktop\C++\ck\cks_icon.ico"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
; Start Menu shortcut uses ck_icon.ico
Name: "{group}\CCK"; Filename: "{app}\cck.exe"; IconFilename: "{app}\ck_icon.ico"

[Run]
; Add CCK folder to system PATH (optional)
Filename: "cmd"; Parameters: "/C setx PATH ""%PATH%;{app}"""; StatusMsg: "Adding CCK to PATH..."

[Registry]
; Register .ck file type with its icon
Root: HKCR; Subkey: ".ck"; ValueType: string; ValueData: "CKFile"; Flags: uninsdeletevalue
Root: HKCR; Subkey: "CKFile\DefaultIcon"; ValueType: string; ValueData: "{app}\ck_icon.ico"; Flags: uninsdeletekey
Root: HKCR; Subkey: "CKFile\shell\open\command"; ValueType: string; ValueData: """{app}\cck.exe"" ""%1"""; Flags: uninsdeletekey

; Register .cks file type with its icon
Root: HKCR; Subkey: ".cks"; ValueType: string; ValueData: "CKSFile"; Flags: uninsdeletevalue
Root: HKCR; Subkey: "CKSFile\DefaultIcon"; ValueType: string; ValueData: "{app}\cks_icon.ico"; Flags: uninsdeletekey
Root: HKCR; Subkey: "CKSFile\shell\open\command"; ValueType: string; ValueData: """{app}\cck.exe"" ""%1"""; Flags: uninsdeletekey
