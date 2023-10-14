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
	Example_ImGui_Docking.cpp
	Example_ImGui_Docking.h
	stdafx.h
	IconsMaterialDesign.h
	../Editor/ModelImporter_GLTF.cpp
)

if (WIN32)
	add_executable(Editor_ImGui_Prototype WIN32 ${SOURCE_FILES})
else()
	add_executable(Editor_ImGui_Prototype ${SOURCE_FILES})
endif ()

target_link_libraries(Editor_ImGui_Prototype PUBLIC
	Crucible
	$<$<PLATFORM_ID:Linux>:
		SDL2::SDL2
		Threads::Threads
	>
	${LUA_LIBRARIES}
	${IMGUI_LIBRARY}
	${IMGUIZMO_LIBRARY}
)

if (WIN32)
	set(LIB_DXCOMPILER "dxcompiler.dll")
else()
	set(LIB_DXCOMPILER "libdxcompiler.so")
endif ()

if (MSVC)
	set_target_properties(Editor_ImGui_Prototype PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRT.lib")
	set_property(TARGET Editor_ImGui_Prototype PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
endif ()

# Copy shaders to build and source folders just to be safe:
add_custom_command(
	TARGET Editor_ImGui_Prototype POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/ImGuiPS.hlsl ${CMAKE_CURRENT_BINARY_DIR}
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/ImGuiVS.hlsl ${CMAKE_CURRENT_BINARY_DIR}
)

# Copy content to build folder:
add_custom_command(
	TARGET Editor_ImGui_Prototype POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/Crucible/${LIB_DXCOMPILER} ${CMAKE_CURRENT_BINARY_DIR}
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Content ${CMAKE_CURRENT_BINARY_DIR}/Content
)
