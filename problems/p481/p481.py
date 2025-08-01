import sys
sys.set_int_max_str_digits(100000)


def C(n, k):
    if k < 0 or n < k:
        return 0

    k = min(k, n - k)
    ret = 1
    for i in range(n - k + 1, n + 1):
        ret *= i

    for i in range(1, k + 1):
        ret //= i

    return ret


def cnt_graphs(n, m):
    # Let's consider the number of graphs without the condition for
    # number of connected components being 1.
    #
    # We can simply think of the adjacency matrix, and figure out
    # how we can have exactly n cells in it active. Note that it's
    # bidirectional and there are no self loops, so we can simply
    # look at the area above the diagonal. This is (n - 1) * n / 2 cells
    # and we want to activate n of them. This is a simply a combination
    # C((n - 1) * n / 2, n), but we need long integers in this problem,
    # so we use python.
    return C(n * (n - 1) // 2, m)


def solve_slow_dp(N):
    # dp[num_vertices][num_edges] - connected graphs satisfying this.
    # We can compute this by getting all graphs cnt_graphs(num_vertices, num_edges),
    # and subtracting the disconnected ones. This can be done by selecting vertex 1
    # and taking a look at the component it is in.

    dp = [[0 for _ in range(N + 1)] for _ in range(N + 1)]

    for n in range(1, N + 1):
        for m in range(0, N + 1):
            dp[n][m] = cnt_graphs(n, m)
            for size_of_1 in range(1, n):
                for edges_in_1 in range(0, m + 1):
                    # Multiply by C(n, size_of_1 - 1) is to select the other vertices
                    # in the connected component.
                    dp[n][m] -= (
                        dp[size_of_1][edges_in_1]
                        * C(n - 1, size_of_1 - 1)
                        * cnt_graphs(n - size_of_1, m - edges_in_1)
                    )

    return dp[N][N]


def solve_quick_dp(N):
    # Using the above DP, we can find the actual sequence:
    # https://oeis.org/A057500

    f = 1
    for i in range(3, N):
        f *= i

    ans = f
    for i in range(N - 1, 2, -1):
        f *= N
        f //= N - i
        ans += f

    return ans


def main():
    N = int(input().strip())
    print(solve_quick_dp(N))

    # print(",".join([str(solve_slow_dp(i)) for i in range(N)]))


if __name__ == "__main__":
    main()
