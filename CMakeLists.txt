
cmake_minimum_required( VERSION 3.18 )

project( "libaaf"
	LANGUAGES C
	DESCRIPTION "libaaf is a C coded library for Advanced Authoring Format \(AAF\) file handling"
	HOMEPAGE_URL "https://github.com/agfline/LibAAF" )

option( BUILD_STATIC_LIB "Build the static library"                  OFF )
option( BUILD_SHARED_LIB "Build the shared library"                  ON  )
option( BUILD_DOC        "Build documentation"                       ON  )
option( BUILD_UNIT_TEST  "Build unit test programs"                  ON  )
option( BUILD_AAFTOOL           "Build aaftool"                             ON  )
option( BUILD_WINDOWS_ON_LINUX  "Cross compile libaaf on Linux for Windows" OFF )

set( LIBAAF_VERSION "GIT" CACHE STRING "Set version manualy, git version used otherwise" )
set( LIBAAF_LIB_OUTPUT_NAME "aaf" )

set( LIBAAF_LIB_SRC_PATH      ${PROJECT_SOURCE_DIR}/src       )
set( LIBAAF_TOOLS_SRC_PATH    ${PROJECT_SOURCE_DIR}/tools     )
set( LIBAAF_DOC_INPUT_PATH    ${PROJECT_SOURCE_DIR}/doc       )
set( LIBAAF_DOC_OUTPUT_PATH   ${PROJECT_SOURCE_DIR}/build/doc )
set( LIBAAF_TEST_PATH         ${PROJECT_SOURCE_DIR}/test      )

set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )

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
endfunction()


# include/libaaf/version.h exists ONLY in released zip sources
if ( NOT EXISTS ${PROJECT_SOURCE_DIR}/include/libaaf/version.h )
	laaf_set_version()
	configure_file( ${PROJECT_SOURCE_DIR}/src/version.h.in ${CMAKE_BINARY_DIR}/include/libaaf/version.h @ONLY )
else()
	file( STRINGS ${PROJECT_SOURCE_DIR}/include/libaaf/version.h RES REGEX "#define LIBAAF_VERSION \"(v[^\"]+)\"" )
	string( REGEX MATCH "v[^\"]+" LIBAAF_VERSION ${RES} )
endif()

message( "libaaf version: ${LIBAAF_VERSION}" )


if ( BUILD_DOC )
	find_package(Doxygen)
	if ( DOXYGEN_FOUND )

		set( DOXYGEN_PROJECT_NAME ${PROJECT_NAME} )
		set( DOXYGEN_PROJECT_NUMBER ${LIBAAF_VERSION} )
		set( DOXYGEN_PROJECT_BRIEF ${PROJECT_BRIEF} )

		set( DOXYGEN_USE_MDFILE_AS_MAINPAGE ${PROJECT_SOURCE_DIR}/README.md )
		set( DOXYGEN_INPUT_DIR "${PROJECT_SOURCE_DIR}/include/ ${PROJECT_SOURCE_DIR}/src/ ${PROJECT_SOURCE_DIR}/README.md" )
		set( DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/doxygen )

		configure_file( ${LIBAAF_DOC_INPUT_PATH}/doxygen.in ${PROJECT_SOURCE_DIR}/build/doxygen.conf @ONLY )

		FILE( MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR} )

		# note the option ALL which allows to build the docs together with the application
		add_custom_target( doc
			COMMAND ${DOXYGEN_EXECUTABLE} ${PROJECT_SOURCE_DIR}/build/doxygen.conf
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
			COMMENT "Generating API documentation with Doxygen"
			VERBATIM )

	else (DOXYGEN_FOUND)
	  message( "Doxygen need to be installed to generate the documentation" )
	endif (DOXYGEN_FOUND)
endif (BUILD_DOC)


configure_file( ${PROJECT_SOURCE_DIR}/libaaf.pc.in ${CMAKE_BINARY_DIR}/libaaf.pc @ONLY )



if ( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
	set( LIBAAF_COMPILE_OPTIONS
		-W -Wstrict-prototypes -Wmissing-prototypes
		-Wall -Wcast-qual -Wcast-align -Wextra -Wwrite-strings -Wunsafe-loop-optimizations -Wlogical-op -std=c99 -pedantic -Wshadow # = Ardour's Waf --strict --c-strict
		-Wsign-conversion
		-Wconversion
		-Wcast-align=strict
		-O3
		-g
		-D_XOPEN_SOURCE=500
		# -pg
		-fdebug-prefix-map=${SRC_PATH}=.
	)
	# set( LIBAAF_LINK_OPTIONS -pg )
	if ( BUILD_WINDOWS_ON_LINUX )
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
		include( GNUInstallDirs )
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

# TODO: not required with gcc but "missing header" whith mingw-w64
include_directories( ${PROJECT_SOURCE_DIR}/include )



#########################
#     L i b r a r y     #
#########################

set( LIBAAF_LIB_SOURCES
	${LIBAAF_LIB_SRC_PATH}/LibCFB/LibCFB.c
	${LIBAAF_LIB_SRC_PATH}/LibCFB/CFBDump.c

	${LIBAAF_LIB_SRC_PATH}/AAFCore/AAFCore.c
	${LIBAAF_LIB_SRC_PATH}/AAFCore/AAFClass.c
	${LIBAAF_LIB_SRC_PATH}/AAFCore/AAFToText.c
	${LIBAAF_LIB_SRC_PATH}/AAFCore/AAFDump.c

	${LIBAAF_LIB_SRC_PATH}/AAFIface/AAFIface.c
	${LIBAAF_LIB_SRC_PATH}/AAFIface/AAFIParser.c
	${LIBAAF_LIB_SRC_PATH}/AAFIface/AAFIEssenceFile.c
	${LIBAAF_LIB_SRC_PATH}/AAFIface/RIFFParser.c
	${LIBAAF_LIB_SRC_PATH}/AAFIface/URIParser.c
	${LIBAAF_LIB_SRC_PATH}/AAFIface/ProTools.c
	${LIBAAF_LIB_SRC_PATH}/AAFIface/Resolve.c
	${LIBAAF_LIB_SRC_PATH}/AAFIface/MediaComposer.c

	${LIBAAF_LIB_SRC_PATH}/common/utils.c
	# ${LIBAAF_LIB_SRC_PATH}/common/ConvertUTF.c
	${LIBAAF_LIB_SRC_PATH}/common/log.c
)


if ( BUILD_SHARED_LIB )
	add_library( aaf-shared SHARED ${LIBAAF_LIB_SOURCES} )
	target_compile_options( aaf-shared PUBLIC ${LIBAAF_COMPILE_OPTIONS} )
	target_link_options( aaf-shared PUBLIC ${LIBAAF_LINK_OPTIONS} )
	target_include_directories( aaf-shared PUBLIC "${CMAKE_BINARY_DIR}/include/" )
	set_target_properties( aaf-shared PROPERTIES
		OUTPUT_NAME ${LIBAAF_LIB_OUTPUT_NAME}
		PREFIX "${LIBAAF_PREFIX}"
		SUFFIX "${LIBAAF_SHARED_SUFFIX}"
	)
	set( LINK_LIB aaf-shared )
endif( BUILD_SHARED_LIB )


if ( BUILD_STATIC_LIB )
	add_library( aaf-static STATIC ${LIBAAF_LIB_SOURCES} )
	target_compile_options( aaf-static PUBLIC ${LIBAAF_COMPILE_OPTIONS} )
	target_link_options( aaf-static PUBLIC ${LIBAAF_LINK_OPTIONS} )
	target_include_directories( aaf-static PUBLIC "${CMAKE_BINARY_DIR}/include/" )
	set_target_properties( aaf-static PROPERTIES
		OUTPUT_NAME ${LIBAAF_LIB_OUTPUT_NAME}
		PREFIX "${LIBAAF_PREFIX}"
		SUFFIX "${LIBAAF_STATIC_SUFFIX}"
	)
	set( LINK_LIB aaf-static )
endif( BUILD_STATIC_LIB )




#########################
#    P r o g r a m s    #
#########################

if ( BUILD_AAFTOOL )

	if ( ${LINK_LIB} MATCHES "aaf-static" )
		message( "Building aaftool upon static library" )
	elseif( ${LINK_LIB} MATCHES "aaf-shared" )
		message( "Building aaftool upon shared library" )
	endif()

	if ( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
		link_libraries( ${LINK_LIB} "-lm" )
	else()
		link_libraries( ${LINK_LIB} )
	endif()

	add_executable( aaftool
		${LIBAAF_TOOLS_SRC_PATH}/AAFTool.c
		${LIBAAF_TOOLS_SRC_PATH}/thirdparty/libTC.c
	)

	set_target_properties( aaftool PROPERTIES SUFFIX "${PROG_SUFFIX}" )

	target_include_directories( aaftool PRIVATE "${CMAKE_BINARY_DIR}/include/" )
	target_compile_options( aaftool PUBLIC ${LIBAAF_COMPILE_OPTIONS} )
	target_link_options( aaftool PUBLIC ${LIBAAF_LINK_OPTIONS} )

endif( BUILD_AAFTOOL )


if ( BUILD_UNIT_TEST )

	if ( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
		link_libraries( ${LINK_LIB} "-lm" )
	else()
		link_libraries( ${LINK_LIB} )
	endif()

	add_executable( test_utils
		${LIBAAF_TEST_PATH}/units/test_utils.c )

	add_executable( test_libtc
		${LIBAAF_TEST_PATH}/units/test_libtc.c
		${LIBAAF_TOOLS_SRC_PATH}/thirdparty/libTC.c )

	add_executable( test_uri
		${LIBAAF_TEST_PATH}/units/test_uri.c )

	set_target_properties( test_utils PROPERTIES SUFFIX "${PROG_SUFFIX}" )
	set_target_properties( test_libtc PROPERTIES SUFFIX "${PROG_SUFFIX}" )
	set_target_properties( test_uri   PROPERTIES SUFFIX "${PROG_SUFFIX}" )

endif( BUILD_UNIT_TEST )


if ( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )

	if ( BUILD_SHARED_LIB )
		install( TARGETS aaf-shared DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR} )
		install( FILES ${CMAKE_BINARY_DIR}/libaaf.pc DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}/pkgconfig/ )
	endif()

	if ( BUILD_STATIC_LIB )
		install( TARGETS aaf-static DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR} )
	endif()

	if ( BUILD_AAFTOOL )
		install( TARGETS aaftool DESTINATION ${CMAKE_INSTALL_FULL_BINDIR} )
	endif()

	install( DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR} FILES_MATCHING PATTERN "*.h*" )
	install( DIRECTORY ${CMAKE_BINARY_DIR}/include/ DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR} FILES_MATCHING PATTERN "*.h*" )

