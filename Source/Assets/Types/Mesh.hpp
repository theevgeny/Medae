#pragma once

#include "Assets/AssetHandle.hpp"
#include "Common/interface/BasicMath.hpp"
#include <vector>

namespace Medae::Assets {
struct Mesh
{
	std::vector<Diligent::float3> vertices;
	std::vector<Diligent::uint3> indices;
	std::vector<Diligent::float2> uvs;
	MaterialHandle material;
};
} // namespace Medae::Assets