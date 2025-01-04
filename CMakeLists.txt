cmake_minimum_required(VERSION 3.10)
set(ARCH i686)
set(CMAKE_SYSTEM_NAME Generic-ELF)
project(hazel-kernel LANGUAGES C ASM)

file(GLOB_RECURSE SOURCES
        src/*.S
        src/*.c
        src/arch/${ARCH}/*.S
        src/arch/${ARCH}/*.c
)

add_executable(kernel ${SOURCES})
target_include_directories(kernel PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/src/arch/${ARCH})
target_compile_options(kernel PRIVATE
        -std=gnu99
        -ffreestanding
        -Wall
        -Wextra
        -Werror
)
target_link_options(kernel PRIVATE
        -ffreestanding
        -nostdlib
        -T ${CMAKE_CURRENT_SOURCE_DIR}/src/arch/${ARCH}/linker.ld
)
target_link_libraries(kernel gcc)
