# B4 – Growable Donor List and Engineering Decision

The donor array begins with capacity 2 and doubles whenever size equals capacity. The required sample additions demonstrate capacities 2, 2, 4, 4 and 8. The realloc result is first stored in a temporary pointer: on failure, the original allocation remains valid and can still be freed. The program checks capacity arithmetic, loads donor records from donors_input.txt, creates a nightly eligibility report (90-day demonstration threshold), measures report CPU time and allocated capacity, and frees the list.

For a network expanded to 25 hospitals and 100 times the current donor count:
- Keeping all donors in memory gives fast repeated searches but requires O(N) memory.
- Batch processing reduces working memory to O(batch size), but may require repeated scans and complicates cross-batch matching.
- A sorted on-disk index supports binary search over IDs with lower RAM use, but inserts/updates require index maintenance and disk I/O.

The appropriate choice depends on measured memory, report deadline, update frequency and search pattern. Attach real sanitizer output; a successful tested run is evidence for those test cases, not a proof of all possible inputs.
