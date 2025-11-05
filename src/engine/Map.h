#ifndef MAP_H
#define MAP_H
#include <vector>
#include <cstddef>


class Map {
public:
    explicit Map(int w = 0, int h = 0) : width(w), height(h), tiles(w * h, 0) {}
    Map(const Map&) = default;
    Map& operator=(const Map&) = default;

    int getWidth() const { return width; }
    int getHeight() const { return height; }

    int at(int x, int y) const {
        return tiles.at(y * width + x);
    }

    template<size_t W, size_t H>
    void copyFrom(const int (&arr)[W][H]) {
        width = W;
        height = H;
        tiles.assign(W * H, 0);
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                tiles[y * width + x] = arr[y][x];
            }
        }
    }

private:
    int width{0};
    int height{0};
    std::vector<int> tiles;
};


#endif // MAP_H