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
