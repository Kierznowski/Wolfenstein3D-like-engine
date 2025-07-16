cmake_minimum_required(VERSION 3.31)
project(2_5D_shooter)
set(CMAKE_CXX_STANDARD 23)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

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
        src/Raycaster.h
        src/TextureManager.cpp
        src/TextureManager.h)

target_link_libraries(2_5D_shooter ${SDL2_LIBRARIES} SDL2_image::SDL2_image)