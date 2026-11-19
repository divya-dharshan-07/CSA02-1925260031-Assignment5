# A5 – Text and Binary Persistence

## Format choice
Store donor details as text, one donor per line, using `fprintf`. Text is readable and easier to inspect, exchange and debug; parsing must validate field count, delimiters and buffer lengths. Store stock as fixed-size records using `fwrite`/ `fread`; fixed record size permits direct positioning. Raw C structs are not universally portable because padding, endianness and integer representation can differ.

## In-place binary update pseudocode
```text
open stock file in "r+b"
if open fails: report error and stop
offset = record_index * sizeof(Stock)
if fseek(file, offset, SEEK_SET) fails: report error
if fread(&record, sizeof record, 1, file) != 1: report invalid index/read error
record.units = new_units
if fseek(file, -(long)sizeof(Stock), SEEK_CUR) fails: report error
if fwrite(&record, sizeof record, 1, file) != 1: report write error
close file and check close status
```
The index must be within the number of records; check multiplication and conversion to long before fseek. Check every I/O return value.

Binary update is O(1) record access for fixed-size records and changes only the selected record. Updating a text record generally requires reading and rewriting the file (often through a temporary file), O(n) work. Binary files are compact and fast for same-program access; text files are more portable and human-readable. Binary size is approximately count × sizeof(Stock); text size varies with digits, labels and separators.
