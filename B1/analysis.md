# B1 – Recursive Functions and Instrumentation

The completed halving-power function makes exactly one recursive call for exponent/2, stores that result, squares it, and multiplies by the base only for odd exponents. For 2^30 the linear version returns 1073741824 in 31 calls and the halving version returns the same value in 6 calls.

For linear recursion C(n)=C(n-1)+1 with C(0)=1, so C(n)=n+1 and stack depth is O(n). For halving recursion C(n)=C(floor(n/2))+1 with C(0)=1; for n>=1 this is floor(log2 n)+2 calls and O(log n) stack depth. The program uses base 1 for the large call-count experiments to avoid arithmetic overflow.

The deliberate two-branch variant computes the same half-power twice. Its recurrence is C(n)=2C(floor(n/2))+1, giving O(n) calls rather than O(log n); memoizing/reusing the first result is the efficient solution. Iterative square-and-multiply retains O(log n) time with O(1) auxiliary space. Find the linear recursion crash threshold only in a controlled local test; the threshold depends on compiler, stack limit and machine. Never record a guessed threshold.
