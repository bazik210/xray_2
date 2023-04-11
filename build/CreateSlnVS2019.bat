@echo off

IF EXIST "CreateSln_Log.txt" (
	CALL ClearSln.bat
)

echo Generating Visual Studio Solution...
START /B /WAIT cmake --graphviz=dependency_graph.dot --fresh -G "Visual Studio 16 2019" .. > "CreateSln_Log.txt" 2>&1

>nul findstr /c:"CMake Error" CreateSln_Log.txt && (
	echo CMake couldn't generate the solution! Opening the log file...
	CreateSln_Log.txt
	exit
) || (
	echo Removing Exception Handling from all generated projects...
	START /B /WAIT RemoveExceptions.bat
	echo Done!
)