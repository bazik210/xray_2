@echo off

echo Replacing MD preprocessor definition to MT for Release configuration...

set currentDir=%~dp0

START /B /WAIT python %currentDir%\ReplaceMD_DefinitionMT.py