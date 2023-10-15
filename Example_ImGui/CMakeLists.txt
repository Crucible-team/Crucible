if (NOT WIN32)
	find_package(Threads REQUIRED)
endif ()

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
	Example_ImGui.cpp
	Example_ImGui.h
	stdafx.h
)

set(EXAMPLE_IMGUI_LIBRARIES
	Crucible
)

if (UNIX)
	list(APPEND EXAMPLE_IMGUI_LIBRARIES SDL2::SDL2 Threads::Threads)
endif()

if (WIN32)
	add_executable(Example_ImGui WIN32 ${SOURCE_FILES})
else()
	add_executable(Example_ImGui ${SOURCE_FILES})
endif ()

target_link_libraries(Example_ImGui PUBLIC
	Crucible
	${EXAMPLE_IMGUI_LIBRARIES}
	${LUA_LIBRARIES}
	${IMGUI_LIBRARY}
)

if (WIN32)
	set(LIB_DXCOMPILER "dxcompiler.dll")
else()
	set(LIB_DXCOMPILER "libdxcompiler.so")
endif ()

if (MSVC)
	set_target_properties(Example_ImGui PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRT.lib")
	set_property(TARGET Example_ImGui PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
endif ()

# Copy shaders to build and source folders just to be safe:
add_custom_command(
	TARGET Example_ImGui POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/ImGuiPS.hlsl ${CMAKE_CURRENT_BINARY_DIR}
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/ImGuiVS.hlsl ${CMAKE_CURRENT_BINARY_DIR}
)

# Copy content to build folder:
add_custom_command(
	TARGET Example_ImGui POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/Crucible/${LIB_DXCOMPILER} ${CMAKE_CURRENT_BINARY_DIR}
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Content ${CMAKE_CURRENT_BINARY_DIR}/../Content
)
