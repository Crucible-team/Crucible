# Install script for directory: /home/deck/Crucible/WickedEngine

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/WickedEngine/offlineshadercompiler" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/WickedEngine/offlineshadercompiler")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/WickedEngine/offlineshadercompiler"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/WickedEngine" TYPE EXECUTABLE FILES "/home/deck/Crucible/BUILD/WickedEngine/offlineshadercompiler")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/WickedEngine/offlineshadercompiler" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/WickedEngine/offlineshadercompiler")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/WickedEngine/offlineshadercompiler")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/WickedEngine" TYPE DIRECTORY FILES "/home/deck/Crucible/Content")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/WickedEngine" TYPE STATIC_LIBRARY FILES "/home/deck/Crucible/BUILD/WickedEngine/libWickedEngine_Linux.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/WickedEngine" TYPE FILE FILES
    "/home/deck/Crucible/WickedEngine/WickedEngine.h"
    "/home/deck/Crucible/WickedEngine/CommonInclude.h"
    "/home/deck/Crucible/WickedEngine/logo.h"
    "/home/deck/Crucible/WickedEngine/sdl2.h"
    "/home/deck/Crucible/WickedEngine/wiApplication.h"
    "/home/deck/Crucible/WickedEngine/wiApplication_BindLua.h"
    "/home/deck/Crucible/WickedEngine/wiArchive.h"
    "/home/deck/Crucible/WickedEngine/wiArguments.h"
    "/home/deck/Crucible/WickedEngine/wiAudio.h"
    "/home/deck/Crucible/WickedEngine/wiAudio_BindLua.h"
    "/home/deck/Crucible/WickedEngine/wiBacklog.h"
    "/home/deck/Crucible/WickedEngine/wiBacklog_BindLua.h"
    "/home/deck/Crucible/WickedEngine/wiCanvas.h"
    "/home/deck/Crucible/WickedEngine/wiColor.h"
    "/home/deck/Crucible/WickedEngine/wiECS.h"
    "/home/deck/Crucible/WickedEngine/wiEmittedParticle.h"
    "/home/deck/Crucible/WickedEngine/wiEnums.h"
    "/home/deck/Crucible/WickedEngine/wiEventHandler.h"
    "/home/deck/Crucible/WickedEngine/wiFadeManager.h"
    "/home/deck/Crucible/WickedEngine/wiFFTGenerator.h"
    "/home/deck/Crucible/WickedEngine/wiFont.h"
    "/home/deck/Crucible/WickedEngine/wiGPUBVH.h"
    "/home/deck/Crucible/WickedEngine/wiGPUSortLib.h"
    "/home/deck/Crucible/WickedEngine/wiGraphics.h"
    "/home/deck/Crucible/WickedEngine/wiGraphicsDevice.h"
    "/home/deck/Crucible/WickedEngine/wiGraphicsDevice_DX12.h"
    "/home/deck/Crucible/WickedEngine/wiGraphicsDevice_Vulkan.h"
    "/home/deck/Crucible/WickedEngine/wiGUI.h"
    "/home/deck/Crucible/WickedEngine/wiHairParticle.h"
    "/home/deck/Crucible/WickedEngine/wiHelper.h"
    "/home/deck/Crucible/WickedEngine/wiImage.h"
    "/home/deck/Crucible/WickedEngine/wiImageParams_BindLua.h"
    "/home/deck/Crucible/WickedEngine/wiInitializer.h"
    "/home/deck/Crucible/WickedEngine/wiInput.h"
    "/home/deck/Crucible/WickedEngine/wiInput_BindLua.h"
    "/home/deck/Crucible/WickedEngine/wiJobSystem.h"
    "/home/deck/Crucible/WickedEngine/wiLoadingScreen.h"
    "/home/deck/Crucible/WickedEngine/wiLoadingScreen_BindLua.h"
    "/home/deck/Crucible/WickedEngine/wiLua.h"
    "/home/deck/Crucible/WickedEngine/wiLua_Globals.h"
    "/home/deck/Crucible/WickedEngine/wiLuna.h"
    "/home/deck/Crucible/WickedEngine/wiMath.h"
    "/home/deck/Crucible/WickedEngine/wiMath_BindLua.h"
    "/home/deck/Crucible/WickedEngine/wiNetwork.h"
    "/home/deck/Crucible/WickedEngine/wiNetwork_BindLua.h"
    "/home/deck/Crucible/WickedEngine/wiNoise.h"
    "/home/deck/Crucible/WickedEngine/wiOcean.h"
    "/home/deck/Crucible/WickedEngine/wiPhysics.h"
    "/home/deck/Crucible/WickedEngine/wiPhysics_BindLua.h"
    "/home/deck/Crucible/WickedEngine/wiPlatform.h"
    "/home/deck/Crucible/WickedEngine/wiPrimitive.h"
    "/home/deck/Crucible/WickedEngine/wiPrimitive_BindLua.h"
    "/home/deck/Crucible/WickedEngine/wiProfiler.h"
    "/home/deck/Crucible/WickedEngine/wiRandom.h"
    "/home/deck/Crucible/WickedEngine/wiRawInput.h"
    "/home/deck/Crucible/WickedEngine/wiRectPacker.h"
    "/home/deck/Crucible/WickedEngine/wiRenderer.h"
    "/home/deck/Crucible/WickedEngine/wiRenderer_BindLua.h"
    "/home/deck/Crucible/WickedEngine/wiRenderPath.h"
    "/home/deck/Crucible/WickedEngine/wiRenderPath2D.h"
    "/home/deck/Crucible/WickedEngine/wiRenderPath2D_BindLua.h"
    "/home/deck/Crucible/WickedEngine/wiRenderPath3D.h"
    "/home/deck/Crucible/WickedEngine/wiRenderPath3D_BindLua.h"
    "/home/deck/Crucible/WickedEngine/wiRenderPath3D_PathTracing.h"
    "/home/deck/Crucible/WickedEngine/wiRenderPath_BindLua.h"
    "/home/deck/Crucible/WickedEngine/wiResourceManager.h"
    "/home/deck/Crucible/WickedEngine/wiScene.h"
    "/home/deck/Crucible/WickedEngine/wiScene_BindLua.h"
    "/home/deck/Crucible/WickedEngine/wiScene_Decl.h"
    "/home/deck/Crucible/WickedEngine/wiScene_Components.h"
    "/home/deck/Crucible/WickedEngine/wiSDLInput.h"
    "/home/deck/Crucible/WickedEngine/wiShaderCompiler.h"
    "/home/deck/Crucible/WickedEngine/wiSheenLUT.h"
    "/home/deck/Crucible/WickedEngine/wiSpinLock.h"
    "/home/deck/Crucible/WickedEngine/wiSprite.h"
    "/home/deck/Crucible/WickedEngine/wiSprite_BindLua.h"
    "/home/deck/Crucible/WickedEngine/wiSpriteAnim_BindLua.h"
    "/home/deck/Crucible/WickedEngine/wiSpriteFont.h"
    "/home/deck/Crucible/WickedEngine/wiSpriteFont_BindLua.h"
    "/home/deck/Crucible/WickedEngine/wiTexture_BindLua.h"
    "/home/deck/Crucible/WickedEngine/wiTextureHelper.h"
    "/home/deck/Crucible/WickedEngine/wiTimer.h"
    "/home/deck/Crucible/WickedEngine/wiUnorderedMap.h"
    "/home/deck/Crucible/WickedEngine/wiUnorderedSet.h"
    "/home/deck/Crucible/WickedEngine/wiVector.h"
    "/home/deck/Crucible/WickedEngine/wiVersion.h"
    "/home/deck/Crucible/WickedEngine/wiXInput.h"
    "/home/deck/Crucible/WickedEngine/wiConfig.h"
    "/home/deck/Crucible/WickedEngine/wiTerrain.h"
    "/home/deck/Crucible/WickedEngine/wiAllocator.h"
    "/home/deck/Crucible/WickedEngine/wiBVH.h"
    "/home/deck/Crucible/WickedEngine/wiLocalization.h"
    "/home/deck/Crucible/WickedEngine/wiVideo.h"
    "/home/deck/Crucible/WickedEngine/wiKeyValue.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/WickedEngine" TYPE STATIC_LIBRARY FILES "/home/deck/Crucible/BUILD/WickedEngine/BULLET/libBullet.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/WickedEngine" TYPE STATIC_LIBRARY FILES "/home/deck/Crucible/BUILD/WickedEngine/LUA/libLUA.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/WickedEngine" TYPE STATIC_LIBRARY FILES "/home/deck/Crucible/BUILD/WickedEngine/Utility/libUtility.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/WickedEngine" TYPE STATIC_LIBRARY FILES "/home/deck/Crucible/BUILD/WickedEngine/Utility/FAudio/libFAudio.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/WickedEngine" TYPE FILE FILES "/home/deck/Crucible/WickedEngine/libdxcompiler.so")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/WickedEngine/WickedEngineTargets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/WickedEngine/WickedEngineTargets.cmake"
         "/home/deck/Crucible/BUILD/WickedEngine/CMakeFiles/Export/6ef02ec1396f135134ad88ac62f9726f/WickedEngineTargets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/WickedEngine/WickedEngineTargets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/WickedEngine/WickedEngineTargets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/WickedEngine" TYPE FILE FILES "/home/deck/Crucible/BUILD/WickedEngine/CMakeFiles/Export/6ef02ec1396f135134ad88ac62f9726f/WickedEngineTargets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/WickedEngine" TYPE FILE FILES "/home/deck/Crucible/BUILD/WickedEngine/CMakeFiles/Export/6ef02ec1396f135134ad88ac62f9726f/WickedEngineTargets-release.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/WickedEngine" TYPE FILE FILES "/home/deck/Crucible/BUILD/cmake/install/WickedEngineConfig.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/WickedEngine" TYPE DIRECTORY FILES "/home/deck/Crucible/WickedEngine/shaders" REGEX "/spirv$" EXCLUDE REGEX "/hlsl6$" EXCLUDE REGEX "/[^/]*\\.vcxitems[^/]*$" EXCLUDE)
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/deck/Crucible/BUILD/WickedEngine/BULLET/cmake_install.cmake")
  include("/home/deck/Crucible/BUILD/WickedEngine/LUA/cmake_install.cmake")
  include("/home/deck/Crucible/BUILD/WickedEngine/Utility/cmake_install.cmake")

endif()

