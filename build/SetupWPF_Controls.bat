@echo off

echo Switching to properly converted wpf_controls with grouped cs and xaml...

set currentDir=%~dp0

START /B /WAIT python %currentDir%\SetupWPF_Controls.py