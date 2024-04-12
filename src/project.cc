#include "../include/project.hh"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "../include/chart.hh"
#include "../include/util.hh"

using lines = std::vector<std::string>;

int project::lex(std::vector<std::vector<lines>>& chartConfigs,
                 const std::filesystem::path& working,
                 const std::vector<apkg::chart>& charts,
                 const std::vector<std::string>& directories,
                 const std::vector<std::string>& settingsFile) {
  std::ifstream projectSettings;
  std::string buffer;
  std::string projectYaml;
  lines configLines;
  std::vector<lines> config;
  std::stringstream bufferStream;
  lines lineBuffer;

  size_t idx;

  for (size_t i = 0; i < charts.size(); ++i) {
    projectYaml = "";

    projectSettings.open(working / directories[i] / settingsFile[i]);
    while (std::getline(projectSettings, buffer)) projectYaml += buffer + "\n";
    projectSettings.close();

    configLines = {};
    uint8_t x = 0;
    while (++x) {
      idx = projectYaml.find("\n-");
      if (idx == std::string::npos) {
        configLines.push_back(projectYaml);
        break;
      } else {
        configLines.push_back(projectYaml.substr(0, idx));
        projectYaml = std::move(projectYaml.substr(idx + 2));
      }
    }

    configLines.erase(configLines.begin());

    config = {};
    for (std::string piece : configLines) {
      bufferStream = std::stringstream(piece);

      while (std::getline(bufferStream, buffer)) {
        lineBuffer.push_back(util::trim(std::move(buffer)));
      }
      config.push_back(lineBuffer);
    }

    chartConfigs.push_back(config);
  }

  return 0;
}