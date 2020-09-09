cmake_minimum_required(VERSION 3.17)
project(mkpin C)

set(CMAKE_C_STANDARD 99)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

set(CMAKE_C_FLAGS "-Wall -Wextra -fstack-protector -fstack-protector-all -pipe")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")


if (CMAKE_SYSTEM_NAME EQUAL "Darwin")
    set(CMAKE_C_FLAGS, "${CMAKE_C_FLAGS} -arch x86_64  -mmacosx-version-min=10.15")
ENDIF ()


include_directories(.)

add_executable(mkpin pin.c)
