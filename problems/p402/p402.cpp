#include <bits/stdc++.h>
#define endl '\n'

#pragma GCC optimize("O333")
#pragma GCC target("sse4")

using namespace std;
template<class T, class T2>
inline void chkmax(T& x, const T2& y) {
    if(x < y) {
        x = y;
    }
}
template<class T, class T2>
inline void chkmin(T& x, const T2& y) {
    if(x > y) {
        x = y;
    }
}
const int MAXN = (1 << 20);

template<class FlowT>
struct max_flow {
    const static FlowT finf = 1e9 + 42 + 17;
    const static FlowT feps = 0;

    struct edge {
        FlowT flow, cap;
        int idx, rev, to;
        edge() {
            flow = 0;
            cap = 0;
            rev = 0;
            idx = 0;
            to = 0;
        }
        edge(int _to, int _rev, FlowT _flow, FlowT _cap, int _idx) {
            to = _to;
            rev = _rev;
            flow = _flow;
            cap = _cap;
            idx = _idx;
        }
    };

    vector<edge> G[MAXN];
    int n, dist[MAXN], po[MAXN];

    bool bfs(int s, int t) {
        dist[s] = -1, po[s] = 0;
        dist[t] = -1, po[t] = 0;
        for(int v = 0; v <= n; v++) {
            dist[v] = -1, po[v] = 0;
        }

        queue<int> Q;
        Q.push(s);
        dist[s] = 0;

        while(!Q.empty()) {
            int u = Q.front();
            Q.pop();

            for(edge e: G[u]) {
                if(dist[e.to] == -1 && e.flow < e.cap) {
                    dist[e.to] = dist[u] + 1;
                    Q.push(e.to);
                }
            }
        }

        return dist[t] != -1;
    }

    FlowT dfs(int u, int t, FlowT fl = finf) {
        if(u == t) {
            return fl;
        }

        for(; po[u] < (int)G[u].size(); po[u]++) {
            auto& e = G[u][po[u]];
            if(dist[e.to] == dist[u] + 1 && e.flow < e.cap) {
                FlowT f = dfs(e.to, t, min(fl, e.cap - e.flow));

                e.flow += f;
                G[e.to][e.rev].flow -= f;

                if(f > 0) {
                    return f;
                }
            }
        }

        return 0;
    }

    void init(int _n) {
        n = _n;
        for(int i = 0; i <= n; i++) {
            G[i].clear();
        }
    }

    void add_edge(int u, int v, FlowT w, int idx = -1) {
        G[u].push_back(edge(v, G[v].size(), 0, w, idx));
        G[v].push_back(edge(u, G[u].size() - 1, 0, 0, -1));
    }

    bool used_cut[MAXN];

    void dfs_min_cut(int u) {
        used_cut[u] = 1;
        for(auto e: G[u]) {
            if(!used_cut[e.to] && e.cap > e.flow) {
                dfs_min_cut(e.to);
            }
        }
    }

    void find_cut(int s) {
        for(int i = 0; i <= n; i++) {
            used_cut[i] = 0;
        }
        dfs_min_cut(s);
    }

    FlowT flow(int s, int t) {
        if(s == t) {
            return finf;
        }

        FlowT ret = 0, to_add;
        while(bfs(s, t)) {
            while((to_add = dfs(s, t))) {
                ret += to_add;
            }
        }

        return ret;
    }
};

int n, m;
pair<int, pair<int, int> > ed[MAXN];
vector<int> adj[MAXN];

void read() {
    cin >> n >> m;
    for(int i = 1; i <= m; i++) {
        cin >> ed[i].first >> ed[i].second.first >> ed[i].second.second;
        adj[ed[i].first].push_back(ed[i].second.first);
        adj[ed[i].second.first].push_back(ed[i].first);
    }
}

int answer;
vector<int> ans;
max_flow<int> mf;

void check(int t, int S, int T) {
    mf.init(n);
    for(int i = 1; i <= m; i++) {
        if(ed[i].first != t && ed[i].second.first != t) {
            mf.add_edge(
                ed[i].first, ed[i].second.first, ed[i].second.second, i
            );
            mf.add_edge(
                ed[i].second.first, ed[i].first, ed[i].second.second, i
            );
        }
    }

    int curr = mf.flow(S, T);
    if(answer == -1 || curr < answer) {
        mf.find_cut(S);

        answer = curr;
        ans.clear();
        for(int i = 0; i <= n; i++) {
            for(auto it: mf.G[i]) {
                if(mf.used_cut[i] && !mf.used_cut[it.to] && it.idx != -1) {
                    ans.push_back(it.idx);
                }
            }
        }

        sort(ans.begin(), ans.end());
        ans.erase(unique(ans.begin(), ans.end()), ans.end());
    }
}

void solve_ap(int r) {
    for(int u: adj[r]) {
        for(int v: adj[r]) {
            if(u < v && r != u && r != v) {
                check(r, u, v);
            }
        }
    }
}

void solve() {
    for(int i = 1; i <= n; i++) {
        adj[0].push_back(i);
    }

    answer = -1;
    for(int i = 0; i <= n; i++) {
        solve_ap(i);
    }

    cout << answer << endl;
    cout << ans.size() << endl;
    for(auto it: ans) {
        cout << it << " ";
    }
    if(ans.size()) {
        cout << endl;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    read();
    solve();
    return 0;
}
