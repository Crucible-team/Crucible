#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "WickedEngine::WickedEngine_Linux" for configuration "Release"
set_property(TARGET WickedEngine::WickedEngine_Linux APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(WickedEngine::WickedEngine_Linux PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/WickedEngine/libWickedEngine_Linux.a"
  )

list(APPEND _cmake_import_check_targets WickedEngine::WickedEngine_Linux )
list(APPEND _cmake_import_check_files_for_WickedEngine::WickedEngine_Linux "${_IMPORT_PREFIX}/lib/WickedEngine/libWickedEngine_Linux.a" )

# Import target "WickedEngine::Bullet" for configuration "Release"
set_property(TARGET WickedEngine::Bullet APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(WickedEngine::Bullet PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/WickedEngine/libBullet.a"
  )

list(APPEND _cmake_import_check_targets WickedEngine::Bullet )
list(APPEND _cmake_import_check_files_for_WickedEngine::Bullet "${_IMPORT_PREFIX}/lib/WickedEngine/libBullet.a" )

# Import target "WickedEngine::LUA" for configuration "Release"
set_property(TARGET WickedEngine::LUA APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(WickedEngine::LUA PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/WickedEngine/libLUA.a"
  )

list(APPEND _cmake_import_check_targets WickedEngine::LUA )
list(APPEND _cmake_import_check_files_for_WickedEngine::LUA "${_IMPORT_PREFIX}/lib/WickedEngine/libLUA.a" )

# Import target "WickedEngine::Utility" for configuration "Release"
set_property(TARGET WickedEngine::Utility APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(WickedEngine::Utility PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C;CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/WickedEngine/libUtility.a"
  )

list(APPEND _cmake_import_check_targets WickedEngine::Utility )
list(APPEND _cmake_import_check_files_for_WickedEngine::Utility "${_IMPORT_PREFIX}/lib/WickedEngine/libUtility.a" )

# Import target "WickedEngine::FAudio" for configuration "Release"
set_property(TARGET WickedEngine::FAudio APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(WickedEngine::FAudio PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/WickedEngine/libFAudio.a"
  )

list(APPEND _cmake_import_check_targets WickedEngine::FAudio )
list(APPEND _cmake_import_check_files_for_WickedEngine::FAudio "${_IMPORT_PREFIX}/lib/WickedEngine/libFAudio.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
