cmake_minimum_required(VERSION 3.10)
project(COLORS1 LANGUAGES CXX RC)

# Set MSVC 14.51.36231 toolchain explicitly
set(MSVC_ROOT "C:/Program Files (x86)/Microsoft Visual Studio/18/BuildTools/VC/Tools/MSVC/14.51.36231")
set(CMAKE_CXX_COMPILER "${MSVC_ROOT}/bin/Hostx64/x64/cl.exe")
set(CMAKE_RC_COMPILER "${MSVC_ROOT}/../../../Windows Kits/10/bin/10.0.22621.0/x64/rc.exe")
set(CMAKE_LINKER "${MSVC_ROOT}/bin/Hostx64/x64/link.exe")

# Include directories
set(CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES "${MSVC_ROOT}/include")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_definitions(-DUNICODE -D_UNICODE -DNOMINMAX)
add_compile_options("/utf-8")
add_compile_options("/std:c++latest")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_WIN32_EXECUTABLE ON)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /MANIFEST:NO")

add_executable(COLORS1 WIN32
        COLORS1.cpp
        COLORS1.rc
    )

target_include_directories(COLORS1 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} "${MSVC_ROOT}/include")
target_link_libraries(COLORS1 PRIVATE user32 gdi32 comctl32 d2d1 ole32)

add_compile_options(/MP /std:c++latest /linker /SUBSYSTEM:WINDOWS)