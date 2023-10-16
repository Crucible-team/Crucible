<img align="left" src="Content/crucible.svg" width="180px"/>

# Crucible Engine

[![Github Build Status](https://github.com/turanszkij/WickedEngine/workflows/Build/badge.svg)](https://github.com/turanszkij/WickedEngine/actions)
[![Discord chat](https://img.shields.io/discord/1003210044390776923?logo=discord)](https://discord.gg/2zFb4ndaHJ)


<br/>
<img align="right" src="https://github.com/turanszkij/wickedengine-gifs/raw/main/videoprojectors.gif" width="320px"/>
Crucible Engine is an engine focused on game development foundation using the <a href="https://github.com/turanszkij/WickedEngine">Wicked Engine</a> <br/>
This project is hosted on <a href="https://github.com/Crucible-team/Crucible">GitHub</a>.

- [Documentation](Content/Documentation/CrucibleEngine-Documentation.md)<br/>
- [Scripting API Documentation](Content/Documentation/ScriptingAPI-Documentation.md)<br/>
- [Features](features.txt)<br/>

You can get the full source code by using Git version control and cloning https://github.com/Crucible-team/Crucible.git, or downloading it as zip.

<img align="right" src="https://github.com/turanszkij/wickedengine-gifs/raw/main/swimming.gif" width="320px"/>

## :warning: READ THIS BEFORE PROCEEDING :warning:
This `README.md` has been trimmed down to only show the Crucible specific changes made to the Wicked Engine; the full readme can be found [here.](https://github.com/turanszkij/WickedEngine/blob/master/README.md)

## :floppy_disk: Platforms:
- Windows 10 or newer
- Linux
- UWP
- Xbox Series X|S
- PlayStation 5 [in progress]

## :wrench: How to build: 

### :computer: Windows
To build Crucible Engine for Windows (10 or later), run the `cmake-vs2022.bat` this would generate `Crucible.sln` found in `buildx86/` (for x86 operating systems) or `buildx64/` (for x64 operating systems). By simply pressing F5, the Editor application will be built. There are other example projects that you can build as well within the solution.

If you want to develop a C++ application that uses Crucible Engine, you can build the CrucibleEngine static library project, and link against it. Including the `"WickedEngine.h"` header will attempt to link the binaries for the appropriate platform, but search directories should be set up beforehand. For example, you can set additional library directories for examples, see the `Template`, `Tests`, and `Editor` projects. 

### :iphone: Windows UWP
To build for UWP platform, use the latest version of Visual Studio and run the provided `cmake-vs2022-uwp.bat` bat file. The solution file can be found in `build_uwp_x86/` (for x86 operating systems) or `build_uwp_x64/` (for x64 operating systems) The CrucibleEngine Project will build the engine for UWP platform as static library. The Template_UWP and Editor are two applications that will work on UWP platform that you can try. But first you must also build the binary shaders and embed them into the executable. To build and embed shaders, run the `OfflineShaderCompiler` projects with the `hlsl6 shaderdump` command line arguments. Then Rebuild the `Crucible` to create the engine with embedded shaders included. Now you can build an UWP application and run it on PC or Xbox.

 - To run the UWP application on **Xbox**, enable developer mode on your Xbox, and choose "Remote Machine" as a debugging target in Visual Studio. Enter the IP address of the Xbox into the Machine Name field of debugging project settings (make sure that you are modifying the debug settings for Remote Machine). The authentication mode should be set to "Universal (Unencrypted Protocol)" and upon launching the application from Visual Studio, you will need to enter the security PIN that you can view on the Xbox developer settings.<br/>
**Note that to utilize the full performance of Xbox Series, it is required to build with the native Xbox SDK build tools instead of UWP**

### :penguin: Linux
To build the engine for Linux, run `cmake-linux-%configurationtype%.sh` for Make or `cmake-ninja-linux-%configurationtype%.sh` for Ninja the generate project can be found in `buildDb/`. On the Linux operating system, you will need to ensure some additional dependencies are installed, such as Cmake (3.7 or newer), g++ compiler (C++ 17 compliant version) For Ubuntu 20.04, you can use the following commands to install dependencies:

```bash
sudo apt update
sudo apt install libsdl2-dev
sudo apt install build-essential
```

If you want to develop an application that uses Crucible Engine, you will have to link to `libCrucibleEngine.a` and `#include "WickedEngine.h"` into the source code. For examples, look at the Cmake files, or the Tests and the Editor applications.

### :electric_plug: Xbox Series X|S
To build for Xbox Series natively, download and install the Xbox SDK from your Xbox developer account. Using the latest version of Visual Studio, create a new static library project for the Xbox Series platform and reference the WickedEngine_SOURCE shared project. Xbox specific extension files required for building, or sample projects will be provided for registered Xbox developers on request.

### :electric_plug: PlayStation 5
To build for PlayStation 5, download and install the PlayStation 5 SDK from your PlayStation developer account. Using the latest Visual Studio, create a new PlayStation 5 static library project and reference the WickedEngine_SOURCE shared project. PlayStation 5 specific extension files requierd for building, or sample projects will be provided for registered PlayStation developers on request.