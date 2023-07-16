
cmake_minimum_required( VERSION 3.11 )

project( "libaaf" C )

option( BUILD_STATIC_LIB "Build the static library"                  ON )
option( BUILD_SHARED_LIB "Build the shared library"                  ON )
option( BUILD_TOOLS      "Build AAFInfo and AAFExtract programs"     ON )

option( XBUILD_WIN       "Cross compile libaaf on Linux for Windows" OFF )


if ( NOT BUILD_SHARED_LIB AND NOT BUILD_STATIC_LIB )
  message( FATAL_ERROR "You need to enable at least one lib build (BUILD_STATIC_LIB or BUILD_SHARED_LIB)." )
endif()




# cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
find_package( Git )
add_custom_target( version
  ${CMAKE_COMMAND} -D SRC=${PROJECT_SOURCE_DIR}/src/version.h.in
                   -D DST=${PROJECT_SOURCE_DIR}/include/libaaf/version.h
                   -D PROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR}
                   -D GIT_EXECUTABLE=${GIT_EXECUTABLE}
                   -P ${CMAKE_SOURCE_DIR}/GenerateVersionHeader.cmake
  )




if ( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
  set( LIBAAF_COMPILE_OPTIONS
    -W -Wstrict-prototypes -Wmissing-prototypes
    -Wall -Wcast-qual -Wcast-align -Wextra -Wwrite-strings -Wunsafe-loop-optimizations -Wlogical-op -std=c99 -pedantic -Wshadow # = Ardour's Waf --strict --c-strict
    # -Wconversion
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
    set( LIBAAF_PREFIX "lib" )
    set( LIBAAF_SHARED_SUFFIX ".so" )
    set( LIBAAF_STATIC_SUFFIX ".a" )
    set( PROG_SUFFIX "" )
    set( BIN_INSTALL_PATH     "/usr/local/bin/" )
    set( LIB_INSTALL_PATH     "/usr/local/lib/" )
    set( HEADERS_INSTALL_PATH "/usr/local/include/" )
  endif()
elseif ( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
  set( CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS 1 )
  set( LIBAAF_PREFIX "lib" )
  set( LIBAAF_SHARED_SUFFIX ".dll" )
  set( LIBAAF_STATIC_SUFFIX ".obj" ) # MSVC
  set( PROG_SUFFIX ".exe" )
endif()


set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin )


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
)


if ( BUILD_SHARED_LIB )
  add_library( aaf-shared SHARED ${LIBAAF_LIB_SOURCES} )
  add_dependencies( aaf-shared version )
  target_compile_options( aaf-shared PUBLIC ${LIBAAF_COMPILE_OPTIONS} )
  set_target_properties( aaf-shared PROPERTIES OUTPUT_NAME "aaf" )
  set_target_properties( aaf-shared PROPERTIES
    PREFIX "${LIBAAF_PREFIX}"
    SUFFIX "${LIBAAF_SHARED_SUFFIX}"
  )
  set( LINK_LIB aaf-shared )
endif( BUILD_SHARED_LIB )


if ( BUILD_STATIC_LIB )
  add_library( aaf-static STATIC ${LIBAAF_LIB_SOURCES} )
  add_dependencies( aaf-static version )
  target_compile_options( aaf-static PUBLIC ${LIBAAF_COMPILE_OPTIONS} )
  set_target_properties( aaf-static PROPERTIES OUTPUT_NAME "aaf" )
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
    ${TOOLS_SRC_PATH}/thirdparty/libTC.c
  )

  add_executable( AAFExtract
    ${TOOLS_SRC_PATH}/AAFExtract.c
    ${TOOLS_SRC_PATH}/thirdparty/libTC.c
  )

  set_target_properties( AAFInfo    PROPERTIES SUFFIX "${PROG_SUFFIX}" )
  set_target_properties( AAFExtract PROPERTIES SUFFIX "${PROG_SUFFIX}" )

  target_compile_options( AAFInfo    PUBLIC ${LIBAAF_COMPILE_OPTIONS} )
  target_compile_options( AAFExtract PUBLIC ${LIBAAF_COMPILE_OPTIONS} )

endif( BUILD_TOOLS )



if ( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
  install( TARGETS aaf-shared DESTINATION ${LIB_INSTALL_PATH} )
  install( TARGETS aaf-static DESTINATION ${LIB_INSTALL_PATH} )
  install( TARGETS AAFInfo    DESTINATION ${BIN_INSTALL_PATH} )
  install( TARGETS AAFExtract DESTINATION ${BIN_INSTALL_PATH} )
  install( DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION ${HEADERS_INSTALL_PATH} FILES_MATCHING PATTERN "*.h*" )
endif()
