@echo off

echo Applying maximum optimizations for Release configuration...

set currentDir=%~dp0

START /B /WAIT python %currentDir%\MaxOptimizations.py