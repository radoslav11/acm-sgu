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
vector<vector<int>> adj;

void read() {
    cin >> n >> m;
    adj.assign(n + 1, {});
    for(int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }
}

void solve() {
    // Stations split by degree: deg 2 are interior vertices of exactly one
    // area, deg 4 are junctions shared by two adjacent areas. Each area is a
    // cycle whose two junctions split its interior into two chains (arcs);
    // the areas themselves form a meta-cycle of length k whose edges are the
    // junctions.
    //
    // We want a proper 2-coloring of the induced subgraph after removing the
    // minimum number of stations, i.e. the remaining graph must be bipartite,
    // which means every surviving cycle must be even.
    //
    // For each area the two arcs have edge-lengths a, b with a + b = size;
    // when size is even, a and b share the same parity, call it arc_parity.
    // Walking the meta-cycle, the macro cycle (going around through one arc
    // of each area) has length parity equal to the XOR of arc_parity over
    // all areas.
    //
    // Removing a junction has two effects at once: both incident areas
    // become paths (always bipartite), and the macro cycle is broken. So a
    // junction removal is never worse than an interior removal:
    //
    //   - every odd area needs at least one of its incident junctions
    //     removed;
    //
    //   - pair adjacent odd areas along the meta-cycle and remove their
    //     shared junction (one removal kills two odd cycles);
    //
    //   - for an unmatched odd area, remove either of its two bounding
    //     junctions (covers one odd, plus collaterally an even neighbor
    //     which is fine).
    //
    // Minimum removals when there is at least one odd area equals
    // (#odd areas) - (max matching of adjacent odd pairs in the meta-cycle).
    //
    // If no area is odd, the only remaining obstacle is the macro cycle
    // parity; if it is 1, remove any single junction to break it.
    //
    // Implementation: arcs are recovered by walking from each junction edge
    // through interior vertices until hitting the next junction, with the
    // first edge incident to a junction marked on both walking directions
    // to deduplicate. Grouping arcs by unordered junction pair recovers the
    // areas - each pair has exactly two arcs that together form one area.
    // Each junction is in exactly two areas, so walking from area 0 via its
    // j2 side and always taking the "other" area at the current junction
    // traverses the meta-cycle, producing area_order[0..k-1] and
    // ordered_junctions[i] = the shared junction of area_order[i] and
    // area_order[(i+1) % k]. For the matching step, when the meta-cycle has
    // both odd and even positions, linearize starting at any even position
    // so each maximal run of consecutive odd positions is a contiguous
    // interval and pair them up greedily inside each run; when all
    // positions are odd (p == k) handle separately by pairing (0,1),(2,3),
    // ... and if k is odd take one more junction at the wrap-around for the
    // leftover. Finally BFS-2-color the graph minus the removed junctions;
    // by construction all surviving cycles are even, so coloring succeeds.

    vector<int> deg(n + 1);
    for(int v = 1; v <= n; v++) {
        deg[v] = (int)adj[v].size();
    }

    struct Arc {
        int j1, j2;
        int interior_count;
    };

    vector<Arc> arcs;
    set<pair<int, int>> seen_first_edge;

    for(int j = 1; j <= n; j++) {
        if(deg[j] != 4) {
            continue;
        }

        for(int x: adj[j]) {
            if(seen_first_edge.count({j, x})) {
                continue;
            }

            int interior_count = 0;
            int prev = j, cur = x;
            while(deg[cur] == 2) {
                interior_count++;
                int nxt = (adj[cur][0] == prev) ? adj[cur][1] : adj[cur][0];
                prev = cur;
                cur = nxt;
            }

            seen_first_edge.insert({j, x});
            seen_first_edge.insert({cur, prev});
            arcs.push_back({j, cur, interior_count});
        }
    }

    map<pair<int, int>, vector<int>> by_pair;
    for(int i = 0; i < (int)arcs.size(); i++) {
        int a = min(arcs[i].j1, arcs[i].j2);
        int b = max(arcs[i].j1, arcs[i].j2);
        by_pair[{a, b}].push_back(i);
    }

    struct Area {
        int j1, j2;
        int size;
        int arc_parity;
        bool odd;
    };

    vector<Area> areas;
    for(auto& [key, ids]: by_pair) {
        int len1 = arcs[ids[0]].interior_count;
        int len2 = arcs[ids[1]].interior_count;
        int sz = 2 + len1 + len2;
        Area ar;
        ar.j1 = key.first;
        ar.j2 = key.second;
        ar.size = sz;
        ar.odd = (sz % 2 == 1);
        ar.arc_parity = (len1 + 1) % 2;
        areas.push_back(ar);
    }

    int k = (int)areas.size();

    map<int, vector<int>> jun_areas;
    for(int i = 0; i < k; i++) {
        jun_areas[areas[i].j1].push_back(i);
        jun_areas[areas[i].j2].push_back(i);
    }

    vector<int> area_order;
    area_order.reserve(k);
    int cur_area = 0;
    area_order.push_back(0);
    int next_junction = areas[0].j2;
    while((int)area_order.size() < k) {
        auto& vv = jun_areas[next_junction];
        int next_area = (vv[0] == cur_area) ? vv[1] : vv[0];
        area_order.push_back(next_area);
        int prev_j = next_junction;
        next_junction = (areas[next_area].j1 == prev_j) ? areas[next_area].j2
                                                        : areas[next_area].j1;
        cur_area = next_area;
    }

    vector<int> ordered_junctions(k);
    for(int i = 0; i < k; i++) {
        int a = area_order[i], b = area_order[(i + 1) % k];
        int sj;
        if(areas[a].j1 == areas[b].j1 || areas[a].j1 == areas[b].j2) {
            sj = areas[a].j1;
        } else {
            sj = areas[a].j2;
        }

        ordered_junctions[i] = sj;
    }

    vector<bool> is_odd(k);
    for(int i = 0; i < k; i++) {
        is_odd[i] = areas[area_order[i]].odd;
    }

    int p = 0;
    for(int i = 0; i < k; i++) {
        if(is_odd[i]) {
            p++;
        }
    }

    vector<bool> remove_junction(k, false);

    if(p == 0) {
        int macro_parity = 0;
        for(int i = 0; i < k; i++) {
            macro_parity ^= areas[area_order[i]].arc_parity;
        }
        if(macro_parity != 0) {
            remove_junction[0] = true;
        }
    } else if(p == k) {
        for(int i = 0; i + 1 < k; i += 2) {
            remove_junction[i] = true;
        }
        if(k % 2 == 1) {
            remove_junction[k - 1] = true;
        }
    } else {
        int start_pos = 0;
        while(is_odd[start_pos]) {
            start_pos = (start_pos + 1) % k;
        }

        int i = 0;
        while(i < k) {
            int pos = (start_pos + i) % k;
            if(!is_odd[pos]) {
                i++;
                continue;
            }

            int run_len = 0;
            while(i + run_len < k && is_odd[(start_pos + i + run_len) % k]) {
                run_len++;
            }

            for(int j = 0; j + 1 < run_len; j += 2) {
                int p1 = (start_pos + i + j) % k;
                remove_junction[p1] = true;
            }

            if(run_len % 2 == 1) {
                int last_pos = (start_pos + i + run_len - 1) % k;
                remove_junction[last_pos] = true;
            }

            i += run_len;
        }
    }

    set<int> removed;
    for(int i = 0; i < k; i++) {
        if(remove_junction[i]) {
            removed.insert(ordered_junctions[i]);
        }
    }

    vector<int> color(n + 1, -1);
    for(int s = 1; s <= n; s++) {
        if(removed.count(s) || color[s] != -1) {
            continue;
        }

        queue<int> q;
        q.push(s);
        color[s] = 0;
        while(!q.empty()) {
            int v = q.front();
            q.pop();
            for(int u: adj[v]) {
                if(removed.count(u)) {
                    continue;
                }
                if(color[u] == -1) {
                    color[u] = 1 - color[v];
                    q.push(u);
                }
            }
        }
    }

    vector<int> c0, c1;
    for(int v = 1; v <= n; v++) {
        if(color[v] == 0) {
            c0.push_back(v);
        } else if(color[v] == 1) {
            c1.push_back(v);
        }
    }

    cout << c0.size();
    for(int v: c0) {
        cout << ' ' << v;
    }

    cout << '\n';
    cout << c1.size();
    for(int v: c1) {
        cout << ' ' << v;
    }
    cout << '\n';
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
