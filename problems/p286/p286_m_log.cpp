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
}

template<typename T>
ostream& operator<<(ostream& out, const vector<T>& a) {
    for(auto x: a) {
        out << x << ' ';
    }
    return out;
}

template<class T>
void make_larger_if_needed(vector<T>& v, int size) {
    if(v.size() < size) {
        v.resize(size);
    }
}

struct Edge {
    int u, v, idx;
    Edge(int _u = 0, int _v = 0, int _idx = 0) : u(_u), v(_v), idx(_idx) {}
};

class BipartiteColoring {
  private:
    vector<vector<pair<Edge, int>>> edges_for_ver;
    vector<bool> used;
    vector<vector<pair<int, int>>> adj;
    vector<int> memory_m;

    template<class T>
    static void make_larger_if_needed(vector<T>& v, int size) {
        if(v.size() < size) {
            v.resize(size);
        }
    }

    pair<vector<Edge>, vector<Edge>> partition_edges_euler(
        const vector<Edge>& edges, const vector<int>& vers, int n
    ) {
        make_larger_if_needed(adj, 2 * n);
        make_larger_if_needed(memory_m, edges.size());

        for(int v: vers) {
            adj[v].clear();
            adj[v + n].clear();
        }

        for(int ei = 0; ei < (int)edges.size(); ei++) {
            auto e = edges[ei];
            adj[e.u].push_back({e.v + n, ei});
            adj[e.v + n].push_back({e.u, ei});
            memory_m[ei] = 0;
        }

        function<void(int, vector<Edge>&, vector<Edge>&)> dfs =
            [&](int v, vector<Edge>& subgraph_0, vector<Edge>& subgraph_1) {
                while(!adj[v].empty()) {
                    auto [u, ei] = adj[v].back();
                    adj[v].pop_back();
                    if(memory_m[ei] == 1) {
                        continue;
                    }
                    memory_m[ei] = 1;
                    dfs(u, subgraph_0, subgraph_1);
                    if(v < n) {
                        subgraph_0.push_back(edges[ei]);
                    } else {
                        subgraph_1.push_back(edges[ei]);
                    }
                    break;
                }
            };

        vector<Edge> subgraph_0, subgraph_1;
        for(int v: vers) {
            while(!adj[v].empty()) {
                dfs(v, subgraph_0, subgraph_1);
            }
        }

        return {subgraph_0, subgraph_1};
    }

    // We don't actually use this function as it's actually slower than the
    // above one.
    vector<Edge> fast_one_colour(
        const vector<Edge>& _edges, int n, int original_m,
        vector<vector<Edge>>& answer
    ) {
        static vector<int> memory_m;
        make_larger_if_needed(memory_m, original_m);

        int m = _edges.size();
        int d = m / n;

        int l = 0;
        while((1 << l) <= m) {
            l++;
        }

        int alpha = (1 << l) / d;
        int beta = (1 << l) - d * alpha;

        vector<Edge> edges = _edges;
        vector<int> multiplicity(edges.size(), alpha);
        for(int i = 0; i < m; i++) {
            auto& e = edges[i];
            memory_m[e.idx] = i;
        }

        for(int i = 0; i < n; i++) {
            edges.push_back(Edge(i, i, -1));
            multiplicity.push_back(beta);
        }

        vector<int> vers(n);
        iota(vers.begin(), vers.end(), 0);
        while(l--) {
            vector<Edge> new_edges;
            for(int i = 0; i < (int)edges.size(); i++) {
                if(multiplicity[i] % 2) {
                    new_edges.push_back(edges[i]);
                }
                multiplicity[i] /= 2;
            }

            auto [subgraph_0, subgraph_1] =
                partition_edges_euler(new_edges, vers, n);

            int cnt0 = 0, cnt1 = 0;
            for(auto& e: subgraph_0) {
                if(e.idx == -1) {
                    cnt0++;
                }
            }

            for(auto& e: subgraph_1) {
                if(e.idx == -1) {
                    cnt1++;
                }
            }

            if(cnt0 > cnt1) {
                swap(subgraph_0, subgraph_1);
            }

            for(int i = 0; i < (int)subgraph_0.size(); i++) {
                auto& e = subgraph_0[i];
                if(e.idx == -1) {
                    multiplicity[m + e.u] += 1;
                } else {
                    int multiplicity_idx = memory_m[e.idx];
                    multiplicity[multiplicity_idx] += 1;
                }
            }
        }

        vector<Edge> answer_edges, subgraph_rest;
        for(int i = 0; i < m; i++) {
            auto& e = edges[i];
            if(multiplicity[i]) {
                answer_edges.push_back(e);
            } else {
                subgraph_rest.push_back(e);
            }
        }

        answer.push_back(answer_edges);
        return subgraph_rest;
    }

