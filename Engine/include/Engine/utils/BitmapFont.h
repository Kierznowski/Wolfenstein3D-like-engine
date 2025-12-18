#pragma once
#include <memory>
#include <string>
#include <unordered_map>

#include "Engine/Texture.h"
#include "../renderer/Renderer.h"


class BitmapFont {
public:
    BitmapFont(const std::string& path,
        int charW,
        int charH,
        const std::string& charset);

    void drawText(Renderer& renderer,
        int x, int y, const std::string& text) const;

private:
    std::unique_ptr<Texture> texture_;
    int charWidth;
    int charHeight;
    std::unordered_map<char, SDL_Rect> glyphs_;
};
