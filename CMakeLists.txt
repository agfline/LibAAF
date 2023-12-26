
cmake_minimum_required( VERSION 3.11 )

project( "libaaf"
	LANGUAGES C
	DESCRIPTION "libaaf is a C coded library for Advanced Authoring Format \(AAF\) file handling"
	HOMEPAGE_URL "https://github.com/agfline/LibAAF" )

option( BUILD_STATIC_LIB "Build the static library"                  ON )
option( BUILD_SHARED_LIB "Build the shared library"                  ON )
option( BUILD_TOOLS      "Build AAFInfo and AAFExtract programs"     ON )

option( XBUILD_WIN       "Cross compile libaaf on Linux for Windows" OFF )

set( LIBAAF_VERSION "GIT" CACHE STRING "Set version manualy, git version used otherwise" )
set( LIBAAF_LIB_OUTPUT_NAME "aaf" )

if ( NOT BUILD_SHARED_LIB AND NOT BUILD_STATIC_LIB )
  message( FATAL_ERROR "You need to enable at least one lib build (BUILD_STATIC_LIB or BUILD_SHARED_LIB)." )
endif()




function( laaf_set_version )

	find_package( Git )

	if ( LIBAAF_VERSION STREQUAL "GIT" AND GIT_EXECUTABLE )
		# Generate a git-describe version string from Git repository tags
		execute_process(
				COMMAND ${GIT_EXECUTABLE} describe --tags --dirty --match "v*"
				WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
				OUTPUT_VARIABLE GIT_DESCRIBE_VERSION
				RESULT_VARIABLE GIT_DESCRIBE_ERROR_CODE
				OUTPUT_STRIP_TRAILING_WHITESPACE
			)
		if ( NOT GIT_DESCRIBE_ERROR_CODE )
			set( LIBAAF_VERSION ${GIT_DESCRIBE_VERSION} )
		endif()
	endif()

	# fallback: Just use a bogus version string that is semantically older
	# than anything else and spit out a warning to the developer.
	if ( LIBAAF_VERSION STREQUAL "GIT" )
		set( LIBAAF_VERSION v0.0.0-unknown )
		message( WARNING "Failed to determine LIBAAF_VERSION from Git tags. Using default version \"${LIBAAF_VERSION}\"." )
	endif()

	set(LIBAAF_VERSION ${LIBAAF_VERSION} PARENT_SCOPE)

	message( "libaaf version: ${LIBAAF_VERSION}" )
endfunction()


laaf_set_version()

configure_file( ${PROJECT_SOURCE_DIR}/src/version.h.in ${CMAKE_BINARY_DIR}/include/libaaf/version.h @ONLY )
configure_file( ${PROJECT_SOURCE_DIR}/libaaf.pc.in ${CMAKE_BINARY_DIR}/libaaf.pc @ONLY )


if ( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
  set( LIBAAF_COMPILE_OPTIONS
    -W -Wstrict-prototypes -Wmissing-prototypes
    -Wall -Wcast-qual -Wcast-align -Wextra -Wwrite-strings -Wunsafe-loop-optimizations -Wlogical-op -std=c99 -pedantic -Wshadow # = Ardour's Waf --strict --c-strict
    # -Wconversion
    -Wcast-align=strict
    -O3
    -g
    -fdebug-prefix-map=${SRC_PATH}=.
  )
  if ( XBUILD_WIN )
    message( "┌─────────────────────────────┐" )
    message( "│ Cross compiling for Windows │" )
    message( "└─────────────────────────────┘" )
    set( CMAKE_SYSTEM_NAME "Windows" )
    set( CMAKE_CROSSCOMPILING true )
    set( CMAKE_C_COMPILER "x86_64-w64-mingw32-gcc" )
    set( LIBAAF_PREFIX "lib" )
    set( LIBAAF_SHARED_SUFFIX ".dll" )
    set( LIBAAF_STATIC_SUFFIX ".lib" )
    set( PROG_SUFFIX ".exe" )
  else()
    message( "┌─────────────────────┐" )
    message( "│ Compiling for Linux │" )
    message( "└─────────────────────┘" )
		include(GNUInstallDirs)
    set( LIBAAF_PREFIX "lib" )
    set( LIBAAF_SHARED_SUFFIX ".so" )
    set( LIBAAF_STATIC_SUFFIX ".a" )
    set( PROG_SUFFIX "" )
  endif()
elseif ( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
  set( CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS 1 )
  set( LIBAAF_PREFIX "lib" )
  set( LIBAAF_SHARED_SUFFIX ".dll" )
  set( LIBAAF_STATIC_SUFFIX ".obj" ) # MSVC
  set( PROG_SUFFIX ".exe" )
endif()


set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )


set( LIB_SRC_PATH      ${PROJECT_SOURCE_DIR}/src )
set( TOOLS_SRC_PATH    ${PROJECT_SOURCE_DIR}/tools )
set( EXAMPLES_SRC_PATH ${PROJECT_SOURCE_DIR}/examples )



# file( READ ${VERSION_FILE} VERSION_STR )
#
# string( REGEX MATCH "^([0-9]*)\.([0-9]*)\.([0-9]*)" _ ${VERSION_STR} )
#
# message( "Building ${VERSION_STR}" )
#
# set( PROJECT_VERSION_MAJOR ${CMAKE_MATCH_1} )
# set( PROJECT_VERSION_MINOR ${CMAKE_MATCH_2} )
# set( PROJECT_VERSION_PATCH ${CMAKE_MATCH_3} )



include_directories( ${PROJECT_SOURCE_DIR}/include )



#########################
#     L i b r a r y     #
#########################

set( LIBAAF_LIB_SOURCES
  ${LIB_SRC_PATH}/LibCFB/LibCFB.c
  ${LIB_SRC_PATH}/LibCFB/CFBDump.c

  ${LIB_SRC_PATH}/AAFCore/AAFCore.c
  ${LIB_SRC_PATH}/AAFCore/AAFClass.c
  ${LIB_SRC_PATH}/AAFCore/AAFToText.c
  ${LIB_SRC_PATH}/AAFCore/AAFDump.c

  ${LIB_SRC_PATH}/AAFIface/AAFIface.c
  ${LIB_SRC_PATH}/AAFIface/AAFIParser.c
  ${LIB_SRC_PATH}/AAFIface/AAFIAudioFiles.c
  ${LIB_SRC_PATH}/AAFIface/RIFFParser.c
  ${LIB_SRC_PATH}/AAFIface/URIParser.c
  ${LIB_SRC_PATH}/AAFIface/ProTools.c
  ${LIB_SRC_PATH}/AAFIface/Resolve.c

  ${LIB_SRC_PATH}/common/utils.c
  ${LIB_SRC_PATH}/debug.c
)


if ( BUILD_SHARED_LIB )
  add_library( aaf-shared SHARED ${LIBAAF_LIB_SOURCES} )
  # add_dependencies( aaf-shared version )
  target_compile_options( aaf-shared PUBLIC ${LIBAAF_COMPILE_OPTIONS} )
  target_include_directories ( aaf-shared PUBLIC "${CMAKE_BINARY_DIR}/include/" )
  set_target_properties( aaf-shared PROPERTIES OUTPUT_NAME LIBAAF_LIB_OUTPUT_NAME )
  set_target_properties( aaf-shared PROPERTIES
    PREFIX "${LIBAAF_PREFIX}"
    SUFFIX "${LIBAAF_SHARED_SUFFIX}"
  )
  set( LINK_LIB aaf-shared )
endif( BUILD_SHARED_LIB )


if ( BUILD_STATIC_LIB )
  add_library( aaf-static STATIC ${LIBAAF_LIB_SOURCES} )
  # add_dependencies( aaf-static version )
  target_compile_options( aaf-static PUBLIC ${LIBAAF_COMPILE_OPTIONS} )
  target_include_directories ( aaf-static PUBLIC "${CMAKE_BINARY_DIR}/include/" )
  set_target_properties( aaf-static PROPERTIES OUTPUT_NAME LIBAAF_LIB_OUTPUT_NAME )
  set_target_properties( aaf-static PROPERTIES
    PREFIX "${LIBAAF_PREFIX}"
    SUFFIX "${LIBAAF_STATIC_SUFFIX}"
  )
  set( LINK_LIB aaf-static )
endif( BUILD_STATIC_LIB )




#########################
#    P r o g r a m s    #
#########################

if ( BUILD_TOOLS )

  if ( ${LINK_LIB} MATCHES "aaf-static" )
    message( "Building AAFIface and AAFExtract upon static library" )
  elseif( ${LINK_LIB} MATCHES "aaf-shared" )
    message( "Building AAFIface and AAFExtract upon shared library" )
  endif()

  if ( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
    link_libraries( ${LINK_LIB} "-lm" )
  else()
    link_libraries( ${LINK_LIB} )
  endif()

  add_executable( AAFInfo
    ${TOOLS_SRC_PATH}/AAFInfo.c
    ${TOOLS_SRC_PATH}/common.c
    ${TOOLS_SRC_PATH}/thirdparty/libTC.c
  )

  add_executable( AAFExtract
    ${TOOLS_SRC_PATH}/AAFExtract.c
    ${TOOLS_SRC_PATH}/common.c
    ${TOOLS_SRC_PATH}/thirdparty/libTC.c
  )

  target_include_directories ( AAFInfo    PRIVATE "${CMAKE_BINARY_DIR}/include/" )
  target_include_directories ( AAFExtract PRIVATE "${CMAKE_BINARY_DIR}/include/" )

  set_target_properties( AAFInfo    PROPERTIES SUFFIX "${PROG_SUFFIX}" )
  set_target_properties( AAFExtract PROPERTIES SUFFIX "${PROG_SUFFIX}" )

  target_compile_options( AAFInfo    PUBLIC ${LIBAAF_COMPILE_OPTIONS} )
  target_compile_options( AAFExtract PUBLIC ${LIBAAF_COMPILE_OPTIONS} )

endif( BUILD_TOOLS )



if ( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )

	if ( BUILD_SHARED_LIB )
  	install( TARGETS aaf-shared DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR} )
		install( FILES ${CMAKE_BINARY_DIR}/libaaf.pc DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}/pkgconfig/ )
	endif()

	if ( BUILD_STATIC_LIB )
  	install( TARGETS aaf-static DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR} )
	endif()

	if ( BUILD_TOOLS )
  	install( TARGETS AAFInfo    DESTINATION ${CMAKE_INSTALL_FULL_BINDIR} )
  	install( TARGETS AAFExtract DESTINATION ${CMAKE_INSTALL_FULL_BINDIR} )
	endif()

  install( DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR} FILES_MATCHING PATTERN "*.h*" )
  install( DIRECTORY ${CMAKE_BINARY_DIR}/include/ DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR} FILES_MATCHING PATTERN "*.h*" )

endif()
