cmake_minimum_required(VERSION 3.10)
set(CMAKE_SYSTEM_NAME Generic-ELF)
project(hazel-kernel LANGUAGES C ASM)

set(ARCH i686)
set(CC "${ARCH}-elf-gcc")

file(GLOB_RECURSE SOURCES
        src/*.S
        src/*.c
        src/arch/${ARCH}/*.S
        src/arch/${ARCH}/*.c
)

set(CMAKE_C_COMPILER ${CC})
set(CMAKE_ASM_COMPILER ${CC})
set(CMAKE_C_FLAGS "-std=gnu99 -ffreestanding -Wall -Wextra -Werror")
set(CMAKE_EXE_LINKER_FLAGS "-ffreestanding -nostdlib -T ${CMAKE_CURRENT_SOURCE_DIR}/src/arch/${ARCH}/linker.ld")

add_executable(kernel ${SOURCES})
target_include_directories(kernel PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/src/arch/${ARCH})
target_link_libraries(kernel gcc)