#if !defined(APKG_RD_CHART)
#define APKG_RD_CHART

#include <string>

namespace apkg {

struct chart {
 public:
  chart() = delete;
  chart(std::string _identifier, std::string _title, std::string _composer,
        std::string _charter, bool _side, long double _chartConstant,
        long long _baseBPM, std::string _alias = "",
        std::string _illustrator = "", std::string _difficulty = "",
        std::string _displayedConstant = "", std::string _bpmText = "",
        std::string _searchTags = "", std::string _pack = "");

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
