def first_primes(t):
    p, primes = 2, []
    while len(primes) < t:
        is_prime = True
        for x in primes:
            if x * x > p:
                break
            if p % x == 0:
                is_prime = False
                break
        if is_prime:
            primes.append(p)
        p += 1 if p == 2 else 2
    return primes


def rank_gf2(vectors, t):
    r = 0
    for i in range(t):
        pivot = -1
        for j in range(r, len(vectors)):
            if vectors[j][i] == 1:
                pivot = j
                break
        if pivot < 0:
            continue
        vectors[r], vectors[pivot] = vectors[pivot], vectors[r]
        for k in range(len(vectors)):
            if k != r and vectors[k][i] == 1:
                vectors[k] = [(x ^ y) for x, y in zip(vectors[k], vectors[r])]
        r += 1
    return r


t, m = map(int, input().split())
b = list(map(int, input().split()))
pr = first_primes(t)

vectors = []
for num in b:
    exps = [0] * t
    tmp = num
    for i, prime in enumerate(pr):
        while tmp % prime == 0:
            exps[i] ^= 1
            tmp //= prime
        if tmp == 1:
            break
    vectors.append(exps)

r = rank_gf2(vectors, t)
print((1 << (m - r)) - 1)
