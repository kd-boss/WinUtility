cmake_minimum_required(VERSION 3.10)
project(SystemMetricsD2D1 LANGUAGES CXX RC)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_definitions(-DUNICODE -D_UNICODE -DNOMINMAX)
add_compile_options("/utf-8")
add_compile_options("/std:c++latest")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_WIN32_EXECUTABLE ON)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /MANIFEST:NO")

add_executable(SystemMetricsD2D1 WIN32
		About.cpp
        SystemMetricsD2D1.cpp
		Main.cpp
        SystemMetricsD2D1res.rc
    )

target_include_directories(SystemMetricsD2D1 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
target_link_libraries(SystemMetricsD2D1 PRIVATE user32 gdi32 comctl32 d2d1 ole32 dwrite)

add_compile_options(/MP /std:c++latest)