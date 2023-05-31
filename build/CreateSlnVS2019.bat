@echo off

IF EXIST "CreateSln_Log.txt" (
	CALL ClearSln.bat
)

echo Generating Visual Studio Solution...
START /B /WAIT cmake --fresh -G "Visual Studio 16 2019" .. > "CreateSln_Log.txt" 2>&1

>nul findstr /c:"CMake Error" CreateSln_Log.txt && (
	echo CMake couldn't generate the solution! Opening the log file...
	CreateSln_Log.txt
	exit
) || (
	echo Removing Exception Handling from all generated projects...
	CALL RemoveExceptions.bat
	echo Replacing nothrownew.obj.lib with nothrownew.obj in BugTrap linkage settings...
	CALL SetupBugTrap.bat
	echo Done!
	TIMEOUT /T 8
	exit
)