//
// Created by Ravi Lebgue on 11/20/25.
//

#include "Font.h"

#include <string>

#include "SDL3_ttf/SDL_ttf.h"

TTF_TextEngine* Font::engine = nullptr;

std::shared_ptr<Texture> Font::render(const std::string& text, float size) {
    TTF_Font* font = nullptr;
    if (fonts.contains(size)) {
        font = fonts[size];
    } else {
        font = TTF_OpenFont(path.c_str(), size);
    }
    //TTF_Text* drawText = TTF_CreateText(engine, font, text.c_str(), 0);
    //TTF_GPUAtlasDrawSequence* drawData = TTF_GetGPUTextDrawData(drawText);
    //TTF_DestroyText(drawText);


    return nullptr;
}

std::shared_ptr<Font> Font::make(const std::shared_ptr<Renderer>& renderer, const std::string& path) {
    return std::shared_ptr<Font>(new Font(renderer, path));
}

Font::Font(const std::shared_ptr<Renderer>& renderer, const std::string& path) {
    if (!engine) {
        engine = TTF_CreateGPUTextEngine(static_cast<SDL_GPUDevice*>(renderer->getInternal()));
    }
    this->path = path;
}
