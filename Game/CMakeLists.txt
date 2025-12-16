# Game/CMakeLists.txt

project(Game)

add_executable(Game
        src/main.cpp
        src/Game.cpp
        src/MainMenuState.cpp
        src/GameplayState.cpp
        include/Game/entities/HealthEntity.h
)

target_include_directories(Game
        PRIVATE
            include
)

target_link_libraries(Game
        PRIVATE Engine
)