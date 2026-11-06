#include "String.hpp"

#include <algorithm>
#include <charconv>

std::string toLower(const std::string_view& string)
{
	std::string s = std::string(string);
	std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); });
	return s;
}
std::string toUpper(const std::string_view& string)
{
	std::string s = std::string(string);
	std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::toupper(c); });
	return s;
}

std::optional<std::vector<std::string>> Medae::Utils::splitRespectQuotes(std::string_view line)
{
	std::vector<std::string> out;
	size_t i = 0;
	while (i < line.size()) {
		while (i < line.size() && (line[i] == ' ' || line[i] == '\t'))
			++i;
		if (i >= line.size())
			break;

		std::string token;
		while (i < line.size() && line[i] != ' ' && line[i] != '\t') {
			if (line[i] == '"') {
				++i;
				while (i < line.size() && line[i] != '"')
					token += line[i++];
				if (i >= line.size())
					return std::nullopt;
				++i;
			} else {
				token += line[i++];
			}
		}
		out.push_back(std::move(token));
	}
	return out;
}

bool Medae::Utils::isValidRelativePath(std::string_view p)
{
	if (p.empty() || p.size() > 4096)
		return false;
	if (p.front() == '/' || p.front() == '\\')
		return false; // абсолютный
	if (p.size() >= 2 && p[1] == ':')
		return false; // C:...
	if (p.find('\\') != std::string_view::npos)
		return false; // только '/'
	if (p.find(':') != std::string_view::npos)
		return false; // streams / drive

	size_t i = 0;
	while (i <= p.size()) {
		size_t j = p.find('/', i);
		if (j == std::string_view::npos)
			j = p.size();
		std::string_view comp = p.substr(i, j - i);

		if (comp.empty())
			return false; // foo//bar, /foo, foo/
		if (comp == "." || comp == "..")
			return false;

		for (char ch : comp) {
			unsigned char u = (unsigned char)ch;
			if (u < 0x20 || u == 0x7F)
				return false;
		}

		char last = comp.back();
		if (last == '.' || last == ' ')
			return false;

		std::string_view stem = comp;
		if (auto dot = stem.find('.'); dot != std::string_view::npos)
			stem = stem.substr(0, dot);
		if (isReservedStem(stem))
			return false;

		i = j + 1;
	}
	return true;
}

bool Medae::Utils::isReservedStem(std::string_view stem)
{
	auto eqi = [](std::string_view a, std::string_view b) {
		if (a.size() != b.size())
			return false;
		for (size_t i = 0; i < a.size(); ++i)
			if (std::tolower((unsigned char)a[i]) != std::tolower((unsigned char)b[i]))
				return false;
		return true;
	};
	static constexpr std::string_view names[] = {"CON", "PRN", "AUX", "NUL", "COM1", "COM2", "COM3", "COM4", "COM5",
		"COM6", "COM7", "COM8", "COM9", "LPT1", "LPT2", "LPT3", "LPT4", "LPT5", "LPT6", "LPT7", "LPT8", "LPT9"};
	for (auto n : names)
		if (eqi(stem, n))
			return true;
	return false;
}

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