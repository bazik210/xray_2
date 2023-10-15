echo Running post-generation scripts...
CALL RemoveExceptions.bat
CALL RemoveDebugNDebug.bat
CALL SetupNothrownew.bat
CALL SetupTools.bat
CALL SetupWPF_Controls.bat
echo Done!