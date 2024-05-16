@echo off

echo Replacing NDEBUG preprocessor definition to NDEBUG;XRAY_STATIC_LIBRARIES; for Release configuration...

set currentDir=%~dp0

START /B /WAIT python %currentDir%\ReplaceNDebugDefinitionStatic.py