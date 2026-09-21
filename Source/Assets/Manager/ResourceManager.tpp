#pragma once

#include "ResourceManager.hpp"
#include "ResourceSlot.hpp"
#include <memory>

namespace Medae::Assets {

template<typename T, typename U> ResourceManager<T, U>::ResourceManager(int initialCapacity) : ResourceManager()
{
	slots.resize(initialCapacity);
	for (int i; i < initialCapacity; ++i) {
		free.push(i);
	}
}
template<typename T, typename U> T ResourceManager<T, U>::add(const U& resource)
{
	if (free.empty()) {
		slots.push_back(ResourceSlot<U>(resource));
	} else {
		static_cast<ResourceSlot<U>>(slots[free.top()]).set(resource);
		free.pop();
	}
}
template<typename T, typename U> void ResourceManager<T, U>::reset(const ResourceHandleBase& handle)
{
	if (!handle.isValid())
		return;
	static_cast<ResourceSlot<U>>(slots[handle.index]).reset(); // IDE not see type
	free.push(handle.index);
}
template<typename T, typename U> std::shared_ptr<U> ResourceManager<T, U>::get(const ResourceHandleBase& handle)
{
	if (!handle.isValid())
		return std::unique_ptr<U>();
	return static_cast<ResourceSlot<U>>(slots[handle.index]).get(); // IDE not see type
}

} // namespace Medae::Assets