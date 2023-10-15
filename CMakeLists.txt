cmake_minimum_required(VERSION 3.8)

# Check if vcpkg exist in our source folder
if(EXISTS "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
	set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
	SET(VCPKG_OVERLAY_PORTS
		"${CMAKE_SOURCE_DIR}/.vcpkg/overrides/imguizmo/"
	)
	message("Vcpkg found and enable, Happy programming!")
else()
	message( FATAL_ERROR "Vcpkg wasn't not found, aborting, make sure to git clone this with all submodules!")
endif()

option(CRUCIBLE_DYNAMIC_LIBRARY "Build Crucible as a dynamic library" OFF)

option(CRUCIBLE_EDITOR "Build Crucible editor (JOE)" ON)
option(CRUCIBLE_TESTS "Build Crucible tests" ON)
option(CRUCIBLE_IMGUI_EXAMPLE "Build Crucible imgui example" ON)
if (WIN32)
	option(CRUCIBLE_WINDOWS_TEMPLATE "Build Crucible Windows template" ON)
elseif(WINDOWS_STORE)
	option(CRUCIBLE_UWP_TEMPLATE "Build Crucible UWP template" ON)
elseif(UNIX)
	option(CRUCIBLE_LINUX_TEMPLATE "Build Crucible Linux template" ON)
endif()
option(BIN_2_HEADER "Bin to Header utility for Crucible assets" OFF)
option(CRUCIBLE_NIGHTLY_VERSIONING "Nightly versioning scheme (timestamp - commit hash)" OFF)

# Configure CMake global variables
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Use solution folders to organize projects
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(Crucible)

if(UNIX)
	# Alsa
	find_package("ALSA" MODULE REQUIRED)

	# Threads
	find_package(Threads REQUIRED)
endif()

# Intel's Open Image Denoisers (This should be use in windows to)
find_package(OpenImageDenoise "${MIN_OpenImageDenoise_VERSION}" QUIET)
if(NOT ${OpenImageDenoise_FOUND})
	message("OpenImageDenoise>=${MIN_OpenImageDenoise_VERSION} not found, it will be disabled.")
else()
	message("OpenImageDenoise ${OpenImageDenoise_VERSION} Found.")
endif()

# SDL2 (Not used by windows... yet)
find_package(SDL2 REQUIRED)
if (${SDL_FOUND})
	include_directories(${SDL2_INCLUDE_DIR})

	add_compile_definitions(SDL2=1)

	# minimum sdl version is 2.0.14 for controller LED support
	if((${SDL_VERSION_MAJOR} GREATER_EQUAL 2) AND (${SDL2_VERSION_MINOR} GREATER_EQUAL 0) AND (${SDL2_VERSION_PATCH} GREATER_EQUAL 14))
		add_compile_definitions(SDL2_FEATURE_CONTROLLER_LED=1)
	endif()
else()
	# TODO: do something when we didn't find SDL2
endif()

# Imgui
find_path(IMGUI_INCLUDE_DIRS "imgui.h")
find_library(IMGUI_LIBRARY imgui)
include_directories(${IMGUI_INCLUDE_DIRS})

# ImGuizmo
find_path(IMGUIZMO_INCLUDE_DIRS "ImGuizmo.h" PATH_SUFFIXES "imguizmo")
find_library(IMGUIZMO_LIBRARY NAMES imguizmo)
include_directories(${IMGUIZMO_INCLUDE_DIRS})

# Lua
include(FindLua)
find_package(Lua REQUIRED)
include_directories(${LUA_INCLUDE_DIR})

# Meshoptimizer
find_path(MESHOPTIMIZER_INCLUDE_DIRS "meshoptimizer.h" PATH_SUFFIXES "meshoptimizer")
find_library(MESHOPTIMIZER_LIBRARY NAMES meshoptimizer)
include_directories(${MESHOPTIMIZER_INCLUDE_DIRS})

# Recast (Not hooked yet)
find_path(RECAST_INCLUDE_DIRS "Recast.h" PATH_SUFFIXES "recastnavigation")
find_library(RECAST_LIBRARY NAMES Recast)
include_directories(${RECAST_INCLUDE_DIRS})

# Detours: Part of Recast (Not hooked yet)
find_library(DETOUR_LIBRARY NAMES Detour)
find_library(DETOURCROWD_LIBRARY NAMES DetourCrowd)
find_library(DETOURTILECACHE_LIBRARY NAMES DetourTileCache)
set(RECAST_LIBRARIES ${DETOUR_LIBRARY} ${DETOURCROWD_LIBRARY} ${DETOURTILECACHE_LIBRARY} ${RECAST_LIBRARY})

# Json
find_path(JSON_INCLUDE_DIRS "json.hpp" PATH_SUFFIXES "nlohmann")
find_package(nlohmann_json CONFIG REQUIRED)
set(JSON_LIBRARY nlohmann_json::nlohmann_json)
include_directories(${JSON_INCLUDE_DIRS})

if (WIN32)
	if (WINDOWS_STORE)
		set(PLATFORM "UWP")
	else()
		set(PLATFORM "Windows")
	endif()
elseif(UNIX)
    set(PLATFORM "SDL2")
endif()

add_subdirectory(Crucible)

if (CRUCIBLE_EDITOR)
    add_subdirectory(Editor)
	add_subdirectory(Imgui_Editor_Prototype)
endif()

if (CRUCIBLE_TESTS)
    add_subdirectory(Tests)
endif()

if (CRUCIBLE_IMGUI_EXAMPLE)
    add_subdirectory(Example_ImGui)
    add_subdirectory(Example_ImGui_Docking)
endif()

if (WIN32)
	if (CRUCIBLE_WINDOWS_TEMPLATE)
		add_subdirectory(Template_Windows)
	endif()
	if (CRUCIBLE_UWP_TEMPLATE)
		add_subdirectory(Template_UWP)
	endif()
else()
	if (CRUCIBLE_LINUX_TEMPLATE)
		add_subdirectory(Template_Linux)
	endif()
endif()

if (BIN_2_HEADER)
    add_subdirectory(Bin2H)
endif()
