"""
This is the classical example of the Burnside lemma. There are
many tutorials online, as well as the Wiki page, that go through 
the full idea, but roughly we count the number of fixed points for
every individual "view", which ensures that in every equivalent class,
the "total count" is the same and equal to the number of possible transformations.
This is the non-formal and somewhat vague explanation, where more details can be 
found in https://cp-algorithms.com/combinatorics/burnside.html, as well as 
solution to this particular problem.

In this problem, the transformations are the cyclic shifts, and a way of counting the
number of fixed points is by iterating through the cyclic shift "i" (1 to N), and counting
how many binary strings are a fixed point under this. This is a simple combinatorics problem
as essentially we know that 1, 1 + i, 1 + 2i, ... are all the same, or we will have gcd(N, i)
such groups which we can choose independently. In other words, the final answer is:

(SUM 2^gcd(N, i)) / N

In this problem N is fairly large, but we can notice that gcd(N, i) can take O(sqrt(N)) values
(technically even less as it's the number of divisors which is closer to O(N^1/3)), so we only
need to calculate 2^K a few times. 

Main reason we implement this in Python instead of C++ is because it requires BigInteger.
"""
import math
import sys

sys.set_int_max_str_digits(1000000)


def fast_pow(p, k):
    r = 1
    while k:
        if k & 1:
            r *= p
        k >>= 1
        p *= p

    return r


def main():
    n = int(input())

    cnt = [0] * (n + 1)
    for i in range(1, n + 1):
        cnt[math.gcd(n, i)] += 1

    ans = 0
    for v in range(1, n + 1):
        if cnt[v] == 0:
            continue
        ans += cnt[v] * fast_pow(2, v)

    ans //= n
    print(ans)


if __name__ == "__main__":
    main()
