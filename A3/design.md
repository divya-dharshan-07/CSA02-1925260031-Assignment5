# A3 – Pointer Design

For `int stock[8]; int *p=stock;`:
- `p+3`: address of stock[3].
- `*(p+3)` and `p[3]`: value in stock[3].
- `*p++`: value at current p, then advance p by one int (post-increment binds to p).

Memory diagram (illustrative addresses): p→stock[0] at 1000; stock[1] 1004; stock[2] 1008; stock[3] 1012. Actual addresses depend on platform.

```c
int *lowest_stock(int stock[], size_t n) {
    if (n == 0) return NULL;
    int *lowest = &stock[0];
    for (size_t i=1; i<n; ++i)
        if (stock[i] < *lowest) lowest = &stock[i];
    return lowest;
}
```
Returning a pointer to a local automatic variable is invalid after the function returns. Returning an index is safer if the array may be reallocated; both searches are O(n) time/O(1) extra space.

CLI design: `bloodbank find O+`, `bloodbank issue A- 2`; validate argc, command, group and positive quantity, print usage and return nonzero for invalid input.
