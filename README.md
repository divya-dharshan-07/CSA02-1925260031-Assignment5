# C Programming – Assignment 5 (CO5)
**Student:** Divya Dharshan | **Register number:** 1925260031  
**Programme:** B.Tech AIML | **Institution:** SIMATS Engineering  
**Compiler:** GCC (record the exact version from `gcc --version`).

## Assignment theme
District Blood Bank Network – modular program design, recursion, pointers, dynamic memory allocation and persistent file handling.

## Repository contents
- `A1/`–`A5/`: Part A design, pseudocode, interfaces, traces and engineering explanations.
- `B1/`: recursive digital root and power implementations with call-count experiment.
- `B2/`: bounded input, command-line validation and file-open error handling.
- `B3/`: donor text/binary persistence, round-trip validation, timing and in-place stock update.
- `B4/`: growable donor list using realloc and memory-management decision.

## Compile and run (PowerShell)
Open the required task folder in VS Code terminal:
```powershell
gcc --version
gcc -std=c11 -Wall -Wextra -pedantic main.c -o main.exe
.\main.exe
```
B2 requires one donor-name argument:
```powershell
.\main.exe Divya
.\main.exe ThisNameIsMuchTooLong
```
AddressSanitizer (if supported by the installed GCC):
```powershell
gcc -std=c11 -Wall -Wextra -fsanitize=address -g main.c -o main.exe
.\main.exe
```

## Evidence still to capture locally
Add genuine terminal screenshots under each B task's `screenshots/` folder. Record the actual GCC version, execution outputs, sanitizer result, machine-specific recursion limit (if tested), and measured read/write/update timings. Timings vary by machine and filesystem; do not invent or copy sample measurements. For B3, retain the generated `donors.txt`, `donors.bin`, and `stock.bin` test artifacts if required by the faculty rubric.

## Progressive commits
Commit each task's design/code and then its test evidence separately as work progresses. This repository content is a prepared implementation; local execution and authentic screenshots/measurements are required before submission.
