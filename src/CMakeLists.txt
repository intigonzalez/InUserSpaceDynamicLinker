cmake_minimum_required(VERSION 2.8.4)
project(LoadingElf)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32 -std=gnu99")

set(SOURCE_FILES main.c FileManagement.c Allocator.c elfloader.c MyHashTable.c)
add_executable(LoadingElf ${SOURCE_FILES})