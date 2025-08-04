import sys

sys.set_int_max_str_digits(100000)


def solve(K):
    # Find the smallest positive integer N such that N has exactly K divisors.
    #
    # The key insight is that if N = p1^a1 * p2^a2 * ... * pm^am, then
    # the number of divisors is (a1+1) * (a2+1) * ... * (am+1).
    #
    # So we need to find all factorizations of K = b1 * b2 * ... * bm where
    # each bi >= 2, and then N = p1^(b1-1) * p2^(b2-1) * ... * pm^(bm-1)
    # where p1, p2, ..., pm are the first m primes.
    #
    # We use Python for potentially large integers when K is prime, as the
    # answer would be 2^(K-1) which can be astronomically large.
    #
    # This approach is fast because there aren't that many factorizations
    # of a number <= 10^5. In particular, an easy upper bound is the log(K)-th Bell 
    # number equal to ~1e9 for 16. However, this is only possible for K = 2^c, in 
    # which case the factors are the same and if we don't generate them multiple times
    # we significantly reduce the above figure. For example if K = 2^c a tighter upper
    # bound for the number of unique factorizations is only 2^(c-1) - think of this as 
    # setting the separators between the groups.

    if K == 1:
        return 1

    # Generate enough primes (we won't need more than log2(K) primes)
    # since the smallest factorization would be 2 * 2 * ... * 2
    max_primes_needed = K.bit_length()
    primes = []
    is_prime = [True] * (max_primes_needed * 10)  # Generous upper bound

    for i in range(2, len(is_prime)):
        if is_prime[i]:
            primes.append(i)
            if len(primes) >= max_primes_needed:
                break
            for j in range(i * i, len(is_prime), i):
                is_prime[j] = False

    # Find all ordered factorizations of K using iterative approach
    # Each factorization represents exponents+1 for our prime factorization
    factorizations = []

    # Stack: (remaining_value, current_factorization, min_factor)
    stack = [(K, [], 2)]

    while stack:
        remaining, current, min_factor = stack.pop()

        if remaining == 1:
            factorizations.append(current[::-1])
            continue

        # Try all factors from min_factor to remaining
        # This ensures we generate factorizations in non-increasing order
        for factor in range(min_factor, remaining + 1):
            q, r = divmod(remaining, factor)
            if r == 0:
                new_current = current + [factor]
                stack.append((q, new_current, factor))

    # For each factorization, compute the corresponding N
    min_n = None

    for factors in factorizations:
        # factors = [b1, b2, ..., bm] where K = b1 * b2 * ... * bm
        # N = p1^(b1-1) * p2^(b2-1) * ... * pm^(bm-1)

        assert len(factors) <= len(primes)

        n = 1
        for i, factor in enumerate(factors):
            n *= primes[i] ** (factor - 1)

            # Early termination if n is already too large
            if min_n is not None and n >= min_n:
                break

        if min_n is None or n < min_n:
            min_n = n

    return min_n


def main():
    K = int(input().strip())
    print(solve(K))


if __name__ == "__main__":
    main()
