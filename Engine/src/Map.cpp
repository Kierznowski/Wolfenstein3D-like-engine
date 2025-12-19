#include "Engine/Map.h"
#include <sstream>

Map::Map() {
}

int Map::at(const int x, const int y) const {
    return tiles_.at(y * width_ + x);
}

void Map::load(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file with map: " + filename);
    }

    std::vector<std::vector<int>> rows;
    std::string line;

    while (std::getline(file, line)) {
        std::stringstream row(line);
        std::string tile;
        std::vector<int> values;

        while (std::getline(row, tile, ',')) {
            values.emplace_back(std::stoi(tile));
        }

        if (width_ == 0) {
            width_ = static_cast<int>(values.size());
        }

        rows.emplace_back(std::move(values));
    }

    height_ = static_cast<int>(rows.size());
    tiles_.reserve(width_ * height_);

    for (int y = height_ - 1; y >= 0; --y) {
        for (int x = 0; x < width_; ++x) {
            tiles_.push_back(rows[y][x]);
        }
    }
}

