cmake_minimum_required(VERSION 3.7)
project(PCG)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(CMAKE_MODULE_PATH "/usr/share/SFML/cmake/Modules;${CMAKE_MODULE_PATH}")
find_package(SFML 2 REQUIRED network audio graphics window system)

set(EXECUTABLE_NAME PCGDemo)
add_subdirectory(Includes)
add_executable(${EXECUTABLE_NAME}
	main.cpp
    Sources/Enemy.cpp
    Sources/Entity.cpp
    Sources/Game.cpp
    Sources/Gem.cpp
    Sources/Gold.cpp
    Sources/Heart.cpp
    Sources/Humanoid.cpp
    Sources/Input.cpp
    Sources/Item.cpp
    Sources/Key.cpp
    Sources/Level.cpp
    Sources/Object.cpp
    Sources/PCH.cpp
    Sources/Player.cpp
    Sources/Potion.cpp
    Sources/Projectile.cpp
    Sources/Slime.cpp
    Sources/SoundBufferManager.cpp
    Sources/TextureManager.cpp
    Sources/Torch.cpp)

include_directories(${SFML_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/Includes)
target_link_libraries(${EXECUTABLE_NAME} MainLibrary)
