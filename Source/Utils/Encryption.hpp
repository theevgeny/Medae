#pragma once

#include <string>

#include <Utils/Compression.hpp>

namespace Medae::Utils {

using PublicKey = std::string;
using PrivateKey = std::string;

Data encrypt(Data data, PublicKey key);
Data decrypt(Data data, PrivateKey key);

} // namespace Medae::Utils
