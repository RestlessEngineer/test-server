cmake_minimum_required(VERSION 3.16.0)

project(myserver VERSION 0.1.0)

find_package(PkgConfig REQUIRED)
pkg_search_module(OPENSSL REQUIRED openssl)

if( OPENSSL_FOUND )
    include_directories(${OPENSSL_INCLUDE_DIRS})
    message(STATUS "Using OpenSSL ${OPENSSL_VERSION}")
else()
    message(STATUS "Error: you need to install openssl library. Try to employ \"sudo apt install libssl-dev\"")
endif()

set(CMAKE_C_FLAGS "-DLIBUS_NO_SSL")
set(CMAKE_C_FLAGS_DEBUG "-DLIBUS_NO_SSL")
set(CMAKE_C_FLAGS_RELEASE "-DLIBUS_NO_SSL")

set(CMAKE_CXX_FLAGS "-DLIBUS_NO_SSL -std=c++17 -fconcepts")
set(CMAKE_CXX_FLAGS_DEBUG "-DLIBUS_NO_SSL -std=c++17 -ggdb -fconcepts")
set(CMAKE_CXX_FLAGS_RELEASE "-DLIBUS_NO_SSL -std=c++17 -fconcepts")

set(UWEBSOCKETS_INCLUDE_DIRS "~/.local/include/uWebSockets")

include_directories(${UWEBSOCKETS_INCLUDE_DIRS})
include_directories("~/.local/uSockets/src")
include_directories("~/.local/uSockets/src/crypto")
include_directories("~/.local/uSockets/src/boringssl/include")

add_executable(myserver myserver.cpp)

target_link_libraries(myserver $(echo $HOME)/.local/lib/uSockets.a) # something may going wronng. Use full path to library
target_link_libraries(myserver ${OPENSSL_LIBRARIES})
target_link_libraries(myserver "/lib/x86_64-linux-gnu/libz.so") # it is maybe different