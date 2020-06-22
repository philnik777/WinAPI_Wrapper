cmake_minimum_required(VERSION 3.10)
project(winapi VERSION 0.5)

if (LINUX)
    message(FATAL_ERROR "Linux has no WinAPI, idiot")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(src)

set(WINAPI_LIBRARIES
    ${SUBPROJECTS})

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(
        -fuse-ld=lld 
        -Weverything 
        -Wno-padded 
        -Wno-c++98-compat 
        -Wno-c++98-compat-pedantic)

    if (CMAKE_BUILD_TYPE STREQUAL "Release")
        target_compile_options(hausverwaltung PRIVATE -flto=thin)
    endif()
endif()
