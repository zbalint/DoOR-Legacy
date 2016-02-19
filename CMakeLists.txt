cmake_minimum_required(VERSION 2.8)
project(DoOR)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c11")

set(SOURCE_FILES main.c ramdisk.c ramdisk.h)
add_executable(DoOR ${SOURCE_FILES})


exec_program(
        "git"
        ${CMAKE_CURRENT_SOURCE_DIR}
        ARGS "describe --match build"
        OUTPUT_VARIABLE VERSION)

string(REGEX MATCH "[0-9]+" COMMIT_COUNT ${VERSION})
string(REGEX MATCH "-g.*$" VERSION_SHA1 ${VERSION})
string(REGEX REPLACE "[-g]" "" VERSION_SHA1 ${VERSION_SHA1})

add_definitions(-DVERSION="0.0.1")
add_definitions(-DCOMMIT_NUMBER="${COMMIT_COUNT}")
add_definitions(-DGIT_SHA1="${VERSION_SHA1}")