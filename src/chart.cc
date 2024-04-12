#include "../include/chart.hh"

#include <algorithm>
#include <string>

apkg::chart::chart(std::string _identifier, std::string _title,
                   std::string _composer, std::string _charter, uint8_t _side,
                   long double _chartConstant, unsigned long long _baseBPM,
                   std::string _alias, std::string _illustrator,
                   std::string _difficulty, std::string _displayedConstant,
                   std::string _bpmText, std::string _searchTags,
                   std::string _pack)
    : identifier(std::move(_identifier)),
      title(std::move(_title)),
      composer(std::move(_composer)),
      charter(std::move(_charter)),
      side(_side),
      chartConstant(_chartConstant),
      baseBPM(_baseBPM),
      alias(_alias),
      illustrator(_illustrator),
      difficulty(_difficulty),
      displayedConstant(_displayedConstant),
      bpmText(_bpmText),
      searchTags(_searchTags),
      pack(_pack) {}
