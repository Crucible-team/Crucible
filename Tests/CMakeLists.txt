set (SOURCE_FILES
	$<$<PLATFORM_ID:Windows>:
		main_Windows.cpp
		main_Windows.h
		Tests.rc
	>
	$<$<PLATFORM_ID:Linux>:
		main_SDL2.cpp
	>
	stdafx.cpp
	Tests.cpp
	Tests.h
	stdafx.h
)

if (WIN32)
	add_executable(Tests WIN32 ${SOURCE_FILES})
else()
	add_executable(Tests ${SOURCE_FILES})
endif ()

target_link_libraries(Tests PUBLIC
	$<IF:$<PLATFORM_ID:Windows>,Crucible_Windows,Crucible> 
	${LUA_LIBRARIES}
)

if (WIN32)
	set(LIB_DXCOMPILER "dxcompiler.dll")
else()
	set(LIB_DXCOMPILER "libdxcompiler.so")
endif ()

if (MSVC)
	set_target_properties(Tests PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRT.lib")
	set_property(TARGET Tests PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
endif ()

# Copy content to build folder:
add_custom_command(
	TARGET Tests POST_BUILD
	# Copy the shader library next to the executable
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/Crucible/${LIB_DXCOMPILER} ${CMAKE_CURRENT_BINARY_DIR}
	# Copy the resources
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/images ${CMAKE_CURRENT_BINARY_DIR}/images
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/test_script.lua ${CMAKE_CURRENT_BINARY_DIR}/test_script.lua
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Content ${CMAKE_CURRENT_BINARY_DIR}/../Content
	# Copy font test resources
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/yumin.ttf ${CMAKE_CURRENT_BINARY_DIR}/
)