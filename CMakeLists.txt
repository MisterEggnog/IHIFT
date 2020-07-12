cmake_minimum_required(VERSION 0.2)
project("I Hope I Finish This")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(IHIFT
IHIFT.cpp
entity.cpp
monster.cpp
player.cpp
projectile.cpp
utils.cpp)

target_link_libraries(IHIFT
-lsfml-graphics
-lsfml-window
-lsfml-system
-lsfml-audio)
