import sys
from math import gcd, isqrt


def mod_exp(base, exp, mod):
    result = 1
    base %= mod
    while exp > 0:
        if exp & 1:
            result = (result * base) % mod
        base = (base * base) % mod
        exp >>= 1
    return result


def extended_gcd(a, b):
    if b == 0:
        return (a, 1, 0)
    g, x1, y1 = extended_gcd(b, a % b)
    x = y1
    y = x1 - (a // b) * y1
    return (g, x, y)


def inv_mod(a, m):
    g, x, _ = extended_gcd(a, m)
    if g != 1:
        return None
    return x % m


def factorize(n):
    factors = []
    lim = isqrt(n) + 1
    for i in range(2, lim):
        if n % i == 0:
            factors.append(i)
            while n % i == 0:
                n //= i
    if n > 1:
        factors.append(n)
    return factors


def find_primitive_root(p):
    if p == 2:
        return 1

    phi = p - 1
    prime_factors = factorize(phi)

    def is_generator(g):
        for f in prime_factors:
            if mod_exp(g, phi // f, p) == 1:
                return False
        return True

    MAX_CANDIDATE = 200000
    for g in range(2, MAX_CANDIDATE):
        if is_generator(g):
            return g
    return None


def baby_step_giant_step(a, b, p):
    if b == 1:
        return 0
    a %= p
    b %= p

    m = isqrt(p - 1) + 2

    baby = {}
    cur = 1
    for j in range(m):
        if cur not in baby:
            baby[cur] = j
        cur = (cur * a) % p

    a_inv_m = mod_exp(a, (p - 1) - m, p)

    giant = b
    for i in range(m):
        if giant in baby:
            return i * m + baby[giant]
        giant = (giant * a_inv_m) % p
    return None


def solve_kth_roots(P, K, A):
    if A == 0:
        return (1, [0])

    d = gcd(K, P - 1)
    if mod_exp(A, (P - 1) // d, P) != 1:
        return (0, [])

    alpha = find_primitive_root(P)
    if alpha is None:
        return (0, [])

    T = baby_step_giant_step(alpha, A, P)
    if T is None:
        return (0, [])

    if T % d != 0:
        return (0, [])

    Tprime = T // d
    M = (P - 1) // d
    Kprime = K // d

    inv_Kprime = inv_mod(Kprime, M)
    if inv_Kprime is None:
        return (0, [])

    L0 = (Tprime * inv_Kprime) % M

    roots = []
    for i in range(d):
        exponent = (L0 + i * M) % (P - 1)
        x = mod_exp(alpha, exponent, P)
        roots.append(x)

    roots = sorted(set(roots))
    return (len(roots), roots)


def main():
    data = sys.stdin.read().split()
    P, K, A = map(int, data)
    count, rts = solve_kth_roots(P, K, A)
    print(count)
    if count > 0:
        print(" ".join(map(str, rts)))


if __name__ == "__main__":
    main()
