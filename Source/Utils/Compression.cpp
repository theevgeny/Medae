#include "Compression.hpp"

#include <spdlog/spdlog.h>
#include <zstd.h>

using namespace Medae::Utils;

void Medae::Utils::compress(Data& data)
{
	data.size = ZSTD_compress(data.content, data.size, data.content, data.size, 3);

	if (ZSTD_isError(data.size) != 0) {
		spdlog::error("Error in compression: {}", ZSTD_getErrorName(data.size));
	}
}

void Medae::Utils::decompress(Data& data)
{
	data.size = ZSTD_decompress(data.content, data.size, data.content, data.size);

	if (ZSTD_isError(data.size) != 0) {
		spdlog::error("Error in compression: {}", ZSTD_getErrorName(data.size));
	}
}
