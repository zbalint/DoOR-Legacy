cmake_minimum_required(VERSION 2.8)
project(door)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c11")

set(SOURCE_FILES main.c stdgc.c stdgc.h ramdisk.c ramdisk.h config_loader.c config_loader.h installer.c installer.c installer.h rsync.c rsync.h module_loader.c module_loader.h)
add_executable(door ${SOURCE_FILES})

add_definitions(-DVERSION="1.1.0")

exec_program(
        "git"
        ${CMAKE_CURRENT_SOURCE_DIR}
        ARGS "describe --match build"
        OUTPUT_VARIABLE VERSION)

string(REGEX MATCH "[0-9]+" COMMIT_COUNT ${VERSION})
string(REGEX MATCH "-g.*$" VERSION_SHA1 ${VERSION})
string(REGEX REPLACE "[-g]" "" VERSION_SHA1 ${VERSION_SHA1})


add_definitions(-DCOMMIT_NUMBER="${COMMIT_COUNT}")
add_definitions(-DGIT_SHA1="${VERSION_SHA1}")