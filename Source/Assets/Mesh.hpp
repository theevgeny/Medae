#pragma once

#include "Assets/Manager/ResourceHandle.hpp"
#include "Jolt/Jolt.h"
#include "Jolt/Math/Vec3.h"
#include <memory>
#include <vector>

namespace Medae::Assets {
class Mesh
{
  public:
	Mesh(float* verteсes, int vcount, int* indices, int icount, JPH::Vec3 location);
	JPH::Vec3 getLocation();
	float* getRawVerteces();
	MaterialHandle& getMaterial();
	int* getRawIndices();
	void addMesh(std::shared_ptr<Mesh> child);

  private:
	JPH::Vec3 location;
	MaterialHandle material;
	std::vector<float> verteсes;
	std::vector<int> indices;
	std::vector<std::shared_ptr<Mesh>> childs;
};
} // namespace Medae::Assets