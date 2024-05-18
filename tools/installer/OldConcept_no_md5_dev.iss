;
; S.T.A.L.K.E.R. 2 Old Concept
; Script for Inno Setup 5 compiler
; First version: 2014.04.08
; First author to blame: utak3r
; Last modification: 2024.05.16
; Last modifier: loxotron
;
; put the game files in the [game_distrib_files] folder.
; after this, call prepare_archives.cmd script.
;
; This script is prepared for and meant to be used with:
; Inno Setup 5.5.4 unicode [ http://www.jrsoftware.org/isdl.php ]
; Inno Script Studio 2.1.0.20  [ https://www.kymoto.org/products/inno-script-studio/downloads ]
;
;
; The directory structure to build the installer is as follows:
;
; LostAlpha.iss (this script)
; 7za.exe
; prepare_archives.cmd
;
; [installer_images]
; installer_images\LAinstallerImage.bmp
; installer_images\LAinstallerSmallImage.bmp
; installer_images\logo_stk2_v2.ico
;
; [Output\3rdparties] (additional software to be installed)
; Output\3rdparties\directx_Jun2010_redist.exe (from: http://www.microsoft.com/en-us/download/details.aspx?id=8109 )
; Output\3rdparties\oalinst.exe
; Output\3rdparties\vcredist_x86.exe (this should be from: http://www.microsoft.com/en-us/download/details.aspx?id=40784 )
; Output\3rdparties\Xvid-1.3.2-20110601.exe
;
; [game_distrib_files] (complete release of the game to be installed)
; [Output] (the compiled installer will be put here)
; [Output\game] (archives of the games files will be put here using prepare_archives.cmd script)
;
;

; dirs used:
#define STK2OC_game_files ".\game_distrib_files"
#define STK2OC_installer_support_files "."

; versions, names etc.:
#define STK2OC_shortcut_name "S.T.A.L.K.E.R. 2 OC Developer"
#define STK2OC_app_name "S.T.A.L.K.E.R. 2 OC Developer"
#define STK2OC_directory_name "S.T.A.L.K.E.R. 2 Old Concept"
#define STK2OC_copyright "dezowave"
#define STK2OC_version "0.42"
#define STK2OC_version_text "0.42"

[Types]
Name: "full"; Description: "Full installation"
Name: "custom"; Description: "Custom installation"; Flags: iscustom

[Components]
Name: "stk2oc"; Description: "Developer Components"; Types: full custom; Flags: fixed
;///////////////////////////////////////////////
Name: "xray"; Description: "Engine sources"; Types: full custom
Name: "textures"; Description: "Textures Sources"; Types: full custom
Name: "tools"; Description: "Tools and Scripts"; Types: full custom
Name: "tests"; Description: "Coders Tests"; Types: full custom
Name: "docs"; Description: "Coders Docs"; Types: full custom
;///////////////////////////////////////////////

[Files]
Source: "{#STK2OC_game_files}\xray\*"; DestDir: "{app}\xray\"; Flags: ignoreversion createallsubdirs recursesubdirs skipifsourcedoesntexist; Components: xray
Source: "{#STK2OC_game_files}\third_party\*"; DestDir: "{app}\third_party\"; Flags: ignoreversion createallsubdirs recursesubdirs skipifsourcedoesntexist; Components: xray
Source: "{#STK2OC_game_files}\build\*"; DestDir: "{app}\build\"; Flags: ignoreversion createallsubdirs recursesubdirs skipifsourcedoesntexist; Components: xray
Source: "{#STK2OC_game_files}\CMakeLists.txt"; DestDir: "{app}"; Components: xray
Source: "{#STK2OC_game_files}\README.md"; DestDir: "{app}"; Components: xray
Source: "{#STK2OC_game_files}\dependency_graph.pdf"; DestDir: "{app}"; Components: xray
Source: "{#STK2OC_game_files}\tools\*"; DestDir: "{app}\tools\"; Flags: ignoreversion createallsubdirs recursesubdirs skipifsourcedoesntexist; Components: tools
Source: "{#STK2OC_game_files}\tests\*"; DestDir: "{app}\tests\"; Flags: ignoreversion createallsubdirs recursesubdirs skipifsourcedoesntexist; Components: tests
Source: "{#STK2OC_game_files}\3rd-party_wiki\*"; DestDir: "{app}\3rd-party_wiki\"; Flags: ignoreversion createallsubdirs recursesubdirs skipifsourcedoesntexist; Components: docs
Source: "{#STK2OC_game_files}\resources\*.psd"; DestDir: "{app}\resources\"; Flags: ignoreversion createallsubdirs recursesubdirs skipifsourcedoesntexist; Components: textures

[Run]

[InstallDelete]

[Icons]

[Setup]
PrivilegesRequired=admin
AppName={#STK2OC_app_name}
AppVersion={#STK2OC_version_text}
AppCopyright={#STK2OC_copyright}
DefaultDirName={commonpf}\{#STK2OC_directory_name}
DisableDirPage=no
DisableProgramGroupPage=yes
AppPublisher={#STK2OC_copyright}
VersionInfoVersion={#STK2OC_version}
VersionInfoCompany={#STK2OC_copyright}
VersionInfoDescription={#STK2OC_app_name}
VersionInfoTextVersion={#STK2OC_version_text}
VersionInfoCopyright={#STK2OC_copyright}
VersionInfoProductName={#STK2OC_app_name}
VersionInfoProductVersion={#STK2OC_version}
VersionInfoProductTextVersion={#STK2OC_version_text}
MinVersion=6.1sp1
WizardImageFile={#STK2OC_installer_support_files}\installer_images\LAinstallerImage.bmp
SetupIconFile={#STK2OC_installer_support_files}\installer_images\logo_stk2_v2.ico
WizardSmallImageFile={#STK2OC_installer_support_files}\installer_images\LAinstallerSmallImage.bmp
DiskSpanning = no
DiskSliceSize = 2100000000
OutputBaseFilename = old_concept_dev
Compression = lzma2/ultra64
SolidCompression = yes
LZMAUseSeparateProcess = yes
LZMANumBlockThreads = 6

[CustomMessages]
msgInstallingBins=Installing binaries
msgInstallingGamedata=Installing game data files
msgInstallingAppdata=Installing application data files
msgInstallingMaindir=Installing main game files
msgInstallingVcredist=Installing Microsoft VC++ runtimes
msgInstallingDXredist=DirectX runtimes
msgInstallingOAL=Installing audio codec
msgInstallingXvid=Installing video codec
msgDeletingUnwantedFiles=Deleting not needed files
;rus.WipeUser=Рекомендуется пересоздать настройки с нуля (user.ltx будет стёрт). Вы согласны?
;eng.WipeUser=It is recommended to recreate your settings for the game (user.ltx will be erased). Do you want to do it?

[Languages]
Name: rus; MessagesFile: compiler:Languages\Russian.isl
Name: eng; MessagesFile: compiler:Default.isl

[UninstallDelete]

[Code]
#IFDEF UNICODE
  #DEFINE AW "W"
#ELSE
  #DEFINE AW "A"
#ENDIF
type
  INSTALLSTATE = Longint;
const
  INSTALLSTATE_INVALIDARG = -2;  // An invalid parameter was passed to the function.
  INSTALLSTATE_UNKNOWN = -1;     // The product is neither advertised or installed.
  INSTALLSTATE_ADVERTISED = 1;   // The product is advertised but not installed.
  INSTALLSTATE_ABSENT = 2;       // The product is installed for a different user.
  INSTALLSTATE_DEFAULT = 5;      // The product is installed for the current user.

  VC_2005_REDIST_X86 = '{A49F249F-0C91-497F-86DF-B2585E8E76B7}';
  VC_2005_REDIST_X64 = '{6E8E85E8-CE4B-4FF5-91F7-04999C9FAE6A}';
  VC_2005_REDIST_IA64 = '{03ED71EA-F531-4927-AABD-1C31BCE8E187}';
  VC_2005_SP1_REDIST_X86 = '{7299052B-02A4-4627-81F2-1818DA5D550D}';
  VC_2005_SP1_REDIST_X64 = '{071C9B48-7C32-4621-A0AC-3F809523288F}';
  VC_2005_SP1_REDIST_IA64 = '{0F8FB34E-675E-42ED-850B-29D98C2ECE08}';
  VC_2005_SP1_ATL_SEC_UPD_REDIST_X86 = '{837B34E3-7C30-493C-8F6A-2B0F04E2912C}';
  VC_2005_SP1_ATL_SEC_UPD_REDIST_X64 = '{6CE5BAE9-D3CA-4B99-891A-1DC6C118A5FC}';
  VC_2005_SP1_ATL_SEC_UPD_REDIST_IA64 = '{85025851-A784-46D8-950D-05CB3CA43A13}';

  VC_2008_REDIST_X86 = '{FF66E9F6-83E7-3A3E-AF14-8DE9A809A6A4}';
  VC_2008_REDIST_X64 = '{350AA351-21FA-3270-8B7A-835434E766AD}';
  VC_2008_REDIST_IA64 = '{2B547B43-DB50-3139-9EBE-37D419E0F5FA}';
  VC_2008_SP1_REDIST_X86 = '{9A25302D-30C0-39D9-BD6F-21E6EC160475}';
  VC_2008_SP1_REDIST_X64 = '{8220EEFE-38CD-377E-8595-13398D740ACE}';
  VC_2008_SP1_REDIST_IA64 = '{5827ECE1-AEB0-328E-B813-6FC68622C1F9}';
  VC_2008_SP1_ATL_SEC_UPD_REDIST_X86 = '{1F1C2DFC-2D24-3E06-BCB8-725134ADF989}';
  VC_2008_SP1_ATL_SEC_UPD_REDIST_X64 = '{4B6C7001-C7D6-3710-913E-5BC23FCE91E6}';
  VC_2008_SP1_ATL_SEC_UPD_REDIST_IA64 = '{977AD349-C2A8-39DD-9273-285C08987C7B}';
  VC_2008_SP1_MFC_SEC_UPD_REDIST_X86 = '{9BE518E6-ECC6-35A9-88E4-87755C07200F}';
  VC_2008_SP1_MFC_SEC_UPD_REDIST_X64 = '{5FCE6D76-F5DC-37AB-B2B8-22AB8CEDB1D4}';
  VC_2008_SP1_MFC_SEC_UPD_REDIST_IA64 = '{515643D1-4E9E-342F-A75A-D1F16448DC04}';

  VC_2010_REDIST_X86 = '{196BB40D-1578-3D01-B289-BEFC77A11A1E}';
  VC_2010_REDIST_X64 = '{DA5E371C-6333-3D8A-93A4-6FD5B20BCC6E}';
  VC_2010_REDIST_IA64 = '{C1A35166-4301-38E9-BA67-02823AD72A1B}';
  VC_2010_SP1_REDIST_X86 = '{F0C3E5D1-1ADE-321E-8167-68EF0DE699A5}';
  VC_2010_SP1_REDIST_X64 = '{1D8E6291-B0D5-35EC-8441-6616F567A0F7}';
  VC_2010_SP1_REDIST_IA64 = '{88C73C1C-2DE5-3B01-AFB8-B46EF4AB41CD}';

  VC_2013_REDIST_X86 = '{CE085A78-074E-4823-8DC1-8A721B94B76D}'; // 12.0.21005

function MsiQueryProductState(szProduct: string): INSTALLSTATE;
  external 'MsiQueryProductState{#AW}@msi.dll stdcall';

function VCVersionInstalled(const ProductID: string): Boolean;
begin
  Result := MsiQueryProductState(ProductID) = INSTALLSTATE_DEFAULT;
end;