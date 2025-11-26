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

int64_t n, two_n_minus_one;

void read() {
    cin >> two_n_minus_one;
    n = (two_n_minus_one + 1) / 2;
}

void solve() {
    // The necklace is beautiful iff there exist two black beads at distance
    // exactly n+1 (because then one of the open arcs has exactly n beads
    // strictly between them). The graph with edges i -- (i + (n+1)) mod (2n-1)
    // is a disjoint union of g = gcd(2n-1, n+1) cycles, each of length
    // (2n-1)/g. In a cycle of length len you can colour at most floor(len/2)
    // vertices black without creating a monochromatic edge. Hence the maximal
    // number of black beads without a beautiful configuration is g * floor(
    // ((2n-1)/g) / 2 ) and the minimal K that forces beauty for every possible
    // necklace is exactly one more.

    int64_t L = two_n_minus_one;
    int64_t dist = n + 1;
    int64_t g = gcd(L, dist);
    int64_t max_avoid = g * ((L / g) / 2);
    int64_t ans = max_avoid + 1;

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
