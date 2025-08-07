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

int n, B;
vector<vector<int>> adj;

void read() {
    cin >> n >> B;
    adj.assign(n, {});
    for(int i = 1; i < n; i++) {
        int u, v;
        cin >> u >> v;
        u--, v--;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
}

int dfs(
    int u, int par, vector<int>& comp, vector<int>& capitals, vector<int>& st
) {
    int cnt_here = 0;
    for(int v: adj[u]) {
        if(v == par) {
            continue;
        }

        cnt_here += dfs(v, u, comp, capitals, st);
        if(cnt_here >= B) {
            capitals.push_back(u + 1);

            assert(cnt_here <= st.size());
            while(cnt_here) {
                comp[st.back()] = capitals.size();
                st.pop_back();
                cnt_here--;
            }
        }
    }

    st.push_back(u);
    cnt_here++;
    return cnt_here;
}

void solve() {
    // We can solve this "recursively" after considering an arbitrary root.
    // In particular, let dfs(u) solve the problem for the subtree rooted at u
    // if we consider the overall root is 0. Let the "highest" component covered
    // be S. We can reuse a part of S, and then join it with other components at
    // par(u). Note that the capitals don't necessary have to be a part of S, so
    // we can join multiple children of par(u), and then simply use capital(s) =
    // par(u). We have the constraint that each group should have size within
    // [B; 3 * B]. We can always cut the components (e.g. S) when it reaches
    // size >= B, and this guarantees the above. One thing we should be careful about
    // is to make sure the final component in subree(u) is connected, before joining
    // with the other children of par(u). One way to do this is by having a post-order
    // of the tree.

    vector<int> capitals, st;
    vector<int> comp(n);

    dfs(0, 0, comp, capitals, st);

    if(capitals.empty()) {
        capitals.push_back(1);
    }

    while(!st.empty()) {
        comp[st.back()] = capitals.size();
        st.pop_back();
    }

    cout << capitals.size() << endl;
    cout << comp << endl;
    cout << capitals << endl;
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
