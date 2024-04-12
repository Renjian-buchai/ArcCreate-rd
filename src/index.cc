#include "../include/index.hh"

#include <fstream>

#include "../include/util.hh"

int index::read(const std::filesystem::path& working,
                std::vector<lines>& pack) {
  std::ifstream index(working / "index.yml");

  std::stringstream bufferStream;
  std::string buffer;
  lines configLines;
  while (std::getline(index, buffer, '-')) {
    configLines = {};
    bufferStream = std::stringstream(buffer);
    while (std::getline(bufferStream, buffer)) {
      apkg::util::rtrim(buffer);
      configLines.push_back(buffer);
    }
    pack.push_back(configLines);
  }

  pack.erase(pack.begin());

  index.close();

  return 0;
}

int index::parse(const std::vector<lines>& pack,
                 std::vector<apkg::chart>& charts,
                 std::vector<std::string>& directories,
                 std::vector<std::string>& settingsFile) {
  std::string packName;
  size_t packIdx = -1;
  for (size_t i = 0; i < pack.size(); ++i) {
    for (const std::string& config : pack[i]) {
      if (config.substr(0, 4) == "type") {
        if (apkg::util::trim(config.substr(6)) == "pack") {
          packIdx = charts.size() - 1;
        }
      } else if (config.substr(0, 9) == "directory") {
        directories.push_back(config.substr(11));
      } else if (config.substr(0, 10) == "identifier") {
        charts.push_back(apkg::chart(apkg::util::trim(config.substr(12))));
      } else if (config.substr(0, 12) == "settingsFile") {
        settingsFile.push_back(config.substr(14));
      } else if (config.substr(0, 7) == "version") {
        continue;
      }
    }
  }

  if (packIdx != -1ull) {
    charts.erase(charts.begin() + packIdx);
    settingsFile.erase(settingsFile.begin() + packIdx);

    for (apkg::chart& chart : charts) {
      chart.pack = directories[packIdx];
    }

    directories.erase(directories.begin() + packIdx);
  }

  return 0;
}