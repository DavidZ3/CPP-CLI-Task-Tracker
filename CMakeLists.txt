# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(MyProject VERSION 1.0)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the include directory to the include paths
include_directories(${CMAKE_SOURCE_DIR}/include)

# Define source files
file(GLOB SOURCES
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

# Add the executable target
add_executable(task_cli ${SOURCES})

# Copy the executable to the root directory after building
add_custom_command(TARGET task_cli POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:task_cli> ${CMAKE_SOURCE_DIR}
)
