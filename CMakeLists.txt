cmake_minimum_required(VERSION 3.28)
project(2_5D_shooter)
set(CMAKE_CXX_STANDARD 23)

find_package(SDL2 REQUIRED)
include_directories(${SDL_INCLUDE_DIRS})

find_package(SDL2_image REQUIRED)

add_executable(2_5D_shooter
        src/main.cpp
        src/engine/Renderer.cpp
        src/engine/Renderer.h
        src/engine/Game.cpp
        src/engine/Game.h
        src/engine/Texture.cpp
        src/engine/Texture.h
)

target_include_directories(2_5D_shooter PRIVATE src)
target_link_libraries(2_5D_shooter PRIVATE ${SDL2_LIBRARIES} SDL2_image::SDL2_image)