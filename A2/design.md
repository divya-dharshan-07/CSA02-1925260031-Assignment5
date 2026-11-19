# A2 – Recursive Design

Digital root recursive design: if n<10 return n; otherwise return digital_root(sum_digits(n)). Base case is one digit; recursive case reduces digit sum. For positive n, digital root is also 1+(n−1)%9 (with 0 handled separately).

Power: if exponent==0 return 1; compute h=power(base, exponent/2), result=h*h; if exponent is odd multiply by base. This uses one recursive call per level. Linear power makes n+1 calls; halving power makes floor(log2 n)+1 calls. Recurrence for linear calls C(n)=C(n−1)+1; halving C(n)=C(floor(n/2))+1. Missing base case causes unbounded recursion/stack overflow. Use iteration when depth is large or stack use must be constant.

Example trace: power(2,5) → power(2,2) → power(2,1) → power(2,0), then unwind 1,2,4,32. Digital root 9875: 9+8+7+5=29; 2+9=11; 1+1=2.
