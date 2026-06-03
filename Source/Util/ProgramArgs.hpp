#pragma once

#include <string>
#include <unordered_map>
#include <vector>

class ProgramArgs {
    std::vector<std::string> flags_m;
    std::unordered_map<std::string, std::string> flagsWithValue_m;

  public:
    ProgramArgs(int argc, char **argv);

    bool getFlag(const std::string& flagName) const;
    std::string getValue(const std::string& flagName) const;
};