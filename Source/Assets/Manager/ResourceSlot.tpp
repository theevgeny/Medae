#pragma once

#include "ResourceSlot.hpp"
#include <memory>

namespace Medae::Assets {

template<typename T> bool ResourceSlot<T>::hasResource() const
{
	return resource != nullptr;
}
template<typename T> uint32_t ResourceSlot<T>::getGeneration() const
{
	return generation;
}
template<typename T> const std::shared_ptr<T> ResourceSlot<T>::get() const
{
	return resource;
}
template<typename T> std::shared_ptr<T> ResourceSlot<T>::get()
{
	return resource;
}
template<typename T> void ResourceSlot<T>::reset()
{
	resource.reset();
	++generation;
}
template<typename T> ResourceSlot<T>& ResourceSlot<T>::set(T&& resource)
{
	this->resource = std::unique_ptr<T>(std::move(resource));
}
template<typename T> ResourceSlot<T>& ResourceSlot<T>::set(const T& resource)
{
	this->resource = std::unique_ptr<T>(resource);
}
template<typename T> ResourceSlot<T>& ResourceSlot<T>::set(std::unique_ptr<T>&& resource)
{
	this->resource = std::move(resource);
}
} // namespace Medae::Assets