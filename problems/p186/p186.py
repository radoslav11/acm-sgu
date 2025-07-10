def minimum_time_to_connect_chains():
    import sys

    input = sys.stdin.read
    data = input().split()

    n = int(data[0])
    lengths = list(map(int, data[1:]))
    lengths.sort()

    left, right = 0, n - 1
    time = 0

    while left < right:
        if lengths[left] > 0:
            lengths[left] -= 1
            right -= 1
            time += 1
            if lengths[left] == 0:
                left += 1
        else:
            left += 1

    print(time)


if __name__ == "__main__":
    minimum_time_to_connect_chains()
