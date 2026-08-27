#pragma once

#include <string>

#include "Util/Macros.hpp"

namespace Medae::Server {

class ArgumentsParser
{
  private:
	std::string m_propertiesFilePath = "./properties.toml"; // --properties-file
	bool m_showHelp = false;

  public:
	NODIS const std::string& getPropertiesFilePath() const;
	NODIS bool isShowHelp() const;

	ArgumentsParser(int argc, char** argv);
};

} // namespace Medae::Server