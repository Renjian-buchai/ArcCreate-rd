#if !defined(APKG_RD_PROJECT)
#define APKG_RD_PROJECT

#include <filesystem>
#include <string>
#include <vector>

#include "../include/chart.hh"
using lines = std::vector<std::string>;

namespace project {
int lex(std::vector<std::vector<lines>>& chartConfigs,
        const std::filesystem::path& working,
        const std::vector<apkg::chart>& charts,
        const std::vector<std::string>& directories,
        const std::vector<std::string>& settingsFile);
}  // namespace project

#endif  // APKG_RD_PROJECT
