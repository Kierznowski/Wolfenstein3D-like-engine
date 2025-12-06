#include "Engine/Map.h"
#include <sstream>

Map::Map() {
}

int Map::at(int x, int y) const {
    return tiles.at(y * width + x);
}

void Map::load(const std::string& filename) {
    clear();

    std::fstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file " + filename);
    }
    std::string line;

    bool getWidth = true;
    while (std::getline(file, line)) {
        height++;

        std::stringstream row(line);
        std::string tile;

        while (std::getline(row, tile, ',')) {
            tiles.push_back(std::stoi(tile));
            if (getWidth) width++;
        }
        getWidth = false;
    }
}

void Map::clear() {
    width = 0;
    height = 0;
    tiles.clear();
}
