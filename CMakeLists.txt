cmake_minimum_required(VERSION 3.31)
project(2_5D_shooter)
set(CMAKE_CXX_STANDARD 23)

find_package(SDL2 REQUIRED)

include_directories(${SDL_INCLUDE_DIRS})

add_executable(2_5D_shooter src/main.cpp
        src/Game.cpp
        src/Game.h
        src/Renderer.cpp
        src/Renderer.h
        src/Map.cpp
        src/Map.h
        src/Player.cpp
        src/Player.h
        src/Raycaster.cpp
        src/Raycaster.h)

target_link_libraries(2_5D_shooter ${SDL2_LIBRARIES})