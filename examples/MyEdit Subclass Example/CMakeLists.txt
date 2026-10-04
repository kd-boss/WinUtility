cmake_minimum_required(VERSION 3.20)

project(ContainedEditTest LANGUAGES CXX)

add_compile_definitions(UNICODE _UNICODE)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(ContainedEditTest WIN32
    ContainedEditTest.cpp
    ContainedEditTest.h
)

target_compile_definitions(ContainedEditTest PRIVATE
    NOMINMAX
    UNICODE
    _UNICODE
    WINVER=0x0A00
    _WIN32_WINNT=0x0A00
)

target_include_directories(ContainedEditTest PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ContainedEditTest PRIVATE
    user32
    gdi32
    comctl32
)
