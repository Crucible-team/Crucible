cmake_minimum_required(VERSION 3.8)
project(Template_Windows)

# if the engine is installed system-wise,
# you can set INSTALLED_ENGINE to ON
# moving your project folder wherever you want
set(INSTALLED_ENGINE OFF)
# you may also want to remove the copy mechanism of the startup.lua file

if (${INSTALLED_ENGINE})
    find_package(Crucible REQUIRED)
endif()

file(GLOB HEADERS_FILES *.h)
source_group("Header Files" FILES ${HEADERS_FILES})

file(GLOB SOURCE_FILES *.cpp )
source_group("Source Files" FILES ${SOURCE_FILES})

file(GLOB RESOURCES_FILES *.rc  resource.h  *.ico)
source_group("Resource Files" FILES ${RESOURCES_FILES})

set(LIB_DXCOMPILER "dxcompiler.dll")

add_executable(Template_Windows WIN32 ${SOURCE_FILES} ${HEADERS_FILES} ${RESOURCES_FILES})

target_link_libraries(Template_Windows PUBLIC
    # These are cmake generator expressions. They seem more dauting than they are
    # Basically if the variable is ON it will give Crucible,
    # if it's off, it will give Crucible::Crucible
	${LUA_LIBRARIES}
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
message("dxcompiler found at ${LIBDXCOMPILER_PATH}")
message("startup lua found at ${STARTUP_LUA}")

add_custom_command(
    TARGET Template_Windows POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${LIBDXCOMPILER_PATH} ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${STARTUP_LUA} ${CMAKE_CURRENT_BINARY_DIR}/startup.lua
)
