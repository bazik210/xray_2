@echo off

echo Checking working directory...

SET wdPath=%~dp0
SET buildDir=%wdPath:~-6%

IF NOT %buildDir%==build\ (
	echo Incorrect build directory! Aborting to avoid deletion of important files!
	echo Please don't change the file layout of the repository after cloning!
	exit /B 100
) ELSE (
	echo Deleting previous CMake project files...
	for /f %%F in ('dir /b /a-d ^| findstr /vile ".bat .py CMakeGraphVizOptions.cmake"') do del "%%F"
	for /f %%D in ('dir /b /ad') do rmdir /s /q "%%D"
	exit /b 0
)
