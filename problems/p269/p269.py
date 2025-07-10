def main():
    _, k = map(int, input().split())
    b = sorted(list(map(int, input().split())))

    dp = [0] * (k + 1)
    dp[0] = 1

    for v in b:
        for i in range(k, 0, -1):
            dp[i] += dp[i - 1] * (v - i + 1)

    print(dp[k])

if __name__ == "__main__":
    main()
