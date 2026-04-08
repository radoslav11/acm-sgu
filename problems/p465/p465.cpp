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

int n, m, r;
vector<int> prob;
vector<tuple<int, int, int>> edges;
vector<vector<double>> dist;

void read() {
    cin >> n >> m >> r;
    prob.resize(n);
    for(auto& p: prob) {
        cin >> p;
    }
    edges.resize(m);
    dist.assign(n, vector<double>(n, 1e18));
    for(int i = 0; i < n; i++) {
        dist[i][i] = 0;
    }
    for(auto& [a, b, l]: edges) {
        cin >> a >> b >> l;
        a--;
        b--;
        dist[a][b] = dist[b][a] = min(dist[a][b], (double)l);
    }
}

void solve() {
    // In this problem, just looking at the constraint we can see that N is
    // fairly small, and something like O(N^3 log N) or O(N^3) would pass. Let's
    // start with performing a Floyd so we know the shortest distance between
    // each pair of cities. Afterwards, we can try in O(M) = O(N^2) all roads in
    // the network that have length >= R^2, and check what's the best placement
    // of the fire station in the allowed area. Let the road be (u, v), and say
    // we place the station at distance D >= R (and len(u,v) - D >= R) from u.
    // Then, each city x != u, v, would either have it's shortest path to the
    // station pass through u or v, with distance D + dist[u][x], or len(u,v) -
    // D + dist[v][x]. We can see that there is always some D' after which the
    // path from v' will dominate, so we can compute these O(N) events for each
    // edge, sort them in O(N log N), and then try placing D at each one of
    // them. This way we get a O(N^3 log N). However, the events represent
    // piecewise-linear functions: each city x contributes min(D + dist[u][x], L
    // - D + dist[v][x]), which is concave (min of an increasing and a
    // decreasing linear function). The sum of concave functions is concave, so
    // the total expected cost E(D) is concave on [R, L - R]. The minimum of a
    // concave function on a closed interval is always at an endpoint, so we
    // only need to check D = R and D = L - R for each edge. This simplifies the
    // solution to O(N * M) after Floyd, or O(N^3) overall.

    for(int k = 0; k < n; k++) {
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < n; j++) {
                dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
            }
        }
    }

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if(dist[i][j] > 1e17) {
                cout << -1 << endl;
                return;
            }
        }
    }

    double best = 1e18;
    if(r == 0) {
        for(int c = 0; c < n; c++) {
            double cost = 0;
            for(int x = 0; x < n; x++) {
                cost += prob[x] * dist[c][x];
            }
            best = min(best, cost);
        }
    }

    for(auto& [a, b, l]: edges) {
        if(l < 2 * r) {
            continue;
        }
        for(double d: {(double)r, (double)(l - r)}) {
            double cost = 0;
            for(int x = 0; x < n; x++) {
                cost +=
                    prob[x] * min(d + dist[a][x], (double)l - d + dist[b][x]);
            }
            best = min(best, cost);
        }
    }

    if(best > 1e17) {
        cout << -1 << endl;
    } else {
        cout << fixed << setprecision(5) << best / 10000.0 << endl;
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
