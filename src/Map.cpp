#include "Map.h"
#include <print>

Map::Map() : width(0), height(0) {
}

Map::~Map() {
}

void Map::loadMap(const std::string& path) {
    width = 0;
    height = 0;
    worldMap.clear();

    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error opening map file " << path << std::endl;
        exit(EXIT_FAILURE);
    }
    std::string line;
    while (std::getline(file, line)) {
        std::vector<int> row;
        std::stringstream ss(line);
        std::string token;
        int tmpWidth{};

        while (std::getline(ss, token, ',')) {
            try {
                row.push_back(std::stoi(token));
                tmpWidth++;
            } catch (const std::exception &e) {
                std::cerr << "Error reading map file " << path << "at line " << line << std::endl;
                std::cerr << e.what() << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        width = std::max(this->width, tmpWidth);
        worldMap.push_back(row);
        height++;
    }
}

int Map::getWidth() const {
    return width;
}

int Map::getHeight() const {
    return height;
}

bool Map::isWall(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) return true;
    return worldMap[y][x] > 0;
}

int Map::getWallType(int x, int y) const {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        // return worldMap[y][x] - 1;
        return (worldMap[y][x] > 2) ? 0 : 1;
    }
    return -1;
}
