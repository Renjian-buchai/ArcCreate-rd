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
  skin: 
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

| index int PK | identifier text | title text | composer text | charter text | alias text | illustrator text | chartConstant real | difficulty\* text | displayedConstant\*\* text | baseBPM int | bpmText text | side text^ | searchTags text |
| ------------ | --------------- | ---------- | ------------- | ------------ | ---------- | ---------------- | ------------------ | ----------------- | -------------------------- | ----------- | ------------ | --------- | --------------- |

\* Difficulty: Only includes the difficulty level, not the displayed chart constant (i.e. Past, Present, Future, Beyond, Eternal)

\*\* displayedConstant: Only includes the displayed chart constant (i.e. 1-12, 7+, 8+, 9+, 10+)

^ side: "light" or "conflict" only. Does not include the actual skin.
