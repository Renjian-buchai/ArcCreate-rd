
#include <sqlite3.h>

#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "../include/chart.hh"
#include "../include/index.hh"
#include "../include/project.hh"
#include "../include/util.hh"

int main(int argc, const char** argv, const char** envp) {
  (void)argc, (void)argv, (void)envp;

  char* errmsg = nullptr;

  sqlite3* database = nullptr;
  if (int err = sqlite3_open("C:/docs/archive.db", &database)) {
    return err;
  }

  sqlite3_exec(database,
               "CREATE TABLE IF NOT EXISTS main.charts (\n"
               "id INTEGER PRIMARY KEY,\n"
               "  identifier TEXT NOT NULL,\n"
               "  title TEXT NOT NULL,\n"
               "  composer TEXT NOT NULL,\n"
               "  charter TEXT NOT NULL,\n"
               "  alias TEXT ,\n"
               "  illustrator TEXT ,\n"
               "  chartConstant REAL NOT NULL,\n"
               "  difficulty TEXT ,\n"
               "  displayedConstant TEXT ,\n"
               "  baseBPM INTEGER NOT NULL,\n"
               "  bpmText TEXT ,\n"
               "  side INTEGER NOT NULL,\n"
               "  searchTags TEXT ,"
               "  pack TEXT \n"
               ");",
               nullptr, nullptr, &errmsg);

  if (errmsg) {
    std::cout << errmsg;
    return 1;
  }
#define testcase
#ifdef testcase
  // Cancer
  if (int err = system(("7z x " + std::string(argv[1]) + " -otmp/" +
                        apkg::util::unext(std::string(argv[1])) +
                        " -y -bb0 > tmp/del.txt")
                           .c_str())) {
    std::cerr << "Unable to extract file.";
    return err;
  }

#endif

  std::filesystem::path working =
      std::filesystem::current_path() / "tmp" /
      std::filesystem::path(apkg::util::unext(std::string(argv[1])));

  std::vector<lines> pack;
  (void)index::read(working, pack);

  std::vector<apkg::chart> charts{};
  std::vector<std::string> directories{};
  std::vector<std::string> settingsFile{};

  (void)index::parse(pack, charts, directories, settingsFile);

  std::vector<std::vector<lines>> packConfigs;
  (void)apkg::project::lex(packConfigs, working, charts, directories,
                           settingsFile);

  std::vector<apkg::chart> difficulties;

  {
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
            anchor = apkg::util::trim(config.substr(6));
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

        if (difficulty.chartConstant == -1) {
          std::reverse(difficulty.displayedConstant.begin(),
                       difficulty.displayedConstant.end());

          size_t it =
              std::find_first_of(difficulty.displayedConstant.begin(),
                                 difficulty.displayedConstant.end(),
                                 std::begin(numbers), std::end(numbers)) -
              difficulty.displayedConstant.begin();

          std::reverse(difficulty.displayedConstant.begin(),
                       difficulty.displayedConstant.end());

          try {
            difficulty.chartConstant =
                std::stoi(difficulty.displayedConstant.substr(
                    0, difficulty.displayedConstant.size() - it));
          } catch (const std::invalid_argument& e) {
            difficulty.chartConstant = -1;
          }

          std::cout << difficulty.chartConstant;
        }

        difficulties.push_back(difficulty);
      }
    }
  }

  // Cancer
  for (apkg::chart& chart : charts) {
    // std::cout << chart.identifier << "\n" << chart.pack;
    sqlite3_exec(database,
                 ("INSERT INTO main.charts ("
                  "  identifier,"
                  "  title,"
                  "  composer,"
                  "  charter,"
                  "  chartConstant,"
                  "  baseBPM,"
                  "  side"
                  ") VALUES ("
                  "  \"" +
                  chart.identifier + "\", \"\", \"\", \"\", 0, 1, 0) ")
                     .c_str(),
                 nullptr, nullptr, &errmsg);
    if (errmsg) {
      std::cerr << errmsg;
      return 1;
    }
  }

  sqlite3_close_v2(database);

  return 0;
}
