#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace std;
using namespace __gnu_pbds;

#pragma GCC optimize("O3")
#pragma GCC target("avx2")
#pragma GCC optimize("unroll-loops")

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

struct custom_hash {
    static uint64_t splitmix64(uint64_t x) {
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }
    size_t operator()(uint64_t x) const {
        static const uint64_t FIXED_RANDOM =
            chrono::steady_clock::now().time_since_epoch().count();
        return splitmix64(x + FIXED_RANDOM);
    }
};

int n, m, q;
vector<vector<int>> know;

void read() {
    cin >> n >> m;
    know.assign(n, {});
    for(int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        a--;
        b--;
        know[a].push_back(b);
    }
    cin >> q;
}

void solve() {
    // The core idea in the solution is to do small to large trick. Whenever two
    // groups merge (query 2), we will choose the larger set and keep it as as,
    // and manually move everyone from the smaller group. This amortizes to O(N
    // log^2 N) or O(N log N), depending on what structure we use. The tricky
    // part is maintaining the correct identifiers. Particularly, if we change
    // the id of some group, we should also change it in the knowledge sets of
    // other groups. To make this convenient, when we merge groups u, v -> k
    // (size(u) > size(v)), what we will do is keep id[k] = id[u], and go
    // through each occurrence of id[v] in some knowledge list, and change it to
    // id[u]. Because of small to large, each id[u] can change at most O(log N)
    // times, making this part also O(M log^2 N). It's O(log^2 N), mostly
    // because we want to use sets so that the query is easy to implement. With
    // a hash table this might be a bit faster, as we don't really need the
    // sorted order in each of these sets. It's a bit hard to pass given the
    // constraints on codeforces, so we have to make some optimizations.
    // Notably, we could actually do the id updates lazily, by only doing the
    // addition at the time of the update, and remove the old ones at the time
    // of the merge. Also using a better hash table like gp_hash_table makes
    // this considerably quicker. Overall the complexity below is O((N + M + Q)
    // log N * c), where c ~ O(1) from the hash table.

    vector<int> id(n);
    iota(id.begin(), id.end(), 0);

    vector<bool> active(n, true);
    vector<vector<int>> appears(n);
    for(int i = 0; i < n; i++) {
        for(int j: know[i]) {
            appears[j].push_back(i);
        }
    }

    gp_hash_table<int64_t, null_type, custom_hash> edges;
    int64_t MOD = n + 1LL;
    for(int i = 0; i < n; i++) {
        for(int j: know[i]) {
            edges.insert(i * MOD + j);
        }
    }

    for(int qi = 0; qi < q; qi++) {
        int type, a, b;
        cin >> type >> a >> b;
        a--;
        b--;

        if(type == 1) {
            int pa = id[a];
            int pb = id[b];
            cout << (edges.find(pa * MOD + pb) != edges.end() ? "YES" : "NO")
                 << "\n";
        } else {
            int u = id[a], v = id[b];
            if(u == v) {
                continue;
            }

            if(know[u].size() + appears[u].size() <
               know[v].size() + appears[v].size()) {
                swap(u, v);
            }

            for(int x: know[v]) {
                if(!active[x]) {
                    continue;
                }
                int target = (x == v ? u : x);
                know[u].push_back(target);
                appears[target].push_back(u);
                edges.insert(u * MOD + target);
            }
            for(int x: appears[v]) {
                if(!active[x]) {
                    continue;
                }
                int source = (x == v ? u : x);
                know[source].push_back(u);
                appears[u].push_back(source);
                edges.insert(source * MOD + u);
            }

            know[v].clear();
            appears[v].clear();
            active[v] = false;

            id.push_back(u);
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T = 1;
    for(int test = 1; test <= T; test++) {
        read();
        solve();
    }
    return 0;
}
