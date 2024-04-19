# Schema

## .yml format

### index.yml

```yml
- directory: <string (Chart name, usually)>
  identifier: <string (Charter name, usually)>.<string (Chart name)> 
  version: <int>
  type: <string [level | pack]>
- * 
```

### project.arcproj

Note: An alias for a .yml file.

```yml
lastOpenedChartPart: <string (Difficulty file)>.aff
charts: 
- chartPath: <string (Difficulty file)>.aff
  audioPath: <string (Audio file + extension)> 
  jacketPath: <string (Image file + extension)>
  baseBPM: <int (BPM of the song, before timing points)>
  bpmText: <string (The text that you want to show in the BPM section)>
  syncBaseBPM: <bool (Idk what this is)>
  backgroundPath: <string (Image file + extension)>
  title: <string (Title)>
  composer: <string (Composer of the song, including 'feat.'s and 'vs.'s)>
  charter: <string (Charter name)>
  alias: <string (Charter displayed in game)>
  illustrator: <string (illustrator name)>
  difficulty: <string (Difficulty level, usually [Past | Present | Future | Beyond]) + displayed Chart Constant>
  chartConstant: <float (Actual chart constant, usually to 1 decimal place)>
  difficultyColor: <string (Colour of the background in the difficulty displayed, in RGBA format)>
  skin: <string Anchor>
    side: <string (Chart side, usually [light | conflict])> 
    particles: <string (Particle name)>
    track: <string (Floor name)>
    singleLine: <string (??)>
  lastWorkingTiming: <int (Probably the last time the chart was saved)>
  previewStart: <int (Preview timing start)>
  previewEnd: <int (Preview timing end)>
- *
```

## Database (SQL)

- Columns
  - index
    - Integer, primary key
  - identifier
    - Text, not null
  - title
    - Text, not null
  - composer
    - Text, not null
  - charter
    - Text, not null
  - side
    - Integer, not null
    - 0 (Light) is inputted if unspecified.
  - base BPM
    - Integer, not null
  - chartConstant
    - Integer
    - -100 is inputted if unspecified. In a failed attempt to minimise rounding errors, chart constants are scaled by 100.
  - alias
    - Text
  - illustrator
    - Text
  - difficulty
    - Text
    - This refers to Past, Present, Future, Beyond or whatever is included, to be displayed to the user.
  - displayedConstant
    - Text
    - This refers to the chart constant displayed to the user. This is important as it may vary from the actual chart constant of the chart (e.g., Dropdead FTR's chart constant should be a 9, but it's shown as a 8.)
  - bpm text
    - Text
    - This refers to the BPM displayed to the user. It usually only differs from baseBPM when the actual BPM is a range.
  - search tags
    - Text
  - pack
    - This depends on the pack listed in the .arcpkg.
