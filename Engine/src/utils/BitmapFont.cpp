#include "Engine/utils/BitmapFont.h"

BitmapFont::BitmapFont(
    const std::string &path,
    const int charW,
    const int charH,
    const std::string &charset)
        : texture_(std::make_unique<Texture>(path)), charWidth(charW), charHeight(charH)
{
    const int cols = texture_->getWidth() / charWidth;

    for (int i = 0; i < charset.size(); i++) {
        const int x = (i % cols) * charWidth;
        const int y = (i / cols) * charHeight;

        glyphs_[charset[i]] = {x, y, charWidth, charHeight};
    }
}

void BitmapFont::drawText(Renderer& renderer, const int x, const int y, const std::string& text) const {
    int cursorX = x;

    for (char c : text) {
        auto it = glyphs_.find(c);
        if (it == glyphs_.end()) {
            cursorX += charWidth;
            continue;
        }

        renderer.drawTexturedRect(
            cursorX, y, charWidth, charHeight, texture_.get(), it->second);

        cursorX += charWidth;
    }
}
