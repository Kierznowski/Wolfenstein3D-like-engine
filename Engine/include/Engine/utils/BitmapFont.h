#pragma once

#include "Engine/Texture.h"
#include "../renderer/Renderer.h"

#include <memory>
#include <string>
#include <unordered_map>

class BitmapFont {
public:
    BitmapFont(const std::string& path,
        int charW,
        int charH,
        const std::string& charset);

    void drawText(Renderer& renderer, int x, int y, const std::string& text) const;

private:
    std::unique_ptr<Texture> texture_;
    int charWidth_;
    int charHeight_;
    std::unordered_map<char, SDL_Rect> glyphs_;
};
