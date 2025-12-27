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

int n;
vector<int> S, D;

void read() {
    cin >> n;
    S.resize(n);
    D.resize(n);
    cin >> S >> D;
}

void solve() {
    // It's clear that it's impossible when the sum of S and D are different. We
    // can further generalize this argument. Consider a set of positions
    // i[1], ..., i[k] with sum of S and D being equal - we can always make the
    // values equal in k - 1 steps. Unless there are some equal values, or more
    // generally another subset that has equal sum between S and D, we can't do
    // better than k - 1 steps. We can prove this because if there is no
    // subset of equal sum, we would have to have a direct or indirect
    // "connection" between each of the elements in the subset. Trivially to
    // have something connected, we need at least k - 1 edges (number of edges
    // in a spanning tree).
    //
    // This intuition leads us to the idea that we want to split the N elements
    // into the largest number subsets of equal sum between S and D. For
    // simplicity, we could instead look at the difference between S and D as
    // then we are interested in sum / balance equal to 0. To do this, we could
    // do a bitmask dp[mask][sum_subset_so_far]. Alternatively, we could do a
    // 3^n approach. However, this might still be a bit too slow and instead we
    // can notice the other main observations - consider some arbitrary order
    // p[1], ..., p[n] of elements we will consider. Let's also do pref[i],
    // being the prefix sum over this order. We have the incentive to do 0-sum
    // subsets as often as possible, meaning that whenever the prefix sum
    // reaches 0, we should always snap a new group. Particularly, if a 0-group
    // starts at p[l] and ends at p[r], then it's sum is psum[r] - psum[l - 1],
    // but we know that psum[l - 1] = 0 (because the prefix until l - 1 is
    // decomposed into 0-sum segments).
    //
    // To summarize, this means that we don't actually need to keep the second
    // dimension of the DP - we can always just consider the sum of D and S in
    // the current mask, and "subtract one" every time it reaches 0.

    if(accumulate(S.begin(), S.end(), 0) != accumulate(D.begin(), D.end(), 0)) {
        cout << -1 << endl;
        return;
    }

    vector<int> diff(n);
    for(int i = 0; i < n; i++) {
        diff[i] = S[i] - D[i];
    }

    vector<int> sum((1 << n), 0);
    for(int mask = 1; mask < (1 << n); mask++) {
        int lb = mask & (-mask);
        int bit = __builtin_ctz(lb);
        sum[mask] = sum[mask ^ lb] + diff[bit];
    }

    vector<int> dp((1 << n), 0);
    for(int mask = 1; mask < (1 << n); mask++) {
        for(int i = 0; i < n; i++) {
            if(mask & (1 << i)) {
                dp[mask] = max(dp[mask], dp[mask ^ (1 << i)]);
            }
        }
        if(sum[mask] == 0) {
            dp[mask]++;
        }
    }

    cout << n - dp[(1 << n) - 1] << endl;
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
