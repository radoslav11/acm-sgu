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

const int64_t inf = numeric_limits<int64_t>::max();

string sigma, lambda, mu;
vector<vector<int>> cost;

void read() {
    cin >> sigma >> lambda >> mu;
    cost.assign(sigma.size(), vector<int>(sigma.size()));
    cin >> cost;
}

void solve() {
    // We can solve this problem by creating a dp with states (i, j), meaning
    // that the strings alpha and beta already contain lambda[:i] and mu[:j]
    // respectively. Then we have 3 options for the transition:
    //
    //    1) We add lambda[i] to alpha and mu[j] to beta at the given cost. This
    //       moves both i and j by 1.
    //
    //    2) We add lambda[i] to alpha and the best character based on the cost
    //       matrix for lambda[i]. This doesn't move j.
    //
    //    3) We add mu[j] to beta and the best character based on the cost
    //    matrix
    //       for mu[j]. This doesn't change i. This is a quadratic DP.
    //
    // We will precompute the best characters so that this lookup is O(1). The
    // answer is dp[|lambda|][|mu|], but we will also maintain a table
    // opt_move[i][j] equal to 0, 1, 2 (so int8_t is fine to not blowup memory),
    // meaning which of the above 3 moves was taken to arrive to (i, j). We use
    // this to recover the strings. Overall the complexity is O(|sigma|^2 +
    // |lambda| * |mu|).

    int n = lambda.size();
    int m = mu.size();
    int k = sigma.size();

    vector<int> sigma_idx(256, -1);
    for(int i = 0; i < k; i++) {
        sigma_idx[sigma[i] + 128] = i;
    }

    vector<int> best_for_a(k);
    vector<int> best_for_b(k);

    for(int i = 0; i < k; i++) {
        int min_cost = numeric_limits<int>::max();
        for(int j = 0; j < k; j++) {
            if(cost[i][j] < min_cost) {
                min_cost = cost[i][j];
                best_for_a[i] = j;
            }
        }
    }

    for(int j = 0; j < k; j++) {
        int min_cost = numeric_limits<int>::max();
        for(int i = 0; i < k; i++) {
            if(cost[i][j] < min_cost) {
                min_cost = cost[i][j];
                best_for_b[j] = i;
            }
        }
    }

    vector<vector<int64_t>> dp(n + 1, vector<int64_t>(m + 1, inf));
    vector<vector<int8_t>> opt_move(n + 1, vector<int8_t>(m + 1, -1));

    dp[0][0] = 0;

    for(int i = 0; i <= n; i++) {
        for(int j = 0; j <= m; j++) {
            if(dp[i][j] == inf) {
                continue;
            }

            if(i < n && j < m) {
                int a_idx = sigma_idx[lambda[i] + 128];
                int b_idx = sigma_idx[mu[j] + 128];
                int64_t new_cost = dp[i][j] + cost[a_idx][b_idx];
                if(new_cost < dp[i + 1][j + 1]) {
                    dp[i + 1][j + 1] = new_cost;
                    opt_move[i + 1][j + 1] = 0;
                }
            }

            if(i < n) {
                int a_idx = sigma_idx[lambda[i] + 128];
                int best_b = best_for_a[a_idx];
                int64_t new_cost = dp[i][j] + cost[a_idx][best_b];
                if(new_cost < dp[i + 1][j]) {
                    dp[i + 1][j] = new_cost;
                    opt_move[i + 1][j] = 1;
                }
            }

            if(j < m) {
                int b_idx = sigma_idx[mu[j] + 128];
                int best_a = best_for_b[b_idx];
                int64_t new_cost = dp[i][j] + cost[best_a][b_idx];
                if(new_cost < dp[i][j + 1]) {
                    dp[i][j + 1] = new_cost;
                    opt_move[i][j + 1] = 2;
                }
            }
        }
    }

    string alpha, beta;
    int i = n, j = m;

    while(i > 0 || j > 0) {
        int move = opt_move[i][j];

        if(move == 0) {
            alpha = lambda[i - 1] + alpha;
            beta = mu[j - 1] + beta;
            i--;
            j--;
        } else if(move == 1) {
            int a_idx = sigma_idx[lambda[i - 1] + 128];
            int best_b = best_for_a[a_idx];
            alpha = lambda[i - 1] + alpha;
            beta = sigma[best_b] + beta;
            i--;
        } else {
            int b_idx = sigma_idx[mu[j - 1] + 128];
            int best_a = best_for_b[b_idx];
            alpha = sigma[best_a] + alpha;
            beta = mu[j - 1] + beta;
            j--;
        }
    }

    cout << dp[n][m] << "\n";
    cout << alpha << "\n";
    cout << beta << "\n";
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
