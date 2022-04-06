cmake_minimum_required(VERSION 3.21)

project(mkpin C)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


set(CMAKE_C_STANDARD 17)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

set(CMAKE_C_FLAGS "-Wall -Wextra -fstack-protector -fstack-protector-all -pipe")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")


if ((CMAKE_HOST_SYSTEM_NAME EQUAL "Darwin") OR (CMAKE_C_COMPILER_ID STREQUAL "AppleClang"))
    execute_process(COMMAND zsh -c "uname -m" OUTPUT_VARIABLE MACOS_ARCH)
    execute_process(COMMAND zsh -c "sw_vers | awk '/^ProductVersion/{print $2}'" OUTPUT_VARIABLE MACOS_VER)
    # add_compile_options(-arch ${MACOS_ARCH} -mmacosx-version-min=${MACOS_VER})
    string(REPLACE "\n" "" MACOS_VER ${MACOS_VER})
    string(REPLACE "\n" "" MACOS_ARCH ${MACOS_ARCH})

    add_compile_options(-mmacosx-version-min=${MACOS_VER} -arch ${MACOS_ARCH})
endif ()

include_directories(.)

add_executable(mkpin pin.c)
