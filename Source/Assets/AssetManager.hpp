#pragma once

#include <cstdint>
#include <memory>
#include <stack>
#include <unordered_map>
#include <vector>

#include "Assets/Asset.hpp"
#include "Assets/AssetHandle.hpp"
#include "Utils/CPP.hpp"

namespace Medae::Assets {

template<typename TAsset> class AssetSlot
{
public:
	AssetSlot() noexcept = default;
	AssetSlot(TAsset&& resource) noexcept;

	void RemoveResource() noexcept;
	void SetResource(TAsset&& resource) noexcept;
	TAsset* GetResource() const noexcept;
	uint32_t GetGeneration() const noexcept;

private:
	uint32_t m_generation = 0;
	std::unique_ptr<TAsset> m_resource;
};

template<typename TAsset, typename THandle> class AssetManager
{
	static_assert(std::derived_from<TAsset, Asset>, "AssetManager: TAsset must derive from Asset");
	static_assert(Utils::is_instantiation_of_v<THandle, AssetHandle>, "AssetManager: THandle must be an AssetHandle");

public:
	bool contains(const std::string& id) noexcept;
	const THandle Get(const std::string& id) const noexcept;
	void PushResource(const std::string& id, TAsset&& resource) noexcept;
	void ResetResource(const THandle& handle) noexcept;
	TAsset& GetResource(const THandle& handle) noexcept;

private:
	std::unordered_map<std::string, uint32_t> m_idMap;
	std::stack<uint32_t> m_emptySlots{};
	std::vector<AssetSlot<TAsset>> m_slots{};
};

} // namespace Medae::Assets

#include "AssetManager.tpp"