#include "../include/util.hh"

#include <algorithm>

std::string apkg::util::unext(std::string str) {
  size_t value = str.find_last_of('.');
  if (value == std::string::npos) {
    return str;
  }
  return str.substr(0, value);
}

std::string apkg::util::trim(std::string&& in) {
  in.erase(std::find_if(in.rbegin(), in.rend(),
                        [](unsigned char ch) { return !std::isspace(ch); })
               .base(),
           in.end());

  in.erase(in.begin(), std::find_if(in.begin(), in.end(), [](unsigned char ch) {
             return !std::isspace(ch);
           }));

  return in;
}

void apkg::util::rtrim(std::string& in) {
  in.erase(std::find_if(in.rbegin(), in.rend(),
                        [](unsigned char ch) { return !std::isspace(ch); })
               .base(),
           in.end());

  in.erase(in.begin(), std::find_if(in.begin(), in.end(), [](unsigned char ch) {
             return !std::isspace(ch);
           }));
}
