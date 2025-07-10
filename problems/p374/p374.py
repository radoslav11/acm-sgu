def mult_poly(poly1, poly2):
    res = [0] * (len(poly1) + len(poly2) - 1)
    for i in range(len(poly1)):
        for j in range(len(poly2)):
            res[i + j] += poly1[i] * poly2[j]
    return res


def main():
    a, b, k = map(int, input().split())

    poly = [b, a]
    res = [1]
    while k > 0:
        if k % 2 == 1:
            res = mult_poly(poly, res)
        poly = mult_poly(poly, poly)
        k >>= 1

    print(sum(res))
    

if __name__ == "__main__":
    main()
