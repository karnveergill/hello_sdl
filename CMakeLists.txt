cmake_minimum_required(VERSION 3.10)
project(hello_sdl)
set(SOURCE_FILES hello_sdl.cpp)

# Find SDL2 Library
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Add C++ source file to project
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Link SDL library to project
target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES})