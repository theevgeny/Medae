#pragma once

#include <Utils/Macros.hpp>
#include <cstdint>
#include <memory>

namespace Medae::Assets {
template<typename T> class ResourceSlot
{
  public:
	NODIS bool hasResource() const;
	NODIS uint32_t getGeneration() const;
	NODIS const std::shared_ptr<T> get() const;
	NODIS std::shared_ptr<T> get();
	void reset();
	ResourceSlot<T>() = default;
	ResourceSlot<T>& set(T&& resource);
	ResourceSlot<T>& set(const T& resource);
	ResourceSlot<T>& set(std::unique_ptr<T>&& resource);

	ResourceSlot(ResourceSlot&&) noexcept = default;
	ResourceSlot& operator=(ResourceSlot&&) noexcept = default;
	ResourceSlot(const ResourceSlot&) = delete;
	ResourceSlot& operator=(const ResourceSlot&) = delete;

  private:
	uint32_t generation = 1;
	std::unique_ptr<T> resource;
};

} // namespace Medae::Assets

#include "ResourceSlot.tpp"