#pragma once

#include <cstdint>

#include <boost/container/small_vector.hpp>

namespace Medae::UI {

enum Anchor : uint8_t
{
	TOP_LEFT,
	TOP_MIDDLE,
	TOP_RIGHT,
	LEFT_MIDDLE,
	CENTER,
	RIGHT_MIDDLE,
	BOTTOM_LEFT,
	BOTTOM_MIDDLE,
	BOTTOM_RIGHT
};

enum LType : uint8_t
{
	PX,
	PERCANT_PARENT_X,
	PERCANT_PARENT_Y,
	PERCANT_THIS_X,
	PERCANT_THIS_Y,
	PERCANT_CHILD_X,
	PERCANT_CHILD_Y,
	PERCANT_BIGGEST_CHILD_X,
	PERCANT_BIGGEST_CHILD_Y
};

struct alignas(4) LTypeOperation
{
	uint16_t type = LType::PX;
	int16_t value = 0;
};

struct Axis
{
	boost::container::small_vector<LTypeOperation, 2> args;
};

enum AnimType : uint8_t
{
	NONE,
	LINEAR,
	EASE_IN,
	EASE_OUT,
	EASE_IN_OUT
};

struct KeyFrame
{
	Axis size, offset;
	uint16_t alpha = 255;
	uint16_t animation = AnimType::NONE;
	uint16_t time = 0;
};

struct ElementData
{
	boost::container::small_vector<KeyFrame, 1> args;
};

} // namespace Medae::UI