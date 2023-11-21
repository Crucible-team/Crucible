

set (SOURCE_FILES
	main.cpp
)

if (WIN32)
	list (APPEND SOURCE_FILES
		main.cpp
	)

	add_executable(Bin2H WIN32 ${SOURCE_FILES})

	target_link_libraries(Bin2H PUBLIC
		Crucible_Windows
	)
else()
	list (APPEND SOURCE_FILES
		main.cpp
	)

	add_executable(Bin2H ${SOURCE_FILES})

	target_link_libraries(Bin2H PUBLIC
		Crucible
	)

endif ()

if (MSVC)
	set_property(TARGET Bin2H PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
endif ()


# Copy content to build folder:
#add_custom_command(
	#TARGET Bin2H POST_BUILD
	# Copy the shader library next to the executable
	#COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/WickedEngine/${LIB_DXCOMPILER} ${CMAKE_CURRENT_BINARY_DIR}
	# Copy the resources
	#COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/images ${CMAKE_CURRENT_BINARY_DIR}/images
	#COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/test_script.lua ${CMAKE_CURRENT_BINARY_DIR}/test_script.lua
	#COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Content ${CMAKE_CURRENT_BINARY_DIR}/../Content
	# Copy font test resources
	#COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/yumin.ttf ${CMAKE_CURRENT_BINARY_DIR}/
#)
