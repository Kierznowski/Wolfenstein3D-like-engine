#include "Map.h"
#include <print>

Map::Map() : width(0), height(0) {
}

Map::~Map() {
}

void Map::loadMap(const int worldIndex) {
    loadWallsMap(wallsMaps[worldIndex]);
    loadFloorMap(floorMaps[worldIndex]);
    loadCeilingMap(ceilingMaps[worldIndex]);
}

void Map::loadWallsMap(const std::string& path) {
    // Clear loaded walls map
    width = 0;
    height = 0;
    wallsMap.clear();
    // Load new walls
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
        wallsMap.push_back(row);
        height++;
    }
}

void Map::loadFloorMap(const std::string& path) {
    // Clear loaded walls map
    width = 0;
    height = 0;
    floorMap.clear();
    // Load new walls
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
        floorMap.push_back(row);
        height++;
    }
}

void Map::loadCeilingMap(const std::string& path) {
    // Clear loaded walls map
    width = 0;
    height = 0;
    ceilingMap.clear();
    // Load new walls
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
        ceilingMap.push_back(row);
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
    return wallsMap[y][x] > 0;
}

int Map::getWallType(int x, int y) const {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        // return worldMap[y][x] - 1;
        return (wallsMap[y][x] > 2) ? 0 : 1;
    }
    return -1;
}

int Map::getFloorType(int x, int y) const {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        return floorMap[y][x];
    }
    return 1;
}

int Map::getCeilingType(int x, int y) const {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        return ceilingMap[y][x];
    }
    return 1;
}
