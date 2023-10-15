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

set(EXAMPLE_DOCK_IMGUI_LIBRARIES
	Crucible
)

if (UNIX)
	list(APPEND EXAMPLE_DOCK_IMGUI_LIBRARIES SDL2::SDL2 Threads::Threads)
endif()

if (WIN32)
	add_executable(Example_ImGui_Docking WIN32 ${SOURCE_FILES})
else()
	add_executable(Example_ImGui_Docking ${SOURCE_FILES})
endif()

target_link_libraries(Example_ImGui_Docking PUBLIC
	Crucible
	${EXAMPLE_DOCK_IMGUI_LIBRARIES}
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
	set_target_properties(Example_ImGui_Docking PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRT.lib")
	set_property(TARGET Example_ImGui_Docking PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
endif ()

# Copy shaders to build and source folders just to be safe:
add_custom_command(
	TARGET Example_ImGui_Docking POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/ImGuiPS.hlsl ${CMAKE_CURRENT_BINARY_DIR}
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/ImGuiVS.hlsl ${CMAKE_CURRENT_BINARY_DIR}
)

# Copy content to build folder:
add_custom_command(
	TARGET Example_ImGui_Docking POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/Crucible/${LIB_DXCOMPILER} ${CMAKE_CURRENT_BINARY_DIR}
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Content ${CMAKE_CURRENT_BINARY_DIR}/Content
)
