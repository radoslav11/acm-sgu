def extended_gcd(a: int, b: int) -> tuple[int, int, int]:
    if b == 0:
        return a, 1, 0
    gcd, x1, y1 = extended_gcd(b, a % b)
    x = y1
    y = x1 - (a // b) * y1
    return gcd, x, y


def solve_problem(x1: int, x2: int, p: int, k: int) -> None:
    g, p1, p2 = extended_gcd(x1, x2)

    if p % g != 0:
        print("NO")
        return

    dx = x2 // g
    dy = x1 // g
    p1 *= p // g
    p2 *= p // g

    while abs(p1 + dx) + abs(p2 - dy) < abs(p1) + abs(p2):
        p1 += dx
        p2 -= dy
    while abs(p1 - dx) + abs(p2 + dy) < abs(p1) + abs(p2):
        p1 -= dx
        p2 += dy

    if abs(p1) + abs(p2) > k:
        print("NO")
        return

    n1, n2 = 0, 0
    last = k - abs(p1) - abs(p2)
    if last % 2 == 0:
        if p1 < 0:
            n1 = -p1
            p1 = 0
        if p2 < 0:
            n2 = -p2
            p2 = 0
        p1 += last // 2
        n1 += last // 2
    else:
        if (dx + dy) % 2 == 0:
            print("NO")
            return
        if abs(p1 + dx) + abs(p2 - dy) < abs(p1 - dx) + abs(p2 + dy):
            p1 += dx
            p2 -= dy
        else:
            p1 -= dx
            p2 += dy
        if abs(p1) + abs(p2) > k:
            print("NO")
            return
        last = k - abs(p1) - abs(p2)
        if p1 < 0:
            n1 = -p1
            p1 = 0
        if p2 < 0:
            n2 = -p2
            p2 = 0
        p1 += last // 2
        n1 += last // 2

    print("YES")
    print(p1, n1, p2, n2)


def main():
    x1, x2, p, k = map(int, input().split())
    solve_problem(x1, x2, p, k)


if __name__ == "__main__":
    main()
