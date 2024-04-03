#include <cstdlib>
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

  if (int r = system(("7z x " + std::string(argv[1]) + " -otmp/" +
                      unext(std::string(argv[1])) + " -y -bb0")
                         .c_str())) {
    std::cerr << "Unable to extract file.";
    return r;
  }

  return 0;
}
