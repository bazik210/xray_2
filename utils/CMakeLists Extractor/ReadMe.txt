Info:
PowerShell script searches current directory + all subdirectories recursively and copies all CMakeLists.txt files to ../_ExtractedCMakeLists directory.

How to use:
1. Copy ExtractCMakeLists.ps1 into source directory (example: F:\xr2-prog)
2. Run ExtractCMakeLists.ps1 using PowerShell*

*You might have to bypass script execution policy.
If it's needed open PowerShell in admin mode, traverse to target directory and run the following command: PowerShell.exe -ExecutionPolicy Bypass -File .\ExtractCMakeLists.ps1

Outputs info:
1. All CMakeLists.txt files found in source directory copied preserving the original folder structure.