#include <bits/stdc++.h>
// #include <coding_library/data_structures/fenwick.hpp>

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

template<class T>
class Fenwick {
  private:
    int sz, log_size;
    vector<T> tr;

  public:
    void init(int n) {
        sz = n + 1;
        log_size = 31 - __builtin_clz(sz);
        tr.assign(sz + 1, 0);
    }

    void update(int idx, T val) {
        if(idx <= 0) {
            assert(false);
            return;
        }
        for(; idx <= sz; idx += (idx & -idx)) {
            tr[idx] += val;
        }
    }

    T query(int idx) {
        T ans = 0;
        for(; idx >= 1; idx -= (idx & -idx)) {
            ans += tr[idx];
        }

        return ans;
    }

    T query(int l, int r) { return query(r) - query(l - 1); }

    int find_kth(T k) {
        int idx = 0;
        for(int i = log_size; i >= 0; i--) {
            if(idx + (1 << i) < sz && tr[idx + (1 << i)] < k) {
                k -= tr[idx + (1 << i)];
                idx += (1 << i);
            }
        }
        return idx + 1;
    }
};

int n;
vector<vector<int>> up_larger, left_larger;

void read() {
    cin >> n;
    up_larger.assign(n, vector<int>(n, 0));
    left_larger.assign(n, vector<int>(n, 0));
    cin >> up_larger >> left_larger;
}

void solve() {
    // We can solve this problem by creating a compressed graph with O(n^2)
    // nodes and O(n^2) edges and then finding a topological sort of it. In
    // particular,  for every row and column, we can solve the more-standard
    // problem in a permutation, find the orders of the cells, and then add the
    // corresponding n-1 edges between consecutive cells. Finding the order in
    // every row and column can be done in O(n log n) time, and then we can find
    // the topological sort in O(n^2) time, making the total complexity O(n^2
    // log n).
    // We can notice that the answer is 0 if and only if we can't construct the
    // order in a row / column, or if there is a cycle in the graph. The former
    // happens only when up_larger[i][j] > i or left_larger[i][j] > j, as
    // otherwise we always have a construction.

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if(left_larger[i][j] > j || up_larger[i][j] > i) {
                cout << 0 << '\n';
                return;
            }
        }
    }

    Fenwick<int> fenwick;
    fenwick.init(n + 4);

    vector<vector<int>> adj(n * n);
    for(int i = 0; i < n; i++) {
        for(int j = 1; j <= n; j++) {
            fenwick.update(j, 1);
        }

        vector<int> vals(n, 0);
        for(int j = n - 1; j >= 0; j--) {
            int pos = fenwick.find_kth(left_larger[i][j] + 1);
            int idx = i * n + j;
            vals[pos - 1] = idx;
            fenwick.update(pos, -1);
        }

        for(int j = 1; j < n; j++) {
            adj[vals[j]].push_back(vals[j - 1]);
        }
    }

    for(int j = 0; j < n; j++) {
        for(int i = 1; i <= n; i++) {
            fenwick.update(i, 1);
        }

        vector<int> vals(n, 0);
        for(int i = n - 1; i >= 0; i--) {
            int pos = fenwick.find_kth(up_larger[i][j] + 1);
            int idx = i * n + j;
            vals[pos - 1] = idx;
            fenwick.update(pos, -1);
        }

        for(int i = 1; i < n; i++) {
            adj[vals[i]].push_back(vals[i - 1]);
        }
    }

    vector<vector<int>> ans(n, vector<int>(n, 0));

    queue<int> q;
    int current_node = 1;
    vector<int> in_degree(n * n, 0);

    for(int u = 0; u < n * n; u++) {
        for(int v: adj[u]) {
            in_degree[v]++;
        }
    }

    for(int u = 0; u < n * n; u++) {
        if(in_degree[u] == 0) {
            q.push(u);
        }
    }

    while(!q.empty()) {
        int u = q.front();
        q.pop();
        int i = u / n, j = u % n;
        ans[i][j] = current_node++;

        for(int v: adj[u]) {
            in_degree[v]--;
            if(in_degree[v] == 0) {
                q.push(v);
            }
        }
    }

    if(current_node != n * n + 1) {
        cout << 0 << '\n';
        return;
    }

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            cout << ans[i][j] << ' ';
        }
        cout << '\n';
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
