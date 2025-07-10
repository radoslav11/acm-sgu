#include <bits/stdc++.h>
#define endl '\n'

// #pragma GCC optimize ("O3")
// #pragma GCC target ("sse4")

using namespace std;
template<class T, class T2>
inline int chkmax(T& x, const T2& y) {
    return x < y ? x = y, 1 : 0;
}
template<class T, class T2>
inline int chkmin(T& x, const T2& y) {
    return x > y ? x = y, 1 : 0;
}
const int MAXN = (1 << 20);
const int B = 200;

struct dsu {
    int n;
    vector<int> par, sz;
    void init(int _n) {
        n = _n;
        par.assign(n + 1, 0);
        sz.assign(n + 1, 1);
        for(int i = 0; i <= n; i++) {
            par[i] = i;
        }
    }
    int root(int x) { return par[x] == x ? x : par[x] = root(par[x]); }
    bool connected(int u, int v) { return root(u) == root(v); }
    void unite(int u, int v) {
        u = root(u);
        v = root(v);
        if(u == v) {
            return;
        }
        if(sz[u] < sz[v]) {
            swap(u, v);
        }
        par[v] = u;
        sz[u] += sz[v];
    }
};

struct edge {
    int u, v, w, i;
    edge() { u = v = w = i = 0; }
    edge(int _u, int _v, int _w, int _i) {
        u = _u;
        v = _v;
        w = _w;
        i = _i;
    }
};

bool cmp(const edge& x, const edge& y) { return x.w < y.w; }

int n, m;
edge ed[MAXN], nw_li[MAXN], important[MAXN];
pair<int, int> que[MAXN];

void read() {
    cin >> n >> m;
    for(int i = 0; i < m; i++) {
        cin >> ed[i].u >> ed[i].v >> ed[i].w;
        ed[i].i = i;
    }
}

int64_t answer[MAXN];
int pos_edge[MAXN], imp_sz;
bool used[MAXN], used2[MAXN];
int comp_id[MAXN];
dsu additional_d, tmp, d;

int get(int x) { return comp_id[additional_d.root(x)]; }

void solve() {
    sort(ed, ed + m, cmp);
    for(int i = 0; i < m; i++) {
        pos_edge[ed[i].i] = i;
    }

    int q;
    cin >> q;

    for(int i = 0; i < q; i++) {
        cin >> que[i].first >> que[i].second;
        que[i].first--;
    }

    for(int st = 0; st < q; st += B) {
        int en = min(q - 1, st + B - 1);
        for(int i = 0; i < m; i++) {
            used[i] = used2[i] = 0;
        }
        for(int i = st; i <= en; i++) {
            used[que[i].first] = 1;
        }

        tmp.init(n);
        additional_d.init(n);
        for(int i = 0; i < m; i++) {
            if(used[ed[i].i]) {
                tmp.unite(ed[i].u, ed[i].v);
            }
        }

        int64_t sum = 0;
        for(int i = 0; i < m; i++) {
            if(!used[ed[i].i] && !tmp.connected(ed[i].u, ed[i].v)) {
                sum += ed[i].w;
                tmp.unite(ed[i].u, ed[i].v);
                additional_d.unite(ed[i].u, ed[i].v);
            }
        }

        int cnt = 0;
        for(int i = st; i <= en; i++) {
            answer[i] = sum;
        }
        for(int i = 1; i <= n; i++) {
            if(i == additional_d.root(i)) {
                comp_id[i] = cnt++;
            }
        }

        d.init(cnt);
        imp_sz = 0;
        for(int i = 0; i < m; i++) {
            if(used[ed[i].i]) {
                continue;
            }

            int u = get(ed[i].u), v = get(ed[i].v);
            if(!d.connected(u, v)) {
                d.unite(u, v);
                important[imp_sz++] = ed[i];
            }
        }

        for(int i = 0; i < imp_sz; i++) {
            used2[important[i].i] = 1;
        }

        for(int i = 0; i < m; i++) {
            if(used[ed[i].i]) {
                important[imp_sz++] = ed[i];
            }
        }

        sort(important, important + imp_sz, cmp);

        for(int i = st; i <= en; i++) {
            int w = que[i].second, idx = que[i].first;

            for(int ii = 0; ii < imp_sz; ii++) {
                if(important[ii].i == idx) {
                    important[ii].w = w;
                }
            }

            for(int ii = 1; ii < imp_sz; ii++) {
                if(cmp(important[ii], important[ii - 1])) {
                    swap(important[ii], important[ii - 1]);
                }
            }
            for(int ii = imp_sz - 2; ii >= 0; ii--) {
                if(cmp(important[ii + 1], important[ii])) {
                    swap(important[ii], important[ii + 1]);
                }
            }

            d.init(cnt);
            for(int ii = 0; ii < imp_sz; ii++) {
                int u = get(important[ii].u), v = get(important[ii].v),
                    w = important[ii].w;
                if(!d.connected(u, v)) {
                    answer[i] += w, d.unite(u, v);
                }
            }
        }

        int p = 0, I = 0, J = 0;
        while(I < m && J < imp_sz) {
            if(used[ed[I].i]) {
                I++;
                continue;
            }
            if(used2[important[J].i]) {
                J++;
                continue;
            }

            if(cmp(ed[I], important[J])) {
                nw_li[p++] = ed[I++];
            } else {
                nw_li[p++] = important[J++];
            }
        }

        while(I < m) {
            if(used[ed[I].i]) {
                I++;
                continue;
            }
            nw_li[p++] = ed[I++];
        }

        while(J < imp_sz) {
            if(used2[important[J].i]) {
                J++;
                continue;
            }
            nw_li[p++] = important[J++];
        }

        for(int i = 0; i < m; i++) {
            ed[i] = nw_li[i];
        }
        for(int i = 0; i < m; i++) {
            pos_edge[ed[i].i] = i;
        }
    }

    for(int i = 0; i < q; i++) {
        cout << answer[i] << endl;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    read();
    solve();
    return 0;
}
