cmake_minimum_required(VERSION 3.0.0)
project(pseudo VERSION 0.0.1 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
  ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
  ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
  ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

add_subdirectory(src)
add_subdirectory(external)

# first we can indicate the documentation build as an option and set it to ON by default
#option(BUILD_DOC "Build documentation" ON)

enable_testing()
add_subdirectory(tests)

#find_package(Doxygen)

#if (DOXYGEN_FOUND)
#    # set input and output files
#    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
#    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.out)
#
#    # request to configure the file
#    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
#    message("Doxygen build started")
#
#    # Note: do not put "ALL" - this builds docs together with application EVERY TIME!
#    add_custom_target( docs
#        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
#        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
#        COMMENT "Generating API documentation with Doxygen"
#        VERBATIM )
#else (DOXYGEN_FOUND)
#  message("Doxygen need to be installed to generate the doxygen documentation")
#endif (DOXYGEN_FOUND)