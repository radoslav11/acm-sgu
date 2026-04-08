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

int n;
vector<vector<bool>> has1, has2;
vector<vector<int>> adj2;
vector<pair<int, int>> input_edges1;

void read() {
    cin >> n;
    has1.assign(n + 1, vector<bool>(n + 1, false));
    has2.assign(n + 1, vector<bool>(n + 1, false));
    for(int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        has1[u][v] = has1[v][u] = true;
        input_edges1.emplace_back(u, v);
    }
    adj2.resize(n + 1);
    for(int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        has2[u][v] = has2[v][u] = true;
        adj2[u].push_back(v);
        adj2[v].push_back(u);
    }
}

vector<int> find_path(int start, int end) {
    vector<int> par(n + 1, -1);
    par[start] = start;
    queue<int> q;
    q.push(start);
    while(!q.empty()) {
        int u = q.front();
        q.pop();
        if(u == end) {
            break;
        }
        for(int v: adj2[u]) {
            if(par[v] == -1) {
                par[v] = u;
                q.push(v);
            }
        }
    }
    vector<int> path;
    for(int v = end; v != start; v = par[v]) {
        path.push_back(v);
    }
    path.push_back(start);
    reverse(path.begin(), path.end());
    return path;
}

void solve() {
    // The problem is actually fairly easy - we want the exact same set of
    // *labeled* edges, and this has nothing to do with isomorphism. There is a
    // simple algorithm that trivially works. First find an edge that exists in
    // the first tree, but doesn't in the second. Then you create a cycle, and
    // we can easily show one of the edges in it doesn't exist in the second
    // tree. We remove that edge. It doesn't matter which tree we apply these
    // operations to, so we can simply do this on the first one. We keep both
    // an adjacency list and a matrix so that each cycle finding via BFS is
    // O(N), and because we have at most O(N) operations, inserting / erasing
    // from the adjacency list is quick and gives us an O(N^2) solution.

    vector<tuple<int, int, int, int, int>> ops;

    for(auto [a, b]: input_edges1) {
        if(has2[a][b]) {
            continue;
        }

        auto path = find_path(a, b);

        int ra = -1, rb = -1;
        for(int i = 0; i + 1 < (int)path.size(); i++) {
            if(!has1[path[i]][path[i + 1]]) {
                ra = path[i];
                rb = path[i + 1];
                break;
            }
        }

        has2[ra][rb] = has2[rb][ra] = false;
        adj2[ra].erase(find(adj2[ra].begin(), adj2[ra].end(), rb));
        adj2[rb].erase(find(adj2[rb].begin(), adj2[rb].end(), ra));
        has2[a][b] = has2[b][a] = true;
        adj2[a].push_back(b);
        adj2[b].push_back(a);

        ops.emplace_back(2, a, b, ra, rb);
    }

    cout << ops.size() << "\n";
    for(auto [p, a1, b1, a2, b2]: ops) {
        cout << p << " " << a1 << " " << b1 << " " << a2 << " " << b2 << "\n";
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
