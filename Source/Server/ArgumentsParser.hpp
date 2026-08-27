#pragma once

#include <string>

namespace Medae::Server {

class ArgumentsParser
{
  private:
	std::string m_propertiesFilePath = "./properties.toml"; // --properties-file
	bool m_showHelp = false;

  public:
	[[nodiscard]] const std::string& getPropertiesFilePath() const;
	[[nodiscard]] bool isShowHelp() const;

	void init(int argc, char** argv);
};

} // namespace Medae::Server