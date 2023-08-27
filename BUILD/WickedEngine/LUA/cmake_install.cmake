# Install script for directory: /home/deck/Crucible/WickedEngine/LUA

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/WickedEngine/LUA" TYPE FILE FILES
    "/home/deck/Crucible/WickedEngine/LUA/lapi.h"
    "/home/deck/Crucible/WickedEngine/LUA/lauxlib.h"
    "/home/deck/Crucible/WickedEngine/LUA/lcode.h"
    "/home/deck/Crucible/WickedEngine/LUA/lctype.h"
    "/home/deck/Crucible/WickedEngine/LUA/ldebug.h"
    "/home/deck/Crucible/WickedEngine/LUA/ldo.h"
    "/home/deck/Crucible/WickedEngine/LUA/lfunc.h"
    "/home/deck/Crucible/WickedEngine/LUA/lgc.h"
    "/home/deck/Crucible/WickedEngine/LUA/llex.h"
    "/home/deck/Crucible/WickedEngine/LUA/llimits.h"
    "/home/deck/Crucible/WickedEngine/LUA/lmem.h"
    "/home/deck/Crucible/WickedEngine/LUA/lobject.h"
    "/home/deck/Crucible/WickedEngine/LUA/lopcodes.h"
    "/home/deck/Crucible/WickedEngine/LUA/lparser.h"
    "/home/deck/Crucible/WickedEngine/LUA/lprefix.h"
    "/home/deck/Crucible/WickedEngine/LUA/lstate.h"
    "/home/deck/Crucible/WickedEngine/LUA/lstring.h"
    "/home/deck/Crucible/WickedEngine/LUA/ltable.h"
    "/home/deck/Crucible/WickedEngine/LUA/ltm.h"
    "/home/deck/Crucible/WickedEngine/LUA/lua.h"
    "/home/deck/Crucible/WickedEngine/LUA/luaconf.h"
    "/home/deck/Crucible/WickedEngine/LUA/lualib.h"
    "/home/deck/Crucible/WickedEngine/LUA/lundump.h"
    "/home/deck/Crucible/WickedEngine/LUA/lvm.h"
    "/home/deck/Crucible/WickedEngine/LUA/lzio.h"
    )
endif()

