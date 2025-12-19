#pragma once

#include <vector>
#include <fstream>

class Map {
public:
    explicit Map();
    Map(const Map&) = default;

    int getWidth() const { return width_; }
    int getHeight() const { return height_; }

    int at(int x, int y) const;
    void load(const std::string& filename);

private:
    int width_{0};
    int height_{0};
    std::vector<int> tiles_;
};