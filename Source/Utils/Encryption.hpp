#pragma once

#include <cstdint>
#include <string>

namespace Medae::Encryption {

	struct Data { // NOLINT
		uint8_t* content;
		uint16_t size;
	};

	using PublicKey = std::string;
	using PrivateKey = std::string;

	Data encrypt(Data data, PublicKey key);
	Data decrypt(Data data, PrivateKey key);

}

