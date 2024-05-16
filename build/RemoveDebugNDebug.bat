@echo off

echo Removing DEBUG, _DEBUG and NDEBUG preprocessor definitions from RelWithDebInfo and Release configuration...

set currentDir=%~dp0

START /B /WAIT python %currentDir%\RemoveDebugDefinition.py
START /B /WAIT python %currentDir%\RemoveDebugDefinition2.py