  public:
    int euler_colour(
        const vector<Edge>& edges, int n, int m, vector<vector<Edge>>& answer
    ) {
        static vector<int> memory;
        make_larger_if_needed(memory, n);

        vector<int> vers, _vers;
        for(auto e: edges) {
            _vers.push_back(e.u);
            _vers.push_back(e.v);
        }

        int max_degree = 0;
        for(int v: _vers) {
            memory[v] = -1;
        }
        for(int v: _vers) {
            if(memory[v] == -1) {
                vers.push_back(v);
                memory[v] = 0;
            }
        }
        for(auto e: edges) {
            memory[e.u]++;
            max_degree = max(max_degree, memory[e.u]);
        }

        if(max_degree == 0) {
            return 0;
        }
        if(max_degree == 1) {
            answer.push_back({});
            for(auto e: edges) {
                answer.back().push_back(e);
            }
            return 1;
        }

        if(max_degree % 2 == 1) {
            auto subgraph = fast_one_colour(edges, n, m, answer);
            return 1 + euler_colour(subgraph, n, m, answer);
        }

        auto [subgraph_0, subgraph_1] =
            partition_edges_euler(edges, vers, n);
        int colour_num_subgraph_0 = euler_colour(subgraph_0, n, m, answer);

        int d = max_degree, q = 0;
        while((1 << q) < (max_degree / 2)) {
            q++;
        }
        int to_remove_count = (1 << q) - (max_degree / 2);
        if(to_remove_count > 0 && colour_num_subgraph_0 >= to_remove_count) {
            for(int i = answer.size() - 1; i >= answer.size() - to_remove_count;
                i--) {
                for(auto& e: answer[i]) {
                    subgraph_1.push_back(e);
                }
            }
            answer.erase(answer.end() - to_remove_count, answer.end());
        }

        int colour_num_subgraph_1 = euler_colour(subgraph_1, n, m, answer);
        return colour_num_subgraph_0 + colour_num_subgraph_1;
    }
};
vector<Edge> orient_graph(
    int n, int m, const vector<int>& a, const vector<int>& b
) {
    vector<vector<pair<int, int>>> adj(n);
    vector<Edge> edges;
    for(int i = 0; i < m; i++) {
        int u = a[i], v = b[i];
        adj[u].push_back({v, i});
        adj[v].push_back({u, i});
        edges.emplace_back(u, v, i + 1);
    }

    vector<bool> used(m, false);
    vector<Edge> directed_edges;
    function<void(int)> dfs = [&](int v) {
        while(!adj[v].empty()) {
            auto [u, ei] = adj[v].back();
            adj[v].pop_back();
            if(used[ei]) {
                continue;
            }
            used[ei] = true;
            dfs(u);
            directed_edges.push_back({v, u, ei + 1});
            break;
        }
    };

    for(int v = 0; v < n; v++) {
        while(!adj[v].empty()) {
            dfs(v);
            break;
        }
    }

    return directed_edges;
}

int main() {
    int n, k;
    cin >> n >> k;
    vector<int> a, b;
    int m = n * k / 2;
    a.resize(m);
    b.resize(m);
    for(int i = 0; i < m; i++) {
        cin >> a[i] >> b[i];
        a[i]--;
        b[i]--;
    }

    vector<Edge> edges = orient_graph(n, m, a, b);
    
    vector<int> degree(n, 0);
    for(auto& e: edges) {
        degree[e.u]++;
    }
    for(int i = 0; i < n; i++) {
        if(degree[i] != k / 2) {
            assert(false);
        }
    }

    vector<vector<Edge>> ans;
    BipartiteColoring bc;
    bc.euler_colour(edges, n, m, ans);

    // assert((int)ans.size() == k / 2);
    // assert((int)ans[0].size() == n);

    cout << "YES" << endl;
    vector<int> roads;
    for(auto& e: ans[0]) {
        roads.push_back(e.idx);
    }
    sort(roads.begin(), roads.end());
    for(int i = 0; i < n; i++) {
        cout << roads[i] << endl;
    }

    return 0;
}
