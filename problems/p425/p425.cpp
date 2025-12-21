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
vector<vector<int>> tbl;

void read() {
    cin >> n >> m;
    tbl.assign(n, vector<int>(m, 0));
    cin >> tbl;
}

void solve() {
    // One of the main observations in this problem is that we want to use
    // values <= 50. This is an interesting value, and particularly, it's
    // O(sqrt(N)). We should keep this in mind.
    //
    // Often in problems like this, it's useful to either find cases where the
    // answer is No, or figure out why the answer is always Yes. It's hard to
    // find cases where it's important, as for one, N should be > 50, so it
    // should suggest us to try figuring how to always construct a table with 50
    // as the largest value. The worst case for us would be if every row matches
    // in exactly one column with the first one, as it eliminates the
    // optionality. Hence, we can consider the case where there is only 1
    // mismatch per row, and we want to choose <= 50 values such that the
    // mismatches per row aren't mapped to the same value. This should remind us
    // of a famous graph theory problem - the graph coloring. Essentially, every
    // value is a vertex and we have N - 1 edges (the mismatches), and we want
    // to choose <= 50 colors. The simple greedy algorithm of going in
    // increasing order of degrees achieves this O(sqrt(edges)) bound on number
    // of colors. It's fairly easy to prove, but here is also a link for this:
    //
    // https://math.stackexchange.com/questions/4114699/graph-coloring-why-is-fm-leq-sqrtm
    //
    // We can remember the initial observation we had: sqrt(n-1) is well below
    // the 50 bound we had, so this solution should work. The only case left is
    // when there are 2 or more mismatches between row 1 and some other row. We
    // briefly mentioned it above, but this just gives us more optionality, so
    // we can just leave one of the two possible edges randomly, as the greedy
    // guarantees we can color with sqrt(edges) colors.

    vector<int> values;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            values.push_back(tbl[i][j]);
        }
    }

    sort(values.begin(), values.end());
    values.erase(unique(values.begin(), values.end()), values.end());

    map<int, vector<int>> adj;
    map<int, int> degree, colors;

    for(int i = 1; i < n; i++) {
        for(int j = 0; j < m; j++) {
            int u = tbl[i][j], v = tbl[0][j];
            if(u != v) {
                degree[u]++;
                degree[v]++;
                adj[u].push_back(v);
                adj[v].push_back(u);
                break;
            }
        }
    }

    vector<pair<int, int>> vertices;
    for(auto [v, d]: degree) {
        vertices.push_back({d, v});
    }

    sort(vertices.begin(), vertices.end());

    for(auto [d, v]: vertices) {
        set<int> used = {0};
        for(int u: adj[v]) {
            if(colors.count(u)) {
                used.insert(colors[u]);
            }
        }
        int c = 1;
        while(used.count(c)) {
            c++;
        }
        colors[v] = c;
    }

    cout << "Yes\n";
    for(int v: values) {
        auto it = colors.find(v);
        cout << v << " -> ";
        if(it == colors.end()) {
            cout << "0\n";
        } else {
            cout << it->second << '\n';
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
        // cout << "Case #" << test << ": ";
        solve();
    }

    return 0;
}
