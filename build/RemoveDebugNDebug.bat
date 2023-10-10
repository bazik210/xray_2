@echo off

echo Removing DEBUG, _DEBUG and NDEBUG preprocessor definitions from RelWithDebInfo configuration...

set currentDir=%~dp0

START /B /WAIT python %currentDir%\RemoveDebugDefinition.py