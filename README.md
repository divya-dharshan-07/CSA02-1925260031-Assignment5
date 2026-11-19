# C Programming – Assignment 5 (CO5)
**Student:** Divya Dharshan | **Register number:** 1925260031  
**Programme:** B.Tech AIML | **Institution:** SIMATS Engineering  
**Compiler:** GCC (record the exact version from `gcc --version`).

## Assignment theme
District Blood Bank Network – modular program design, recursion, pointers, dynamic memory allocation and persistent file handling.

## Repository contents
- `A1/`–`A5/`: Part A design, pseudocode, function interfaces, call traces and engineering explanations.
- `B1/`: digital root, linear/halving power, iterative square-and-multiply and recursive call-count experiments.
- `B2/`: command-line validation, bounded scanf/fgets input comparison and file-open error handling.
- `B3/`: ten-donor text/binary verification, file size and read/write timings, stock update and 50,000th donor update comparison.
- `B4/`: realloc-based donor list, donor file loading and nightly eligibility report.

## Compile and run (PowerShell)
Open a task folder in VS Code terminal, then:
```powershell
gcc --version
gcc -std=c11 -Wall -Wextra -pedantic main.c -o main.exe
.\\main.exe
```
B2 needs a command-line name and two interactive inputs:
```powershell
.\\main.exe Divya
.\\main.exe ThisNameIsMuchTooLong
```
B4 must be run from its folder so `donors_input.txt` is found.
AddressSanitizer, if supported by the installed GCC:
```powershell
gcc -std=c11 -Wall -Wextra -fsanitize=address -g main.c -o main.exe
.\\main.exe
```

## Evidence to capture locally
The `screenshots/` folders are placeholders. Add genuine terminal screenshots showing compile commands, outputs, invalid-input tests and sanitizer runs. Record the exact GCC version, machine-specific recursion crash threshold (if tested), measured read/write/update timings and actual file sizes. Do not invent measurements; results vary by machine and filesystem. Keep generated `donors.txt`, `donors.bin`, `stock.bin` and test artifacts if required by the faculty rubric.

## Submission checklist
- [ ] Run all four B programs locally and fix any compiler/runtime errors.
- [ ] Capture genuine screenshots in each B task's `screenshots/` folder.
- [ ] Record GCC version and explain observed timing/size results in analysis files.
- [ ] Run AddressSanitizer (or Valgrind where available) and include its output.
- [ ] Make progressive Git commits for each task and evidence update.
