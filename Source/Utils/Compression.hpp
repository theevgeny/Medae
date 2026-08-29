#pragma once

#include <cstdint>

namespace Medae::Utils {

struct Data
{
	uint8_t* content = nullptr;
	uint16_t size = 0;
};

void compress(Data&);
void decompress(Data&);

} // namespace Medae::Utils
