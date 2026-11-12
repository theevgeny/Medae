#pragma once

#include <cstddef>
#include <cstdint>
#include <fstream>
#include <string>
#include <type_traits>
#include <vector>

namespace Medae::Assets {

class BinaryFileHandler
{
public:
	BinaryFileHandler(std::fstream&& file, const std::string& path) noexcept;

	bool IsValid() const noexcept;
	void ToStart() noexcept;
	std::size_t GetReadPos() const noexcept;

	std::uint8_t GetByte() noexcept;
	std::vector<std::uint8_t> GetBytes(std::size_t size) noexcept;
	std::vector<std::uint8_t> GetBytesToEnd() noexcept;
	std::vector<std::uint8_t> GetAllBytes() noexcept;
	template<typename TDataType> TDataType GetType() noexcept
	{
		static_assert(std::is_trivially_copyable_v<TDataType>, "GetType<T> requires a trivially copyable type");

		TDataType value{};
		ReadRaw(&value, sizeof(TDataType));
		return value;
	}

	const std::string& GetPath() const noexcept;

	std::size_t GetSize() const noexcept;

private:
	void ReadRaw(void* dst, std::size_t size) noexcept;

	std::fstream m_file;
	std::string m_path;
	std::size_t m_readPos = 0;
	std::size_t m_size = 0;
	bool m_valid = false;
};

} // namespace Medae::Assets