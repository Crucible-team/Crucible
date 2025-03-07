cmake_minimum_required(VERSION 3.8)
project(Game_Executable)

# if the engine is installed system-wise,
# you can set INSTALLED_ENGINE to ON
# moving your project folder wherever you want
set(INSTALLED_ENGINE OFF)
# you may also want to remove the copy mechanism of the startup.lua file

if (${INSTALLED_ENGINE})
    find_package(Crucible REQUIRED)
endif()

set (SOURCE_FILES
    main.cpp
    stdafx.h
	EmbeddedResources.cpp
)

set(LIB_DXCOMPILER "libdxcompiler.so")

add_executable(Crucible_Welder ${SOURCE_FILES})

target_link_libraries(Crucible_Welder PUBLIC
    # These are cmake generator expressions. They seem more dauting than they are
    # Basically if the variable is ON it will give Crucible,
    # if it's off, it will give Crucible::Crucible
    $<$<NOT:$<BOOL:${INSTALLED_ENGINE}>>:Crucible>
    $<$<BOOL:${INSTALLED_ENGINE}>:Crucible::Crucible>
)

if (${INSTALLED_ENGINE})
    get_property(LIBDXCOMPILER_PATH
        TARGET Crucible::dxcompiler
        PROPERTY IMPORTED_LOCATION)
    get_filename_component(CRUCIBLE_LIBFOLDER ${LIBDXCOMPILER_PATH} DIRECTORY)
    set(STARTUP_LUA "${CRUCIBLE_LIBFOLDER}/Editor/startup.lua")
else()
    set(LIBDXCOMPILER_PATH "${CMAKE_SOURCE_DIR}/Crucible/${LIB_DXCOMPILER}")
    set(STARTUP_LUA "${CMAKE_SOURCE_DIR}/Editor/startup.lua")
endif()
message("libdxcompiler found at ${LIBDXCOMPILER_PATH}")
message("startup lua found at ${STARTUP_LUA}")

add_custom_command(
    TARGET Crucible_Welder POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${LIBDXCOMPILER_PATH} ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${STARTUP_LUA} ${CMAKE_CURRENT_BINARY_DIR}/startup.lua
)
