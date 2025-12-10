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

int n, k;
vector<vector<int>> G;

void read() {
    cin >> n >> k;
    G.assign(n, vector<int>(n, 0));
    cin >> G;
}

bool solve_even() {
    vector<vector<int>> dist(n, vector<int>(n, (int)1e9));
    for(int i = 0; i < n; i++) {
        dist[i][i] = 0;
    }
    for(int i = 0; i < n; i++) {
        for(int j = i + 1; j < n; j++) {
            if(G[i][j] == 0) {
                dist[i][j] = 1;
                dist[j][i] = 1;
            }
        }
    }

    for(int k = 0; k < n; k++) {
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < n; j++) {
                dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
            }
        }
    }

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if(dist[i][j] > 3) {
                return false;
            }
        }
    }
    return true;
}

void solve() {
    // In problems like this, it's always good to ask the question of when this
    // is actually possible. Turns out it always is. In particular, let's
    // consider the case of only 2 colours, as reducing k colours to that case
    // is trivial - just split into "odd" and "even" colours.
    //
    // The claim is that for 2 colours, one of them always satisfies that the
    // distance between each pair is <= 3. Let's consider the case of n=4 first.
    // We can go through all examples to convince ourselves, but there is a
    // simpler argument for why this is true - for any graph G, either G or K_n
    // - G is connected (this is a standard results which we can get convinced
    // about by thinking about the complementary edges and that they connect all
    // connected components in G), and with n nodes the distance is always <= 3
    // (as 4 edges means a cycle and so not a simple path).
    //
    // Now let's look at n > 4 and assume for contradiction. This means that for
    // both colours, there are 2 vertices (u_black, v_black) and (u_white,
    // v_white) such that the black distance between u_black and v_black is > 3,
    // and the white distance between u_white and v_white is > 3. However, we
    // already have a result for n = 4: consider the subgraph with V = {u_black,
    // v_black, u_white, v_black} and the result implying that there is at least
    // one colour that has distance less than or equal to 3 between all pairs.
    // This means there is a trivial contradiction between one of the two
    // assumptions.
    //
    // Now that we have some results, let's actually solve the problem. The
    // first step is to split the K colours into 2. This will be done via the
    // parity. Afterwards, we can just do Floyd to check if all distances are
    // less than or equal to 3, as the constraints are n <= 200. We should
    // technically be able to do this a bit faster with bitsets - it's enought
    // to compute G^1, G^2 and G^3, but for this problem Floyd is enough.

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            G[i][j] = G[i][j] % 2;
        }
    }

    vector<int> ans;
    for(int i = 1 + solve_even(); i <= k; i += 2) {
        ans.push_back(i);
    }

    cout << ans.size() << endl;
    cout << ans << endl;
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
