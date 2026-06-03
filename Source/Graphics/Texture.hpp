#pragma once

#include <Graphics/Color.hpp>
#include <cstdio>
#include <std_image.h>

class Texture final {
    Color *m_texture;
    int m_width, m_height;

    Texture &operator=(Texture &) = delete;
    Texture(Texture &) = delete;
    Texture(Texture &&) = delete;
    Texture &operator=(Texture &&) = delete;

  public:
    ~Texture();
    Texture(FILE file);
    Texture(int wigth, int height);
    Texture(int wigth, int height, Color baseColor);

    Color get(int x, int y);
    Color &at(int x, int y);
    void set(int x, int y, Color color);

    Color *getRawData();
};