cmake_minimum_required(VERSION 3.10)
project(hazel-kernel LANGUAGES C ASM)

file(GLOB_RECURSE SOURCES
        src/*.S
        src/*.c
        src/arch/${ARCH}/*.S
        src/arch/${ARCH}/*.c
)

add_executable(kernel ${SOURCES})
target_include_directories(kernel PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/src/arch/${ARCH})
target_link_libraries(kernel gcc)

add_custom_command(
        OUTPUT  ${CMAKE_CURRENT_SOURCE_DIR}/Hazel.iso
        DEPENDS kernel
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/boot/grub/grub.cfg
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND wsl rm -rf Hazel.iso boot/kernel.elf
        COMMAND "${CMAKE_COMMAND}" -E copy ${CMAKE_CURRENT_BINARY_DIR}/kernel.elf ${CMAKE_CURRENT_SOURCE_DIR}/boot
        COMMAND wsl grub-mkrescue -o Hazel.iso .
)

add_custom_target(iso
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Hazel.iso
)