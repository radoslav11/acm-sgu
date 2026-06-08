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
vector<vector<int>> edges;

int nb;
vector<vector<int>> adj, kids;
vector<int> dep, par, up_target;
vector<char> seen;
vector<map<int, vector<array<int, 3>>>> plan;
vector<int> country;
int next_country;

int encode(int big, int target) { return big * nb + (target + 1); }

void dfs(int v, int p, int d) {
    seen[v] = 1;
    par[v] = p;
    dep[v] = d;
    up_target[v] = -1;
    for(int u: adj[v]) {
        if(u == p) {
            continue;
        }
        if(!seen[u]) {
            kids[v].push_back(u);
            dfs(u, v, d + 1);
        } else if(dep[u] < d) {
            up_target[v] = max(up_target[v], dep[u]);
        }
    }
}

void build_dp(int v) {
    for(int c: kids[v]) {
        build_dp(c);
    }

    map<int, vector<array<int, 3>>> cur;
    cur[encode(0, up_target[v])] = {};
    for(int c: kids[v]) {
        map<int, vector<array<int, 3>>> nxt;
        for(auto& [state, decisions]: cur) {
            int target = state % nb - 1;
            for(auto& [child_state, _]: plan[c]) {
                if(child_state / nb == 1) {
                    if(!nxt.count(state)) {
                        nxt[state] = decisions;
                        nxt[state].push_back({c, 0, child_state});
                    }
                    break;
                }
            }

            for(auto& [child_state, _]: plan[c]) {
                int child_target = child_state % nb - 1;
                if(child_target <= dep[v] - 1) {
                    int merged = encode(1, max(target, child_target));
                    if(!nxt.count(merged)) {
                        nxt[merged] = decisions;
                        nxt[merged].push_back({c, 1, child_state});
                    }
                }
            }
        }

        cur.swap(nxt);
    }

    plan[v] = cur;
}

void build_country(int v, int state, int id) {
    country[v] = id;
    for(auto& [c, action, child_state]: plan[v][state]) {
        build_country(c, child_state, action == 1 ? id : ++next_country);
    }
}

bool partition_from(int root) {
    kids.assign(n + 1, {});
    dep.assign(n + 1, 0);
    par.assign(n + 1, 0);
    up_target.assign(n + 1, 0);
    seen.assign(n + 1, 0);
    plan.assign(n + 1, {});

    dfs(root, 0, 0);
    build_dp(root);

    int whole = encode(1, -1);
    if(!plan[root].count(whole)) {
        return false;
    }

    country.assign(n + 1, 0);
    next_country = 1;
    build_country(root, whole, 1);
    return true;
}

void read() {
    cin >> n >> m;
    edges.assign(m, {});
    for(auto& e: edges) {
        e.resize(2);
        cin >> e[0] >> e[1];
    }
}

void solve() {
    // A country must induce a tree, so we look for divisions in which every
    // country is a connected subtree of a rooted DFS spanning tree. With a
    // DFS tree every non-tree edge is a back edge between an ancestor and a
    // descendant, so the task becomes: cut some tree edges so that each
    // resulting piece has at least two vertices and the two endpoints of every
    // back edge land in different pieces (otherwise the piece would close that
    // back edge into a cycle and stop being a tree).
    //
    // We decide the cuts with a bottom-up DP. For each vertex v its "block" is
    // the piece containing v inside v's subtree; the relevant facts about it
    // are whether it already has two vertices and up_target, the depth of the
    // closest ancestor it still reaches through a back edge. The block must be
    // cut before climbing to that ancestor. Processing v's children one by one
    // we either cut a child's block off as a finished country (allowed only
    // when that block already has size two or more) or merge it into v's block
    // (allowed only when the child's deepest pending back edge stays strictly
    // above v, so no back edge becomes internal). plan[v] stores, for every
    // reachable (size, deepest-target) state, which choice was made per child,
    // which lets build_country replay the cuts.
    //
    // A valid division can need a country that is connected only through a back
    // edge, which no single rooted tree expresses. So we try each vertex as the
    // root and accept the first DFS tree whose whole block is finishable with
    // size at least two.

    nb = n + 2;
    adj.assign(n + 1, {});
    for(auto& e: edges) {
        adj[e[0]].push_back(e[1]);
        adj[e[1]].push_back(e[0]);
    }

    int root = 1;
    while(root <= n && !partition_from(root)) {
        root++;
    }

    vector<int> label(n + 2, 0);
    int last = 0;
    for(int v = 1; v <= n; v++) {
        if(!label[country[v]]) {
            label[country[v]] = ++last;
        }
        country[v] = label[country[v]];
    }

    for(int v = 1; v <= n; v++) {
        cout << country[v] << " \n"[v == n];
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T = 1;
    cin >> T;
    for(int test = 1; test <= T; test++) {
        read();
        solve();
    }

    return 0;
}
