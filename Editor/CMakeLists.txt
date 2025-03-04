
set (SOURCE_FILES
	main_${PLATFORM}.cpp
	#$<$<STREQUAL:${PLATFORM},Windows>:App_${PLATFORM}.cpp>
	AnimationWindow.cpp
	CameraWindow.cpp
	CameraComponentWindow.cpp
	DecalWindow.cpp
	Editor.cpp
	EmitterWindow.cpp
	EnvProbeWindow.cpp
	ForceFieldWindow.cpp
	HairParticleWindow.cpp
	IKWindow.cpp
	LayerWindow.cpp
	LightWindow.cpp
	MaterialWindow.cpp
	MaterialPickerWindow.cpp
	MeshWindow.cpp
	ModelImporter_GLTF.cpp
	ModelImporter_OBJ.cpp
	NameWindow.cpp
	ObjectWindow.cpp
	PaintToolWindow.cpp
	GraphicsWindow.cpp
	SoundWindow.cpp
	VideoWindow.cpp
	SpringWindow.cpp
	ScriptWindow.cpp
	stdafx.cpp
	TransformWindow.cpp
	Translator.cpp
	WeatherWindow.cpp
	RigidBodyWindow.cpp
	SoftBodyWindow.cpp
	ColliderWindow.cpp
	HierarchyWindow.cpp
	ExpressionWindow.cpp
	ArmatureWindow.cpp
	OptionsWindow.cpp
	ComponentsWindow.cpp
	TerrainWindow.cpp
	HumanoidWindow.cpp
	GeneralWindow.cpp
	ProfilerWindow.cpp
	SpriteWindow.cpp
	FontWindow.cpp
	xatlas.cpp
	EmbeddedResources.cpp
	ResponseWindow.cpp
	IOWindow.cpp
	#ModelImporter_VMF.cpp
	HealthWindow.cpp
	ArmorWindow.cpp
	SplineWindow.cpp
)

add_subdirectory(meshoptimizer)

if (WIN32)
	list (APPEND SOURCE_FILES
		Editor.rc
	)

	add_executable(JOE WIN32 ${SOURCE_FILES})

	target_link_libraries(JOE PUBLIC
		Crucible_Windows
		meshoptimizer
	)

	set_property(TARGET JOE PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
	set(LIB_DXCOMPILER "dxcompiler.dll")
else ()
	add_executable(JOE ${SOURCE_FILES})

	target_link_libraries(JOE PUBLIC
		Crucible
		meshoptimizer
	)
	set(LIB_DXCOMPILER "libdxcompiler.so")

endif ()

# Copy content to build folder:
add_custom_command(
	TARGET JOE POST_BUILD
	# Copy shader compiler library in the source folder
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/Crucible/${LIB_DXCOMPILER} ${CMAKE_CURRENT_BINARY_DIR}
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Crucible/shaders ${CMAKE_CURRENT_BINARY_DIR}/Data/shadersrc
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/terrain ${CMAKE_CURRENT_BINARY_DIR}/Data/terrain
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/languages ${CMAKE_CURRENT_BINARY_DIR}/Data/languages
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/fonts ${CMAKE_CURRENT_BINARY_DIR}/Data/fonts
)

include(GNUInstallDirs)
set(EDITOR_INSTALL_FOLDER "${CMAKE_INSTALL_LIBDIR}/Crucible/Editor")

# wicked-engine.sh (cd install folder then execs editor)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/Linux/Installer/Distribution/wicked-engine.template.sh"
		"${CMAKE_CURRENT_BINARY_DIR}/wicked-engine.sh")
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/wicked-engine.sh"
		DESTINATION ${CMAKE_INSTALL_BINDIR}
		PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ WORLD_EXECUTE WORLD_READ)

# JOE executable
install(TARGETS JOE RUNTIME DESTINATION ${EDITOR_INSTALL_FOLDER})

# install editor assets
# mandatory terrain folder
install(DIRECTORY
		"${CMAKE_CURRENT_SOURCE_DIR}/Data"
		"${CMAKE_CURRENT_SOURCE_DIR}/terrain"
		"${CMAKE_CURRENT_SOURCE_DIR}/languages"
		"${CMAKE_CURRENT_SOURCE_DIR}/fonts"
		DESTINATION ${EDITOR_INSTALL_FOLDER})
# optional .ini .ico .lua
install(FILES
		${CMAKE_CURRENT_SOURCE_DIR}/config.ini
		${CMAKE_CURRENT_SOURCE_DIR}/Editor.ico
		${CMAKE_CURRENT_SOURCE_DIR}/startup.lua
		DESTINATION ${EDITOR_INSTALL_FOLDER})

# Installing icon and application "shortcut" on Linux
if (UNIX)
	install(FILES ${CMAKE_SOURCE_DIR}/Content/logo_small.png
			RENAME wicked.png
			DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/icons/")

	configure_file("${CMAKE_CURRENT_SOURCE_DIR}/Linux/Installer/Distribution/JOE.template.desktop"
			"${CMAKE_CURRENT_BINARY_DIR}/JOE.desktop")

	install(FILES "${CMAKE_CURRENT_BINARY_DIR}/JOE.desktop"
			DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/applications/")
endif()
