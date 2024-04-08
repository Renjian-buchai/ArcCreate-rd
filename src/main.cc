
#include <sqlite3.h>

#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "../include/chart.hh"

using lines = std::vector<std::string>;

std::string unext(std::string str) {
  size_t value = str.find_last_of('.');
  if (value == std::string::npos) {
    return str;
  }
  return str.substr(0, value);
}

inline std::string trim(std::string&& in) {
  in.erase(std::find_if(in.rbegin(), in.rend(),
                        [](unsigned char ch) { return !std::isspace(ch); })
               .base(),
           in.end());

  in.erase(in.begin(), std::find_if(in.begin(), in.end(), [](unsigned char ch) {
             return !std::isspace(ch);
           }));

  return in;
}

inline void rtrim(std::string& in) {
  in.erase(std::find_if(in.rbegin(), in.rend(),
                        [](unsigned char ch) { return !std::isspace(ch); })
               .base(),
           in.end());

  in.erase(in.begin(), std::find_if(in.begin(), in.end(), [](unsigned char ch) {
             return !std::isspace(ch);
           }));
}

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
               "  alias TEXT DEFAULT \"\",\n"
               "  illustrator TEXT DEFAULT \"\",\n"
               "  chartConstant REAL NOT NULL,\n"
               "  difficulty TEXT DEFAULT \"\",\n"
               "  displayedConstant TEXT DEFAULT \"\",\n"
               "  baseBPM INTEGER NOT NULL,\n"
               "  bpmText TEXT DEFAULt \"\",\n"
               "  side INTEGER NOT NULL,\n"
               "  searchTags TEXT DEFAULT \"\","
               "  pack TEXT DEFAULT \"\"\n"
               ");",
               nullptr, nullptr, &errmsg);

  if (errmsg) {
    std::cout << errmsg;
    return 1;
  }

  // Cancer
  if (int err = system(("7z x " + std::string(argv[1]) + " -otmp/" +
                        unext(std::string(argv[1])) + " -y -bb0 > tmp/del.txt")
                           .c_str())) {
    std::cerr << "Unable to extract file.";
    return err;
  }

  std::filesystem::path working =
      std::filesystem::current_path() / "tmp" /
      std::filesystem::path(unext(std::string(argv[1])));

  std::vector<lines> pack;

  {
    std::ifstream index(working / "index.yml");

    std::stringstream bufferStream;
    std::string buffer;
    lines configLines;
    while (std::getline(index, buffer, '-')) {
      configLines = {};
      bufferStream = std::stringstream(buffer);
      while (std::getline(bufferStream, buffer)) {
        rtrim(buffer);
        configLines.push_back(buffer);
      }
      pack.push_back(configLines);
    }

    pack.erase(pack.begin());

    index.close();
  }

  std::vector<apkg::chart> charts{};
  std::vector<std::string> directories{};
  std::vector<std::string> settingsFile{};

  {
    std::string packName;
    size_t packIdx = -1;
    for (size_t i = 0; i < pack.size(); ++i) {
      for (std::string& config : pack[i]) {
        if (config.substr(0, 4) == "type") {
          if (trim(config.substr(6)) == "pack") {
            packIdx = charts.size() - 1;
          }
        } else if (config.substr(0, 9) == "directory") {
          directories.push_back(config.substr(11));
        } else if (config.substr(0, 10) == "identifier") {
          charts.push_back(apkg::chart(trim(config.substr(12))));
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
  }

  std::vector<std::vector<lines>> chartConfigs;

  {
    std::vector<lines> configs{};
    std::ifstream projectSettings;
    std::string buffer;
    lines configLines;
    std::stringstream bufferStream;
    for (size_t i = 0; i < charts.size(); ++i) {
      projectSettings.open(working / directories[i] / settingsFile[i]);

      while (std::getline(projectSettings, buffer, '-')) {
        configLines = {};
        bufferStream = std::stringstream(buffer);
        while (std::getline(bufferStream, buffer, '\n')) {
          rtrim(buffer);
          configLines.push_back(buffer);
        }
        configs.push_back(configLines);
      }

      chartConfigs.push_back(configs);

      configs.erase(configs.begin());

      projectSettings.close();
    }
  }

  for (apkg::chart& chart : charts) {
    std::cout << chart.identifier << "\n" << chart.pack;
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

#if 0

  for (lines chart : pack) {
    for (std::string config : chart) {
      std::cout << config << "\n";
    }
  }

#endif

  sqlite3_close_v2(database);

  return 0;
}
