#ifndef POMEGRANATE_FONT_H
#define POMEGRANATE_FONT_H
#include <string>
#include <unordered_map>

#include "Pipeline.h"
#include "Renderer.h"
#include "Texture.h"


class Font {
public:
    std::shared_ptr<Texture> render(const std::string& text, float size);

    static std::shared_ptr<Font> make(const std::shared_ptr<Renderer>& renderer, const std::string& path);

private:
    Font(const std::shared_ptr<Renderer>& renderer, const std::string& path);

    std::string path;
    std::unordered_map<float, struct TTF_Font*> fonts = {};
    static struct TTF_TextEngine* engine;
    static std::shared_ptr<Pipeline> pipeline;
};


#endif //POMEGRANATE_FONT_H
