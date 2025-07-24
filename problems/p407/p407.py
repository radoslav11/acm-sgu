# In C++ we would need long integers for this problem, so
# we will use Python's built-in int type which can handle large integers.
import sys


def matrix_mult_2x2(
    A_2x2: list[list[int]], B_2x2: list[list[int]]
) -> list[list[int]]:
    return [
        [
            A_2x2[0][0] * B_2x2[0][0] + A_2x2[0][1] * B_2x2[1][0],
            A_2x2[0][0] * B_2x2[0][1] + A_2x2[0][1] * B_2x2[1][1],
        ],
        [
            A_2x2[1][0] * B_2x2[0][0] + A_2x2[1][1] * B_2x2[1][0],
            A_2x2[1][0] * B_2x2[0][1] + A_2x2[1][1] * B_2x2[1][1],
        ],
    ]


def matrix_vector_mult_2x2(
    A_2x2: list[list[int]], v_2: list[int]
) -> list[int]:
    return [
        A_2x2[0][0] * v_2[0] + A_2x2[0][1] * v_2[1],
        A_2x2[1][0] * v_2[0] + A_2x2[1][1] * v_2[1],
    ]


def matrix_pow_2x2(A_2x2: list[list[int]], p: int) -> list[list[int]]:
    result = [[1, 0], [0, 1]]
    while p:
        if p % 2 == 1:
            result = matrix_mult_2x2(result, A_2x2)
        A_2x2 = matrix_mult_2x2(A_2x2, A_2x2)
        p //= 2

    return result


def solve(n: int, m: int) -> int:
    sys.set_int_max_str_digits(10000)

    # The main idea is to unite all of the non-start cells into a single
    # "super" cell. This is to compress the graph. You can notice that there
    # are only 4 options:
    # 1. You go from 0 to one of the n other cells. There are n options to do this,
    #    and this moves us from 0 to the super cell.
    # 2. You go from the super cell to 0. This is the opposite of option 1. Note that
    #    we have united the cells, so there is only one way to do this, not n (otherwise
    #    we overcount).
    # 3. We move from the super cell, to another cell in the super cell. Every cell has a 
    #    degree equal to 3, so there are exactly 2 ways to do this move, if we exclude the
    #    above option 2.
    # 4. We move from cell 0 to cell 0. This is impossible, since we are not allowed to
    #    stay in the same cell, so we do not count this option.

    A_2x2 = [[0, 1], [n, 2]]
    v_2 = [1, 0]
    Am_2x2 = matrix_pow_2x2(A_2x2, m)
    ans = matrix_vector_mult_2x2(Am_2x2, v_2)[0]

    return ans


def main():
    n, m = map(int, input().strip().split())
    result = solve(n, m)
    print(result)


if __name__ == "__main__":
    main()
