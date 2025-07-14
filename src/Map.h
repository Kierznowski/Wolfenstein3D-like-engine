#pragma once
#include <vector>

class Map {
public:
    Map();
    int getWidth() const;
    int getHeight() const;
    bool isWall(int x, int y) const;

private:
    static constexpr int width = 24;
    static constexpr int height = 24;
    std::vector<std::vector<int>> worldMap;
};



