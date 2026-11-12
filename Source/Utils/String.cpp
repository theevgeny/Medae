#include "String.hpp"

#include <algorithm>
#include <cctype>
#include <charconv>
#include <cstddef>

std::string Medae::Utils::ToLower(std::string_view text)
{
	std::string s = std::string(text);
	std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); });
	return s;
}

std::string Medae::Utils::ToUpper(std::string_view text)
{
	std::string s = std::string(text);
	std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::toupper(c); });
	return s;
}

std::string_view Medae::Utils::StripComment(std::string_view line)
{
	bool inQuotes = false;
	for (std::size_t i = 0; i < line.size(); ++i) {
		const char c = line[i];
		if (c == '"') {
			inQuotes = !inQuotes;
		} else if (!inQuotes && c == '/' && i + 1 < line.size() && line[i + 1] == '/') {
			line = line.substr(0, i);
			break;
		}
	}
	while (!line.empty() && (line.front() == ' ' || line.front() == '\t')) {
		line.remove_prefix(1);
	}
	while (!line.empty() && (line.back() == ' ' || line.back() == '\t' || line.back() == '\r')) {
		line.remove_suffix(1);
	}
	return line;
}

std::optional<std::vector<std::string>> Medae::Utils::SplitRespectQuotes(std::string_view line)
{
	std::vector<std::string> out;
	std::size_t i = 0;
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

bool Medae::Utils::IsValidRelativePath(std::string_view p)
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

	std::size_t i = 0;
	while (i <= p.size()) {
		std::size_t j = p.find('/', i);
		if (j == std::string_view::npos)
			j = p.size();
		std::string_view comp = p.substr(i, j - i);

		if (comp.empty())
			return false; // foo//bar, /foo, foo/
		if (comp == "." || comp == "..")
			return false;

		for (char ch : comp) {
			unsigned char u = static_cast<unsigned char>(ch);
			if (u < 0x20 || u == 0x7F)
				return false;
		}

		char last = comp.back();
		if (last == '.' || last == ' ')
			return false;

		std::string_view stem = comp;
		if (auto dot = stem.find('.'); dot != std::string_view::npos)
			stem = stem.substr(0, dot);
		if (IsReservedStem(stem))
			return false;

		i = j + 1;
	}
	return true;
}

bool Medae::Utils::IsReservedStem(std::string_view stem)
{
	auto equalsIgnoreCase = [](std::string_view a, std::string_view b) {
		if (a.size() != b.size())
			return false;
		for (std::size_t i = 0; i < a.size(); ++i)
			if (std::tolower(static_cast<unsigned char>(a[i])) != std::tolower(static_cast<unsigned char>(b[i])))
				return false;
		return true;
	};

	static constexpr std::string_view kNames[] = {"CON", "PRN", "AUX", "NUL", "COM1", "COM2", "COM3", "COM4", "COM5",
		"COM6", "COM7", "COM8", "COM9", "LPT1", "LPT2", "LPT3", "LPT4", "LPT5", "LPT6", "LPT7", "LPT8", "LPT9"};
	for (auto n : kNames)
		if (equalsIgnoreCase(stem, n))
			return true;
	return false;
}

std::vector<std::string_view> Medae::Utils::Split(std::string_view s, char delim)
{
	std::vector<std::string_view> out;
	std::size_t start = 0;
	while (true) {
		std::size_t pos = s.find(delim, start);
		if (pos == std::string_view::npos) {
			out.push_back(s.substr(start));
			break;
		}
		out.push_back(s.substr(start, pos - start));
		start = pos + 1;
	}
	return out;
}

std::vector<std::string_view> Medae::Utils::SplitAny(std::string_view s, std::initializer_list<std::string_view> delims)
{
	std::vector<std::string_view> out;
	std::size_t start = 0;

	while (start <= s.size()) {
		std::size_t bestPos = std::string_view::npos;
		std::size_t bestLen = 0;

		for (auto d : delims) {
			if (d.empty())
				continue;
			std::size_t p = s.find(d, start);
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

std::optional<std::pair<std::size_t, std::size_t>> Medae::Utils::Locate(
	std::string_view haystack, std::string_view needle)
{
	if (needle.empty())
		return std::pair{std::size_t{0}, std::size_t{0}};

	const char* haystackBegin = haystack.data();
	const char* haystackEnd = haystackBegin + haystack.size();
	const char* needleBegin = needle.data();
	const char* needleEnd = needleBegin + needle.size();

	if (needleBegin < haystackBegin || needleEnd > haystackEnd)
		return std::nullopt;

	return std::pair{static_cast<std::size_t>(needleBegin - haystackBegin), needle.size()};
}

bool Medae::Utils::ToInt(std::string_view sv, int& out)
{
	auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), out);
	return ec == std::errc{} && ptr == sv.data() + sv.size();
}

bool Medae::Utils::ToLong(std::string_view sv, long& out)
{
	auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), out);
	return ec == std::errc{} && ptr == sv.data() + sv.size();
}

bool Medae::Utils::ToLongLong(std::string_view sv, long long& out)
{
	auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), out);
	return ec == std::errc{} && ptr == sv.data() + sv.size();
}

bool Medae::Utils::ToFloat(std::string_view sv, float& out)
{
	auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), out);
	return ec == std::errc{} && ptr == sv.data() + sv.size();
}

bool Medae::Utils::ToDouble(std::string_view sv, double& out)
{
	auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), out);
	return ec == std::errc{} && ptr == sv.data() + sv.size();
}

std::optional<int> Medae::Utils::ToInt(std::string_view sv)
{
	int v{};
	if (ToInt(sv, v))
		return v;
	return std::nullopt;
}

std::optional<long> Medae::Utils::ToLong(std::string_view sv)
{
	long v{};
	if (ToLong(sv, v))
		return v;
	return std::nullopt;
}

std::optional<long long> Medae::Utils::ToLongLong(std::string_view sv)
{
	long long v{};
	if (ToLongLong(sv, v))
		return v;
	return std::nullopt;
}

std::optional<float> Medae::Utils::ToFloat(std::string_view sv)
{
	float v{};
	if (ToFloat(sv, v))
		return v;
	return std::nullopt;
}

std::optional<double> Medae::Utils::ToDouble(std::string_view sv)
{
	double v{};
	if (ToDouble(sv, v))
		return v;
	return std::nullopt;
}