endif()



add_custom_target(
	source_release
	DEPENDS test
	WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
	COMMAND
	zip -r "${CMAKE_BINARY_DIR}/LibAAF_Source.zip"
		./LICENSE
		./CMakeLists.txt
		./libaaf.pc.in
		./src
		./include
		./tools
		# ./test
	COMMAND
	cd "${CMAKE_BINARY_DIR}" &&
	zip -r "${CMAKE_BINARY_DIR}/${LIBAAF_VERSION}.zip"
		./include/libaaf/version.h )


add_custom_target( clean-cmake-files
	COMMAND rm -rf "${CMAKE_BINARY_DIR}/CMakeFiles/"
	COMMAND rm -rf "${CMAKE_BINARY_DIR}/CMakeCache.txt"
	COMMAND rm -rf "${CMAKE_BINARY_DIR}/Makefile"
	COMMAND rm -rf "${CMAKE_BINARY_DIR}/*.cmake" )


if ( BUILD_WINDOWS_ON_LINUX )
	add_custom_target( test
		COMMAND wine ${CMAKE_BINARY_DIR}/bin/test_libtc${PROG_SUFFIX}
		COMMAND wine ${CMAKE_BINARY_DIR}/bin/test_uri${PROG_SUFFIX}
		COMMAND wine ${CMAKE_BINARY_DIR}/bin/test_utils${PROG_SUFFIX}
	COMMAND ${LIBAAF_TEST_PATH}/test.py --wine )
elseif ( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
	add_custom_target( test
		COMMAND ${CMAKE_BINARY_DIR}/bin/test_libtc${PROG_SUFFIX}
		COMMAND ${CMAKE_BINARY_DIR}/bin/test_uri${PROG_SUFFIX}
		COMMAND ${CMAKE_BINARY_DIR}/bin/test_utils${PROG_SUFFIX}
		COMMAND ${LIBAAF_TEST_PATH}/test.py --run-from-cmake )
else()
	add_custom_target( test
		COMMAND ${CMAKE_BINARY_DIR}/bin/test_libtc
		COMMAND ${CMAKE_BINARY_DIR}/bin/test_uri
		COMMAND ${CMAKE_BINARY_DIR}/bin/test_utils
		COMMAND ${LIBAAF_TEST_PATH}/test.py --run-from-cmake )
endif()
