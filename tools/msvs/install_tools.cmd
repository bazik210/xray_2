@rem enable multiple writes to environment variables
@setlocal   ENABLEDELAYEDEXPANSION

@rem find out microsoft visual studio installation folder

@regsvr32 /s %VS90COMNTOOLS%\..\IDE\VsWizard.dll

@if "%~1" == "" @(
	@if "%VS90COMNTOOLS%" EQU "" @(
		@echo cannot find installed version of Microsoft Visual Studio 2008
		@echo please, specify Microsoft Visual Studio 2008 folder as a command line parameter
		@pause
		@exit
    )

    @set msvs_folder="%VS90COMNTOOLS%\..\..\"
    @goto :copy_files
)

@set msvs_folder=%~1

:copy_files

@rem Remove quotes from microsoft visual studio installation folder
@set msvs_folder=###%msvs_folder%###
@set msvs_folder=%msvs_folder:"###=%
@set msvs_folder=%msvs_folder:###"=%
@set msvs_folder=%msvs_folder:###=%


@rem install project wizards
@echo -----------------------------
@echo INSTALLING Project Wizards...
@echo -----------------------------

@rmdir /S /Q "%msvs_folder%\vc\vcprojects\x-ray"
@mkdir "%msvs_folder%\vc\vcprojects\x-ray"

@rmdir /S /Q "%msvs_folder%\vc\vcwizards\appwiz\x-ray"
@mkdir "%msvs_folder%\vc\vcwizards\appwiz\x-ray"

@for /D %%i in (wizards/*.*) do @(
	@xcopy /Q /Y "wizards\%%i\*.vsz"	"%msvs_folder%\vc\vcprojects\"
	@xcopy /Q /Y "wizards\%%i\*.ico"	"%msvs_folder%\vc\vcprojects\"
	@xcopy /Q /Y "wizards\%%i\*.vsdir"	"%msvs_folder%\vc\vcprojects\x-ray\"
	@xcopy /Q /S "wizards\%%i\*.*"		"%msvs_folder%\vc\vcwizards\appwiz\x-ray\%%i\"
)


@rem install autoexp.dat for expanding watches
@echo -----------------------------
@echo INSTALLING Watch Extenders...
@echo -----------------------------
@xcopy /Q /S /Y autoexp.dat "%msvs_folder%\common7\packages\debugger\"


@rem install stepover.reg to skip annoying one-line functions call stack walking
@echo -----------------------------
@echo INSTALLING Debug Extenders...
@echo -----------------------------
@regedit /s step_over.reg