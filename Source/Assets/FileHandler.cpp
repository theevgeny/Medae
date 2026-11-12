#include "FileHandler.hpp"

#include <cstddef>
#include <fstream>
#include <string>
#include <utility>

using namespace Medae::Assets;

FileHandler::FileHandler(std::fstream&& file, const std::string& path) noexcept : m_file(std::move(file)), m_path(path)
{
}

std::string FileHandler::GetLine() noexcept
{
	std::string line;
	if (std::getline(m_file, line)) {
		++m_lineNumber;
	}
	return line;
}

void FileHandler::ToStart() noexcept
{
	m_file.clear();
	m_file.seekg(0, std::ios::beg);
	m_lineNumber = 0;
}

std::size_t FileHandler::GetLineNumber() const noexcept
{
	return m_lineNumber;
}

const std::string& FileHandler::GetPath() const noexcept
{
	return m_path;
}