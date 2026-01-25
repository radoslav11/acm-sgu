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

int n;
vector<vector<tuple<int, int, int>>> adj;
vector<int> prices;

void read() {
    cin >> n;
    adj.resize(n + 1);
    prices.resize(n - 1);
    for(int i = 0; i < n - 1; i++) {
        int a, b, t, p;
        cin >> a >> b >> t >> p;
        adj[a].push_back({b, t, i});
        adj[b].push_back({a, t, i});
        prices[i] = p;
    }
}

void solve() {
    // The problem asks to find the least sum of costs of edges that we can
    // replace so that the diameter of the tree decreases. We root the tree
    // and compute depth[u] = max distance to any leaf in subtree u. Then we
    // find the diameter D. The DP is:
    //
    //   dp[u][0] = min cost so depth of subtree < depth[u] (can't participate
    //              in any diameter anymore)
    //
    //   dp[u][1] = min cost so depth of subtree == depth[u] (but no diameter
    //              formed through u)
    //
    // When merging children at u, if two children could form a path of length
    // D through u, we must reduce at least one. If depth[u] == D, then keeping
    // full depth means the diameter is still D, so dp[u][1] = inf.
    // There is a fair bit of case work, but overall the complexity ends up
    // being O(n).

    if(n == 2) {
        cout << prices[0] << "\n1\n1\n";
        return;
    }

    auto bfs = [&](int start) {
        vector<int64_t> dist(n + 1, -1);
        queue<int> q;
        q.push(start);
        dist[start] = 0;
        while(!q.empty()) {
            int cur = q.front();
            q.pop();
            for(auto& [nb, t, idx]: adj[cur]) {
                if(dist[nb] == -1) {
                    dist[nb] = dist[cur] + t;
                    q.push(nb);
                }
            }
        }
        return dist;
    };

    auto d1 = bfs(1);
    int u = 1;
    for(int i = 2; i <= n; i++) {
        if(d1[i] > d1[u]) {
            u = i;
        }
    }

    auto dist_u = bfs(u);
    int v = u;
    for(int i = 1; i <= n; i++) {
        if(dist_u[i] > dist_u[v]) {
            v = i;
        }
    }

    int64_t D = dist_u[v];

    vector<int64_t> depth(n + 1, 0);
    vector<int> parent(n + 1, -1);
    vector<int> parent_edge(n + 1, -1);
    vector<int> parent_weight(n + 1, 0);

    function<void(int, int)> calc_depth = [&](int cur, int par) {
        parent[cur] = par;
        for(auto& [nb, w, idx]: adj[cur]) {
            if(nb != par) {
                parent_edge[nb] = idx;
                parent_weight[nb] = w;
                calc_depth(nb, cur);
                depth[cur] = max(depth[cur], depth[nb] + w);
            }
        }
    };
    calc_depth(1, -1);

    vector<int64_t> dp0(n + 1), dp1(n + 1);
    function<void(int, int)> dfs = [&](int cur, int par) {
        vector<tuple<int64_t, int64_t, int, int, int>> children;
        for(auto& [nb, w, idx]: adj[cur]) {
            if(nb != par) {
                dfs(nb, cur);
                int64_t contrib = depth[nb] + w;
                children.push_back({contrib, w, prices[idx], nb, idx});
            }
        }

        if(children.empty()) {
            dp0[cur] = inf;
            dp1[cur] = 0;
            return;
        }

        int64_t first_contrib = 0, second_contrib = 0;
        for(auto& [contrib, w, price, child, idx]: children) {
            if(contrib > first_contrib) {
                second_contrib = first_contrib;
                first_contrib = contrib;
            } else if(contrib > second_contrib) {
                second_contrib = contrib;
            }
        }

        auto cost_reduce = [&](int64_t contrib, int64_t w, int price,
                               int child) -> int64_t {
            int64_t opt1 = dp0[child];
            int64_t opt2 =
                (w > 0) ? (price + min(dp0[child], dp1[child])) : inf;
            return min(opt1, opt2);
        };

        auto cost_keep = [&](int child) -> int64_t { return dp1[child]; };

        dp0[cur] = 0;
        for(auto& [contrib, w, price, child, idx]: children) {
            if(contrib == depth[cur]) {
                dp0[cur] += cost_reduce(contrib, w, price, child);
            } else {
                dp0[cur] +=
                    min(cost_reduce(contrib, w, price, child),
                        cost_keep(child));
            }
        }

        if(depth[cur] == D) {
            dp1[cur] = inf;
        } else if(first_contrib + second_contrib < D) {
            int64_t base = 0;
            int64_t min_delta = inf;
            for(auto& [contrib, w, price, child, idx]: children) {
                int64_t cr = cost_reduce(contrib, w, price, child);
                int64_t ck = cost_keep(child);
                base += min(cr, ck);
                if(contrib == depth[cur]) {
                    min_delta = min(min_delta, ck - min(cr, ck));
                }
            }
            dp1[cur] = base + min_delta;
        } else {
            int64_t threshold = D - depth[cur];
            int64_t base = 0;
            for(auto& [contrib, w, price, child, idx]: children) {
                if(contrib >= threshold) {
                    base += cost_reduce(contrib, w, price, child);
                } else {
                    base +=
                        min(cost_reduce(contrib, w, price, child),
                            cost_keep(child));
                }
            }
            int64_t min_delta = inf;
            for(auto& [contrib, w, price, child, idx]: children) {
                if(contrib == depth[cur]) {
                    int64_t cr = cost_reduce(contrib, w, price, child);
                    int64_t ck = cost_keep(child);
                    min_delta = min(min_delta, ck - cr);
                }
            }
            dp1[cur] = base + min_delta;
        }
    };

    dfs(1, -1);

    int64_t ans = min(dp0[1], dp1[1]);
    int target_state = (dp0[1] <= dp1[1]) ? 0 : 1;

    vector<int> cut_edges;

    function<void(int, int, int)> reconstruct = [&](int cur, int par,
                                                    int state) {
        vector<tuple<int64_t, int64_t, int, int, int>> children;
        for(auto& [nb, w, idx]: adj[cur]) {
            if(nb != par) {
                int64_t contrib = depth[nb] + w;
                children.push_back({contrib, w, prices[idx], nb, idx});
            }
        }

        if(children.empty()) {
            return;
        }

        int64_t first_contrib = 0, second_contrib = 0;
        for(auto& [contrib, w, price, child, idx]: children) {
            if(contrib > first_contrib) {
                second_contrib = first_contrib;
                first_contrib = contrib;
            } else if(contrib > second_contrib) {
                second_contrib = contrib;
            }
        }

        auto cost_reduce = [&](int64_t contrib, int64_t w, int price,
                               int child) -> int64_t {
            int64_t opt1 = dp0[child];
            int64_t opt2 =
                (w > 0) ? (price + min(dp0[child], dp1[child])) : inf;
            return min(opt1, opt2);
        };

        auto cost_keep = [&](int child) -> int64_t { return dp1[child]; };

        auto do_reduce = [&](int64_t contrib, int64_t w, int price, int child,
                             int idx) {
            int64_t opt1 = dp0[child];
            int64_t opt2 =
                (w > 0) ? (price + min(dp0[child], dp1[child])) : inf;
            if(opt1 <= opt2) {
                reconstruct(child, cur, 0);
            } else {
                cut_edges.push_back(idx);
                if(dp0[child] <= dp1[child]) {
                    reconstruct(child, cur, 0);
                } else {
                    reconstruct(child, cur, 1);
                }
            }
        };

        auto do_opt = [&](int64_t contrib, int64_t w, int price, int child,
                          int idx) {
            int64_t cr = cost_reduce(contrib, w, price, child);
            int64_t ck = cost_keep(child);
            if(ck <= cr) {
                reconstruct(child, cur, 1);
            } else {
                do_reduce(contrib, w, price, child, idx);
            }
        };

        if(state == 0) {
            for(auto& [contrib, w, price, child, idx]: children) {
                if(contrib == depth[cur]) {
                    do_reduce(contrib, w, price, child, idx);
                } else {
                    do_opt(contrib, w, price, child, idx);
                }
            }
        } else {
            if(first_contrib + second_contrib < D) {
                int keep_child = -1;
                int64_t min_delta = inf;
                for(auto& [contrib, w, price, child, idx]: children) {
                    if(contrib == depth[cur]) {
                        int64_t cr = cost_reduce(contrib, w, price, child);
                        int64_t ck = cost_keep(child);
                        int64_t delta = ck - min(cr, ck);
                        if(delta < min_delta) {
                            min_delta = delta;
                            keep_child = child;
                        }
                    }
                }
                for(auto& [contrib, w, price, child, idx]: children) {
                    if(child == keep_child) {
                        reconstruct(child, cur, 1);
                    } else {
                        do_opt(contrib, w, price, child, idx);
                    }
                }
            } else {
                int64_t threshold = D - depth[cur];
                int keep_child = -1;
                int64_t min_delta = inf;
                for(auto& [contrib, w, price, child, idx]: children) {
                    if(contrib == depth[cur]) {
                        int64_t cr = cost_reduce(contrib, w, price, child);
                        int64_t ck = cost_keep(child);
                        if(ck - cr < min_delta) {
                            min_delta = ck - cr;
                            keep_child = child;
                        }
                    }
                }
                for(auto& [contrib, w, price, child, idx]: children) {
                    if(child == keep_child) {
                        reconstruct(child, cur, 1);
                    } else if(contrib >= threshold) {
                        do_reduce(contrib, w, price, child, idx);
                    } else {
                        do_opt(contrib, w, price, child, idx);
                    }
                }
            }
        }
    };

    reconstruct(1, -1, target_state);

    cout << ans << "\n" << cut_edges.size() << "\n";
    for(int i = 0; i < (int)cut_edges.size(); i++) {
        if(i > 0) {
            cout << " ";
        }
        cout << cut_edges[i] + 1;
    }
    cout << " \n";
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
