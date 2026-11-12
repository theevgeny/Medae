#pragma once

#include <cstddef>
#include <initializer_list>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace Medae::Utils {

std::optional<std::vector<std::string>> SplitRespectQuotes(std::string_view line);
std::vector<std::string_view> Split(std::string_view s, char spliterator);
std::optional<std::pair<std::size_t, std::size_t>> Locate(std::string_view haystack, std::string_view needle);
std::vector<std::string_view> SplitAny(std::string_view s, std::initializer_list<std::string_view> delims);
template<typename TValue> std::string VectorToString(const std::vector<TValue>& vec);
bool ToInt(std::string_view sv, int& out);
bool ToLong(std::string_view sv, long& out);
bool ToLongLong(std::string_view sv, long long& out);
bool ToFloat(std::string_view sv, float& out);
bool ToDouble(std::string_view sv, double& out);
std::optional<int> ToInt(std::string_view sv);
std::optional<long> ToLong(std::string_view sv);
std::optional<long long> ToLongLong(std::string_view sv);
std::optional<float> ToFloat(std::string_view sv);
std::optional<double> ToDouble(std::string_view sv);
bool IsReservedStem(std::string_view stem);
bool IsValidRelativePath(std::string_view p);
std::string ToLower(std::string_view text);
std::string ToUpper(std::string_view text);
std::string_view StripComment(std::string_view line);

} // namespace Medae::Utils

#include "String.tpp"