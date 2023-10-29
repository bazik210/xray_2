Info:
Scripts take .csproj files as input and process them.

VCProjParser script parses entire .csproj and looks for files associated with it.

All files that meet the criteria are transformed into source file paths that can be copy pasted into CMakeLists files.

How to use:
1. Place .csproj files in CSProj Filename Extractor directory
2. Run CSProjParser.py using cmd (Python required)
3. Run CMakeSources.py using cmd (Python required)

Outputs info:
1. Parsed .csproj files are outputted into 'parsed' directory. They contain list of files used by each project.
2. Results of previous step are then transformed into source file paths for CMake* and put in 'commands' directory.

*File paths aren't grouped! It has to be done manually!