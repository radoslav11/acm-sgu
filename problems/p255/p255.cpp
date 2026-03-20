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

int64_t m;

void read() { cin >> m; }

void solve() {
    // Let g(K) = #{ports in [K+1, 2K] with exactly 3 set bits}. The problem
    // gives us M = g(K) for some unknown K, and asks if K is uniquely
    // determined. We will prove that the answer is YES iff M >= 2 and (M-1) is
    // a triangular number. The key idea is to consider how g(K) changes as K
    // increases by 1. When K -> K+1, the interval [K+1, 2K] becomes [K+2,
    // 2K+2]. We lose port K + 1 and gain ports 2K + 1 and 2K + 2. So g(K + 1) -
    // g(K) = [popcount(2K+1)==3] + [popcount(2K+2)==3] - [popcount(K+1)==3].
    //
    // Note that 2K+2 = 2*(K+1), so popcount(2K+2) = popcount(K+1).
    // These cancel, giving g(K+1) - g(K) = [popcount(2K+1) == 3].
    // Since 2K+1 is odd, popcount(2K+1) = popcount(K) + 1.
    // So g(K+1) - g(K) = [popcount(K) == 2].
    //
    // Since g(1) = 0 (interval [2,2] has no 3-bit numbers), telescoping
    // gives:
    //
    //   g(K) = #{j in [1, K-1] : popcount(j) == 2}.
    //
    // This is a staircase function: g increases by 1 at each K where
    // popcount(K-1) = 2, and is constant between such points. So M = g(K)
    // determines K uniquely iff the step has width 1, i.e. both K-1 and K
    // have popcount 2 (g jumps at K, and also jumped at K-1).
    //
    // Two consecutive integers both have popcount 2 only around powers
    // of 2: the pairs are (2^n+1, 2^n+2) for n >= 2. At such a point,
    // g(2^n+2) = #{j in [1, 2^n+1] with popcount 2} = C(n,2) + 1
    // (choose 2 of the n low bits, plus 2^n+1 itself). We don't care about the
    // actual K - only whether some K exists. So just check: is M = C(n,2) + 1
    // for some n >= 2? Equivalently, is (M-1) a triangular number?

    if(m < 2) {
        cout << "NO\n";
        return;
    }

    int64_t t = m - 1;
    int64_t k = (int64_t)((-1.0 + sqrt(1.0 + 8.0 * t)) / 2.0);
    while(k * (k + 1) / 2 < t) {
        k++;
    }
    while(k > 0 && k * (k + 1) / 2 > t) {
        k--;
    }
    cout << (k > 0 && k * (k + 1) / 2 == t ? "YES\n" : "NO\n");
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T = 1;
    cin >> T;
    for(int test = 1; test <= T; test++) {
        read();
        solve();
    }

    return 0;
}
