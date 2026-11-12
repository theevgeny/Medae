#pragma once

#include "AssetManager.hpp"
#include "AssetTraits.hpp"
#include "Assets/AssetHandle.hpp"
#include <spdlog/spdlog.h>

using namespace Medae::Assets;

template<typename TAsset> AssetSlot<TAsset>::AssetSlot(TAsset&& resource) noexcept
{
	++m_generation;
	this->m_resource = std::make_unique<TAsset>(std::move(resource));
}

template<typename TAsset> void AssetSlot<TAsset>::RemoveResource() noexcept
{
	++m_generation;
	m_resource.reset();
}

template<typename TAsset> void AssetSlot<TAsset>::SetResource(TAsset&& resource) noexcept
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
void AssetManager<TAsset, THandle>::PushResource(const std::string& id, TAsset&& resource) noexcept
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

template<typename TAsset, typename THandle> bool AssetManager<TAsset, THandle>::contains(const std::string& id) noexcept
{
	return m_idMap.contains(id);
}
template<typename TAsset, typename THandle>
const THandle AssetManager<TAsset, THandle>::Get(const std::string& id) const noexcept
{
	auto it = m_idMap.find(id);
	if (it != m_idMap.end()) {
		const AssetSlot<TAsset>& slot = m_slots[it->second];
		return THandle(slot.GetGeneration(), it->second);
	} else {
		spdlog::warn("AssetManager::Get: could not find a resource (id '{}')", id);
		return THandle{0, 0};
	}
}

template<typename TAsset, typename THandle>
void AssetManager<TAsset, THandle>::ResetResource(const THandle& handle) noexcept
{
	AssetSlot<TAsset>& assetSlot = m_slots.at(handle.index);
	if (assetSlot.GetGeneration() == handle.generation) {
		assetSlot.RemoveResource();
		m_emptySlots.push(handle.index);
	}
}

template<typename TAsset, typename THandle>
TAsset& AssetManager<TAsset, THandle>::GetResource(const THandle& handle) noexcept
{
	if (handle.generation == 0) {
		spdlog::warn("AssetManager::Get: Handle has invalid index {}", handle.index);
		return getFallBackResource(AssetTraits<TAsset>::CreateFallback());
	}
	auto it = m_slots.find(handle.index);
	if (it != m_slots.end()) {
		const AssetSlot<TAsset>& assetSlot = it->second;
		if (assetSlot.GetGeneration() != handle.generation) {
			spdlog::warn("AssetManager::Get: Handle outdated {}", handle.index);
			return getFallBackResource(AssetTraits<TAsset>::CreateFallback());
		}
		assetSlot.GetResource();
	} else {
		spdlog::warn("AssetManager::Get: could not find a resource (index {})", handle.index);
		return getFallBackResource(AssetTraits<TAsset>::CreateFallback());
	}
}