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

int64_t n;

void read() { cin >> n; }

int64_t rotate_left(int64_t x, int k, int len) {
    return ((x << k) | (x >> (len - k))) & ((1LL << len) - 1);
}

int64_t min_rotation(int64_t x, int len) {
    int64_t res = x;
    for(int i = 0; i < len; i++) {
        int64_t r = rotate_left(x, i, len);
        if((r >> (len - 1)) & 1) {
            res = min(res, r);
        }
    }
    return res;
}

void solve() {
    // It's clear that n is an upper bound on the answer, so now we should check
    // if something lower is satisfactory. It's also clear that the answer
    // should have the same number of bits as n as the problem statement
    // requires applying the operation to numbers with highest bit set.
    // Therefore, we are interested in the smallest number from [2^(len-1);n]
    // that could generate all of [2^(len-1);n]. Let's try to get a lower bound
    // for the answer. One candidate is to look at each number from [1;n] and
    // find the smallest shift it can be derived from. Or the lower bound would
    // be:
    //
    //     ans = max(min_rotation(i) for i in [2^(len-1);n])
    //
    // Which after some thinking, we can notice is actually the answer to the
    // problem. We ans >= min_rotation(i) for all i in [2^(len-1);n] by
    // definition, so W(min_rotation(i)) will be generated and so all
    // i in [2^(len-1);n] will also be generated.
    //
    // Now that we know how to find the answer is the lower bound, we should try
    // to find it quickly. The main observation is that for any prefix of n,
    // there is at most one sufficient candidate we can check. Say that
    // n = dec(pref || 1 || suff). Then we have the "i" values of the form
    // dec(pref || 0 || x) for any x. Out of them, it's not hard to notice that
    // min_rotation(dec(pref || 0 || 1...1)) is the largest, so we don't really
    // need to look at any of the others. Therefore, there are only O(log n)
    // different numbers to consider, and find the minimal rotation in O(log n)
    // for each, giving an overall O(log^2 n) solution.

    int len = 64 - __builtin_clzll(n);
    int64_t ans = min_rotation(n, len);

    for(int pos = 0; pos + 1 < len; pos++) {
        if((n >> pos) & 1) {
            int64_t u = (n & ~((1LL << (pos + 1)) - 1)) | ((1LL << pos) - 1);
            ans = max(ans, min_rotation(u, len));
        }
    }

    cout << ans << endl;
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
