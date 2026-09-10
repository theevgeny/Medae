#include "Compression.hpp"

#include <spdlog/spdlog.h>
#include <zstd.h>

using namespace Medae::Utils;

void Medae::Utils::compress(Data& data)
{
	spdlog::debug("Allocation of {} bytes for compression", ZSTD_compressBound(data.size));
	auto* buffer = new uint8_t[ZSTD_compressBound(data.size)];
	size_t newSize = ZSTD_compress(buffer, ZSTD_compressBound(data.size), data.content, data.size, 3);
	data.size = newSize;
	delete[] data.content;
	data.content = buffer;

	if (ZSTD_isError(data.size) != 0) {
		spdlog::error("Error in compression: {}", ZSTD_getErrorName(data.size));
	}
}

void Medae::Utils::decompress(Data& data)
{
	uint16_t size = ZSTD_getFrameContentSize(data.content, data.size);
	spdlog::debug("Allocation of {} bytes for decompression", size);
	auto* buffer = new uint8_t[size];
	size_t newSize = ZSTD_decompress(buffer, size, data.content, data.size);
	delete[] data.content;
	data.content = buffer;

	if (ZSTD_isError(newSize) != 0) {
		spdlog::error("Error in decompression: {}", ZSTD_getErrorName(newSize));
	}

	data.size = newSize;
}
