# B2 – Adversarial Input and Error Handling

Run the program with no argument, extra arguments, a normal name, and an overlong name. It checks argc before accessing argv and rejects a name that cannot fit the destination array. The missing-file case calls perror so the operating system reports the cause.

The source uses fgets for a bounded line read. Compare it with scanf("%15s", buffer): a field width prevents overflow but scanf stops at whitespace and reads only one token; fgets accepts spaces and reads at most buffer-size minus one characters, so the program checks whether the newline arrived and drains any excess input. Never demonstrate unbounded scanf("%s") against an overlong input in a normal run because that can corrupt memory and has undefined behaviour.

Compile with AddressSanitizer where supported:
`gcc -std=c11 -Wall -Wextra -fsanitize=address -g main.c -o main.exe`.
A clean sanitizer run is evidence that the tested paths showed no detected memory error, not a proof that every input is safe. getline (POSIX, not ISO C) can grow its buffer for arbitrarily long lines; its caller must free the allocated buffer.
