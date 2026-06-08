#include <bits/stdc++.h>

using namespace std;

template<typename T1, typename T2>
ostream& operator<<(ostream& out, const pair<T1, T2>& x) {
    return out << x.first << ' ' << x.second;
}

template<typename T1, typename T2>
istream& operator>>(istream& in, pair<T1, T2>& x) {
    return in >> x.first >> x.second;
}

template<typename T>
istream& operator>>(istream& in, vector<T>& a) {
    for(auto& x: a) {
        in >> x;
    }
    return in;
};

template<typename T>
ostream& operator<<(ostream& out, const vector<T>& a) {
    for(auto x: a) {
        out << x << ' ';
    }
    return out;
};

int n, m;
vector<pair<int, int>> edges;

void read() {
    cin >> n >> m;
    edges.resize(m);
    cin >> edges;
}

int center;
vector<int> partner;
vector<list<int>> adj;
vector<array<list<int>::iterator, 2>> edge_iters;

bool has_edge(int u, int v) {
    if(adj[u].size() > adj[v].size()) {
        swap(u, v);
    }
    for(int w: adj[u]) {
        if(w == v) {
            return true;
        }
    }
    return false;
}

int dist_to_center(int v) {
    if(v == center) {
        return 0;
    }
    if(has_edge(v, center)) {
        return 1;
    }
    int p = partner[v];
    if(p != 0 && has_edge(v, p) && has_edge(p, center)) {
        return 2;
    }
    return -1;
}

int query_length(int x, int y) {
    if(x == y) {
        return 0;
    }
    if(x == center || y == center) {
        int other = (x == center) ? y : x;
        return dist_to_center(other);
    }
    if(partner[x] == y) {
        int best = INT_MAX;
        if(has_edge(x, y)) {
            best = min(best, 1);
        }
        if(has_edge(x, center) && has_edge(center, y)) {
            best = min(best, 2);
        }
        return (best == INT_MAX) ? -1 : best;
    }
    int dx = dist_to_center(x);
    int dy = dist_to_center(y);
    if(dx == -1 || dy == -1) {
        return -1;
    }
    return dx + dy;
}

void solve() {
    // The key observation is that "for each two distinct cities there was
    // exactly one common neighbour" is closely related to the Friendship
    // Theorem (Erdos - Renyi - Sos, 1966). In particular, each possible input
    // graph can be constructed as follows:
    //
    //     1) Take some central vertex c.
    //
    //     2) Add k >= 1 pairwise disjoint edges (a matching) on 2k additional
    //        vertices.
    //
    //     3) Connect each of the 2k vertices in (2) to c.
    //
    // In other words, the possible graphs can be constructed as triangles that
    // share one vertex (the central vertex c). Note that the statement implies
    // that the graph is connected, meaning we can always find some vertex c. It
    // also implies that m = O(n), so we can read the input (note the problem
    // statement doesn't give the constraint for m, which is a hint).
    //
    // In terms of queries, we can do a few things. Arguably the simplest
    // approach is to start with finding c (we can choose the highest degree
    // vertex) and then decomposing the input into triangles (c will be shared
    // across all). Now when we have a query, we have two cases:
    //
    //     1) u and v are in the same triangle. Then we can check the 3 edges in
    //       the corresponding triangle and figure out if the answer is 1, 2, or
    //       no path.
    //
    //     2) u and v are in different triangles. Then the path certainly passes
    //        through the center c. We can output query(u, c) + query(c, v).
    //
    // Implementing this with sets for the graph and pairs of vertices that
    // belong to the same triangle is already enough to pass in O((n + q) log
    // n). Here we go one step further and implement the linear O(n + q)
    // version:
    //
    //     - Every non-center vertex has degree at most 2 (its two triangle
    //       neighbours: c and its partner). So has_edge(u, v) can be resolved
    //       by always scanning the smaller adjacency list, giving
    //       O(min(|adj[u]|, |adj[v]|)) = O(1) since at least one endpoint is
    //       non-center.
    //
    //     - For deletions we only need O(1) edge removal. We store each edge in
    //       a std::list adjacency on both endpoints and remember the two
    //       iterators at insertion time; on DELETE we just erase by iterator.

    adj.assign(n + 1, list<int>());
    edge_iters.resize(m);
    for(int i = 0; i < m; i++) {
        auto [u, v] = edges[i];
        edge_iters[i][0] = adj[u].insert(adj[u].end(), v);
        edge_iters[i][1] = adj[v].insert(adj[v].end(), u);
    }

    center = 1;
    for(int v = 2; v <= n; v++) {
        if(adj[v].size() > adj[center].size()) {
            center = v;
        }
    }

    partner.assign(n + 1, 0);
    for(int v = 1; v <= n; v++) {
        if(v == center) {
            continue;
        }
        for(int u: adj[v]) {
            if(u != center) {
                partner[v] = u;
                break;
            }
        }
    }

    string cmd;
    while(cin >> cmd) {
        if(cmd == "DELETE") {
            int x;
            cin >> x;
            x--;
            auto [u, v] = edges[x];
            adj[u].erase(edge_iters[x][0]);
            adj[v].erase(edge_iters[x][1]);
        } else if(cmd == "LENGTH") {
            int x, y;
            cin >> x >> y;
            cout << query_length(x, y) << "\n";
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T = 1;
    // cin >> T;
    for(int test = 1; test <= T; test++) {
        read();
        solve();
    }

    return 0;
}
