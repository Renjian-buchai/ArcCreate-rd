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

std::string apkg::chart::_sanitise(std::string& unSafe) {
  return std::regex_replace(unSafe, std::regex("\'"), "\'\'");
}

void apkg::chart::sanitise() {
  identifier = _sanitise(identifier);
  title = _sanitise(title);
  composer = _sanitise(composer);
  charter = _sanitise(charter);

  alias = _sanitise(alias);
  illustrator = _sanitise(illustrator);
  displayedConstant = _sanitise(displayedConstant);
  bpmText = _sanitise(bpmText);
  searchTags = _sanitise(searchTags);
  pack = _sanitise(pack);
}

std::string apkg::chart::querify() {
  std::string tables =
      "identifier, title, composer, charter, chartConstant, baseBPM, side";
  std::string values =
      "\'" + identifier + "\', \'" + title + "\', \'" + composer + "\', \'" +
      charter + "\', " + std::to_string(static_cast<int>(chartConstant * 100)) +
      ", " + std::to_string(baseBPM) + ", " + std::to_string(side);

  if (alias != "" && alias != "''''") {
    tables += ", alias";
    values += ", \'" + alias + "\'";
  }
  if (illustrator != "" && illustrator != "''''") {
    tables += ", illustrator";
    values += ", \'" + illustrator + "\'";
  }
  if (difficulty != "" && difficulty != "''''") {
    tables += ", difficulty";
    values += ", \'" + difficulty + "\'";
  }
  if (displayedConstant != "" && displayedConstant != "''''") {
    tables += ", displayedConstant";
    values += ", \'" + displayedConstant + "\'";
  }
  if (bpmText != "" && bpmText != "''''") {
    tables += ", bpmText";
    values += ", \'" + bpmText + "\'";
  }
  if (searchTags != "" && searchTags != "''''") {
    tables += ", searchTags";
    values += ", \'" + searchTags + "\'";
  }
  if (pack != "" && pack != "''''") {
    tables += ", pack";
    values += ", \'" + pack + "\'";
  }

  return "INSERT INTO main.charts (" + tables + ") VALUES (" + values + ");";
}
