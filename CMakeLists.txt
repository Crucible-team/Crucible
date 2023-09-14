cmake_minimum_required(VERSION 3.8)

option(CRUCIBLE_DYNAMIC_LIBRARY "Build Crucible as a dynamic library" OFF)

option(CRUCIBLE_EDITOR "Build Crucible editor (JOE)" ON)
option(CRUCIBLE_TESTS "Build Crucible tests" ON)
option(CRUCIBLE_IMGUI_EXAMPLE "Build Crucible imgui example" ON)
option(CRUCIBLE_LINUX_TEMPLATE "Build Crucible Linux template" ON)
option(BIN_2_HEADER "Bin to Header utility for Crucible assets" OFF)
option(CRUCIBLE_NIGHTLY_VERSIONING "Nightly versioning scheme (timestamp - commit hash)" OFF)

# Configure CMake global variables
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Use solution folders to organize projects
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(Crucible)

if (WIN32)
    set(PLATFORM "Windows")
    add_compile_definitions(WIN32=1)
    # add_compile_definitions(_WIN32=1) this is a given from the compiler
    set(DXC_TARGET "${CMAKE_CURRENT_SOURCE_DIR}/Crucible/dxc.exe")
elseif(UNIX)
    set(PLATFORM "SDL2")
    add_compile_definitions(SDL2=1)
    set(DXC_TARGET "dxc")
endif()

add_subdirectory(Crucible)

if (CRUCIBLE_EDITOR)
    add_subdirectory(Editor)
endif()

if (CRUCIBLE_TESTS)
    add_subdirectory(Tests)
endif()

if (CRUCIBLE_IMGUI_EXAMPLE)
    add_subdirectory(Example_ImGui)
    add_subdirectory(Example_ImGui_Docking)
endif()

if (CRUCIBLE_LINUX_TEMPLATE)
    add_subdirectory(Template_Linux)
endif()

if (BIN_2_HEADER)
    add_subdirectory(Bin2H)
endif()
