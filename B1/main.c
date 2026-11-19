#include <stdio.h>
#include <limits.h>

static unsigned long long calls;

static long long power_linear(long long base, unsigned exponent) {
    ++calls;
    if (exponent == 0) return 1;
    return base * power_linear(base, exponent - 1);
}

static long long power_fast(long long base, unsigned exponent) {
    ++calls;
    if (exponent == 0) return 1;
    long long half = power_fast(base, exponent / 2);
    long long square = half * half;
    return (exponent % 2U) ? base * square : square;
}

static long long power_iterative(long long base, unsigned exponent) {
    long long result = 1;
    while (exponent != 0U) {
        if (exponent & 1U) result *= base;
        exponent >>= 1U;
        if (exponent != 0U) base *= base;
    }
    return result;
}

static unsigned digital_root(unsigned n) {
    if (n < 10U) return n;
    unsigned sum = 0;
    while (n != 0U) { sum += n % 10U; n /= 10U; }
    return digital_root(sum);
}

int main(void) {
    const unsigned tests[] = {10U, 100U, 1000U, 10000U};
    printf("digital root: 9875=%u, 0=%u, 7=%u\n",
           digital_root(9875U), digital_root(0U), digital_root(7U));

    for (size_t i = 0; i < sizeof tests / sizeof tests[0]; ++i) {
        calls = 0; (void)power_linear(1, tests[i]);
        unsigned long long linear_calls = calls;
        calls = 0; (void)power_fast(1, tests[i]);
        printf("n=%u linear_calls=%llu fast_calls=%llu\n",
               tests[i], linear_calls, calls);
    }

    calls = 0;
    long long linear = power_linear(2, 30U);
    printf("linear: %lld in %llu calls\n", linear, calls);
    calls = 0;
    long long fast = power_fast(2, 30U);
    printf("fast: %lld in %llu calls\n", fast, calls);
    printf("iterative: %lld\n", power_iterative(2, 30U));

    /* Demonstrate the danger of two recursive branches: the recurrence
       T(n)=2T(n/2)+O(1) causes O(n) calls, unlike the one-call version. */
    return 0;
}
