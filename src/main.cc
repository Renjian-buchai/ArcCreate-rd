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
               "CREATE TABLE IF NOT EXISTS   charts ("
               "  index INTEGER PRIMARY KEY NOT NULL,"
               "  identifier TEXT NOT NULL"
               "  title TEXT NOT NULL,"
               "  composer TEXT NOT NULL,"
               "  charter TEXT NOT NULL,"
               "  alias TEXT,"
               "  illustrator TEXT,"
               "  chartConstant REAL NOT NULL,"
               "  difficulty TEXT,"
               "  displayedConstant TEXT,"
               "  baseBPM INTEGER NOT NULL,"
               "  bpmText TEXT,"
               "  side INTEGER NOT NULL,"
               "  searchTags TEXT"
               "  pack TEXT"
               ");",
               nullptr, nullptr, &errmsg);

  if (int err = system((u8"7z x " + std::string(argv[1]) + " -otmp/" +
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

    std::string buffer;
    std::stringstream bufferStream;
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

    for (lines chart : pack) {
    for (std::string config : chart) {
      std::cout << config << "\n";
    }
  }

  return 0;
}
