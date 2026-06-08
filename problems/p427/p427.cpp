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

int n = 0;
vector<array<string, 3>> vertex_coord;
vector<vector<int>> adj;
vector<vector<char>> connected;
map<array<int64_t, 3>, int> vertex_id;
vector<char> visited;
vector<int> path;

void read() {
    int f;
    cin >> f;

    set<pair<int, int>> edges;
    for(int facet = 0; facet < f; facet++) {
        int k;
        cin >> k;

        vector<int> ids(k);
        for(int i = 0; i < k; i++) {
            array<string, 3> s;
            cin >> s[0] >> s[1] >> s[2];

            array<int64_t, 3> key;
            for(int j = 0; j < 3; j++) {
                key[j] = llround(stod(s[j]) * 1e9);
            }

            auto it = vertex_id.find(key);
            if(it == vertex_id.end()) {
                ids[i] = n++;
                vertex_id[key] = ids[i];
                vertex_coord.push_back(s);
            } else {
                ids[i] = it->second;
            }
        }

        for(int i = 0; i < k; i++) {
            int a = ids[i], b = ids[(i + 1) % k];
            if(a > b) {
                swap(a, b);
            }
            edges.insert({a, b});
        }
    }

    adj.assign(n, {});
    connected.assign(n, vector<char>(n, 0));
    for(auto& e: edges) {
        adj[e.first].push_back(e.second);
        adj[e.second].push_back(e.first);
        connected[e.first][e.second] = 1;
        connected[e.second][e.first] = 1;
    }
}

bool dfs(int cur, int depth) {
    if(depth == n) {
        return connected[cur][path[0]];
    }

    for(int u = 0; u < n; u++) {
        if(visited[u]) {
            continue;
        }

        int avail = 0;
        for(int nb: adj[u]) {
            if(!visited[nb] || nb == cur || nb == path[0]) {
                avail++;
            }
        }

        if(avail < 2) {
            return false;
        }
    }

    int target = n - depth;
    vector<char> seen(n, 0);
    vector<int> reach;
    for(int nb: adj[cur]) {
        if(!visited[nb] && !seen[nb]) {
            seen[nb] = 1;
            reach.push_back(nb);
        }
    }

    int cnt = reach.size();
    while(!reach.empty()) {
        int x = reach.back();
        reach.pop_back();
        for(int nb: adj[x]) {
            if(!visited[nb] && !seen[nb]) {
                seen[nb] = 1;
                cnt++;
                reach.push_back(nb);
            }
        }
    }

    if(cnt != target) {
        return false;
    }

    vector<pair<int, int>> cand;
    for(int nb: adj[cur]) {
        if(visited[nb]) {
            continue;
        }

        int deg = 0;
        for(int x: adj[nb]) {
            if(!visited[x]) {
                deg++;
            }
        }
        cand.push_back({deg, nb});
    }

    sort(cand.begin(), cand.end());
    for(auto& c: cand) {
        int nb = c.second;
        visited[nb] = 1;
        path[depth] = nb;
        if(dfs(nb, depth + 1)) {
            return true;
        }
        visited[nb] = 0;
    }

    return false;
}

void solve() {
    // Geometry to graph: the facets arrive as cyclic vertex lists, so the edge
    // graph is rebuilt by identifying vertices with equal coordinates (rounded
    // to the 9 printed digits, safe because distinct vertices lie at least
    // 5e-7 apart) and joining every facet's consecutive vertices; an edge that
    // borders two facets is simply encountered twice. By Steinitz's theorem
    // this graph is planar and 3-connected, i.e. a genuine polyhedral graph.
    //
    // The solid angle at a vertex is the area its incident faces carve out of a
    // unit sphere centred there. For a vertex of degree d it equals the
    // spherical-polygon excess (the sum of the dihedral angles around the
    // vertex) - (d - 2)*pi, and always lies strictly between 0 and 2*pi.
    // Summing this over all vertices gives S(P), the total solid angle, with a
    // full sphere measuring 4*pi steradians.
    //
    // Barnette's theorem ties S(P) to Hamiltonicity: a 3-polytope's
    // combinatorial type can be realised by polyhedra whose total solid angle
    // is arbitrarily close to 0 exactly when its edge graph has a Hamiltonian
    // cycle, while every non-Hamiltonian type keeps S(P) bounded away from 0.
    // The intuition for one direction is that a Hamiltonian cycle lets one
    // flatten the polyhedron towards that cycle, driving every vertex's solid
    // angle to 0, which no non-Hamiltonian type permits. The problem promises
    // S(P) < 4 steradians, which sits under that non-Hamiltonian bound, so the
    // graph is guaranteed Hamiltonian and the answer is always "Yes".
    // Grinberg's theorem gives a separate, purely combinatorial necessary
    // condition for planar Hamiltonicity but it is not needed here.
    //
    // Constructing the cycle is the only real work, and a guaranteed-polynomial
    // route exists in special cases: a 4-connected planar graph is Hamiltonian
    // and Chiba-Nishizeki finds a cycle in linear time. But small-solid-angle
    // polyhedra need not be 4-connected (a thin prism is only 3-connected), and
    // turning Barnette's "flatten towards the cycle" argument into a direct
    // construction is fiddly, so with at most 100 vertices we instead opt for a
    // heuristic search; the guaranteed existence of a cycle keeps it fast in
    // practice on these instances, though it carries no worst-case bound.
    //
    // The search is a backtracking walk from vertex 0. At each step it extends
    // the path to the unvisited neighbour that itself has the fewest unvisited
    // neighbours (Warnsdorff's rule), which heads into the cramped corners of
    // the graph first and so rarely has to backtrack. A branch is dropped the
    // moment it becomes unsatisfiable: some unvisited vertex is left with fewer
    // than two usable incident edges (it could never get both a predecessor and
    // a successor), or the unvisited vertices stop being reachable from the
    // current endpoint. The path closes into a cycle once all n vertices are
    // used and the final one is adjacent to vertex 0.
    //
    // Some references are:
    // - D. Barnette, "The sum of the solid angles of a d-polytope",
    //   Geometriae Dedicata 1 (1972), 100-102, doi:10.1007/BF00147383.
    // - E. Ja. Grinberg (1968), "Plane homogeneous graphs of degree three
    //   without Hamiltonian circuits"; English translation by D. Zeps,
    //   arXiv:0908.2563.

    visited.assign(n, 0);
    path.assign(n, 0);
    visited[0] = 1;
    path[0] = 0;

    if(dfs(0, 1)) {
        cout << "Yes\n" << n << "\n";
        for(int i = 0; i < n; i++) {
            auto& c = vertex_coord[path[i]];
            cout << c[0] << ' ' << c[1] << ' ' << c[2] << '\n';
        }
    } else {
        cout << "No\n";
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
