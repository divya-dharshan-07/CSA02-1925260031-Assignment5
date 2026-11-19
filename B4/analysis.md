# B4 – Growable Donor List and Nightly Eligibility Report

## Implementation
The donor list stores a pointer, current size and capacity. It begins with capacity 2 and doubles when full. The program demonstrates five additions, producing capacities 2, 2, 4, 4 and 8. The measured allocated capacity is 8 donor slots (96 bytes on the tested OnlineGDB environment).

The growth operation assigns realloc to a temporary pointer. If allocation fails, the original pointer remains available for cleanup. Capacity multiplication is checked before allocation.

## Persistent input and report
For a self-contained OnlineGDB run, the program creates `donors_input.txt` with five sample donor records, then opens and reads that file to populate the list. It writes the nightly result to `eligibility_report.txt`. The demonstration eligibility rule is age 18 through 65 inclusive; this is a simplified assignment test rule, not a claim about a real blood bank's complete donor-screening protocol.

The observed run loaded five donors and reported all five as eligible:
- 7001 Divya (O+), age 20
- 7002 Arun (A+), age 25
- 7003 Kiran (B-), age 30
- 7004 Ravi (AB+), age 22
- 7005 Priya (O-), age 28

## Memory management
The list's allocation is released with free after report generation. On every failure path after allocation, the program also frees the owned list. After successful cleanup, the pointer is set to NULL and size/capacity are reset.

- A memory leak occurs when allocated storage is no longer reachable or is not freed.
- A dangling pointer refers to storage whose lifetime has ended, including storage moved by realloc.
- A double free releases the same allocation more than once.

## Scaling decision
- Keeping all donors in memory gives fast repeated searches but requires O(N) memory.
- Batch processing reduces working memory to O(batch size), but may require repeated scans and complicates cross-batch matching.
- A sorted on-disk index supports lookup with lower RAM use, but inserts and updates require index maintenance and disk I/O.

The appropriate strategy depends on donor count, report deadline, update frequency and search pattern. Attach genuine sanitizer output; a successful run covers only the tested inputs and is not proof against every possible failure.
