@echo off

echo Removing Exception Handling from all generated projects...

set currentDir=%~dp0

START /B /WAIT python %currentDir%\RemoveExceptionHandling.py