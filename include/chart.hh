#if !defined(APKG_RD_CHART)
#define APKG_RD_CHART

#include <string>

namespace apkg {

struct chart {
 public:
  chart() = delete;
  chart(std::string identifier, std::string title = "",
        std::string composer = "", std::string charter = "", bool side = 0,
        long double chartConstant = 0, long long baseBPM = 0,
        std::string alias = "", std::string illustrator = "",
        std::string difficulty = "", std::string displayedConstant = "",
        std::string bpmText = "", std::string searchTags = "",
        std::string pack = "");

  std::string identifier;
  std::string title;
  std::string composer;
  std::string charter;
  bool side;
  long double chartConstant;
  long long baseBPM;

  std::string alias;
  std::string illustrator;
  std::string difficulty;
  std::string displayedConstant;
  std::string bpmText;
  std::string searchTags;
  std::string pack;
};

}  // namespace apkg

#endif  // APKG_RD_CHART
