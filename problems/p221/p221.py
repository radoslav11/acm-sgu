def solve_one_parity(n, k, p):
    per_diag = []
    for i in range(p, n, 2):
        per_diag.append(i + 1)
        if i != n - 1:
            per_diag.append(i + 1)

    dp = [[0] * (k + 1) for _ in range(1 + len(per_diag))]

    dp[0][0] = 1
    for i, v in enumerate(per_diag, 1):
        dp[i][0] = 1
        for j in range(1, min(k, v) + 1):
            dp[i][j] = dp[i - 1][j] + dp[i - 1][j - 1] * (v - j + 1)

    return dp


def solve(n, k):
    if k > 2 * n - 1:
        return 0

    ans = 0
    data_0 = solve_one_parity(n, k, 0)
    data_1 = solve_one_parity(n, k, 1)
    for i in range(0, k + 1):
        ans += data_0[-1][i] * data_1[-1][k - i]

    return ans


n, k = map(int, input().split())
print(solve(n, k))
