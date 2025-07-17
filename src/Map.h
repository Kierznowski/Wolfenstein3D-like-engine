#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

class Map {
public:
    Map();
    ~Map();

    void loadMap(const int worldIndex);
    int getWidth() const;
    int getHeight() const;
    bool isWall(int x, int y) const;
    int getWallType(int x, int y) const;
    int getFloorType(int x, int y) const;
    int getCeilingType(int x, int y) const;

private:
    int width;
    int height;

    std::vector<std::string> wallsMaps = {"./../assets/maps/world1/sample_map_walls.txt"};
    std::vector<std::string> floorMaps = {"./../assets/maps/world1/sample_map_floor.txt"};
    std::vector<std::string> ceilingMaps = {"./../assets/maps/world1/sample_map_ceiling.txt"};
    std::vector<std::vector<int>> wallsMap;
    std::vector<std::vector<int>> floorMap;
    std::vector<std::vector<int>> ceilingMap;

    void loadWallsMap(const std::string& path);
    void loadFloorMap(const std::string& path);
    void loadCeilingMap(const std::string& path);
};



