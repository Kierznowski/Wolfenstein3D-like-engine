#pragma once
#include <vector>
#include <fstream>

class Map {
public:
    explicit Map();
    Map(const Map&) = default;
    Map& operator=(const Map&) = default;

    int getWidth() const { return width; }
    int getHeight() const { return height; }

    int at(int x, int y) const;

    void load(const std::string& filename);

private:
    int width{0};
    int height{0};
    std::vector<int> tiles;

    void clear();
};