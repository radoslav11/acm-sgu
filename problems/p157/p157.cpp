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

void read() { cin >> n; }

void solve() {
    // The solution here is to simply simulate this. We can create a brute force
    // that runs in O(n!), which for n=13 is 6.2B operations, or on modern
    // computers will take less than a minute. Then we precompute the answer for
    // all possible values of n - after all there aren't that many. In terms of
    // the brute force, the smart way is to build the set of all ancestors of
    // the goal in the predecessor graph. Then starting from the goal, we
    // repeatedly add every valid predecessor (states that can move forward into
    // the current set). It's useful to keep a set of states we have visited to
    // avoid duplicates, as this cuts the runtime. The size of the reached set
    // is exactly the answer. Overall, the size of the set grows up to ~118M, so
    // something like this could in theory pass if optimized.

    static const array<int64_t, 14> answers = {
        0ll,   1ll,    2ll,     5ll,      14ll,      47ll,       189ll,
        891ll, 4815ll, 29547ll, 203173ll, 1548222ll, 12966093ll, 118515434ll
    };

    cout << answers[n] << endl;
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
