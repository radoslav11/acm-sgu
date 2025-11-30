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

const int64_t inf = 1e18;

int n, d1, c1, d2, c2;
vector<int> p;
vector<vector<int>> adj;
vector<vector<int>> dist_all;
vector<vector<int>> first_step;

void read() {
    cin >> n >> d1 >> c1 >> d2 >> c2;
    p.resize(n);
    cin >> p;
    adj.resize(n);
    for(int i = 0; i < n - 1; i++) {
        int a, b;
        cin >> a >> b;
        a--;
        b--;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }
}

void precompute_all_pairs() {
    dist_all.assign(n, vector<int>(n, INT_MAX));
    first_step.assign(n, vector<int>(n, -1));
    queue<int> q;
    for(int s = 0; s < n; s++) {
        while(!q.empty()) {
            q.pop();
        }
        dist_all[s][s] = 0;
        for(int u: adj[s]) {
            dist_all[s][u] = 1;
            first_step[s][u] = u;
            q.push(u);
        }
        while(!q.empty()) {
            int v = q.front();
            q.pop();
            for(int u: adj[v]) {
                if(dist_all[s][u] == INT_MAX) {
                    dist_all[s][u] = dist_all[s][v] + 1;
                    first_step[s][u] = first_step[s][v];
                    q.push(u);
                }
            }
        }
    }
}

pair<int64_t, vector<int>> best_subtree_from_root_fast(
    int root, const vector<int>& nodes, int d, int c
) {
    if(d == 0) {
        return c >= 1 ? make_pair((int64_t)p[root], vector<int>{root})
                      : make_pair(-inf, vector<int>{});
    }
    if(c == 1) {
        return {-inf, {}};
    }

    int target = d / 2;
    int64_t sum = p[root];
    vector<int> selected = {root};
    vector<int> boundary;
    boundary.reserve(nodes.size());

    for(int v: nodes) {
        if(v == root) {
            continue;
        }
        int dv = dist_all[root][v];
        if(dv < target) {
            sum += p[v];
            selected.push_back(v);
        } else if(dv == target) {
            boundary.push_back(v);
        }
    }

    if(boundary.size() < 2) {
        return make_pair(-inf, vector<int>{});
    }

    static vector<int> best_val, best_node;
    if((int)best_val.size() < n) {
        best_val.assign(n, INT_MIN);
        best_node.assign(n, -1);
    }
    vector<int> touched;
    touched.reserve(adj[root].size());
    for(int v: boundary) {
        int cid = first_step[root][v];
        if(cid < 0) {
            continue;
        }
        if(p[v] > best_val[cid]) {
            if(best_node[cid] == -1) {
                touched.push_back(cid);
            }
            best_val[cid] = p[v];
            best_node[cid] = v;
        }
    }
    if(touched.size() < 2) {
        for(int cid: touched) {
            best_val[cid] = INT_MIN;
            best_node[cid] = -1;
        }
        return {-inf, {}};
    }

    int first_cid = -1, first_v = -1, fv = INT_MIN;
    for(int cid: touched) {
        if(best_val[cid] > fv) {
            fv = best_val[cid];
            first_cid = cid;
            first_v = best_node[cid];
        }
    }
    int second_v = -1, sv = INT_MIN;
    for(int cid: touched) {
        if(cid != first_cid && best_val[cid] > sv) {
            sv = best_val[cid];
            second_v = best_node[cid];
        }
    }
    if(second_v == -1) {
        for(int cid: touched) {
            best_val[cid] = INT_MIN;
            best_node[cid] = -1;
        }
        return {-inf, {}};
    }
    sum += p[first_v];
    selected.push_back(first_v);
    sum += p[second_v];
    selected.push_back(second_v);

    if(c > 2) {
        vector<int> rem;
        rem.reserve(boundary.size());
        for(int v: boundary) {
            if(v != first_v && v != second_v) {
                rem.push_back(v);
            }
        }
        int to_take = min((int)rem.size(), c - 2);
        if(to_take > 0) {
            nth_element(
                rem.begin(), rem.begin() + to_take, rem.end(),
                [](int a, int b) { return p[a] > p[b]; }
            );
            for(int i = 0; i < to_take; i++) {
                sum += p[rem[i]];
                selected.push_back(rem[i]);
            }
        }
    }
    for(int cid: touched) {
        best_val[cid] = INT_MIN;
        best_node[cid] = -1;
    }
    return {sum, selected};
}

