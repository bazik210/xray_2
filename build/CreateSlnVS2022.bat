@echo off

IF EXIST "CreateSln_Log.txt" (
	CALL ClearSln.bat
	IF ERRORLEVEL 100 (
		TIMEOUT /T 8
		exit
	)
)

echo Generating Visual Studio Solution...
START /B /WAIT cmake --graphviz=dependency_graph.dot --fresh -G "Visual Studio 17 2022" .. > "CreateSln_Log.txt" 2>&1

>nul findstr /c:"CMake Error" CreateSln_Log.txt && (
	echo CMake couldn't generate the solution! Opening the log file...
	CreateSln_Log.txt
	exit
) || (
	echo CMake finished generation successfully! Running post-generation scripts...
	CALL GenerateDependencyGraph.bat
	CALL RemoveExceptions.bat
	CALL SetupBugTrap.bat
	echo Cleaning up...
	CALL RemoveTempFiles.bat
	echo Done!
	TIMEOUT /T 8
	exit
)
