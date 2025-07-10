def gcd(a, b):
    if b == 0:
        return a
    return gcd(b, a % b)


n = int(input())

k = n // 2
while k > 0:
    if gcd(n, k) == 1:
        print(k)
        break

    k -= 1
