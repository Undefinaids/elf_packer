cmake_minimum_required(VERSION 3.17)
project(elf_packer C)

set(CMAKE_C_STANDARD 99)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(elf_packer src/main.c)