@echo off

echo Removing Master Gold and Static from Release configuration...

set currentDir=%~dp0

:: MASTER_GOLD
START /B /WAIT python %currentDir%\RemoveMasterGoldDefinition.py
START /B /WAIT python %currentDir%\RemoveStaticDefinition.py
START /B /WAIT python %currentDir%\UpdateProjectsToDynamic.py
START /B /WAIT python %currentDir%\ReplaceMT_DefinitionMD.py
START /B /WAIT python %currentDir%\EnableOptimizations.py