pair<int64_t, vector<int>> best_subtree_from_edge_fast(
    int u, int v, const vector<int>& nodes, int d, int c
) {
    if(d == 0 || c == 1) {
        return {-inf, {}};
    }
    int target = (d - 1) / 2;
    int64_t sum = p[u] + p[v];
    vector<int> selected = {u, v};
    if(target == 0) {
        return {sum, selected};
    }

    vector<int> bound_u, bound_v;
    bound_u.reserve(nodes.size());
    bound_v.reserve(nodes.size());
    for(int w: nodes) {
        if(w == u || w == v) {
            continue;
        }
        int du = dist_all[u][w];
        int dv = dist_all[v][w];
        if(du < dv) {
            if(du < target) {
                sum += p[w];
                selected.push_back(w);
            } else if(du == target) {
                bound_u.push_back(w);
            }
        } else {
            if(dv < target) {
                sum += p[w];
                selected.push_back(w);
            } else if(dv == target) {
                bound_v.push_back(w);
            }
        }
    }
    if(bound_u.empty() || bound_v.empty()) {
        return {-inf, {}};
    }
    int bu = bound_u[0];
    for(int x: bound_u) {
        if(p[x] > p[bu]) {
            bu = x;
        }
    }
    int bv = bound_v[0];
    for(int x: bound_v) {
        if(p[x] > p[bv]) {
            bv = x;
        }
    }
    sum += p[bu];
    selected.push_back(bu);
    sum += p[bv];
    selected.push_back(bv);
    if(c > 2) {
        vector<int> rem;
        rem.reserve(bound_u.size() + bound_v.size());
        for(int x: bound_u) {
            if(x != bu) {
                rem.push_back(x);
            }
        }
        for(int x: bound_v) {
            if(x != bv) {
                rem.push_back(x);
            }
        }
        int to_take = min((int)rem.size(), c - 2);
        if(to_take > 0) {
            nth_element(
                rem.begin(), rem.begin() + to_take, rem.end(),
                [](int a, int b) { return p[a] > p[b]; }
            );
            for(int i = 0; i < to_take; i++) {
                sum += p[rem[i]];
                selected.push_back(rem[i]);
            }
        }
    }
    return {sum, selected};
}

void dfs_collect(
    int v, int par, int dist, int split, vector<vector<int>>& by_dist,
    int max_dist
) {
    if(v == split) {
        return;
    }
    if(dist > max_dist) {
        return;
    }
    if(dist >= by_dist.size()) {
        by_dist.resize(dist + 1);
    }
    by_dist[dist].push_back(v);
    for(int u: adj[v]) {
        if(u != par && u != split) {
            dfs_collect(u, v, dist + 1, split, by_dist, max_dist);
        }
    }
}

void dfs_mark_subtree(
    int v, int par, int dist, int split, int subtree_id,
    vector<vector<int>>& by_dist, vector<int>& subtree_mark, int max_dist
) {
    if(v == split) {
        return;
    }
    if(dist > max_dist) {
        return;
    }
    if(dist >= by_dist.size()) {
        by_dist.resize(dist + 1);
    }
    by_dist[dist].push_back(v);
    subtree_mark[v] = subtree_id;
    for(int u: adj[v]) {
        if(u != par && u != split) {
            dfs_mark_subtree(
                u, v, dist + 1, split, subtree_id, by_dist, subtree_mark,
                max_dist
            );
        }
    }
}

