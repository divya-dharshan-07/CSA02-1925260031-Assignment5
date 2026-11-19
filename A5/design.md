# A5 – File Storage Design

Donor records are human-readable text, one record per line using fprintf; validate delimiters/field lengths when parsing. Stock records are fixed-size structs suitable for fwrite/fread. Binary storage is compact and supports direct record addressing but is less portable across different endianness, padding and type representations.

For stock record index i: fseek(fp, (long)(i*sizeof(Stock)), SEEK_SET); fread one Stock; change units; fseek back by -(long)sizeof(Stock) from current position; fwrite one Stock. Check every return value. Fixed-size binary records permit O(1) direct update; rewriting a text file requires scanning/reformatting O(n) records. Binary file size is approximately n*sizeof(Stock); text size depends on digits and separators.
