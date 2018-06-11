# Notice name prefix of this variable, set by CMake according
# to value given with "project()" in the root CMakeLists.txt.
include_directories(${PROJECT_SOURCE_DIR}/Includes)
add_library(MainLibrary 
    Enemy.cpp
    Entity.cpp
    Game.cpp
    Gem.cpp
    Gold.cpp
    Heart.cpp
    Humanoid.cpp
    Input.cpp
    Item.cpp
    Key.cpp
    Level.cpp
    Object.cpp
    PCH.cpp
    Player.cpp
    Potion.cpp
    Projectile.cpp
    Slime.cpp
    SoundBufferManager.cpp
    TextureManager.cpp
    Torch.cpp)