# A4 – Growable Donor List

A list stores `Donor *data`, `size` and `capacity`. Initially data=NULL, size=0, capacity=0. Before appending, if size==capacity choose new capacity 2 when empty or capacity*2 otherwise.

Pseudocode:
```text
add_donor(list, donor):
    if list.size == list.capacity:
        new_capacity = (list.capacity == 0) ? 2 : 2 * list.capacity
        temporary = realloc(list.data, new_capacity * sizeof(Donor))
        if temporary == NULL:
            return failure       // old allocation is still valid
        list.data = temporary
        list.capacity = new_capacity
    list.data[list.size] = donor
    list.size = list.size + 1
    return success
```
Always assign realloc to a temporary pointer. If realloc fails, it returns NULL and the original block remains allocated; assigning directly to the only pointer would lose the original address and leak the block. Check multiplication/capacity overflow before allocation in production code.

- **Memory leak:** allocated storage becomes unreachable or is never freed. Free the owning pointer exactly once.
- **Dangling pointer:** pointer refers to freed or moved storage. Do not retain aliases across free/realloc; reacquire after growth.
- **Double free:** same allocation freed twice. Keep one clear owner and set it to NULL after freeing.

With doubling, append is amortized O(1), and n appends copy O(n) elements in total. Fixed increments of 10 may copy O(n²) elements over n appends, though unused capacity is at most 9. Doubling can leave close to half the allocated slots unused immediately after growth, trading memory slack for fewer reallocations.
