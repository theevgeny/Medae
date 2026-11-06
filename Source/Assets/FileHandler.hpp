#pragma once

#include <cstddef>
#include <fstream>
#include <string>

namespace Medae::Assets {

class FileHandler
{
public:
	FileHandler(std::fstream&& file, const std::string& path);

	std::string GetLine();
	void ToStart();
	std::size_t GetLineNumber() const noexcept;
	const std::string& GetPath() const;

private:
	std::fstream m_file;
	std::string m_path;
	std::size_t m_lineNumber = 0;
};

} // namespace Medae::Assets