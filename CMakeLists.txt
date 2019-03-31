cmake_minimum_required(VERSION 3.14)

set (CMAKE_CXX_STANDARD 17)

project(SDLHelloWorld)
SET(CMAKE_CXX_FLAGS -pthread)
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
add_executable(${PROJECT_NAME} "main.cpp" button.cpp onscreenmanager.cpp)
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})

