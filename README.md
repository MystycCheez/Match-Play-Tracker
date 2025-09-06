# Match Play Tracker

Program for keeping track of Goldeneye Match Play matches.  
Designed as an alternative to Google Sheets or notepad, etc.  
Can export to BBCode for posting to [The-Elite.net forums](https://forums.the-elite.net/).

## Disclaimer

Program still in early development, use at your own risk!  
There are still many unimplemented features and functionality that may not work as expected.

## Installation & Usage

Download the latest release via the Releases section on Github.  
Unzip and run tracker.exe.

## Hotkeys

As of now, functions are used via hotkey. This is a small list of hotkeys:

| Hotkey        | Functionality                                                    | Alt Hotkey |
| ------------- | ---------------------------------------------------------------- | ---------- |
| Ctrl + Del    | Clears times (Warning: Does not save!)                           | Ctrl + D   |
| Ctrl + L      | Loads times from times/times.txt (Clears times!)                 |            |
| Ctrl + S      | Saves current times to times/times.txt                           |            |
| Ctrl + B      | Exports times to times/results.bbcode (also copies to clipboard) |            |
| Ctrl + Tab    | Switch levels between PD/GE                                      |            |
| Alt  + C      | Switch veto text color between white and gold                    |            |

An extensive list can be found in [ACTIONS.md](https://github.com/MystycCheez/Match-Play-Tracker/blob/main/ACTIONS.md)  
The action list is also auto-generated and found in the main folder as ACTIONS.md

## Misc

Entering # followed by a six-digit hex code will change that cell's text color

## Build Instructions

Run ```clang -o nob.exe nob.c```  
Run ```nob.exe```

Running nob.exe will compile and run tracker.exe located in the build folder.  

## Used Libraries

* [Raylib](https://www.raylib.com/)
* [Nob.h](https://github.com/tsoding/nob.h)

## References

* [Match Play Spreadsheet](https://docs.google.com/spreadsheets/d/1-aDvgb4x5IuDA9IEChYfun8wZ0e-Zp0XMfhFdTRXf1Q/edit?gid=2120665130#gid=2120665130)
* [Rankings page for visual reference](https://rankings.the-elite.net/)
* [Elite Logo Kit](https://www.dropbox.com/scl/fo/nv1mbqgpwcz3vqcrxa1hc/AGoagjyz8FOr2MS_G1_7034?rlkey=1a6avrc02cbybms9qg93rcrs0&e=1&dl=0)

## Code References (non-exhaustive list)
### X-Macro
* [Wikipedia](https://en.wikipedia.org/wiki/X_macro)
* [Stack Overflow](https://stackoverflow.com/questions/6635851/real-world-use-of-x-macros)
### Other
* [Memory Leaks](https://stackoverflow.com/questions/9074229/detecting-memory-leaks-in-c-programs)