set (SOURCE_FILES
	main.cpp
)

add_executable(Bin2H ${SOURCE_FILES})

target_link_libraries(Bin2H PUBLIC
	$<IF:$<PLATFORM_ID:Windows>,Crucible_Windows,Crucible>
	${LUA_LIBRARIES}
)

if (MSVC)
	set_target_properties(Bin2H PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRT.lib")
	set_property(TARGET Bin2H PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
endif ()