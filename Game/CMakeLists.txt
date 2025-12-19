# Game/CMakeLists.txt

project(Game)

add_executable(Game
        src/main.cpp
        src/Game.cpp
        src/states/MainMenuState.cpp
        src/states/GameplayState.cpp
)

target_include_directories(Game
        PRIVATE
            include
)

target_link_libraries(Game
        PRIVATE Engine
)