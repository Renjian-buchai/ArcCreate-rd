#if !defined(APKG_RD_INDEX)
#define APKG_RD_INDE

#include <filesystem>
#include <vector>

#include "../include/chart.hh"

using lines = std::vector<std::string>;

namespace index {
int read(const std::filesystem::path& working, std::vector<lines>& pack);
int parse(const std::vector<lines>& pack, std::vector<apkg::chart>& charts,
          std::vector<std::string>& directories,
          std::vector<std::string>& settingsFile);
}  // namespace index

#endif  // APKG_RD_INDE
