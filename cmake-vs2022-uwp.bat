@ECHO OFF
@ECHO --------------------------------------------------------------------------------
@ECHO 		CRUCIBLE ENGINE - UNIVERSAL WINDOWS PLATFORM
@ECHO			   VS 2022 PROJECT GENERATION FOR x86 - x64
@ECHO --------------------------------------------------------------------------------
pause
mkdir buildlog > NUL 2>&1
cd buildlog
mkdir x64 > NUL 2>&1
cd..
del /s /q build_uwp_x64 > NUL 2>&1
mkdir build_uwp_x64 > NUL 2>&1
cd build_uwp_x64
@ECHO Generating x64 files
cmake -G "Visual Studio 17" -A "x64" -DCRUCIBLE_WINDOWS_TEMPLATE=OFF -DCMAKE_SYSTEM_NAME=WindowsStore -DCMAKE_SYSTEM_VERSION=10.0.22621.0 ../ > ../buildlog/x64/log_uwp.txt
if ERRORLEVEL == 1 goto ERRORX64
:x86
cd ..
mkdir buildlog > NUL 2>&1
cd buildlog
mkdir x86 > NUL 2>&1
cd ..
del /s /q build_uwp_x86 > NUL 2>&1
mkdir build_uwp_x86 > NUL 2>&1
cd build_uwp_x86
@ECHO Generating x86 files
cmake -G "Visual Studio 17" -A "Win32" -DCRUCIBLE_WINDOWS_TEMPLATE=OFF -DCMAKE_SYSTEM_NAME=WindowsStore -DCMAKE_SYSTEM_VERSION=10.0.22621.0 ../ > ../buildlog/x86/log_uwp.txt
if ERRORLEVEL == 1 goto ERRORX86
pause
exit

:ERRORX64
@ECHO ERROR Generating x64 files
cd ..
rmdir /Q /S build_uwp_x64
goto x86
:ERRORX86
@ECHO ERROR Generating x86 files
cd ..
rmdir /Q /S build_uwp_x86
pause
exit