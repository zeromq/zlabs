#################################################################
#   GENERATED SOURCE CODE, DO NOT EDIT EXCEPT EXPERIMENTALLY    #
#   Please read the README.txt file in the model directory.     #
#################################################################

########################################################################
# Project setup
########################################################################
cmake_minimum_required(VERSION 2.8)
project(zlabs)
enable_language(C)
enable_testing()

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

########################################################################
# determine version
########################################################################
foreach(which MAJOR MINOR PATCH)
    file(STRINGS "${SOURCE_DIR}/include/zlabs.h" ZLABS_VERSION_STRING REGEX "#define ZLABS_VERSION_${which}")
    string(REGEX MATCH "#define ZLABS_VERSION_${which} ([0-9_]+)" ZLABS_REGEX_MATCH "${ZLABS_VERSION_STRING}")
    if (NOT ZLABS_REGEX_MATCH)
        message(FATAL_ERROR "failed to parse ZLABS_VERSION_${which} from zlabs.h")
    endif()
    set(ZLABS_${which}_VERSION ${CMAKE_MATCH_1})
endforeach(which)

set(ZLABS_VERSION ${ZLABS_MAJOR_VERSION}.${ZLABS_MINOR_VERSION}.${ZLABS_PATCH_VERSION})

########################################################################
# platform.h
########################################################################
include(CheckIncludeFile)
CHECK_INCLUDE_FILE("linux/wireless.h" HAVE_LINUX_WIRELESS_H)
CHECK_INCLUDE_FILE("net/if_media.h" HAVE_NET_IF_MEDIA_H)

include(CheckFunctionExists)
CHECK_FUNCTION_EXISTS("getifaddrs" HAVE_GETIFADDRS)
CHECK_FUNCTION_EXISTS("freeifaddrs" HAVE_FREEIFADDRS)

include(CheckIncludeFiles)
check_include_files("sys/socket.h;net/if.h" HAVE_NET_IF_H)
if (NOT HAVE_NET_IF_H)
    CHECK_INCLUDE_FILE("net/if.h" HAVE_NET_IF_H)
endif()

file(WRITE ${BINARY_DIR}/platform.h.in "
#cmakedefine HAVE_LINUX_WIRELESS_H
#cmakedefine HAVE_NET_IF_H
#cmakedefine HAVE_NET_IF_MEDIA_H
#cmakedefine HAVE_GETIFADDRS
#cmakedefine HAVE_FREEIFADDRS
")

configure_file(${BINARY_DIR}/platform.h.in ${BINARY_DIR}/platform.h)

#The MSVC C compiler is too out of date,
#so the sources have to be compiled as c++
if (MSVC)
    enable_language(CXX)
    file(GLOB sources ${SOURCE_DIR}/src/*.c)
    set_source_files_properties(${sources} PROPERTIES LANGUAGE CXX)
    set(MORE_LIBRARIES ws2_32 Rpcrt4 Iphlpapi)
endif()

# required libraries for mingw
if (MINGW)
    set(MORE_LIBRARIES -lws2_32 -lrpcrt4 -liphlpapi)
endif()

########################################################################
# ZeroMQ depedency
########################################################################
list(APPEND CMAKE_MODULE_PATH ${SOURCE_DIR})
find_package(ZeroMQ REQUIRED)

########################################################################
# includes
########################################################################
set (zlabs_headers
    include/zlabs_prelude.h
    include/zpubsub.h
    include/zpubsub_option.h
    include/ztimeout.h
    include/zpubsub_filter.h
)
source_group ("Header Files" FILES ${zlabs_headers})
install(FILES ${zlabs_headers} DESTINATION include)

########################################################################
# library
########################################################################
include_directories(${BINARY_DIR})
include_directories(${SOURCE_DIR}/include)
include_directories(${ZEROMQ_INCLUDE_DIRS})
set (zlabs_sources
    src/zpubsub.c
    src/zpubsub_option.c
    src/ztimeout.c
    src/zpubsub_filter.c
)
source_group ("Source Files" FILES ${zlabs_sources})
add_library(zlabs SHARED ${zlabs_sources})
set_target_properties(zlabs PROPERTIES DEFINE_SYMBOL "LIBZLABS_EXPORTS")
target_link_libraries(zlabs ${ZEROMQ_LIBRARIES} ${MORE_LIBRARIES})

install(TARGETS zlabs
    LIBRARY DESTINATION lib${LIB_SUFFIX} # .so file
    ARCHIVE DESTINATION lib${LIB_SUFFIX} # .lib file
    RUNTIME DESTINATION bin              # .dll file
)

########################################################################
# pkgconfig
########################################################################
set(VERSION "${ZLABS_VERSION}")
set(prefix "${CMAKE_INSTALL_PREFIX}")
set(exec_prefix "\${prefix}")
set(libdir "\${prefix}/lib${LIB_SUFFIX}")
set(includedir "\${prefix}/include")
configure_file(
    ${SOURCE_DIR}/src/libzlabs.pc.in
    ${BINARY_DIR}/libzlabs.pc
@ONLY)

install(
    FILES ${BINARY_DIR}/libzlabs.pc
    DESTINATION lib${LIB_SUFFIX}/pkgconfig
)

########################################################################
# tests
########################################################################
add_executable(zlabs_selftest ${SOURCE_DIR}/src/zlabs_selftest.c)
target_link_libraries(zlabs_selftest zlabs ${ZEROMQ_LIBRARIES})
add_test(zlabs_selftest zlabs_selftest)

########################################################################
# summary
########################################################################
message(STATUS "version: ${ZLABS_VERSION}")
message(STATUS "install: ${CMAKE_INSTALL_PREFIX}")

#################################################################
#   GENERATED SOURCE CODE, DO NOT EDIT EXCEPT EXPERIMENTALLY    #
#   Please read the README.txt file in the model directory.     #
#################################################################
