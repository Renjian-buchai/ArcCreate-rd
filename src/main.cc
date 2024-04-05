#include <sqlite3.h>

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

std::string unext(std::string str) {
  size_t value = str.find_last_of('.');
  if (value == std::string::npos) {
    return str;
  }
  return str.substr(0, value);
}

int main(int argc, const char** argv, const char** envp) {
  (void)argc, (void)argv, (void)envp;

  char* errmsg = nullptr;

  sqlite3* database = nullptr;
  if (int err = sqlite3_open("C:/docs/archive.db", &database)) {
    return err;
  }

  sqlite3_exec(database,
               "create table if not exists charts ("
               "index integer primary key,"
               ""  // TODO complete
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

  std::string data;

  {
    std::ifstream index(working / "index.yml");

    std::string buffer;
    while (std::getline(index, buffer)) {
      data += buffer;
    }

    index.close();
  }

  return 0;
}
