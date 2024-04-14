
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
               "  chartConstant INTEGER NOT NULL,\n"
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
  // Cancer

  if (!std::filesystem::exists("./tmp"))
    std::filesystem::create_directory("./tmp");

  std::string cmd = "7z x \"" + std::string(argv[1]) + "\" -o\"tmp/" +
                    apkg::util::unext(std::string(argv[1])) +
                    "\" -y -bb0 > tmp/del.txt";
  // std::cout << cmd;
  if (int err = system(cmd.c_str())) {
    std::cerr << "Unable to extract file.";
    return err;
  }

  std::filesystem::path working =
      std::filesystem::current_path() / "tmp" /
      std::filesystem::path(apkg::util::unext(std::string(argv[1])));

  std::vector<lines> pack;
  (void)index::read(working, pack);

  std::vector<apkg::chart> charts{};
  std::vector<std::string> directories{};
  std::vector<std::string> settingsFile{};

  (void)index::parse(pack, charts, directories, settingsFile);
  pack.clear();

  std::vector<std::vector<lines>> packConfigs;
  (void)apkg::project::lex(packConfigs, working, charts, directories,
                           settingsFile);
  settingsFile.clear();
  directories.clear();
  working.clear();

  std::vector<apkg::chart> difficulties;
  (void)apkg::project::parse(difficulties, packConfigs, charts);
  charts.clear();
  packConfigs.clear();

  // Cancer
  std::string query;
  for (apkg::chart& chart : difficulties) {
    chart.sanitise();

    query = chart.querify();
    sqlite3_exec(database, query.c_str(), nullptr, nullptr, &errmsg);
    // std::cout << query;
    if (errmsg) {
      std::cerr << errmsg;
      return 1;
    }
  }

  sqlite3_close_v2(database);

  return 0;
}
