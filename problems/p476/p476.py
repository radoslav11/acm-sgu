import sys

sys.set_int_max_str_digits(100000)


def main():
    # We solve this with inclusion-exclusion principle. If k = 0, the answer is
    # simply (3n)!/((3!)^n * n!) - think of all permutations and then there are
    # 3! ways to rearrange each triplet and n! ways to rearrange the triplets.
    # If k > 0, we can look at all the banned triplets and subtract the count
    # given that we surely include some triplet. However, we then have some overlap
    # so we subtract an answer more than once. This yields the inclusion-exclusion
    # solution. We should only be careful about the overlap between the triplets.
    # We implement this in python as otherwise we would need to use a big integers.
    #
    # Calculating this naively would take O(2^k * k) time, but due to the bit integers
    # it will actually be O(2^k * k * n) in practice. To speed it up, we calculate the
    # coefficients of the inclusion-exclusion and do the actual calculation for each cnt
    # only once as (here coef[cnt] is the sum of the inclusion-exclusion (-1)^|mask|):
    # coef[cnt] * (3n - cnt)! / ((3!)^(n - cnt // 3) * (n - cnt // 3)!) in the end.
    #
    # The complexity this way is O(2^k * k + n^2) which is feasible for k <= 20 and n <= 10^3.
    # Depending on implementation, bitmasks might be a bit slow, so we do this via a backtracking.

    n, k = map(int, input().strip().split())
    triplets = [list(map(int, input().strip().split())) for _ in range(k)]

    factorial = [1] * (3 * n + 1)
    power_6 = [1] * (3 * n + 1)
    for i in range(1, 3 * n + 1):
        factorial[i] = factorial[i - 1] * i
        power_6[i] = power_6[i - 1] * 6

    used = [False] * (3 * n)
    coef = [0] * (3 * n + 1)

    def rec(pos, cnt, p):
        if pos == k:
            coef[cnt] += p
            return

        # Don't include triplet at pos.
        rec(pos + 1, cnt, p)

        # Include triplet at pos.
        has_conflict = False
        to_undo = []
        for j in triplets[pos]:
            if used[j - 1]:
                has_conflict = True
                break

            used[j - 1] = True
            to_undo.append(j - 1)

        if not has_conflict:
            rec(pos + 1, cnt + 3, -p)

        for j in to_undo:
            used[j] = False


    # Faster than iterating all masks.
    rec(0, 0, 1)

    ans = 0
    for cnt in range(0, 3 * n + 1):
        if coef[cnt] != 0:
            assert cnt % 3 == 0, "cnt must be a multiple of 3"
            ans += (
                coef[cnt]
                * factorial[3 * n - cnt]
                // (power_6[n - cnt // 3] * factorial[n - cnt // 3])
            )

    print(ans)


if __name__ == "__main__":
    main()
