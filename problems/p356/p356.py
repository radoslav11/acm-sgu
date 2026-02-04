from math import gcd, factorial


def derangements(m):
    """
    Count permutations of m elements with no fixed points.
    """
    if m == 0:
        return 1
    if m == 1:
        return 0
    d_prev2, d_prev1 = 1, 0  # D_0, D_1
    for i in range(2, m + 1):
        d_prev2, d_prev1 = d_prev1, (i - 1) * (d_prev1 + d_prev2)
    return d_prev1


def main():
    # Each matching can be classified by a permutation. Let the secret one
    # be denoted as 1, ..., n. We are interested in the number of permutations
    # p1, ..., pn that match with 1, ..., n in exactly k positions. This is a
    # derangements problem: choose k positions to be fixed points, then the
    # remaining n-k must form a derangement (permutation with no fixed points).
    # The count is C(n, k) * D_{n-k}, where D_m is the m-th derangement number.
    # The probability is C(n, k) * D_{n-k} / n! = D_{n-k} / (k! * (n-k)!).
    #
    # Derangements can be counted via inclusion-exclusion. Let A_i be the set of
    # permutations where i is a fixed point. We want n! - |A_1 U ... U A_n|.
    # By inclusion-exclusion:
    # |A_1 U ... U A_n| = C(n,1) * (n-1)! - C(n,2) * (n-2)! + C(n,3) * (n-3)! - ...
    # So D_n = n! - C(n,1) * (n-1)! + C(n,2) * (n-2)! - ...
    #        = sum_{i=0}^{n} (-1)^i * C(n,i) * (n-i)!
    #        = sum_{i=0}^{n} (-1)^i * n! / i!
    #        = n! * sum_{i=0}^{n} (-1)^i / i!
    #
    # Alternatively, the recurrence D_n = (n-1) * (D_{n-1} + D_{n-2}), with
    # D_0 = 1, D_1 = 0. This follows from considering where element 1 maps: if
    # 1 -> j, then either j -> 1 (giving D_{n-2} ways for the rest) or j maps to
    # something else, equivalent to a derangement of n-1 elements as j and 1 can be
    # treated like one element (D_{n-1} ways).

    k, n = map(int, input().split())
    m = n - k
    d = derangements(m)

    if d == 0:
        print(0)
        return

    numerator = d
    denominator = factorial(k) * factorial(m)

    g = gcd(numerator, denominator)
    numerator //= g
    denominator //= g

    print(f"{numerator}/{denominator}")


if __name__ == "__main__":
    main()
