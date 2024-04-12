#if !defined(APKG_RD_CHART)
#define APKG_RD_CHART

#include <cstdint>
#include <string>

namespace apkg {

struct chart {
 public:
  chart() = delete;
  chart(std::string identifier, std::string title = "",
        std::string composer = "", std::string charter = "",
        uint8_t side = static_cast<char>(-1), long double chartConstant = -1,
        unsigned long long baseBPM = 0, std::string alias = "",
        std::string illustrator = "", std::string difficulty = "",
        std::string displayedConstant = "", std::string bpmText = "",
        std::string searchTags = "", std::string pack = "");

  // Must exist

  std::string identifier;  // Done by index parsing

  std::string title;
  std::string composer;
  std::string charter;
  uint8_t side;
  long double chartConstant;
  unsigned long long baseBPM;

  // Need not exist

  std::string alias;
  std::string illustrator;
  std::string difficulty;
  std::string displayedConstant;
  std::string bpmText;
  std::string searchTags;

  std::string pack;  // Done by index parsing

  // Does not exist
};

}  // namespace apkg

#endif  // APKG_RD_CHART
