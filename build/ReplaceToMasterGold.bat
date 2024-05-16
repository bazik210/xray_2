@echo off

echo Replacing NDEBUG preprocessor definition to NDEBUG;MASTER_GOLD; for Release configuration...

set currentDir=%~dp0

START /B /WAIT python %currentDir%\ReplaceToMasterGold.py