#pragma once

#include "ResourceHandle.hpp"
#include "ResourceSlot.hpp"
#include <memory>
#include <stack>
#include <vector>

namespace Medae::Assets {

template<typename T, typename U> class ResourceManager
{
  public:
	ResourceManager<T>() = default;
	ResourceManager<T>(int initialCapacity);
	T add(const U& resource);
	void reset(const ResourceHandleBase& handle);
	std::shared_ptr<U> get(const ResourceHandleBase& handle);

  private:
	std::vector<ResourceSlot<U>> slots;
	std::stack<int> free;

	static_assert(std::is_base_of_v<ResourceHandleBase, T>, "T must derive from ResourceHandleBase");
};
} // namespace Medae::Assets