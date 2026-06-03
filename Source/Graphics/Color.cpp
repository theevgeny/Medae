#include <Graphics/Color.hpp>

M_ND Color::Color()noexcept: r(0),g(0),b(0),a(1) {};
M_ND Color::Color(float r) noexcept: r(r),g(0),b(0),a(1) {};
M_ND Color::Color(float r, float g) noexcept: r(r),g(g),b(0),a(1) {};
M_ND Color::Color(float r, float g, float b) noexcept: r(r),g(g),b(b),a(1) {};
M_ND Color::Color(float r, float g, float b, float a) noexcept: r(r),g(g),b(b),a(a) {};
constexpr Color &Color::blend(const Color &color, float t) noexcept {
    if (t >= 1) {
        r = color.r;
        g = color.g;
        b = color.b;
        a = color.a;
    } else if (t > 0) {
        r = r * (1 - t) + color.r * t;
        g = g * (1 - t) + color.g * t;
        b = b * (1 - t) + color.b * t;
        a = a * (1 - t) + color.a * t;
    }
    return *this;
}
M_ND constexpr Color Color::blendc(const Color &color, float t) const noexcept {
    if (t >= 1)
        return color;
    if (t <= 0)
        return *this;
    return {r * (1 - t) + color.r * t, g * (1 - t) + color.g * t,
            b * (1 - t) + color.b * t, a * (1 - t) + color.a * t};
}

Color Color::WHITE;
Color Color::BLACK;
Color Color::RED;
Color Color::GREEN;
Color Color::BLUE;