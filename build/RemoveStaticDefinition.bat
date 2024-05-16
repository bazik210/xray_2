@echo off

echo Removing XRAY_STATIC_LIBRARIES preprocessor definition from Release configuration...

set currentDir=%~dp0

START /B /WAIT python %currentDir%\RemoveStaticDefinition.py