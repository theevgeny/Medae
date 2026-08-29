#pragma once

#include <cstdint>

namespace Medae::Compression {

	struct Data { // NOLINT
		uint8_t* content;
		uint16_t size;
	};

	void compress(Data&);
	void decompress(Data&);

}

