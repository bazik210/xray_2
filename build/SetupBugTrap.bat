@echo off

echo Replacing nothrownew.obj.lib with nothrownew.obj in BugTrap linkage settings...

set currentDir=%~dp0

START /B /WAIT python %currentDir%\SetupBugTrapNothrownew.py