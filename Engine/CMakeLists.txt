# Engine/CMakeLists.txt

project(Engine)

add_library(Engine STATIC
        src/Raycaster.cpp
        src/Engine.cpp
        src/Renderer.cpp
        src/Texture.cpp
        src/Map.cpp
        src/utils/Assets.cpp
        src/Player.cpp
        src/hud/HUD.cpp
        src/utils/BitmapFont.cpp
)

target_include_directories(Engine
        PUBLIC
            include
        PRIVATE
            src
)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

target_link_libraries(Engine
        PUBLIC SDL2 SDL2_ttf
)