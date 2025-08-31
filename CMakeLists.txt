cmake_minimum_required(VERSION 3.10)
project(hazel-kernel LANGUAGES C ASM)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

file(GLOB_RECURSE SOURCES
        src/*.S
        src/*.c
        src/arch/${ARCH}/*.S
        src/arch/${ARCH}/*.c
)

add_executable(kernel ${SOURCES})
target_include_directories(kernel PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/include/arch/${ARCH})
target_link_libraries(kernel gcc)

add_subdirectory(init-mod)

if (CMAKE_HOST_WIN32)
    set(WSL "wsl")
endif()

add_custom_command(
        OUTPUT  ${CMAKE_CURRENT_SOURCE_DIR}/Hazel.iso

        DEPENDS kernel
        DEPENDS init-mod
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/boot/grub/grub.cfg

        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}

        COMMAND ${WSL} rm -rf Hazel.iso boot/kernel.elf boot/init-mod.elf
        COMMAND "${CMAKE_COMMAND}" -E copy ${CMAKE_CURRENT_BINARY_DIR}/kernel.elf ${CMAKE_CURRENT_SOURCE_DIR}/boot
        COMMAND "${CMAKE_COMMAND}" -E copy ${CMAKE_CURRENT_BINARY_DIR}/init-mod/init-mod.elf ${CMAKE_CURRENT_SOURCE_DIR}/boot
        COMMAND ${WSL} i686-elf-grub-mkrescue -o Hazel.iso .
)

add_custom_target(iso
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Hazel.iso
)
