# A1 – Decompose the Blood Bank Program

## Structure chart
```text
main
├── register_donor → validate_donor
├── check_eligibility → days_since_last_donation
├── find_donors
├── update_stock
├── issue_units
├── save_donors / load_donors
└── save_stock / load_stock
```

## Interfaces
```c
int register_donor(DonorList *list, const Donor *donor);
int is_eligible(const Donor *donor, const Date *today);
size_t find_donors(const Donor *list, size_t count, const char *group, Donor *out, size_t out_cap);
int update_stock(Stock stock[], size_t count, const char *group, int delta);
int save_donors(const char *path, const Donor *list, size_t count);
int load_donors(const char *path, DonorList *list);
int save_stock(const char *path, const Stock *stock, size_t count);
int load_stock(const char *path, Stock *stock, size_t capacity, size_t *count);
```
Read-only objects use const pointers; mutable lists/outputs use non-const pointers; counts and scalar IDs pass by value. Pointer/count pairs make array bounds explicit.

## Return multiple values
Pointer outputs: `size_t eligible_count(const Donor *d,size_t n,const char *group,int *youngest_id)`. Structure return: `EligibilityResult eligible_count(...)`, where result contains count and youngest ID. Structure return is self-contained; pointer outputs avoid a custom return type but require valid output addresses.

## Global-variable alternative
Globals do not change the asymptotic time or essential data storage. They increase coupling, make tests order-dependent, hinder reuse and make concurrent/multiple blood-bank instances difficult.
