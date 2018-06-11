cmake_minimum_required(VERSION 3.7)
project(PCG)
add_subdirectory(Sources)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

add_executable(PCGDemo ${PROJECT_SOURCE_DIR}/main.cpp)
target_link_libraries(PCGDemo MainLibrary)