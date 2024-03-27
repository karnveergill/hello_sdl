cmake_minimum_required(VERSION 3.10)
project(hello_sdl)

# Add the executable
add_executable(${PROJECT_NAME})

# Add C++ source & header files to project
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.h")
target_sources(${PROJECT_NAME} PRIVATE ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${HEADERS})

# Find & include SDL2 
find_package(SDL2 REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS})

# Find & include SDL2_ttf 
find_package(SDL2_ttf REQUIRED)

# Set the path to SDL2_ttf include directory & library because homebrew installs in weird places
set(SDL2_TTF_INCLUDE_DIRS "/opt/homebrew/include/SDL2" CACHE PATH "Path to SDL2_ttf headers")
set(SDL2_TTF_LIBRARIES "/opt/homebrew/lib/libSDL2_ttf.dylib" CACHE FILEPATH "Path to SDL2_ttf library")
target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_TTF_INCLUDE_DIRS})

# Link SDL2 libraries
target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_TTF_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES}) 
