# A3 – Pointer Arithmetic, Returned Pointers and CLI

For `int stock[8]; int *p=stock;`:
- `p+3`: address of element 3 (fourth element).
- `*(p+3)`: value at element 3.
- `p[3]`: same value; C defines subscripting as `*(p+3)`.
- `*p++`: read the value currently pointed to, then advance p. Post-increment binds more tightly than unary *.

Illustrative memory diagram (assuming 4-byte int; addresses are examples only):

| Element | Address | Expression |
|---|---:|---|
| stock[0] | 1000 | p |
| stock[1] | 1004 | p+1 |
| stock[2] | 1008 | p+2 |
| stock[3] | 1012 | p+3 |

## Pointer-returning lowest-stock function
```c
int *lowest_stock(int stock[], size_t n) {
    if (n == 0) return NULL;
    int *lowest = &stock[0];
    for (size_t i = 1; i < n; ++i)
        if (stock[i] < *lowest) lowest = &stock[i];
    return lowest;
}
```
This returns a pointer into the caller-owned array, whose lifetime continues after the function returns. Never return the address of a local automatic variable: its lifetime ends on return and the pointer dangles. An index-returning version avoids pointer invalidation if a dynamic array is reallocated; both searches take O(n) time and O(1) extra space.

## Command-line interface
Example invocations: `bloodbank find O+` and `bloodbank issue A- 2`. Check `argc` before reading `argv`; validate command name, blood-group token and positive numeric quantity (use `strtol`, check end pointer and range). On invalid arguments print usage to stderr and return a non-zero status. Never assume argv contains an expected argument.
