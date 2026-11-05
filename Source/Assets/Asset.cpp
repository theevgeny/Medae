#include "Asset.hpp"

using namespace Medae::Assets;

AssetState Asset::getState()
{
	return state;
}
void Asset::setState(AssetState state)
{
	this->state = state;
}