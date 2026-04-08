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
vector<vector<int>> dp;
stack<int> pegs[66];

void move_disc(int from, int to) {
    cout << "move " << pegs[from].top() << " from " << from << " to " << to;
    if(!pegs[to].empty()) {
        cout << " atop " << pegs[to].top();
    }
    cout << "\n";
    pegs[to].push(pegs[from].top());
    pegs[from].pop();
}

void rec(int cnt, int num_pegs, int from, int to) {
    if(cnt == 0) {
        return;
    }
    if(cnt == 1) {
        move_disc(from, to);
        return;
    }
    for(int k = 1; k < cnt; k++) {
        if(2 * dp[k][num_pegs] + dp[cnt - k][num_pegs - 1] ==
           dp[cnt][num_pegs]) {
            int top = pegs[from].top();
            for(int j = 1; j <= m; j++) {
                if(j != from && j != to &&
                   (pegs[j].empty() || pegs[j].top() > top)) {
                    rec(k, num_pegs, from, j);
                    rec(cnt - k, num_pegs - 1, from, to);
                    rec(k, num_pegs, j, to);
                    return;
                }
            }
        }
    }
}

void read() { cin >> n >> m; }

void solve() {
    // Often when we learn first about Tower of Hanoi, it's in the context of
    // divide and conquer or dynamic programming. This intuition would be useful
    // here. Let's say we have dp(n, m), being the number of mover required to
    // move n disks using m pegs. One way to think about this is to split the
    // first peg into two: one of size k and another of size n - k. So deciding
    // to split the first column into two, we should first move the two parts to
    // different pegs, which can be done in dp(k, m) and dp(n - k, m - 1). The m
    // - 1 is because one peg would be used by the k-sized group. Afterwards, we
    // can simply leave the (n-k)-sized group stationary, and move the other
    // group on top. This is done in dp(k, m) once again. In other words, we can
    // precompute this dynamic programming over all possible values of m, and
    // then reconstruct the answer. In the constraints m >= 4, which is
    // fortunate because the answer using this algorithm is dp(64, 4) = 18433,
    // which is small enough to display the solution.
    //
    // As a further note, this algorithm in literature is known as Frame-Stewart
    // algorithm, and is conjectured to be optimal for any number of pegs. This
    // has been the case since 1941, and only in 2014, the m=4 case was proven
    // to be indeed optimal. Hence, actually showing that this is the optimal
    // answer is tricky. However, the problem doesn't explicitly ask to find the
    // shortest solution either.

    dp.assign(n + 1, vector<int>(m + 1, 1e8));
    for(int j = 1; j <= m; j++) {
        dp[0][j] = 0;
    }
    for(int j = 2; j <= m; j++) {
        dp[1][j] = 1;
    }

    for(int i = 2; i <= n; i++) {
        for(int j = 3; j <= m; j++) {
            for(int k = 1; k < i; k++) {
                dp[i][j] = min(dp[i][j], 2 * dp[k][j] + dp[i - k][j - 1]);
            }
        }
    }

    cout << dp[n][m] << "\n";
    for(int i = n; i >= 1; i--) {
        pegs[1].push(i);
    }
    rec(n, m, 1, m);
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
