@echo off

echo Replacing MT preprocessor definition to MD for Release configuration...

set currentDir=%~dp0

START /B /WAIT python %currentDir%\ReplaceMT_DefinitionMD.py