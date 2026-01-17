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

const int64_t inf = (int64_t)1e18 + 42;

int n, x, y, m;
vector<vector<pair<int, int>>> adj;

void read() {
    cin >> n >> x >> y;
    x--;
    y--;
    cin >> m;
    adj.assign(n, {});
    for(int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        u--;
        v--;
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }
}

void solve() {
    // We are only interested in shortest paths, so the first step is to build
    // the Dijkstra DAG of optimal edges. The Dijkstra will start from X, but we
    // also want to afterwards compute reachable(u) which will be true if Y is
    // reachable from u through the DAG (this can be done with one DFS).
    //
    // We are now interested in the number of points in the DAG that have
    // distance equal to dist(i) for each i. And we want to compute this for all
    // i quickly. Let's denote this with a function f(a) which will denote the
    // number of points with distance equal to a. We will maintain this in a
    // smart way, so then then we simply want to evaluate the function in N
    // points. We can first separately figure the contribution from points that
    // fall within a vertex - if u is a vertex with reachable(u) = true, then
    // f(dist(u)) should be increased by 1. Now we are left with the case of the
    // contribution coming from points on edges. Let's say we have an edge (u,
    // v), and reachable(v) = true. Then we can be on that edge at any point a,
    // such that dist(u) < a < dist(v). This is essentially a range of values.
    // We only care about integer query points, and we want to be careful about
    // not over-counting points that fall in vertices. Hence, we can increase
    // f(dist(u)+1), ..., f(dist(v)-1) by one, or one range per edge.
    //
    // This now ends up being a standard problem - we have N ranges, and N
    // queries, where for every query we want to figure out by how many ranges
    // it's covered. This can be done with splitting the ranges into a pair of
    // events - an IN event with "balance += 1" at L, and an OUT event with
    // "balance -= 1" at R. And we also add QUERY events that will look at the
    // balance. Priority should be IN, QUERY, OUT. Note that this approach is
    // offline, while online we could do segment trees but this is less pleasant
    // here as the edges have higher weights so we need to use a dynamic segment
    // tree / treap.
    //
    // All steps of the solution are in O((N + M) log M).

    vector<int64_t> dist(n, inf);
    priority_queue<pair<int64_t, int>, vector<pair<int64_t, int>>, greater<>>
        pq;
    dist[x] = 0;
    pq.push({0, x});
    while(!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if(d > dist[u]) {
            continue;
        }
        for(auto [v, w]: adj[u]) {
            if(dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }

    vector<vector<int>> dag_rev(n);
    for(int u = 0; u < n; u++) {
        for(auto [v, w]: adj[u]) {
            if(dist[u] + w == dist[v]) {
                dag_rev[v].push_back(u);
            }
        }
    }

    vector<bool> reachable(n, false);
    function<void(int)> dfs = [&](int u) {
        if(reachable[u]) {
            return;
        }
        reachable[u] = true;
        for(int v: dag_rev[u]) {
            dfs(v);
        }
    };
    dfs(y);

    vector<tuple<int64_t, int, int>> events;
    for(int u = 0; u < n; u++) {
        if(reachable[u]) {
            events.push_back({dist[u], 0, 0});
            events.push_back({dist[u], 2, 0});
        }
        for(auto [v, w]: adj[u]) {
            if(dist[u] + w == dist[v] && reachable[v]) {
                int64_t l = dist[u] + 1;
                int64_t r = dist[v] - 1;
                if(l <= r) {
                    events.push_back({l, 0, 0});
                    events.push_back({r, 2, 0});
                }
            }
        }
    }
    for(int k = 0; k < n; k++) {
        if(reachable[k]) {
            events.push_back({dist[k], 1, k});
        }
    }

    sort(events.begin(), events.end());

    vector<int> answer(n, 0);
    int balance = 0;
    for(auto [pos, type, data]: events) {
        if(type == 0) {
            balance++;
        } else if(type == 1) {
            answer[data] = balance;
        } else {
            balance--;
        }
    }

    cout << answer << '\n';
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T = 1;
    // cin >> T;
    for(int test = 1; test <= T; test++) {
        read();
        // cout << "Case #" << test << ": ";
        solve();
    }

    return 0;
}