pair<int64_t, vector<int>> best_subtree_from_root(
    int root, int split, int d, int c
) {
    if(d == 0) {
        return c >= 1 ? make_pair((int64_t)p[root], vector<int>{root})
                      : make_pair(-inf, vector<int>{});
    }
    if(c == 1) {
        return {-inf, {}};
    }

    vector<vector<int>> by_dist;
    vector<int> subtree_mark(n, -1);

    int subtree_id = 0;
    for(int child: adj[root]) {
        if(child != split) {
            dfs_mark_subtree(
                child, root, 1, split, subtree_id++, by_dist, subtree_mark,
                d / 2
            );
        }
    }

    if(by_dist.empty()) {
        return {-inf, {}};
    }

    subtree_mark[root] = -1;

    int64_t sum = p[root];
    vector<int> selected = {root};
    int target_dist = d / 2;

    for(int dist = 1; dist < by_dist.size() && dist < target_dist; dist++) {
        for(int v: by_dist[dist]) {
            sum += p[v];
            selected.push_back(v);
        }
    }

    if(target_dist < by_dist.size()) {
        vector<int>& cands = by_dist[target_dist];

        if(d > 0) {
            if((int)cands.size() < 2) {
                return {-inf, {}};
            }

            vector<pair<int, int>> best_per(subtree_id, {INT_MIN, -1});
            int distinct = 0;
            for(int v: cands) {
                int sid = subtree_mark[v];
                if(sid < 0 || sid >= subtree_id) {
                    continue;
                }
                if(p[v] > best_per[sid].first) {
                    if(best_per[sid].second == -1) {
                        distinct++;
                    }
                    best_per[sid] = {p[v], v};
                }
            }
            if(distinct < 2) {
                return {-inf, {}};
            }

            int first_subtree = -1, first_v = -1, best_val = INT_MIN;
            for(int sid = 0; sid < subtree_id; sid++) {
                if(best_per[sid].second == -1) {
                    continue;
                }
                if(best_per[sid].first > best_val) {
                    best_val = best_per[sid].first;
                    first_subtree = sid;
                    first_v = best_per[sid].second;
                }
            }
            sum += p[first_v];
            selected.push_back(first_v);

            int second_v = -1, second_val = INT_MIN;
            for(int sid = 0; sid < subtree_id; sid++) {
                if(sid == first_subtree) {
                    continue;
                }
                if(best_per[sid].second == -1) {
                    continue;
                }
                if(best_per[sid].first > second_val) {
                    second_val = best_per[sid].first;
                    second_v = best_per[sid].second;
                }
            }
            if(second_v == -1) {
                return {-inf, {}};
            }
            sum += p[second_v];
            selected.push_back(second_v);

            if(c > 2) {
                vector<int> rem;
                rem.reserve(cands.size());
                for(int v: cands) {
                    if(v != first_v && v != second_v) {
                        rem.push_back(v);
                    }
                }
                int to_take = min((int)rem.size(), c - 2);
                if(to_take > 0) {
                    nth_element(
                        rem.begin(), rem.begin() + to_take, rem.end(),
                        [](int a, int b) { return p[a] > p[b]; }
                    );
                    for(int i = 0; i < to_take; i++) {
                        sum += p[rem[i]];
                        selected.push_back(rem[i]);
                    }
                }
            }
        } else {
            int to_select = min((int)cands.size(), c);
            if(to_select > 0) {
                nth_element(
                    cands.begin(), cands.begin() + to_select, cands.end(),
                    [](int a, int b) { return p[a] > p[b]; }
                );
            }
            for(int i = 0; i < to_select; i++) {
                sum += p[cands[i]];
                selected.push_back(cands[i]);
            }
        }
    } else {
        return {-inf, {}};
    }

    return {sum, selected};
}

