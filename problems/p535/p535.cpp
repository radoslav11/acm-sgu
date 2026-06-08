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

const int INF = 1e9;

string s;
string t;
int n;
vector<vector<int>> dp;
vector<int> split_pts;

void read() { cin >> s; }

int f(int l, int r) {
    if(l > r) {
        return 0;
    }

    int& res = dp[l][r];
    if(res != -1) {
        return res;
    }

    res = INF;

    if(t[l] == '*' && t[r] == '*') {
        res = min(res, f(l + 1, r));
        res = min(res, f(l, r - 1));
    } else if(t[l] == '*') {
        if(isupper(t[r])) {
            res = min(res, f(l, r - 1) + 1);
        }
        res = min(res, f(l + 1, r));
    } else if(t[r] == '*') {
        if(islower(t[l])) {
            res = min(res, f(l + 1, r) + 1);
        }
        res = min(res, f(l, r - 1));
    } else if(islower(t[l]) && isupper(t[r]) && t[l] == (char)tolower(t[r])) {
        res = min(res, f(l + 1, r - 1));
    }

    for(int i: split_pts) {
        int lo = (t[i] == '*') ? l + 1 : l;
        if(i >= lo && i < r) {
            int rl = (t[i] == '*') ? i : i + 1;
            res = min(res, f(l, i) + f(rl, r));
        }
    }

    return res;
}

void solve() {
    // First sweep greedily cancels adjacent matched pairs: lowercase 'x'
    // followed by uppercase 'X' is a real push-pop pair, so pop them off a
    // stack. A lowercase top against a non-matching uppercase is an outright
    // contradiction => -1. After this reduction, t consists of stars and
    // chains shaped like upper-upper-...upper-lower-lower-...lower (any pops
    // that survive must precede any pushes that survive, otherwise they'd
    // have cancelled). Only stars can balance what remains.
    //
    // f(l, r) = min number of extra letters the stars in t[l..r] must produce
    // so that t[l..r] becomes a fully balanced bracket sequence (lowercase =
    // open, uppercase = close). Endpoint cases handle stars at the borders
    // absorbing one neighbor as their matching bracket, and a matched pair at
    // the borders. We also try every "natural split" point: a star (which can
    // split into two stars covering both halves) or a position between an
    // uppercase and a lowercase (the only place an outer balanced sequence
    // can be cut in two without crossing an active matching pair).

    string reduced;
    int kept = 0;
    for(char c: s) {
        if(c != '*') {
            kept++;
        }

        if(c == '*' || islower(c)) {
            reduced.push_back(c);
        } else if(reduced.empty() || reduced.back() == '*' || isupper(reduced.back())) {
            reduced.push_back(c);
        } else if(reduced.back() == (char)tolower(c)) {
            reduced.pop_back();
        } else {
            cout << -1 << '\n';
            return;
        }
    }

    t = reduced;
    n = (int)t.size();

    split_pts.clear();
    for(int i = 0; i < n; i++) {
        if(t[i] == '*' || (i + 1 < n && isupper(t[i]) && islower(t[i + 1]))) {
            split_pts.push_back(i);
        }
    }

    dp.assign(n, vector<int>(n, -1));

    int extra = (n == 0) ? 0 : f(0, n - 1);
    if(extra >= INF) {
        cout << -1 << '\n';
    } else {
        cout << (extra + kept) / 2 << '\n';
    }
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
