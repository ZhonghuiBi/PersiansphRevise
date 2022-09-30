#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "blas" for configuration ""
set_property(TARGET blas APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(blas PROPERTIES
  IMPORTED_IMPLIB_NOCONFIG "${_IMPORT_PREFIX}/lib/libblas${CMAKE_IMPORT_LIBRARY_SUFFIX}"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/bin/libblas.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS blas )
list(APPEND _IMPORT_CHECK_FILES_FOR_blas "${_IMPORT_PREFIX}/lib/libblas${CMAKE_IMPORT_LIBRARY_SUFFIX}" "${_IMPORT_PREFIX}/bin/libblas.dll" )

# Import target "lapack" for configuration ""
set_property(TARGET lapack APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(lapack PROPERTIES
  IMPORTED_IMPLIB_NOCONFIG "${_IMPORT_PREFIX}/lib/liblapack${CMAKE_IMPORT_LIBRARY_SUFFIX}"
  IMPORTED_LINK_INTERFACE_LIBRARIES_NOCONFIG "blas"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/bin/liblapack.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS lapack )
list(APPEND _IMPORT_CHECK_FILES_FOR_lapack "${_IMPORT_PREFIX}/lib/liblapack${CMAKE_IMPORT_LIBRARY_SUFFIX}" "${_IMPORT_PREFIX}/bin/liblapack.dll" )

# Import target "tmglib" for configuration ""
set_property(TARGET tmglib APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(tmglib PROPERTIES
  IMPORTED_IMPLIB_NOCONFIG "${_IMPORT_PREFIX}/lib/libtmglib${CMAKE_IMPORT_LIBRARY_SUFFIX}"
  IMPORTED_LINK_INTERFACE_LIBRARIES_NOCONFIG "lapack"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/bin/libtmglib.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS tmglib )
list(APPEND _IMPORT_CHECK_FILES_FOR_tmglib "${_IMPORT_PREFIX}/lib/libtmglib${CMAKE_IMPORT_LIBRARY_SUFFIX}" "${_IMPORT_PREFIX}/bin/libtmglib.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
