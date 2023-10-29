@echo off

echo Redacting user's subfolders from dependency graph nodes...
set currentDir=%~dp0

START /B /WAIT python %currentDir%\RedactGraph.py

echo Generating dependency graph in .pdf and .png formats...
START /B /WAIT dot -Tpdf dependency_graph.dot -o dependency_graph.pdf
START /B /WAIT dot -Tpng dependency_graph.dot -o dependency_graph.png
