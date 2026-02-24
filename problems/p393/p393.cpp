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
vector<string> edges;

void read() {
    cin >> n >> m;
    edges.resize(m);
    cin >> edges;
}

void solve() {
    // We want to partition the N letters into subsets (buttons), such that     
    // there are less than two edges between each subset. We will do a brute
    // force approach, where if we iterate through all subsets it would be
    // around BellNumber(N) ~= 27M subsets. This might be a bit tight to
    // pass, because we also have an O(M) or O(N) to check if the subsets
    // violate the constraints. One optimization is to do binary search on the
    // K, which should cut the number of subsets we consider by at least
    // a half. Another optimization is to also recursively generate the
    // subsets. Then instead of going through all StirlingNumber(N, K)
    // subsets, we will only go through the ones that are valid based on the
    // constraints. Furthermore, if we find one valid configuration for a
    // given K, we don't have to continue.

    vector<int> out_edges(n, 0);
    vector<int> in_edges(n, 0);
    for(const auto& e: edges) {
        int u = e[0] - 'a';
        int v = e[1] - 'a';
        out_edges[u] |= (1 << v);
        in_edges[v] |= (1 << u);
    }

    int masks = 1 << n;
    vector<int> any_out(masks);
    vector<int> dang_out(masks);
    vector<int> int_cnt(masks);

    auto precompute = [&]() {
        any_out[0] = dang_out[0] = int_cnt[0] = 0;
        for(int mask = 1; mask < masks; mask++) {
            int u = __builtin_ctz(mask);
            int prev = mask ^ (1 << u);
            any_out[mask] = any_out[prev] | out_edges[u];
            dang_out[mask] = dang_out[prev] | (any_out[prev] & out_edges[u]);
            int_cnt[mask] = int_cnt[prev] +
                            __builtin_popcount(out_edges[u] & prev) +
                            __builtin_popcount(in_edges[u] & prev) +
                            ((out_edges[u] >> u) & 1);
        }
    };

    auto compatible = [&](int g, int h) -> bool {
        if(dang_out[g] & h) {
            return false;
        }
        if(__builtin_popcount(any_out[g] & h) > 1) {
            return false;
        }
        if(dang_out[h] & g) {
            return false;
        }
        if(__builtin_popcount(any_out[h] & g) > 1) {
            return false;
        }
        return true;
    };

    precompute();

    int best_k = n;
    vector<int> best_groups(n);
    for(int i = 0; i < n; i++) {
        best_groups[i] = (1 << i);
    }

    vector<int> groups(n);
    bool found;

    function<void(int, int, int)> dfs = [&](int remaining, int used, int k) {
        if(found) {
            return;
        }
        if(remaining == 0) {
            found = true;
            best_k = used;
            for(int i = 0; i < used; i++) {
                best_groups[i] = groups[i];
            }
            return;
        }
        if(used >= k) {
            return;
        }

        int lo = remaining & (-remaining);
        int rest = remaining ^ lo;

        for(int sub = rest;; sub = (sub - 1) & rest) {
            int group = sub | lo;
            if(int_cnt[group] <= 1) {
                bool good = true;
                for(int i = 0; i < used && good; i++) {
                    if(!compatible(group, groups[i])) {
                        good = false;
                    }
                }
                if(good) {
                    groups[used] = group;
                    dfs(remaining ^ group, used + 1, k);
                    if(found) {
                        return;
                    }
                }
            }
            if(sub == 0) {
                break;
            }
        }
    };

    int left = 1, right = n, mid;
    while(left <= right) {
        mid = (left + right) / 2;
        found = false;
        dfs((1 << n) - 1, 0, mid);
        if(found) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }

    cout << best_k << '\n';
    for(int i = 0; i < best_k; i++) {
        for(int j = 0; j < n; j++) {
            if(best_groups[i] & (1 << j)) {
                cout << (char)('a' + j);
            }
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
        solve();
    }

    return 0;
}
