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
vector<tuple<int, int, int, int>> edges;

void read() {
    cin >> n >> m;
    edges.resize(m);
    for(auto& [a, b, d1, d2]: edges) {
        cin >> a >> b >> d1 >> d2;
    }
}

void solve() {
    // The problem asks to convert a bidirected graph to a directed graph using
    // minimal elementary transformations (negating all signs at a vertex).
    // For each edge, d_u * d_v must become -1. Flipping vertex v negates its
    // signs. This gives a system of parity constraints per edge:
    //   d_u * d_v = 1  => x_u != x_v (exactly one must flip)
    //   d_u * d_v = -1 => x_u == x_v (both or neither flip)
    // This is a 2-coloring / bipartiteness check. Per connected component,
    // we pick the color class with fewer vertices to minimize flips.

    vector<vector<pair<int, int>>> adj(n + 1);
    for(auto& [a, b, d1, d2]: edges) {
        int w = d1 * d2;
        adj[a].push_back({b, w});
        adj[b].push_back({a, w});
    }

    vector<int> color(n + 1, -1);
    vector<int> flips;
    bool possible = true;

    for(int start = 1; start <= n && possible; start++) {
        if(color[start] != -1) {
            continue;
        }
        color[start] = 0;

        vector<vector<int>> groups(2);
        queue<int> q;
        q.push(start);
        groups[0].push_back(start);

        while(!q.empty() && possible) {
            int u = q.front();
            q.pop();
            for(auto [v, w]: adj[u]) {
                int need = (w == 1) ? (1 - color[u]) : color[u];
                if(color[v] == -1) {
                    color[v] = need;
                    groups[need].push_back(v);
                    q.push(v);
                } else if(color[v] != need) {
                    possible = false;
                }
            }
        }

        if(possible) {
            auto& smaller =
                (groups[0].size() <= groups[1].size()) ? groups[0] : groups[1];
            for(int v: smaller) {
                flips.push_back(v);
            }
        }
    }

    if(!possible) {
        cout << "NO\n";
        return;
    }

    sort(flips.begin(), flips.end());
    cout << "YES\n";
    cout << flips.size() << "\n";
    for(int v: flips) {
        cout << v << "\n";
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
