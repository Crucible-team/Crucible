@ECHO OFF
@ECHO --------------------------------------------------------------------------------
@ECHO 				CRUCIBLE ENGINE
@ECHO			   VS 2022 PROJECT GENERATION FOR x86 - x64
@ECHO --------------------------------------------------------------------------------
pause
mkdir buildlog > NUL 2>&1
cd buildlog
mkdir x64 > NUL 2>&1
cd..
del /s /q buildx64 > NUL 2>&1
mkdir buildx64 > NUL 2>&1
cd buildx64
@ECHO Generating x64 files
cmake -G "Visual Studio 17" -A "x64" ../ > ../buildlog/x64/log.txt
if ERRORLEVEL == 1 goto ERRORX64
:x86
cd ..
mkdir buildlog > NUL 2>&1
cd buildlog
mkdir x86 > NUL 2>&1
cd ..
del /s /q buildx86 > NUL 2>&1
mkdir buildx86 > NUL 2>&1
cd buildx86
@ECHO Generating x86 files
cmake -G "Visual Studio 17" -A "Win32" ../ > ../buildlog/x86/log.txt
if ERRORLEVEL == 1 goto ERRORX86
pause
exit

:ERRORX64
@ECHO ERROR Generating x64 files
cd ..
rmdir /Q /S buildx64
goto x86
:ERRORX86
@ECHO ERROR Generating x86 files
cd ..
rmdir /Q /S buildx86
pause
exit