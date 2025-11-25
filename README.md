# hellogpu

## Requirements
* [cmake 3.28 or newer](https://cmake.org/)
* One of the following compilers
  * MSVC toolset 14.34 (Visual Studio 17.4 (Visual Studio 2022)) or newer
  * clang 17
* [Ninja](https://ninja-build.org/)

## Building
The project can be build from command line using cmake or by opening the project folder in an IDE that can handle cmake projects (Visual Studio, CLion, etc.). When running cmake build commands from the command line on windows, they must be run from `Developer Command Prompt for VS 20xx` or `Developer PowerShell for VS 20xx`.

Available configuration presets
* x64-win-msvc-debug
* x64-win-msvc-development
* x64-win-msvc-release
* x64-win-clang-debug
* x64-win-clang-development
* x64-win-clang-release

Example build using x64-win-msvc-debug configuration
```
cmake --preset=x64-win-msvc-debug
cmake --build build/x64-win-msvc-debug
```