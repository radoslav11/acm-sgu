#include <bits/stdc++.h>

using namespace std;

template<typename T1, typename T2>
ostream &operator<<(ostream &out, const pair<T1, T2> &x) {
    return out << x.first << ' ' << x.second;
}

template<typename T1, typename T2>
istream &operator>>(istream &in, pair<T1, T2> &x) {
    return in >> x.first >> x.second;
}

template<typename T>
istream &operator>>(istream &in, vector<T> &a) {
    for(auto &x: a) {
        in >> x;
    }
    return in;
};

template<typename T>
ostream &operator<<(ostream &out, const vector<T> &a) {
    for(auto x: a) {
        out << x << ' ';
    }
    return out;
};

int n, m, source, destination;
vector<tuple<bool, int, int, int>> junctions;
vector<vector<pair<int, int>>> adj;

void read() {
    cin >> source >> destination;
    source--, destination--;
    cin >> n >> m;

    junctions.resize(n);
    for(int i = 0; i < n; ++i) {
        char Ci;
        int riC, tiB, tiP;
        cin >> Ci >> riC >> tiB >> tiP;
        junctions[i] = make_tuple(Ci == 'B', riC, tiB, tiP);
    }

    adj.assign(n, {});
    for(int i = 0; i < m; ++i) {
        int from, to, length;
        cin >> from >> to >> length;
        from--, to--;
        adj[from].push_back({to, length});
        adj[to].push_back({from, length});
    }
}

/*
AAAAAAAAABBBBBBBBBBBBBBBAAAAAAAAAA
BBBBBBBBBAAAAAAAAAAAAAAABBBBBBBBBB
*/

void solve() {
    function<int(int, int)> get_color = [&](int u, int t) -> int {
        auto [isBlue, r, tB, tP] = junctions[u];
        if(t < r) {
            return isBlue;
        }

        if(isBlue && t < r + tP) {
            return 0;
        } else if(isBlue) {
            r += tP;
        }

        int cycle = tB + tP;
        int tU = (t - r) % cycle;
        return (int)(tU < tB);
    };

    function<int(int, int)> time_to_next_color = [&](int u, int t) {
        auto [isBlue, r, tB, tP] = junctions[u];
        if(t < r) {
            return r - t;
        }

        int cycle = tB + tP;
        if(isBlue && t < r + tP) {
            return r + tP - t;
        } else if(isBlue) {
            r += tP;
        }

        int tU = (t - r) % cycle;
        return tU < tB ? (tB - tU) : (cycle - tU);
    };

    function<int(int, int, int)> first_time_same_color = [&](int u, int v,
                                                             int t) {
        for(int attempt = 0; attempt < 3; attempt++) {
            if(get_color(v, t) == get_color(u, t)) {
                return t;
            }

            int dtu = time_to_next_color(u, t);
            int dtv = time_to_next_color(v, t);
            t += min(dtu, dtv);
        }

        return -1;
    };

    vector<int> dist(n, INT_MAX);
    vector<int> parent(n, -1);
    priority_queue<
        pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>>
        pq;

    pq.push({0, source});
    dist[source] = 0;
    while(!pq.empty()) {
        auto [t, u] = pq.top();
        pq.pop();

        for(auto [v, w]: adj[u]) {
            int first_time = first_time_same_color(u, v, t);
            if(first_time == -1) {
                continue;
            }

            int new_time = first_time + w;
            if(new_time < dist[v]) {
                parent[v] = u;
                dist[v] = new_time;
                pq.push({new_time, v});
            }
        }
    }

    if(dist[destination] == INT_MAX) {
        cout << "0\n";
        return;
    }

    vector<int> path;
    int u = destination;
    while(u != -1) {
        path.push_back(u + 1);
        u = parent[u];
    }

    reverse(path.begin(), path.end());
    cout << dist[destination] << '\n';
    cout << path << '\n';
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
