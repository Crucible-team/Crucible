
set (SOURCE_FILES
	main_${PLATFORM}.cpp
	$<$<PLATFORM_ID:Windows>:
		Editor.rc
	>
	$<$<PLATFORM_ID:WindowsStore>:
		Assets/Icon.c
	>
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
	xatlas.cpp
	EmbeddedResources.cpp
	ResponseWindow.cpp
	IOWindow.cpp
	#ModelImporter_VMF.cpp
	HealthWindow.cpp
	ArmorWindow.cpp
	SplineWindow.cpp
)

if (WINDOWS_STORE)
	set (ASSETS_FILES
		Assets/LockScreenLogo.scale-200.png
		Assets/SplashScreen.scale-200.png
		Assets/Square44x44Logo.scale-200.png
		Assets/Square44x44Logo.targetsize-24_altform-unplated.png
		Assets/Square150x150Logo.scale-200.png
		Assets/StoreLogo.png
		Assets/Wide310x150Logo.scale-200.png
	)
endif()

if (WINDOWS_STORE)
	list(APPEND SOURCE_FILES ${ASSETS_FILES})
endif()

if (WIN32)
	add_executable(JOE WIN32 ${SOURCE_FILES})
else()
	add_executable(JOE ${SOURCE_FILES})
endif()

target_link_libraries(JOE PUBLIC
	Crucible
	${MESHOPTIMIZER_LIBRARY}
	${LUA_LIBRARIES}
)

if (WIN32)
	set_property(TARGET JOE PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${EDITOR_INSTALL_FOLDER}")

	if (WINDOWS_STORE)
		set_target_properties(JOE PROPERTIES
			VS_PACKAGE_REFERENCES "Microsoft.Windows.CppWinRT_2.0.230706.1"
		)

		set_target_properties(JOE PROPERTIES
			VS_WINRT_EXTENSIONS TRUE
		)

		set_source_files_properties(${ASSETS_FILES} PROPERTIES
			VS_DEPLOYMENT_CONTENT 1
			VS_DEPLOYMENT_LOCATION "Assets"
		)
	endif ()

	set(LIB_DXCOMPILER "dxcompiler.dll")
else ()
	set(LIB_DXCOMPILER "libdxcompiler.so")
endif ()

# Copy content to build folder:
add_custom_command(
	TARGET JOE POST_BUILD
	# Copy shader compiler library in the source folder
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/Crucible/${LIB_DXCOMPILER} ${CMAKE_CURRENT_BINARY_DIR}
	# Data folders
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Crucible/shaders ${CMAKE_CURRENT_BINARY_DIR}/Data/shadersrc
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/terrain ${CMAKE_CURRENT_BINARY_DIR}/Data/terrain
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/languages ${CMAKE_CURRENT_BINARY_DIR}/Data/languages
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/fonts ${CMAKE_CURRENT_BINARY_DIR}/Data/fonts
)

include(GNUInstallDirs)
set(EDITOR_INSTALL_FOLDER "${CMAKE_INSTALL_LIBDIR}/Crucible/Editor")

# crucible-engine.sh (cd install folder then execs editor)
if (UNIX)
	configure_file("${CMAKE_CURRENT_SOURCE_DIR}/Linux/Installer/Distribution/crucible-engine.template.sh"
			"${CMAKE_CURRENT_BINARY_DIR}/crucible-engine.sh")
	install(FILES "${CMAKE_CURRENT_BINARY_DIR}/crucible-engine.sh"
			DESTINATION ${CMAKE_INSTALL_BINDIR}
			PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ WORLD_EXECUTE WORLD_READ)
endif()

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

# Dependencies
if (WIN32)
install(FILES
		${CMAKE_SOURCE_DIR}/Crucible/${LIB_DXCOMPILER}
		${DEPENDENCIES_DIR}/meshoptimizer.dll
		${DEPENDENCIES_DIR}/lua.dll
		DESTINATION ${EDITOR_INSTALL_FOLDER})
endif()

# optional .ini .ico .lua
install(FILES
		${CMAKE_CURRENT_SOURCE_DIR}/config.ini
		${CMAKE_CURRENT_SOURCE_DIR}/joe.ico
		${CMAKE_CURRENT_SOURCE_DIR}/startup.lua
		DESTINATION ${EDITOR_INSTALL_FOLDER})

# Installing icon and application "shortcut" on Linux
if (UNIX)
	install(FILES ${CMAKE_SOURCE_DIR}/Content/crucible.svg
			DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/icons/")

	configure_file("${CMAKE_CURRENT_SOURCE_DIR}/Linux/Installer/Distribution/JOE.template.desktop"
			"${CMAKE_CURRENT_BINARY_DIR}/JOE.desktop")

	install(FILES "${CMAKE_CURRENT_BINARY_DIR}/JOE.desktop"
			DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/applications/")
endif()
