def find_magic_pairs(N, A0, B0):
    pairs = []

    for k in range(N):
        A = (k * A0) % N
        B = (k * B0) % N
        pairs.append((A, B))

    pairs = list(set(pairs))
    pairs.sort()
    return pairs


def main():
    N = int(input())
    A0, B0 = map(int, input().split())

    pairs = find_magic_pairs(N, A0, B0)

    print(len(pairs))
    for A, B in pairs:
        print(A, B)


if __name__ == "__main__":
    main()
