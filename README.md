## X-Ray Engine
X-Ray Engine is a game engine that was used by studio GSC to develop games from S.T.A.L.K.E.R. franchise.

X-Ray v2.0 was used to develop S.T.A.L.K.E.R. 2 until it got cancelled in 2011. This is a restoration project that aims to finish the engine and use it to re-create S.T.A.L.K.E.R. 2 as we know it from all leaked/released materials.

## Requirements
### All platforms
This project supports [**CMake v3.26 or higher**](https://cmake.org/) to build its project files on all platforms (currently only on Windows - time will tell if we can get support on others as well).

Python scripts are part of post-CMake process setup. Ensure that you have [**Python 3.11**](https://www.python.org/getit/) or higher installed.

Graphviz is used to generate project dependency graph after CMake script finishes execution. Its installers are available [**here**](https://graphviz.org/download/).

**Make sure that CMake, Python and Graphviz are added to your system variables.**

### Windows
You should use **Visual Studio 2019** that has the following components (they can be added to your Visual Studio during installation or using Get Tools and Features panel):
- Workloads:
    - Desktop development with C++
    - Game development with C++
    - .NET desktop development
- Individual components:
    - C++ MFC for latest v142 build tools (x86 & x64)
    - .NET Framework 3.5 development tools
    - .NET Framework 4.8 SDK
    - .NET Framework 4.8 Targeting Pack
    - .NET Core 2.1 Runtime
    - .NET Core 3.1 Runtime

It's also possible to use **Visual Studio 2022** with **v143** version of the toolset.

If you don't have it you can download **Visual Studio 2022** [**here**](https://visualstudio.microsoft.com/). To acquire older versions of Visual Studio please refer to this [**this link**](https://visualstudio.microsoft.com/vs/older-downloads/).

Additionally, you need .NET Framework v3.5 and v2.0 to run the SDK. Their installers can be found [**here**](https://dotnet.microsoft.com/en-us/download/dotnet-framework).

The last necessary component is Microsoft DirectX SDK (June 2010) that is available [**here**](https://www.microsoft.com/en-us/download/details.aspx?id=6812). Ideally, it should be installed in default directory suggested by the installer - default path is used by the solution creation script.


## Dependencies
### Windows
Source files of third party libraries aren't part of this repository. Consult with team members responsible for engine development in order to get necessary dependencies.


## Build
### Windows
- Clone repository via SSH using Git Bash ( `git clone git@git.dezowave.com:30001/dezowave/stk/xray.git` ) or use dedicated Git GUI client i.e. SourceTree or any other that supports Personal Access Token authorization to clone repository via HTTPS.
- (Optional - this repository doesn't have any submodules at the moment) Update submodules using Git ( `git submodule update --init --recursive` )
- Add all necessary third party libraries including their CMake scripts.
- Use **CreateSlnVS2019.bat**\* located in **build** directory to generate solution files.
- Open **X-Ray-v2.0.sln** using **Visual Studio 2019** or **Visual Studio 2022** (make sure to use **MSVC v142** build tools).
- Now you can Debug/Run from Visual Studio (ensure that game assets are in the right directory).

\*You can use **CreateSlnVS2022.bat** if you wish to use **v143** build tools.

## Development
### Utility Tools
- Build directory contains several utility tools that you can use in case new project has to be integrated into solution.
