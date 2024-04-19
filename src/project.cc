#include "../include/project.hh"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "../include/chart.hh"
#include "../include/util.hh"

using lines = std::vector<std::string>;

int apkg::project::lex(std::vector<std::vector<lines>>& chartConfigs,
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

#if 0 

// Dead until I can figure out how to fix it
static int inferConstant(apkg::chart& difficulty, const char (&numbers)[11]) {
  std::reverse(difficulty.displayedConstant.begin(),
               difficulty.displayedConstant.end());

  size_t it = std::find_first_of(difficulty.displayedConstant.begin(),
                                 difficulty.displayedConstant.end(),
                                 std::begin(numbers), std::end(numbers)) -
              difficulty.displayedConstant.begin();

  std::reverse(difficulty.displayedConstant.begin(),
               difficulty.displayedConstant.end());

  try {
    difficulty.chartConstant = std::stoi(difficulty.displayedConstant.substr(
        0, difficulty.displayedConstant.size() - it - 1));
    if (difficulty.displayedConstant.back() == '+') {
      difficulty.chartConstant += 0.7f;
    }
  } catch (const std::invalid_argument& e) {
    difficulty.chartConstant = -1;
  }
  return 0;
}

#endif

// I literally don't know how to make this function smaller
// Maybe just get better at programming
// But yes, I agree it's shit style.
int apkg::project::parse(std::vector<apkg::chart>& difficulties,
                         const std::vector<std::vector<lines>>& packConfigs,
                         const std::vector<apkg::chart>& charts) {
  constexpr const char numbers[11] = "0123456789";

  apkg::chart difficulty("");
  // Loops over the charts
  for (size_t i = 0; i < packConfigs.size(); ++i) {
    std::unordered_map<std::string, uint8_t> skinConfigs;
    // Loops over the difficulties
    for (size_t j = 0; j < packConfigs[0].size(); ++j) {
      difficulty = charts[i];  // Fetches identifier and copies it over.
      std::string anchor = "";

      // Loops over the configs in that difficulty
      for (std::string config : packConfigs[i][j]) {
        if (config.substr(0, 5) == "title") {
          difficulty.title = config.substr(7);
        } else if (config.substr(0, 8) == "composer") {
          difficulty.composer = config.substr(10);
        } else if (config.substr(0, 7) == "charter") {
          difficulty.charter = config.substr(9);
        } else if (config.substr(0, 4) == "skin") {
          anchor = config.size() < 6 ? "" : apkg::util::trim(config.substr(6));
        } else if (config.substr(0, 4) == "side") {
          std::string sstr = config.substr(6);
          if (sstr == "light") {
            difficulty.side = 0;
          } else if (sstr == "conflict") {
            difficulty.side = 1;
          } else {
            difficulty.side = 2;
          }
        } else if (config.substr(0, 13) == "chartConstant") {
          char* e;
          difficulty.chartConstant =
              std::strtold(config.substr(15).c_str(), &e);

          if (*e != '\0' or errno != 0) {
            difficulty.chartConstant = -1;
          }
        } else if (config.substr(0, 7) == "baseBpm") {
          difficulty.baseBPM = std::stoull(config.substr(9));
        } else if (config.substr(0, 5) == "alias") {
          difficulty.alias = config.substr(7);
        } else if (config.substr(0, 11) == "illustrator") {
          difficulty.illustrator = config.substr(13);
        } else if (config.substr(0, 11) == "difficulty:") {
          size_t it =
              std::find_first_of(config.begin(), config.end(),
                                 std::begin(numbers), std::end(numbers)) -
              config.begin();
          if (it != config.size() and it > 2) {
            difficulty.difficulty =
                apkg::util::trim(config.substr(12, it - 12));
          }
          difficulty.displayedConstant = config.substr(it);
        } else if (config.substr(0, 7) == "bpmText") {
          difficulty.bpmText = config.substr(9);
        } else if (config.substr(0, 10) == "searchTags") {
          difficulty.searchTags = config.substr(12);
        }
      }

      if (anchor != "") {
        if (skinConfigs.find(anchor) == skinConfigs.end()) {
          skinConfigs[anchor] = difficulty.side;
        } else {
          difficulty.side = skinConfigs[anchor];
        }
      }

      if (difficulty.side == static_cast<uint8_t>(-1)) {
        if (anchor == "") {
          difficulty.side = 0;
        } else {
          difficulty.side = skinConfigs[anchor];
        }
      }

      // if (difficulty.chartConstant == -1) {
      //   (void)inferConstant(difficulty, numbers);
      // }

      difficulties.push_back(difficulty);
    }
  }

  return 0;
}
