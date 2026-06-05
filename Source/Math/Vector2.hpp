#pragma once

#include <Macros.hpp>

/** Dividing by a zero scalar produces a zero vector  */
struct Vector2 {
    double x, y;
    M_ND Vector2() noexcept;
    M_ND Vector2(double x) noexcept;
    M_ND Vector2(double x, double y) noexcept;

    Vector2 &add(double x) noexcept;
    M_ND Vector2 addc(double x) const noexcept;
    Vector2 &add(double x, double y) noexcept;
    M_ND Vector2 addc(double x, double y) const noexcept;
    M_ND Vector2 operator+(const Vector2 &vector2) const noexcept;
    Vector2 &substract(double x) noexcept;
    M_ND Vector2 substractc(double x) const noexcept;
    Vector2 &substract(double x, double y) noexcept;
    M_ND Vector2 substractc(double x, double y) const noexcept;
    M_ND Vector2 operator-(const Vector2 &vector2) const noexcept;
    Vector2 &multiply(double value) noexcept;
    M_ND Vector2 multiplyc(double value) const noexcept;
    M_ND Vector2 operator*(double value) const noexcept;
    Vector2 &devide(double value) noexcept;
    M_ND Vector2 devidec(double value) const noexcept;
    M_ND Vector2 operator/(double value) const noexcept;

    Vector2 &operator+=(const Vector2 &vector2) noexcept;
    Vector2 &operator-=(const Vector2 &vector2) noexcept;
    Vector2 &operator*=(double value) noexcept;
    Vector2 &operator/=(double value) noexcept;

    M_ND constexpr bool operator==(const Vector2 &vector2) const noexcept;
    M_ND constexpr bool operator!=(const Vector2 &vector2) const noexcept;

    M_ND double length() const noexcept;
    M_ND constexpr double squareLength() const noexcept;
    M_ND Vector2 normalizec() const noexcept;
    Vector2 &normalize() noexcept;
    constexpr Vector2 &lerp(const Vector2 vector2, float t) noexcept;
    M_ND constexpr Vector2 lerpc(const Vector2 vector2, float t) noexcept;

    static Vector2 ZERO;
    static Vector2 ONE;
    static Vector2 UP;
    static Vector2 DOWN;
    static Vector2 RIGHT;
    static Vector2 LEFT;
};