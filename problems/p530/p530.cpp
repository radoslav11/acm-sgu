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

const int inf = (int)1e9 + 42;

int n, m;
vector<vector<int>> adj;

void read() {
    cin >> n >> m;
    adj.assign(n, {});
    for(int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        u--, v--;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
}

void bfs(int start, vector<vector<int>>& distances, int genius) {
    vector<int> d(n, -1);
    queue<int> q;
    d[start] = 0;
    q.push(start);
    while(!q.empty()) {
        int u = q.front();
        q.pop();
        for(int v: adj[u]) {
            if(d[v] == -1) {
                d[v] = d[u] + 1;
                q.push(v);
            }
        }
    }
    for(int i = 0; i < n; i++) {
        distances[i][genius] = (d[i] == -1 ? inf : d[i]);
    }
}

void solve() {
    // This solution determines which company recruits two geniuses by
    // analyzing shortest-path distances from the three geniuses (nodes 0, 1,
    // 2). Both companies optimally start with one genius and expand their
    // connected component. The winner is decided by who seizes the critical
    // "junction" vertices that improve access to the other two geniuses.
    //
    // For each of the 6 permutations of genius roles (which genius is primary):
    //     - Sort nodes by increasing distance from the primary (then secondary,
    //       then tertiary as tiebreakers).
    //     - Sweep layer by layer. This simulates the natural order in which
    //       nodes become reachable during alternating hiring turns.
    //     - Maintain the best (minimum) remaining distances to the two target
    //       geniuses seen so far, plus a set of seen (dist_b, dist_c) pairs.
    //     - A node is dominated if it offers no improvement over the current
    //       best distances or its exact pair was already encountered earlier.
    //
    // In other words, the dominated vertices can be though of vertices where
    // there exists some "intersection" that's strictly better. Essentially, if
    // at some point first company occupies a dominated intersection, the second
    // company can occupy (or start), from the one that's strictly better (in at
    // least 2 components). If any node survives undominated across all six
    // orderings, it is an unstealable strategic point that Company 1 can always
    // reach first, allowing it to secure two geniuses regardless of opening
    // moves. Otherwise every node is stealable in at least one scenario, so
    // Company 2 wins with perfect play.

    vector<vector<int>> distances(n, vector<int>(3));
    for(int i = 0; i < 3; i++) {
        bfs(i, distances, i);
    }

    vector<int> nodes(n);
    vector<bool> is_dominated(n, false);
    vector<int> perm = {0, 1, 2};
    iota(nodes.begin(), nodes.end(), 0);

    do {
        int primary = perm[0];
        int secondary = perm[1];
        int tertiary = perm[2];

        sort(nodes.begin(), nodes.end(), [&](int a, int b) {
            for(int k = 0; k < 3; k++) {
                if(distances[a][perm[k]] != distances[b][perm[k]]) {
                    return distances[a][perm[k]] < distances[b][perm[k]];
                }
            }
            return a < b;
        });

        map<pair<int, int>, int> seen;
        int best_sec = inf;
        int best_tert = inf;

        for(int i = 0; i < n;) {
            int layer_start = i;
            int curr_dist = distances[nodes[i]][primary];

            while(i < n && distances[nodes[i]][primary] == curr_dist) {
                int v = nodes[i];
                int db = distances[v][secondary];
                int dc = distances[v][tertiary];

                if(db > best_sec || dc > best_tert || seen.count({db, dc})) {
                    is_dominated[v] = true;
                }
                i++;
            }

            for(int j = layer_start; j < i; j++) {
                int v = nodes[j];
                int db = distances[v][secondary];
                int dc = distances[v][tertiary];
                seen[{db, dc}] = 1;
                best_sec = min(best_sec, db);
                best_tert = min(best_tert, dc);
            }
        }
    } while(next_permutation(perm.begin(), perm.end()));

    bool company_one_wins = false;
    for(bool marked: is_dominated) {
        if(!marked) {
            company_one_wins = true;
            break;
        }
    }

    cout << (company_one_wins ? 1 : 2) << '\n';
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
