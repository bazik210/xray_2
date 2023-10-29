Info:
Scripts take .vcproj files as input and process them.

VCProjParser script parses entire .vcproj and looks for files associated with it.

CMakeSources script scans parsed projects and looks for files that can be compiled in following configurations:
- Debug|Win32
- Debug|x64
- Release|Win32
- Release|Win64
All files that meet the criteria are transformed into source file paths that can be copy pasted into CMakeLists files.

How to use:
1. Place .vcproj files in VCProj Filename Extractor directory
2. Run VCProjParser.py using cmd (Python required)
3. Run CMakeSources.py using cmd (Python required)

Outputs info:
1. Parsed .vcproj files are outputted into 'parsed' directory. They contain list of filters and files, excluded builds per file as well as tools (compilers) per file.
2. Results of previous step are then transformed into source file paths for CMake and put in 'commands' directory.
