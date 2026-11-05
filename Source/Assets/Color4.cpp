#include "Color4.hpp"

using namespace Medae::Assets;

Color4::Color4(uint32_t rgba)
{
	this->rgba = rgba;
}

Color4::Color4(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

Color4 Color4::operator*(const Color4& other)
{
	return Color4((r + other.r) / 2, (g + other.g) / 2, (b + other.b) / 2, (a + other.a) / 2);
}

Color4& Color4::operator*=(const Color4& other)
{
	r = (r + other.r) / 2;
	g = (g + other.g) / 2;
	b = (b + other.b) / 2;
	a = (a + other.a) / 2;
	return *this;
}