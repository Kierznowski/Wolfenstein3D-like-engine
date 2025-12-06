# Game/CMakeLists.txt

project(Game)

add_executable(Game
        src/main.cpp
        src/Game.cpp
        include/Game/GameState.h
        include/Game/MainMenuState.h
        src/MainMenuState.cpp
)

target_include_directories(Game
        PRIVATE
            include
)

target_link_libraries(Game
    PRIVATE Engine
)