#include "String.hpp"

#include <charconv>

std::vector<std::string_view> Medae::Utils::split(std::string_view s, char delim)
{
	std::vector<std::string_view> out;
	size_t start = 0;
	while (true) {
		size_t pos = s.find(delim, start);
		if (pos == std::string_view::npos) {
			out.push_back(s.substr(start));
			break;
		}
		out.push_back(s.substr(start, pos - start));
		start = pos + 1;
	}
	return out;
}

std::vector<std::string_view> Medae::Utils::splitAny(std::string_view s, std::initializer_list<std::string_view> delims)
{
	std::vector<std::string_view> out;
	size_t start = 0;

	while (start <= s.size()) {
		size_t bestPos = std::string_view::npos;
		size_t bestLen = 0;

		for (auto d : delims) {
			if (d.empty())
				continue;
			size_t p = s.find(d, start);
			if (p != std::string_view::npos && (p < bestPos || (p == bestPos && d.size() > bestLen))) {
				bestPos = p;
				bestLen = d.size();
			}
		}

		if (bestPos == std::string_view::npos) {
			out.push_back(s.substr(start));
			break;
		}
		out.push_back(s.substr(start, bestPos - start));
		start = bestPos + bestLen;
	}
	return out;
}

std::optional<std::pair<size_t, size_t>> Medae::Utils::locate(std::string_view haystack, std::string_view needle)
{
	if (needle.empty())
		return std::pair{size_t{0}, size_t{0}};

	const char* hb = haystack.data();
	const char* he = hb + haystack.size();
	const char* nb = needle.data();
	const char* ne = nb + needle.size();

	if (nb < hb || ne > he)
		return std::nullopt;

	return std::pair{static_cast<size_t>(nb - hb), needle.size()};
}

bool Medae::Utils::toInt(std::string_view sv, int& out)
{
	auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), out);
	return ec == std::errc{} && ptr == sv.data() + sv.size();
}

bool Medae::Utils::toLong(std::string_view sv, long& out)
{
	auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), out);
	return ec == std::errc{} && ptr == sv.data() + sv.size();
}

bool Medae::Utils::toLongLong(std::string_view sv, long long& out)
{
	auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), out);
	return ec == std::errc{} && ptr == sv.data() + sv.size();
}

bool Medae::Utils::toFloat(std::string_view sv, float& out)
{
	auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), out);
	return ec == std::errc{} && ptr == sv.data() + sv.size();
}

bool Medae::Utils::toDouble(std::string_view sv, double& out)
{
	auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), out);
	return ec == std::errc{} && ptr == sv.data() + sv.size();
}

std::optional<int> Medae::Utils::toInt(std::string_view sv)
{
	int v{};
	if (toInt(sv, v))
		return v;
	return std::nullopt;
}

std::optional<long> Medae::Utils::toLong(std::string_view sv)
{
	long v{};
	if (toLong(sv, v))
		return v;
	return std::nullopt;
}

std::optional<long long> Medae::Utils::toLongLong(std::string_view sv)
{
	long long v{};
	if (toLongLong(sv, v))
		return v;
	return std::nullopt;
}

std::optional<float> Medae::Utils::toFloat(std::string_view sv)
{
	float v{};
	if (toFloat(sv, v))
		return v;
	return std::nullopt;
}

std::optional<double> Medae::Utils::toDouble(std::string_view sv)
{
	double v{};
	if (toDouble(sv, v))
		return v;
	return std::nullopt;
}