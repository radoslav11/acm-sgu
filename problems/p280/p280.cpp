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

const int inf = (int)1e9;

// Let's consider an arbitrary root x. Then there is a direct
// greedy approach by starting from the bottom and always adding
// a center if there is some uncovered node. Let the maximum and
// minimum depths to a center D1 < D2. Then if D1 + D2 + 2 is
// larger than 2K + 1, it means that something on the path to the D2
// subtree might eventually end up being uncovered, and this is the
// "critical" path.
// If the depth of the critical path at some point ends up being greater
// or equal to 2K + 1, it means we need to make the current root a
// center for sure.

// This can easily be shown to be optimal if we try any x, but we can
// observe that we can choose an arbitrary x by considering rooting from
// a vertex that we are sure will be a center. The solution is the same no
// matter what center we root from, and we can notice that the solution if
// we root from any root between two centers in the optimal solution will be
// the same. Therefore, rooting from any vertex results in the same answer.

int n, k;
vector<vector<int>> adj;

// dp[u] represents distance from node u to closest center.
vector<int> dp;

void read() {
    cin >> n >> k;
    adj.assign(n + 1, {});
    dp.assign(n + 1, 0);
    for(int i = 1; i < n; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
}

int dfs(int u, int par, vector<int>& ans) {
    vector<int> child_dps;

    for(int v: adj[u]) {
        if(v == par) {
            continue;
        }
        dfs(v, u, ans);
        child_dps.push_back(dp[v]);
    }

    if(!child_dps.empty()) {
        int min_dist = *min_element(child_dps.begin(), child_dps.end());
        int max_dist = *max_element(child_dps.begin(), child_dps.end());

        // If closest + farthest centers can cover all nodes within 2*k+1
        // distance
        if(min_dist + max_dist + 2 <= 2 * k + 1) {
            // Minimum distance branch covers everything, so we don't need
            // to worry about any other branch.
            dp[u] = min_dist + 1;
        } else {
            // Minimum distance branch doesn't cover the maximum distance one.
            // We have a new critical path.
            dp[u] = max_dist + 1;
        }
    } else {
        // We assume that we attach K + 1 nodes below a leaf, the
        // lowest of which is a centre.
        dp[u] = k + 1;
    }

    if(dp[u] == 2 * k + 1) {
        dp[u] = 0;  // Distance 0 since we place center here
        ans.push_back(u);
    }

    return dp[u];
}

void solve() {
    vector<int> ans;
    dfs(1, -1, ans);

    if(dp[1] > k) {
        ans.push_back(1);
    }

    sort(ans.begin(), ans.end());

    cout << ans.size() << "\n";
    for(int center: ans) {
        cout << center << "\n";
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
