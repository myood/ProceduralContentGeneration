cmake_minimum_required(VERSION 3.7)
project(PCG)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_MODULE_PATH "/usr/share/SFML/cmake/Modules;${CMAKE_MODULE_PATH}")
find_package(SFML 2 REQUIRED network audio graphics window system)

set(EXECUTABLE_NAME PCGDemo)
add_executable(${EXECUTABLE_NAME}
    Sources/main.cpp
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
    Sources/Torch.cpp

    Includes/Enemy.h
    Includes/Entity.h
    Includes/Game.h
    Includes/Gem.h
    Includes/Gold.h
    Includes/Heart.h
    Includes/Humanoid.h
    Includes/Input.h
    Includes/Item.h
    Includes/Key.h
    Includes/Level.h
    Includes/Object.h
    Includes/PCH.h
    Includes/Player.h
    Includes/Potion.h
    Includes/Projectile.h
    Includes/Slime.h
    Includes/SoundBufferManager.h
    Includes/TextureManager.h
    Includes/Torch.h)


message(${SFML_INCLUDE_DIR})
target_include_directories(${EXECUTABLE_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Includes>
    $<INSTALL_INTERFACE:Includes>)
target_include_directories(${EXECUTABLE_NAME} PUBLIC
	$<BUILD_INTERFACE:${SFML_INCLUDE_DIR}>
	$<INSTALL_INTERFACE:SFML>)
target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
