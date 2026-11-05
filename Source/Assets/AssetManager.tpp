#pragma once

#include "AssetManager.hpp"
#include "AssetTraits.hpp"
#include <X11/X.h>

namespace Medae::Assets {

template<typename TAsset> AssetSlot<TAsset>::AssetSlot(TAsset&& resource)
{
	++m_generation;
	this->m_resource = std::make_unique<TAsset>(std::move(resource));
}

template<typename TAsset> void AssetSlot<TAsset>::RemoveResource()
{
	++m_generation;
	m_resource.reset();
}

template<typename TAsset> void AssetSlot<TAsset>::SetResource(TAsset&& resource)
{
	++m_generation;
	this->m_resource = std::make_unique<TAsset>(std::move(resource));
}

template<typename TAsset> TAsset* AssetSlot<TAsset>::GetResource() const noexcept
{
	return m_resource.get();
}

template<typename TAsset> uint32_t AssetSlot<TAsset>::GetGeneration() const noexcept
{
	return m_generation;
}

template<typename TAsset, typename THandle>
void AssetManager<TAsset, THandle>::PushResource(const std::string& id, TAsset&& resource)
{
	int index;
	if (!m_emptySlots.empty()) {
		index = m_emptySlots.top();
		m_slots.at(index).SetResource(std::move(resource));
		m_emptySlots.pop();
	} else {
		index = m_slots.size();
		m_slots.push_back(std::move(resource));
	}
	m_idMap.emplace(id, index);
}

template<typename TAsset, typename THandle> bool AssetManager<TAsset, THandle>::contains(const std::string& id)
{
	return m_idMap.contains(id);
}
template<typename TAsset, typename THandle> const THandle& AssetManager<TAsset, THandle>::get(const std::string& id)
{
	return m_idMap[id];
}

template<typename TAsset, typename THandle> void AssetManager<TAsset, THandle>::ResetResource(const THandle& handle)
{
	AssetSlot<TAsset>& assetSlot = m_slots.at(handle.index);
	if (assetSlot.GetGeneration() == handle.generation) {
		assetSlot.RemoveResource();
		m_emptySlots.push(handle.index);
	}
}

template<typename TAsset, typename THandle> TAsset& AssetManager<TAsset, THandle>::GetResource(const THandle& handle)
{
	const AssetSlot<TAsset>& assetSlot = m_slots.at(handle.index);
	return assetSlot.GetGeneration() == handle.generation
		? *assetSlot.GetResource()
		: *static_cast<TAsset*>(getFallBackResource(AssetTraits<TAsset>::type));
}

} // namespace Medae::Assets