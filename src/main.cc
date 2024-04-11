
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
#include "../include/index.hh"
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
  if (int err =
          system(("7z x " + std::string(argv[1]) + " -otmp/" +
                  util::unext(std::string(argv[1])) + " -y -bb0 > tmp/del.txt")
                     .c_str())) {
    std::cerr << "Unable to extract file.";
    return err;
  }

  std::filesystem::path working =
      std::filesystem::current_path() / "tmp" /
      std::filesystem::path(util::unext(std::string(argv[1])));

  std::vector<lines> pack;
  (void)index::read(working, pack);

  std::vector<apkg::chart> charts{};
  std::vector<std::string> directories{};
  std::vector<std::string> settingsFile{};

  (void)index::parse(pack, charts, directories, settingsFile);

  // Configs are grouped into difficulties (lines), then grouped into songs,
  // which are then grouped into packs.
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
          util::rtrim(buffer);
          configLines.push_back(buffer);
        }
        configs.push_back(configLines);
      }

      configs.erase(configs.begin());
      chartConfigs.push_back(configs);

      projectSettings.close();
    }
  }

  // Cancer
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
