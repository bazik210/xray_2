@echo off

echo Copying editor project files to sources to avoid specific bug and replace it in sln...

set currentDir=%~dp0

START /B /WAIT python %currentDir%\SetupEditor.py