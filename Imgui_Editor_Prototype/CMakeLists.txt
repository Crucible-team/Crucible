if (NOT WIN32)
	find_package(Threads REQUIRED)
endif ()

set (SOURCE_FILES
	stdafx.cpp
	Example_ImGui_Docking.cpp
	Example_ImGui_Docking.h
	stdafx.h
	ImGui/imconfig.h
	ImGui/imgui.cpp
	ImGui/imgui.h
	ImGui/imgui_demo.cpp
	ImGui/imgui_draw.cpp
	ImGui/imgui_internal.h
	ImGui/imgui_tables.cpp
	ImGui/imgui_widgets.cpp
	ImGui/imstb_rectpack.h
	ImGui/imstb_textedit.h
	ImGui/imstb_truetype.h
	ImGui/imgui_impl_sdl.cpp
	ImGui/imgui_impl_sdl.h
	ImGui/ImGuizmo.cpp
	ImGui/ImGuizmo.h
	ImGui/IconsMaterialDesign.h
	../Editor/ModelImporter_GLTF.cpp
)

if (WIN32)
	list (APPEND SOURCE_FILES
		main_Windows.cpp
		main_Windows.h
		Tests.rc
	)

	add_executable(Editor_ImGui_Prototype WIN32 ${SOURCE_FILES})

	target_link_libraries(Editor_ImGui_Prototype PUBLIC
		Crucible_Windows
	)
	set(LIB_DXCOMPILER "dxcompiler.dll")
else()
	list (APPEND SOURCE_FILES
		main_SDL2.cpp
	)

	add_executable(Editor_ImGui_Prototype ${SOURCE_FILES})

	target_link_libraries(Editor_ImGui_Prototype PUBLIC
		Crucible
		Threads::Threads
	)

	# Copy shaders to build and source folders just to be safe:
	add_custom_command(
		TARGET Editor_ImGui_Prototype POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/ImGuiPS.hlsl ${CMAKE_CURRENT_BINARY_DIR}
		COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/ImGuiVS.hlsl ${CMAKE_CURRENT_BINARY_DIR}
	)

	set(LIB_DXCOMPILER "libdxcompiler.so")
endif ()

if (MSVC)
	set_property(TARGET Editor_ImGui_Prototype PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
endif ()


# Copy content to build folder:
add_custom_command(
	TARGET Editor_ImGui_Prototype POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/Crucible/${LIB_DXCOMPILER} ${CMAKE_CURRENT_BINARY_DIR}
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Content ${CMAKE_CURRENT_BINARY_DIR}/Content
)
