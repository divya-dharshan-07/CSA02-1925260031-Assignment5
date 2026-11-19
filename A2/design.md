# A2 – Recursive Algorithms

## Digital root
```text
digital_root(n):
    n = absolute value of n
    if n < 10: return n
    sum = 0
    while n > 0:
        sum = sum + n % 10
        n = n / 10
    return digital_root(sum)
```
Base case: a single digit (0–9). Recursive case: sum the digits and call again. Example: 9875 → 29 → 11 → 2. For a non-negative integer the arithmetic alternative is 0 for n=0, otherwise 1+(n−1)%9.

## Exponentiation by halving
```text
power(base, n):
    if n == 0: return 1
    half = power(base, floor(n/2))
    answer = half * half
    if n is odd: answer = answer * base
    return answer
```
Call trace for power(2,5): power(2,5) → power(2,2) → power(2,1) → power(2,0). Unwind: 1 → 2 → 4 → 32. Only one recursive call is made at each level.

## Call-count recurrences
Linear recursion: C(0)=1; C(n)=C(n−1)+1, so C(n)=n+1 and time/stack are O(n). Halving recursion: C(0)=1; C(n)=C(floor(n/2))+1, so calls and stack depth are O(log n). The recursive digital-root routine repeatedly reduces the number of digits; digit summation costs O(number of digits) per level.

## Risks and alternatives
A missing base case or an input that does not approach it causes unbounded recursion and stack overflow. Very large n can also exhaust stack in linear recursion. Iterative repeated multiplication is O(n) time/O(1) space; iterative square-and-multiply is O(log n) time/O(1) space. Formula-based digital root is O(1), but the recursive version is useful for demonstrating decomposition. C signed integer overflow is undefined, so power tests must stay within the chosen integer type's range.
