#include <bits/stdc++.h>
// #include <coding_library/graph/scc.hpp>

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

class StronglyConnectedComponents {
  private:
    vector<bool> visited;

    void dfs1(int u) {
        visited[u] = true;
        for(int v: adj[u]) {
            if(!visited[v]) {
                dfs1(v);
            }
        }

        top_sort.push_back(u);
    }

    void dfs2(int u) {
        for(int v: radj[u]) {
            if(comp[v] == -1) {
                comp[v] = comp[u];
                dfs2(v);
            }
        }
    }

  public:
    int n;
    vector<vector<int>> adj, radj;
    vector<int> comp, comp_ids, top_sort;

    StronglyConnectedComponents() {}
    StronglyConnectedComponents(int _n) { init(_n); }

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        radj[v].push_back(u);
    }

    void init(int _n) {
        n = _n;
        comp_ids.clear();
        top_sort.clear();
        adj.assign(n, {});
        radj.assign(n, {});
    }

    void find_components() {
        comp.assign(n, -1);
        visited.assign(n, false);

        for(int i = 0; i < n; i++) {
            if(!visited[i]) {
                dfs1(i);
            }
        }

        reverse(top_sort.begin(), top_sort.end());
        for(int u: top_sort) {
            if(comp[u] == -1) {
                comp[u] = (int)comp_ids.size();
                comp_ids.push_back(comp[u]);
                dfs2(u);
            }
        }
    }
};

int n;
vector<string> ord1, ord2;

void read() {
    cin >> n;
    ord1.resize(n);
    ord2.resize(n);
    cin >> ord1 >> ord2;
}

void solve() {
    // The solution here is fairly simple - we build a graph out of the
    // two orders, and notice that if there are cycles we can't deduce
    // the relative order of the elements of said cycle (and so we
    // revert to lexicographical). This should immediately lead us to think
    // about strongly connected components. Because the edges are simply given
    // by two orders, we can notice that after compressing into strongly
    // connected components we are left with a chain. Part of the problem
    // statement talks about having the same "set" in the answer for every
    // prefix if the two orders match in that prefix. We can notice that the
    // above solution will satisfy this trivially, as if the prefix match, so do
    // the suffixes, so we can cut the 2 edges coming from the two orders and
    // get two disjoint graphs.

    map<string, int> name_to_id;
    for(int i = 0; i < n; i++) {
        name_to_id[ord1[i]] = i;
    }

    StronglyConnectedComponents scc(n);
    for(int i = 1; i < n; i++) {
        scc.add_edge(i, i - 1);
        scc.add_edge(name_to_id[ord2[i]], name_to_id[ord2[i - 1]]);
    }

    scc.find_components();

    vector<int> min_id_in_comp(n, n);
    for(int i = 0; i < n; i++) {
        min_id_in_comp[scc.comp[i]] = min(min_id_in_comp[scc.comp[i]], i);
    }

    vector<pair<int, string>> result;
    for(int i = 0; i < n; i++) {
        result.push_back({min_id_in_comp[scc.comp[i]], ord1[i]});
    }

    sort(result.begin(), result.end());
    for(int i = 0; i < n; i++) {
        cout << result[i].second << endl;
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
