@echo off

echo Deleting temporary files...

for /f %%F in ('dir /b /a-d ^| findstr /ile ".temp"') do del "%%F"