pair<int64_t, vector<int>> best_subtree_from_edge(
    int u, int v, int split, int d, int c
) {
    if(d == 0 || c == 1) {
        return {-inf, {}};
    }
    if(u == split || v == split) {
        return {-inf, {}};
    }

    vector<vector<int>> by_dist_u, by_dist_v;
    dfs_collect(u, v, 0, split, by_dist_u, (d - 1) / 2);
    dfs_collect(v, u, 0, split, by_dist_v, (d - 1) / 2);

    int64_t sum = p[u] + p[v];
    vector<int> selected = {u, v};
    int target_dist = (d - 1) / 2;

    if(target_dist == 0) {
        return {sum, selected};
    }

    for(int dist = 1; dist < target_dist; dist++) {
        if(dist < by_dist_u.size()) {
            for(int w: by_dist_u[dist]) {
                sum += p[w];
                selected.push_back(w);
            }
        }
        if(dist < by_dist_v.size()) {
            for(int w: by_dist_v[dist]) {
                sum += p[w];
                selected.push_back(w);
            }
        }
    }

    vector<int> cands_u, cands_v;
    if(target_dist < by_dist_u.size()) {
        cands_u = by_dist_u[target_dist];
    }
    if(target_dist < by_dist_v.size()) {
        cands_v = by_dist_v[target_dist];
    }

    if(cands_u.empty() || cands_v.empty()) {
        return {-inf, {}};
    }

    int best_u = cands_u[0];
    for(int x: cands_u) {
        if(p[x] > p[best_u]) {
            best_u = x;
        }
    }
    int best_v = cands_v[0];
    for(int x: cands_v) {
        if(p[x] > p[best_v]) {
            best_v = x;
        }
    }

    sum += p[best_u];
    selected.push_back(best_u);
    sum += p[best_v];
    selected.push_back(best_v);

    if(c > 2) {
        vector<int> rem;
        rem.reserve((int)cands_u.size() + (int)cands_v.size());
        for(int x: cands_u) {
            if(x != best_u) {
                rem.push_back(x);
            }
        }
        for(int x: cands_v) {
            if(x != best_v) {
                rem.push_back(x);
            }
        }
        int to_take = min((int)rem.size(), c - 2);
        if(to_take > 0) {
            nth_element(
                rem.begin(), rem.begin() + to_take, rem.end(),
                [](int a, int b) { return p[a] > p[b]; }
            );
            for(int i = 0; i < to_take; i++) {
                sum += p[rem[i]];
                selected.push_back(rem[i]);
            }
        }
    }

    return {sum, selected};
}

pair<int64_t, vector<int>> best_subtree_in_component(
    const vector<int>& nodes, int split, int d, int c
) {
    if(nodes.empty()) {
        return {-inf, {}};
    }

    if(d == 0 && c == 1) {
        int64_t res = -inf;
        int best = -1;
        for(int v: nodes) {
            if(p[v] > res) {
                res = p[v];
                best = v;
            }
        }
        return {res, {best}};
    }

    pair<int64_t, vector<int>> res = {-inf, {}};

    if(d % 2 == 0) {
        for(int root: nodes) {
            auto cur = best_subtree_from_root_fast(root, nodes, d, c);
            if(cur.first > res.first) {
                res = cur;
            }
        }
    } else {
        vector<char> in_comp(n, 0);
        for(int v: nodes) {
            in_comp[v] = 1;
        }
        for(int u: nodes) {
            for(int v: adj[u]) {
                if(u < v && in_comp[v]) {
                    auto cur = best_subtree_from_edge_fast(u, v, nodes, d, c);
                    if(cur.first > res.first) {
                        res = cur;
                    }
                }
            }
        }
    }

    return res;
}

void get_component(int v, int split, vector<bool>& vis, vector<int>& comp) {
    if(vis[v] || v == split) {
        return;
    }
    vis[v] = true;
    comp.push_back(v);
    for(int u: adj[v]) {
        get_component(u, split, vis, comp);
    }
}

