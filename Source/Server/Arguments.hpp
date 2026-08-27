#pragma once

#include <string>

namespace Medae::Server {

class Arguments
{
  private:
	std::string m_propertiesFilePath; // --properties-file
	bool m_showHelp;

  public:
	[[nodiscard]] const std::string& getPropertiesFilePath() const;
	[[nodiscard]] bool isShowHelp() const;

	Arguments(int argc, char** argv);
};

} // namespace Medae::Server