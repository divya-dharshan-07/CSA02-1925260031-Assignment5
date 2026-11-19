# A1 – Decompose the District Blood Bank Network

## 1. Functional requirements and structure chart
The system registers donors, checks eligibility, searches by blood group, updates available units, and persists donor/stock records.

```text
main
├── load_donors ── parse_donor_line
├── load_stock
├── register_donor ── validate_donor
├── check_eligibility ── days_between
├── find_donors ── matches_group
├── update_stock ── find_stock_record
├── issue_units ── check_stock
├── save_donors ── write_donor_line
└── save_stock
```

## 2. Function prototypes
```c
int register_donor(DonorList *list, const Donor *donor);
int is_eligible(const Donor *donor, const Date *today);
size_t find_donors(const Donor *list, size_t count,
                   const char *group, Donor *out, size_t out_capacity);
int update_stock(Stock *stock, size_t count,
                 const char *group, int change);
int save_donors(const char *path, const Donor *list, size_t count);
int load_donors(const char *path, DonorList *list);
int save_stock(const char *path, const Stock *stock, size_t count);
int load_stock(const char *path, Stock *stock,
               size_t capacity, size_t *loaded_count);
```

## 3. Parameter decisions
- IDs, counts, ages, and unit changes are small scalar values, so pass them by value.
- Read-only records, strings and arrays use pointers to const data (for example `const Donor *`) to avoid copying large records and prevent accidental modification.
- Operations that modify a list or stock use non-const pointers.
- Arrays are paired with an explicit count/capacity to make bounds visible.
- Output parameters are pointers because the called function must write the result back to the caller.

## 4. Returning two results
**Pointer-output design:** `size_t eligible_count(const Donor *d, size_t n, const char *group, int *youngest_id);` The function returns the count and writes the youngest donor ID through `youngest_id`. Caller must pass a valid output pointer; use a documented sentinel (e.g. -1) when no match exists.

**Structure-return design:** `EligibilityResult eligible_count(...);` where `typedef struct { size_t count; int youngest_id; } EligibilityResult;`. This packages related results and can represent “no donor” with -1. It is easier to extend and harder to mismatch output arguments, while the pointer form avoids defining a result type and works with older interfaces.

## 5. Why not globals?
Global donor arrays and stock counts make every function depend on shared hidden state. This increases coupling, makes unit tests order-dependent, prevents two independent bank instances, complicates concurrency and reduces reuse. Passing the required state explicitly makes dependencies visible and tests repeatable. Globals do not improve algorithmic complexity.
