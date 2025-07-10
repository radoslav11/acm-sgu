def find_monthly_payment(s, m, p):
    if p == 0:
        return s / m

    r = p / 100
    numerator = r * pow(1 + r, m)
    denominator = pow(1 + r, m) - 1
    x = s * numerator / denominator
    return x


def main():
    s, m, p = map(int, input().split())
    x = find_monthly_payment(s, m, p)
    print(f"{x:.5f}")


if __name__ == "__main__":
    main()
