#if !defined(APKG_RD_UTIL)
#define APKG_RD_UTIL

#include <string>

namespace util {

std::string unext(std::string str);
std::string trim(std::string&& in);
void rtrim(std::string& in);

}  // namespace util

#endif  // APKG_RD_UTIL
