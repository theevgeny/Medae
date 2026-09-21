#include "Mesh.hpp"

using namespace Medae::Assets;

Mesh::Mesh(float* verteсes, int vcount, int* indices, int icount, JPH::Vec3 location) : location(location)
{
	for (int i = 0; i < vcount; ++i) {
		this->verteсes.push_back(verteсes[i]);
	}
	for (int i = 0; i < icount; ++i) {
		this->indices.push_back(indices[i]);
	}
}
JPH::Vec3 Mesh::getLocation()
{
	return location;
}
float* Mesh::getRawVerteces()
{
	return verteсes.data();
}
int* Mesh::getRawIndices()
{
	return indices.data();
}
void Mesh::addMesh(std::shared_ptr<Mesh> child)
{
	childs.push_back(child);
}