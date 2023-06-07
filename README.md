## X-Ray Engine
X-Ray Engine is a game engine that was used by studio GSC to develop games from S.T.A.L.K.E.R. franchise.

X-Ray v2.0 was used to develop S.T.A.L.K.E.R. 2 until it got cancelled in 2011. This is a restoration project that aims to finish the engine and use it to re-create S.T.A.L.K.E.R. 2 as we know it from all leaked/released materials.

## Requirements
### All platforms
This project supports [**CMake v3.26 or higher**](https://cmake.org/) to build its project files on all platforms (currently only on Windows - time will tell if we can get support on others as well).

Python scripts are part of post-CMake process setup. Ensure that you have [**Python 3.11**](https://www.python.org/getit/) or higher installed.

### Windows
You should use **Visual Studio 2019** or **Visual Studio 2022** with **MSVC v142** build tools and **C++ MFC for latest v142 build tools (x86 & x64), .NET 3.5 and .NET 2.0 targeting packs** to build the engine (they can be added to your Visual Studio installation using Get Tools and Features panel). It's also possible to use **Visual Studio 2022** with **v143** version of the toolset.
If you don't have it you can download **Visual Studio 2022** [**here**](https://visualstudio.microsoft.com/). To acquire older versions of Visual Studio please refer to this [**this link**](https://visualstudio.microsoft.com/vs/older-downloads/).

Additionally, you need .NET Framework v3.5 and v2.0 to run the SDK. Their installers can be found [**here**](https://dotnet.microsoft.com/en-us/download/dotnet-framework).


## Dependencies
### Windows
Source files of third party libraries aren't part of this repository. Consult with team members responsible for engine development in order to get necessary dependencies.


## Build
### Windows
- Clone repository via SSH using Git Bash ( `git clone git@git.dezowave.com:dezowave/stk/xray.git` ) or use dedicated Git GUI client i.e. SourceTree or any other that supports Personal Access Token authorization to clone repository via HTTPS.
- (Optional - this repository doesn't have any submodules at the moment) Update submodules using Git ( `git submodule update --init --recursive` )
- Add all necessary third party libraries including their CMake scripts.
- Use **CreateSlnVS2019.bat**\* located in **build** directory to generate solution files.
- Open **X-Ray-v2.0.sln** using **Visual Studio 2019** or **Visual Studio 2022** (make sure to use **MSVC v142** build tools).
- Now you can Debug/Run from Visual Studio (ensure that game assets are in the right directory).

\*You can use **CreateSlnVS2022** if you wish to use **MSVC v143** build tools.

## Development
### Utility Tools
- Build directory contains several utility tools that you can use in case new project has to be integrated into solution.
