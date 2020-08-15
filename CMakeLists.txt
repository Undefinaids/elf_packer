cmake_minimum_required(VERSION 3.17)
project(elf_packer C)

set(CMAKE_C_STANDARD 99)
include_directories(${CMAKE_SOURCE_DIR}/include)
enable_language(ASM_NASM)

set(ASM_SOURCES
        src/stub.asm)

set(SOURCES
        src/main.c
        src/extract_headers.c
        src/utils.c
        src/section.c
        src/dump.c
        ${ASM_SOURCES})

set_source_files_properties(${ASM_SOURCES} PROPERTIES LANGUAGE ASM_NASM)

add_executable(elf_packer ${SOURCES})