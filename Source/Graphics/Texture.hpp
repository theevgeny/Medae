#pragma once

#include <Graphics/Color.hpp>
#include <cstdio>
#include <std_image.h>
#include <string>
#include <vector>

class Texture final {
    std::vector<Color> m_data;
    int m_width, m_height;

  public:
    ~Texture();
    Texture(const std::string& path);
    Texture(int wigth, int height);
    Texture(int wigth, int height, Color baseColor);

    Color get(int x, int y);
    Color &at(int x, int y);
    void set(int x, int y, Color color);

    Color *getRawData();
};