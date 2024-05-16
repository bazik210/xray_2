@echo off

echo Removing MASTER_GOLD preprocessor definitions from Release configuration...

set currentDir=%~dp0

START /B /WAIT python %currentDir%\RemoveMasterGoldDefinition.py