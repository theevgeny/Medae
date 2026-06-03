#include <Math/Vector3.hpp>
#include <cmath>

M_ND Vector3::Vector3() noexcept : x(0), y(0), z(0) {}
M_ND Vector3::Vector3(double x) noexcept : x(x), y(0), z(0) {}
M_ND Vector3::Vector3(double x, double y) noexcept : x(x), y(y), z(0) {}
M_ND Vector3::Vector3(double x, double y, double z) noexcept
    : x(x), y(y), z(z) {}

Vector3 &Vector3::add(double x) noexcept {
    this->x += x;
    return *this;
}
M_ND Vector3 Vector3::addc(double x) const noexcept {
    return Vector3{this->x + x, y, z};
}
Vector3 &Vector3::add(double x, double y) noexcept {
    this->x += x;
    this->y += y;
    return *this;
}
M_ND Vector3 Vector3::addc(double x, double y) const noexcept {
    return Vector3{this->x + x, this->y + y, z};
}
Vector3 &Vector3::add(double x, double y, double z) noexcept {
    this->x += x;
    this->y += y;
    this->z += z;
    return *this;
}
M_ND Vector3 Vector3::addc(double x, double y, double z) const noexcept {
    return Vector3{this->x + x, this->y + y, this->z + z};
}
M_ND Vector3 Vector3::operator+(const Vector3 &vector3) const noexcept {
    return Vector3{x + vector3.x, y + vector3.y, z + vector3.z};
}
Vector3 &Vector3::substract(double x) noexcept {
    this->x -= x;
    return *this;
}
M_ND Vector3 Vector3::substractc(double x) const noexcept {
    return Vector3{this->x - x, y, z};
}
Vector3 &Vector3::substract(double x, double y) noexcept {
    this->x -= x;
    this->y -= y;
    return *this;
}
M_ND Vector3 Vector3::substractc(double x, double y) const noexcept {
    return Vector3{this->x - x, this->y - y, z};
}
Vector3 &Vector3::substract(double x, double y, double z) noexcept {
    this->x -= x;
    this->y -= y;
    this->z -= z;
    return *this;
}
M_ND Vector3 Vector3::substractc(double x, double y, double z) const noexcept {
    return Vector3{this->x - x, this->y - y, this->z - z};
}
M_ND Vector3 Vector3::operator-(const Vector3 &vector3) const noexcept {
    return Vector3{x - vector3.x, y - vector3.y, z - vector3.z};
}
Vector3 &Vector3::multiply(double value) noexcept {
    x *= value;
    y *= value;
    z *= value;
    return *this;
}
M_ND Vector3 Vector3::multiplyc(double value) const noexcept {
    return Vector3{x * value, y * value, z * value};
}
M_ND Vector3 Vector3::operator*(double value) const noexcept {
    return Vector3{x * value, y * value, z * value};
}
Vector3 &Vector3::devide(double value) noexcept {
    if (!value) {
        x = 0;
        y = 0;
        z = 0;
    } else {
        x /= value;
        y /= value;
        z /= value;
    }
    return *this;
}
M_ND Vector3 Vector3::devidec(double value) const noexcept {
    if (!value)
        return ZERO;
    return Vector3{x / value, y / value, z / value};
}
M_ND Vector3 Vector3::operator/(double value) const noexcept {
    if (!value)
        return ZERO;
    return Vector3{x / value, y / value, z / value};
}

Vector3 &Vector3::operator+=(const Vector3 &vector3) noexcept {
    x += vector3.x;
    y += vector3.y;
    z += vector3.z;
    return *this;
}
Vector3 &Vector3::operator-=(const Vector3 &vector3) noexcept {
    x -= vector3.x;
    y -= vector3.y;
    z -= vector3.z;
    return *this;
}
Vector3 &Vector3::operator*=(double value) noexcept {
    x *= value;
    y *= value;
    z *= value;
    return *this;
}
Vector3 &Vector3::operator/=(double value) noexcept {
    if (!value) {
        x = 0;
        y = 0;
        z = 0;
    } else {
        x /= value;
        y /= value;
        z /= value;
    }
    return *this;
}

M_ND constexpr bool Vector3::operator==(const Vector3 &vector3) const noexcept {
    return x == vector3.x && y == vector3.y && z == vector3.z;
}
M_ND constexpr bool Vector3::operator!=(const Vector3 &vector3) const noexcept {
    return !(*this == vector3);
}

M_ND double Vector3::length() const noexcept {
    return std::sqrt(x * x + y * y + z * z);
}
M_ND constexpr double Vector3::squareLength() const noexcept {
    return x * x + y * y + z * z;
}
M_ND Vector3 Vector3::normalizec() const noexcept {
    double len = length();
    if (len == 0)
        return ZERO;
    return Vector3{x / len, y / len, z / len};
}
Vector3 &Vector3::normalize() noexcept {
    double len = length();
    if (len == 0)
        return ZERO;
    x /= len;
    y /= len;
    z /= len;
    return *this;
}
constexpr Vector3 &Vector3::lerp(const Vector3 vector3, float t) noexcept {
    if (t >= 1) {
        x = vector3.x;
        y = vector3.y;
        z = vector3.z;
    } else if (t > 0) {
        x = x * (1 - t) + vector3.x * t;
        y = y * (1 - t) + vector3.y * t;
        z = z * (1 - t) + vector3.z * t;
    }
    return *this;
}
M_ND constexpr Vector3 Vector3::lerpc(const Vector3 vector3, float t) noexcept {
    if (t >= 1)
        return vector3;
    if (t <= 0)
        return *this;
    return *this * (1 - t) + vector3 * t;
}

Vector3 Vector3::ZERO{0, 0, 0};
Vector3 Vector3::ONE{1, 1, 1};
Vector3 Vector3::UP{0, 1, 0};
Vector3 Vector3::DOWN{0, -1, 0};
Vector3 Vector3::RIGHT{1, 0, 0};
Vector3 Vector3::LEFT{-1, 0, 0};
Vector3 Vector3::FORWARD{0, 0, 1};
Vector3 Vector3::BACKWARD{0, 0, -1};