void solve() {
    // The problem asks us to choose two subtrees that don't touch each other
    // and have the largest sum of p[i] of selected vertices (final answer is
    // sum(p) - sum(selected p)). The two trees have constraints in terms of the
    // diameters they have (D1 and D2), and the number of diameter endpoints (C1
    // and C2). The way to start with this problem is to see that n is fairly
    // small - it can be up to 200.
    //
    // We will start by trying every possible "splitting vertex". In other
    // words, we will try to explicitly delete each node. This splits the tree
    // into two smaller trees T1 and T2. We will then try the two options - D1
    // and C1 in T1, and D1 and C1 in T2 (and similarly D2 and C2 in the
    // opposite).
    //
    // The problem is now simpler - given a tree T, find the subtree with
    // largest sum that has diameter D and C diameter endpoints. The easiest way
    // to solve this is to consider two cases:
    //     1) D is even. This means that there is a center node Q for any valid
    //        subtree. We will try every possible vertex Q, do a DFS with Q as
    //        the root and for each node. This partitions all other nodes in
    //        groups based on the distance from Q to them. The strategy
    //        afterwards is simple - we will take all nodes with distance < D
    //        / 2. For distance exactly D / 2, to ensure diameter is actually D,
    //        we must select nodes from at least 2 different child subtrees of
    //        Q. We mark each node by which child subtree it belongs to, then
    //        greedily select the largest node from each subtree (guaranteeing
    //        2+ subtrees), then fill remaining slots up to C with the largest
    //        remaining nodes.
    //     2) D is odd. For this to be the case C > 1 (otherwise impossible). We
    //        will do something similar to case (1), but we will try to select
    //        an edge, or two adjacent vertices Q1 and Q2. We will do DFS from
    //        both (with parent being Q1, Q2, so that we visit only the
    //        corresponding parts of the tree). Essentially, we want to select
    //        all nodes at distance < (D - 1) / 2. When it comes to distance
    //        exactly (D - 1) / 2, we will select the two largest nodes from
    //        both sides, and then from the rest of the nodes select the largest
    //        (C - 2). This can be done with nth_element again, to avoid
    //        sorting.
    //
    // The is also one special case. C = 1 and D = 0. If C = 1 and D > 0 or D =
    // 0, and C != 1, it's impossible. In this special case, we can select a
    // single node, so we just get the maximum p[] value.
    //
    // Overall, if implemented properly, the time complexity is O(N^3). In terms
    // of implementation, to make it clean we can share a lot of the code
    // between the two cases. Implementing well requires a series of
    // precomputes.

    int64_t total_sum = 0;
    for(int x: p) {
        total_sum += x;
    }

    int64_t best = -inf;
    int best_split = -1;
    vector<int> best_selected;

    for(int split = 0; split < n; split++) {
        vector<bool> vis(n);
        vector<vector<int>> components;

        for(int u: adj[split]) {
            if(!vis[u]) {
                vector<int> comp;
                get_component(u, split, vis, comp);
                components.push_back(comp);
            }
        }

        int m = (int)components.size();
        vector<pair<int64_t, vector<int>>> best1(m), best2(m);
        for(int i = 0; i < m; i++) {
            best1[i] = best_subtree_in_component(components[i], split, d1, c1);
            best2[i] = best_subtree_in_component(components[i], split, d2, c2);
        }

        for(int i = 0; i < m; i++) {
            for(int j = i + 1; j < m; j++) {
                int64_t total = best1[i].first + best2[j].first;
                if(total > best) {
                    best = total;
                    best_split = split;
                    best_selected = best1[i].second;
                    best_selected.insert(
                        best_selected.end(), best2[j].second.begin(),
                        best2[j].second.end()
                    );
                }
                total = best2[i].first + best1[j].first;
                if(total > best) {
                    best = total;
                    best_split = split;
                    best_selected = best2[i].second;
                    best_selected.insert(
                        best_selected.end(), best1[j].second.begin(),
                        best1[j].second.end()
                    );
                }
            }
        }
    }

    if(best < 0) {
        cout << -1 << "\n";
    } else {
        vector<bool> kept(n);
        for(int v: best_selected) {
            kept[v] = true;
        }

        vector<int> destroyed;
        int64_t cost = 0;
        for(int i = 0; i < n; i++) {
            if(!kept[i]) {
                destroyed.push_back(i + 1);
                cost += p[i];
            }
        }

        cout << cost << "\n";
        if(!destroyed.empty()) {
            cout << destroyed << "\n";
        } else {
            cout << "\n";
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
        precompute_all_pairs();
        // cout << "Case #" << test << ": ";
        solve();
    }

    return 0;
}
