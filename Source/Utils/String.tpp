#pragma once

#include "String.hpp"

template<typename TValue> std::string Medae::Utils::VectorToString(const std::vector<TValue>& vec)
{
	std::string str;
	str.append("[");
	bool first = true;
	for (const auto& v : vec) {
		if (!first)
			str.append(", ");
		str.append(std::to_string(v));
		first = false;
	}
	str.append("]");
	return str;
}