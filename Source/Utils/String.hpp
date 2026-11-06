#pragma once

#include <cctype>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace Medae::Utils {

std::optional<std::vector<std::string>> splitRespectQuotes(std::string_view line);
std::vector<std::string_view> split(std::string_view s, char spliterator);
std::optional<std::pair<size_t, size_t>> locate(std::string_view haystack, std::string_view needle);
std::vector<std::string_view> splitAny(std::string_view s, std::initializer_list<std::string_view> delims);
template<typename T> std::string vectorToString(const std::vector<T>& vec);
bool toInt(std::string_view sv, int& out);
bool toLong(std::string_view sv, long& out);
bool toLongLong(std::string_view sv, long long& out);
bool toFloat(std::string_view sv, float& out);
bool toDouble(std::string_view sv, double& out);
std::optional<int> toInt(std::string_view sv);
std::optional<long> toLong(std::string_view sv);
std::optional<long long> toLongLong(std::string_view sv);
std::optional<float> toFloat(std::string_view sv);
std::optional<double> toDouble(std::string_view sv);
bool isReservedStem(std::string_view stem);
bool isValidRelativePath(std::string_view p);
std::string toLower(const std::string_view& string);
std::string toUpper(const std::string_view& string);

} // namespace Medae::Utils

#include "String.tpp"