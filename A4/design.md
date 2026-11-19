# A4 – Dynamic Memory Design

DonorList holds Donor *data, size and capacity. Start capacity at 2; when size==capacity, calculate new_capacity=(capacity==0?2:capacity*2). Store realloc result in a temporary pointer. If NULL, return failure while preserving the old allocation; otherwise assign data and capacity, then append and increment size.

Memory errors:
- Leak: overwrite the only pointer with a new allocation or omit free; retain ownership and free once.
- Dangling pointer: keep an alias after free/realloc relocation; reset aliases and reacquire pointers.
- Double free: free the same allocation twice; centralize ownership and set pointer NULL after free.

Doubling gives amortized O(1) append and O(n) total copied elements across n appends. Fixed increments of 10 can cause O(n²) total copying. Doubling can leave nearly half the capacity unused; fixed increments leave at most 9 slots unused but copy more often.
