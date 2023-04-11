@echo off

echo Deleting previous CMake project files...
for /f %%F in ('dir /b /a-d ^| findstr /vile ".bat .py"') do del "%%F"
for /f %%D in ('dir /b /ad') do rmdir /s /q "%%D"
