
# 2.5D Raycasting Engine (C++/SDL2)

A simple Wolfenstein 3D–style 2.5D engine written from scratch in modern C++ using SDL2. The project is part of my personal learning journey to understand game engine internals, graphics programming, and low-level systems.

## Features
- Pure CPU-based renderer
- Raycasting using DDA algorithm (like the original Wolfenstein 3D)
- Textured walls, floor, ceiling and sprites rendering
- Fixed timestep game loop with frame timing and FPS counter

Built as a foundation for future experiments:
- networking
- lighting and shading experiments


## Project Structure
    /root
    └── /Engine               
        └── CmakeLists.txt
        └── /include/Engine       
            ├── Assets.h
            ├── Engine.h           
            ├── ...
            └── Texture.h        
        └── /src                 
            ├── Assets.cpp
            ├── Engine.cpp           
            ├── ...
            └── Texture.cpp   
    └── /Game
        └── CmakeLists.txt
        └── /include/Game
            ├── Game.h                 
            ├── GameState.h
            ├── ...                
            └── MainMenuState.h
         └── /src                 
            ├── main.cpp
            ├── Game.cpp           
            ├── ...
            └── MainMenuState.cpp
    └── CmakeLists.txt
    └── /Resources
        ├── /fonts   
        ├── /maps
        ├── ...
        └── /textures
        

## Screenshots

For now it looks like this:
</br>
</br>
<img src="./wolfengine1.png" />
<img src="./wolfengine2.png" />


## Future Roadmap

- Enemies, pickups
- Basic collision detection (for now only with walls)
- mouse rotation (for now W, A, S, D)
- Dynamic lighting / shading
- Map editor or procedural maps
- Multiplayer experiments (UDP / TCP)
- ECS pattern


## Credits & Inspiration
- John Carmack – you know him
- Lode Vandevenne – legendary raycasting tutorial. I got whole DDA concept from this source.
- Fabien Sanglard – his blog and "Black books"
