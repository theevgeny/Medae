#include <Math/Vector2.hpp>
#include <cmath>

M_ND Vector2::Vector2() noexcept : x(0), y(0) {}
M_ND Vector2::Vector2(double x) noexcept : x(x), y(0) {}
M_ND Vector2::Vector2(double x, double y) noexcept : x(x), y(y) {}

Vector2 &Vector2::add(double x) noexcept {
    this->x += x;
    return *this;
}
M_ND Vector2 Vector2::addc(double x) const noexcept {
    return Vector2{this->x + x, y};
}
Vector2 &Vector2::add(double x, double y) noexcept {
    this->x += x;
    this->y += y;
    return *this;
}
M_ND Vector2 Vector2::addc(double x, double y) const noexcept {
    return Vector2{this->x + x, this->y + y};
}
M_ND Vector2 Vector2::operator+(const Vector2 &vector2) const noexcept {
    return Vector2{x + vector2.x, y + vector2.y};
}
Vector2 &Vector2::substract(double x) noexcept {
    this->x -= x;
    return *this;
}
M_ND Vector2 Vector2::substractc(double x) const noexcept {
    return Vector2{this->x - x, y};
}
Vector2 &Vector2::substract(double x, double y) noexcept {
    this->x -= x;
    this->y -= y;
    return *this;
}
M_ND Vector2 Vector2::substractc(double x, double y) const noexcept {
    return Vector2{this->x - x, this->y - y};
}
M_ND Vector2 Vector2::operator-(const Vector2 &vector2) const noexcept {
    return Vector2{x - vector2.x, y - vector2.y};
}
Vector2 &Vector2::multiply(double value) noexcept {
    x *= value;
    y *= value;
    return *this;
}
M_ND Vector2 Vector2::multiplyc(double value) const noexcept {
    return Vector2{x * value, y * value};
}
M_ND Vector2 Vector2::operator*(double value) const noexcept {
    return Vector2{x * value, y * value};
}
Vector2 &Vector2::devide(double value) noexcept {
    if (!value) {
        x = 0;
        y = 0;
    } else {
        x /= value;
        y /= value;
    }
    return *this;
}
M_ND Vector2 Vector2::devidec(double value) const noexcept {
    if (!value)
        return ZERO;
    return Vector2{x / value, y / value};
}
M_ND Vector2 Vector2::operator/(double value) const noexcept {
    if (!value)
        return ZERO;
    return Vector2{x / value, y / value};
}

Vector2 &Vector2::operator+=(const Vector2 &vector2) noexcept {
    x += vector2.x;
    y += vector2.y;
    return *this;
}
Vector2 &Vector2::operator-=(const Vector2 &vector2) noexcept {
    x -= vector2.x;
    y -= vector2.y;
    return *this;
}
Vector2 &Vector2::operator*=(double value) noexcept {
    x *= value;
    y *= value;
    return *this;
}
Vector2 &Vector2::operator/=(double value) noexcept {
    if (!value) {
        x = 0;
        y = 0;
    } else {
        x /= value;
        y /= value;
    }
    return *this;
}

M_ND constexpr bool Vector2::operator==(const Vector2 &vector2) const noexcept {
    return x == vector2.x && y == vector2.y;
}
M_ND constexpr bool Vector2::operator!=(const Vector2 &vector2) const noexcept {
    return !(*this == vector2);
}

M_ND double Vector2::length() const noexcept {
    return std::sqrt(x * x + y * y);
}
M_ND constexpr double Vector2::squareLength() const noexcept {
    return x * x + y * y;
}
M_ND Vector2 Vector2::normalizec() const noexcept {
    double len = length();
    if (len == 0)
        return ZERO;
    return Vector2{x / len, y / len};
}
Vector2 &Vector2::normalize() noexcept {
    double len = length();
    if (len == 0)
        return ZERO;
    x /= len;
    y /= len;
    return *this;
}
constexpr Vector2 &Vector2::lerp(const Vector2 vector2, float t) noexcept {
    if (t >= 1) {
        x = vector2.x;
        y = vector2.y;
    } else if (t > 0) {
        x = x * (1 - t) + vector2.x * t;
        y = y * (1 - t) + vector2.y * t;
    }
    return *this;
}
M_ND constexpr Vector2 Vector2::lerpc(const Vector2 vector2, float t) noexcept {
    if (t >= 1)
        return vector2;
    if (t <= 0)
        return *this;
    return *this * (1 - t) + vector2 * t;
}

Vector2 Vector2::ZERO{0, 0};
Vector2 Vector2::ONE{1, 1};
Vector2 Vector2::UP{0, 1};
Vector2 Vector2::DOWN{0, -1};
Vector2 Vector2::RIGHT{1, 0};
Vector2 Vector2::LEFT{-1, 0};
