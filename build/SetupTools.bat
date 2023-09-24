@echo off

echo Not all tools could be compiled without boost exceptions...

set currentDir=%~dp0

START /B /WAIT python %currentDir%\SetupTools.py