#pragma once

#include "Common/interface/BasicMath.hpp"
#include <vector>

namespace Medae::Assets {
struct MeshNode
{
	uint32_t index = 0;
	uint32_t parentIndex = -1;
	Diligent::float4x4 localTransform;
	std::vector<uint32_t> meshIndices;
};
} // namespace Medae::Assets