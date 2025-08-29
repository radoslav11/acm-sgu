import sys


class Dinic:
    def __init__(self, n):
        self.n = n
        self.adj = [[] for _ in range(n)]
        self.level = [0] * n
        self.it = [0] * n

    def add_edge(self, u, v, c):
        self.adj[u].append([v, c, len(self.adj[v])])
        self.adj[v].append([u, 0, len(self.adj[u]) - 1])

    def bfs(self, s, t):
        from collections import deque

        self.level = [-1] * self.n
        q = deque([s])
        self.level[s] = 0
        while q:
            u = q.popleft()
            for v, cap, _ in self.adj[u]:
                if cap > 0 and self.level[v] < 0:
                    self.level[v] = self.level[u] + 1
                    q.append(v)
        return self.level[t] >= 0

    def dfs(self, u, t, f):
        if u == t:
            return f
        for i in range(self.it[u], len(self.adj[u])):
            self.it[u] = i
            v, cap, _ = self.adj[u][i]
            if cap > 0 and self.level[v] == self.level[u] + 1:
                pushed = self.dfs(v, t, min(f, cap))
                if pushed:
                    self.adj[u][i][1] -= pushed
                    rv = self.adj[u][i][2]
                    self.adj[v][rv][1] += pushed
                    return pushed
        return 0

    def maxflow(self, s, t):
        flow = 0
        INF = 10**9
        while self.bfs(s, t):
            self.it = [0] * self.n
            while True:
                pushed = self.dfs(s, t, INF)
                if not pushed:
                    break
                flow += pushed
        return flow


def main():
    # The main idea is to solve this problem with max flow. We don't have
    # to assign all students, so we can assume that each university is
    # visited by exactly two students. Then we can form this as a maxflow
    # problam. Let's build a bipartite graph for max flow:
    #   Source -> each university (cap 2)
    #   University -> student (cap 1 if student can attend)
    #   Student -> Sink (cap 1)
    # There is a feasible configuration if and only if the maximum flow is
    # exactly 2*K. Then we can recover assignment by looking at the paths.

    data = list(map(int, sys.stdin.read().strip().split()))
    if not data:
        return

    it = iter(data)
    N = next(it)
    K = next(it)

    pref = [[] for _ in range(N)]
    for i in range(N):
        cnt = next(it)
        lst = [next(it) for _ in range(cnt)]
        pref[i] = lst

    S = 0
    uni_offset = 1
    stu_offset = uni_offset + K
    T = stu_offset + N
    V = T + 1

    din = Dinic(V)

    for u in range(1, K + 1):
        din.add_edge(S, uni_offset + (u - 1), 2)

    for s_idx in range(N):
        for u in pref[s_idx]:
            if 1 <= u <= K:
                din.add_edge(uni_offset + (u - 1), stu_offset + s_idx, 1)

    for s_idx in range(N):
        din.add_edge(stu_offset + s_idx, T, 1)

    need = 2 * K
    flow = din.maxflow(S, T)

    if flow != need:
        print("NO")
        return

    print("YES")
    for u in range(K):
        uni_node = uni_offset + u
        assigned = []
        for v, cap, _ in din.adj[uni_node]:
            if stu_offset <= v < stu_offset + N and cap == 0:
                student_id = v - stu_offset + 1
                assigned.append(student_id)
        print(len(assigned), *assigned)


if __name__ == "__main__":
    main()
