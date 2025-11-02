cmake_minimum_required(VERSION 3.16)
project(beat-bounce LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add raylib (expects its own CMakeLists.txt)
add_subdirectory(external/raylib)

# Include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/external/raylib/src
    ${PROJECT_SOURCE_DIR}/external/miniaudio
)

# Collect source files
file(GLOB_RECURSE SOURCES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

# Define the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link raylib
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

# Platform-specific libraries
if(APPLE)
  target_link_libraries(${PROJECT_NAME} PRIVATE
        "-framework IOKit"
        "-framework Cocoa"
        "-framework OpenGL"
        "-framework CoreVideo"
  )
elseif(UNIX)
  target_link_libraries(${PROJECT_NAME} PRIVATE pthread dl m)
endif()

# Preprocessor definition for miniaudio implementation (optional)
# If you include miniaudio implementation in one source file, e.g. audio.cpp:
#   #define MINIAUDIO_IMPLEMENTATION
#   #include "miniaudio.h"
#
# then no need to define it globally.

