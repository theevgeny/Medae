#pragma once

#include <Macros.hpp>

/** Деление на нулевой скаляр даёт нулевой вектор  */
struct Vector3 {
    double x, y, z;
    M_ND Vector3() noexcept;
    M_ND Vector3(double x) noexcept;
    M_ND Vector3(double x, double y) noexcept;
    M_ND Vector3(double x, double y, double z) noexcept;

    Vector3 &add(double x) noexcept;
    M_ND Vector3 addc(double x) const noexcept;
    Vector3 &add(double x, double y) noexcept;
    M_ND Vector3 addc(double x, double y) const noexcept;
    Vector3 &add(double x, double y, double z) noexcept;
    M_ND Vector3 addc(double x, double y, double z) const noexcept;
    M_ND Vector3 operator+(const Vector3 &Vector3) const noexcept;
    Vector3 &substract(double x) noexcept;
    M_ND Vector3 substractc(double x) const noexcept;
    Vector3 &substract(double x, double y) noexcept;
    M_ND Vector3 substractc(double x, double y) const noexcept;
    Vector3 &substract(double x, double y, double z) noexcept;
    M_ND Vector3 substractc(double x, double y, double z) const noexcept;
    M_ND Vector3 operator-(const Vector3 &Vector3) const noexcept;
    Vector3 &multiply(double value) noexcept;
    M_ND Vector3 multiplyc(double value) const noexcept;
    M_ND Vector3 operator*(double value) const noexcept;
    Vector3 &devide(double value) noexcept;
    M_ND Vector3 devidec(double value) const noexcept;
    M_ND Vector3 operator/(double value) const noexcept;

    Vector3 &operator+=(const Vector3 &vector3) noexcept;
    Vector3 &operator-=(const Vector3 &vector3) noexcept;
    Vector3 &operator*=(double value) noexcept;
    Vector3 &operator/=(double value) noexcept;

    M_ND constexpr bool operator==(const Vector3 &vector3) const noexcept;
    M_ND constexpr bool operator!=(const Vector3 &vector3) const noexcept;

    M_ND double length() const noexcept;
    M_ND constexpr double squareLength() const noexcept;
    M_ND Vector3 normalizec() const noexcept;
    Vector3 &normalize() noexcept;
    constexpr Vector3 &lerp(const Vector3 vector3, float t) noexcept;
    M_ND constexpr Vector3 lerpc(const Vector3 vector3, float t) noexcept;

    static Vector3 ZERO;
    static Vector3 ONE;
    static Vector3 UP;
    static Vector3 DOWN;
    static Vector3 RIGHT;
    static Vector3 LEFT;
    static Vector3 FORWARD;
    static Vector3 BACKWARD;
};