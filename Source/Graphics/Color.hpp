#pragma once

#include <Macros.hpp>

struct Color final {
    float r, g, b, a;
    M_ND Color() noexcept;
    M_ND Color(float red) noexcept;
    M_ND Color(float red, float green) noexcept;
    M_ND Color(float red, float green, float blue) noexcept;
    M_ND Color(float red, float green, float blue, float alpha) noexcept;
    constexpr Color &blend(const Color &color, float t) noexcept;
    M_ND constexpr Color blendc(const Color &color, float t) const noexcept;

    static Color WHITE;
    static Color BLACK;
    static Color RED;
    static Color GREEN;
    static Color BLUE;
};