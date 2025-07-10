def matrix_multiply(A, B, mod):
    n = len(A)
    C = [[0] * n for _ in range(n)]
    for i in range(n):
        for j in range(n):
            for k in range(n):
                C[i][j] += A[i][k] * B[k][j]
            C[i][j] %= mod

    return C


def matrix_vector_multiply(A, v, mod):
    n = len(A)
    result = [0] * n
    for i in range(n):
        for j in range(n):
            result[i] += A[i][j] * v[j]
        result[i] %= mod

    return result


def matrix_power(matrix, n, mod):
    result = [
        [1 if i == j else 0 for j in range(len(matrix))]
        for i in range(len(matrix))
    ]
    while n > 0:
        if n & 1:
            result = matrix_multiply(result, matrix, mod)
        matrix = matrix_multiply(matrix, matrix, mod)
        n >>= 1
    return result


def count_nice_patterns(N, M, P):
    transition_matrix = [[0 for _ in range(1 << M)] for _ in range(1 << M)]

    for current in range(1 << M):
        for next_state in range(1 << M):
            valid = True
            for i in range(1, M):
                if (
                    (current >> (i - 1)) & 1
                    and (current >> i) & 1
                    and (next_state >> (i - 1)) & 1
                    and (next_state >> i) & 1
                ):
                    valid = False
                    break

                if (
                    not ((current >> (i - 1)) & 1)
                    and not ((current >> i) & 1)
                    and not ((next_state >> (i - 1)) & 1)
                    and not ((next_state >> i) & 1)
                ):
                    valid = False
                    break

            if valid:
                transition_matrix[current][next_state] = 1

    start_state = [1] * (1 << M)

    result_matrix = matrix_power(transition_matrix, N - 1, P)
    start_state = matrix_vector_multiply(result_matrix, start_state, P)

    return sum(start_state) % P


N, M, P = map(int, input().split())
print(count_nice_patterns(N, M, P))
