#pragma once

#include <cstdint>

#include <boost/container/small_vector.hpp>

namespace Medae::UI {

enum class LType : uint8_t
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
	bool isPositive = true;
	LType type = LType::PX;
	uint16_t value = 0;
};

struct MonoOperation
{
};
struct BiOperation
{
};
struct TriOperation
{
};

struct Axis
{
	uint8_t args;
	union
	{
	}
}

} // namespace Medae::UI