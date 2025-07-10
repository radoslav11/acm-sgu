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

int n;
vector<vector<pair<int, int>>> adj;

void read() {
    cin >> n;
    adj.assign(n, {});
    for(int i = 1; i < n; i++) {
        int u, v;
        cin >> u >> v;
        u--, v--;

        string type;
        getline(cin, type);
        if(type[1] == 'a') {
            adj[u].emplace_back(v, i);
            adj[v].emplace_back(u, i);
        } else if(type[1] == 'p') {
            adj[u].emplace_back(v, -1);
            adj[v].emplace_back(u, -1);
        }
    }
}

void solve() {
    vector<int> ans;
    set<pair<int, int>> almost_protected;
    function<void(int, int, int)> dfs = [&](int u, int p, int depth) {
        if(almost_protected.size() * 2 > depth) {
            auto lowest = almost_protected.begin();
            ans.push_back(lowest->second);
            almost_protected.erase(lowest);
        }

        for(auto [v, i]: adj[u]) {
            if(v == p) {
                continue;
            }
            if(i != -1) {
                almost_protected.insert({depth, i});
            }
            dfs(v, u, depth + 1);
            if(i != -1 && almost_protected.count({depth, i})) {
                almost_protected.erase({depth, i});
            }
        }
    };

    dfs(0, -1, 0);

    cout << ans.size() << endl;
    for(int i = 0; i < ans.size(); i++) {
        cout << ans[i] << " \n"[i + 1 == ans.size()];
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
