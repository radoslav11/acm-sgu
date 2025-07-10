from collections import deque


def solve():
    n = int(input())
    graph = [list(map(int, input().split())) for _ in range(n)]
    k = int(input())
    path = list(map(int, input().split()))

    queue = deque([(0, 0)])
    visited = [[False] * (k + 1) for _ in range(n)]
    visited[0][0] = True
    possible = set()

    while queue:
        node, pos = queue.popleft()

        if pos == k:
            possible.add(node)
            continue

        for next_node in range(n):
            if (
                graph[node][next_node] == path[pos]
                and not visited[next_node][pos + 1]
            ):
                visited[next_node][pos + 1] = True
                queue.append((next_node, pos + 1))

    possible = sorted(list(possible))
    print(len(possible))
    if possible:
        print(*[x + 1 for x in possible])


solve()
