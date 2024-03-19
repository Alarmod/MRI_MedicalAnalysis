#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "flann::flann_cpp" for configuration "Release"
set_property(TARGET flann::flann_cpp APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(flann::flann_cpp PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/flann_cpp.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/flann_cpp.dll"
  )

list(APPEND _cmake_import_check_targets flann::flann_cpp )
list(APPEND _cmake_import_check_files_for_flann::flann_cpp "${_IMPORT_PREFIX}/lib/flann_cpp.lib" "${_IMPORT_PREFIX}/bin/flann_cpp.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
