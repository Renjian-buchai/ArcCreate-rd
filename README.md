# Arcpkg-rd

An application to extract the metadata out of .arcpkg files, and export it to a database.

Usage:

```ps
# Make the database directory. It is C:/docs/ by default, on windows.
# If you're using linux, you may need to alter the source code.
mkdir C:/docs/

# General usage: arcpkg-rd <filename>
# To extract the metadata of an example .arcpkg file, package.arcpkg: 
arcpkg-rd package.arcpkg

# Batch usage: arcpkg-rd -dir
# This iterates through every file in the current directory (Non-recursively). This will not work if there is a file that is not an arcpkg file.
# Note that arcpkg files need not have the .arcpkg extension.

# The executable creates a tmp/ directory in its current directory. It can be deleted with `rm -r /tmp/`.
```

## Dependencies

- 7zip
- sqlite3

## Building

- Source files:
  - src/util.cc
  - src/main.cc
  - src/chart.cc
  - src/index.cc
  - src/project.cc
- C++ standard:
  - C++17
- Linked libraries:
  - sqlite3
- Binary dependencies:
  - 7z

**Note, 7zip is run through a system call. It has to be findable in your environment variables.**

No, I'm not proud of this hack, but I was simply bodging through this. In the future, perhaps I can use 7zip's library instead of its application.

## Schema

See [schema.md](./doc/schema.md) for more details

- Mandatory fields
  - identifier
  - title
  - composer
  - charter
  - side
  - base BPM
- Nullable fields
  - chartConstant (Unfortunately)
  - alias
  - illustrator
  - difficulty
  - displayedConstant
  - bpm text
  - search tags
  - pack
