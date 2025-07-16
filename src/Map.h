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

    void loadMap(const std::string& path);
    int getWidth() const;
    int getHeight() const;
    bool isWall(int x, int y) const;
    int getWallType(int x, int y) const;

private:
    int width;
    int height;
    std::vector<std::vector<int>> worldMap;
};



