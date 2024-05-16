@echo off

echo Switching Release to Master Gold and Static configuration...

set currentDir=%~dp0

:: MASTER_GOLD
START /B /WAIT python %currentDir%\ReplaceToMasterGold.py
START /B /WAIT python %currentDir%\ReplaceNDebugDefinitionStatic.py
START /B /WAIT python %currentDir%\UpdateProjectsToStatic.py
START /B /WAIT python %currentDir%\ReplaceMD_DefinitionMT.py
START /B /WAIT python %currentDir%\DisableOptimizations.py