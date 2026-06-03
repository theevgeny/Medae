#include <Util/ProgramArgs.hpp>
#include <algorithm>

ProgramArgs::ProgramArgs(int argc, char **argv) {
    for (int i = 0; i < argc; ++i) {
        std::string arg{argv[i]};
        if (arg.starts_with("--")) {
            size_t pos = arg.find_first_of('=');
            if (pos != std::string::npos) {
                flagsWithValue_m.emplace(arg.substr(2, pos - 2),
                                         arg.substr(pos + 1));
            } else {
                flags_m.push_back(arg.substr(2));
            }
        } else if (arg.starts_with("-")) {
            flags_m.push_back(arg.substr(1));
        }
    }
}

bool ProgramArgs::getFlag(const std::string& flagName) {
    return std::find(flags_m.begin(), flags_m.end(), flagName) != flags_m.end();
}

std::string ProgramArgs::getValue(const std::string& flagName) {
    auto it = flagsWithValue_m.find(flagName);
    if (it != flagsWithValue_m.cend()) {
        return it->second;
    }
    return {};
}