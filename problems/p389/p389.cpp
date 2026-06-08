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

const int64_t mod = (int64_t)1e9 + 7;

string cap[3];

void read() { cin >> cap[0] >> cap[1] >> cap[2]; }

int64_t pw(int64_t a, int64_t e, int64_t m) {
    int64_t r = 1 % m;
    a %= m;
    while(e > 0) {
        if(e & 1) {
            r = r * a % m;
        }
        a = a * a % m;
        e >>= 1;
    }
    return r;
}

void solve() {
    // On a unit hypercube, Manhattan distance between two vertices equals the
    // Hamming distance between their bit strings. So d_j(v) is just the number
    // of coordinates where v disagrees with capital j.
    //
    // The distances decompose coordinate by coordinate, so let's classify each
    // of the n coordinates by the triple of capital bits (a_i, b_i, c_i). Up
    // to flipping v_i, only the "who is the odd one out" pattern matters,
    // giving us 4 groups:
    //
    //     x[0]: all three capitals agree
    //     x[1]: cap1 is the lone one (caps look like (1,0,0) or (0,1,1))
    //     x[2]: cap2 is the lone one
    //     x[3]: cap3 is the lone one
    //
    // Coordinates in x[0] contribute equally to d1, d2, d3 regardless of v_i,
    // so they don't constrain neutrality at all. Each one independently
    // doubles the answer, giving a final factor of 2^x[0].
    //
    // For group x[1], let k1 be the number of coords where v matches the lone
    // capital (cap1). Each such coord adds 0 to d1 and 1 to d2, d3; the other
    // (x[1] - k1) coords add 1 to d1 and 0 to d2, d3. Define k2, k3 similarly
    // for groups x[2] and x[3]. Writing out d1, d2, d3 and using
    // d1 = d2 = d3:
    //
    //     d1 - d2 = 0  =>  2*(k1 - k2) = x[1] - x[2]
    //     d2 - d3 = 0  =>  2*(k2 - k3) = x[2] - x[3]
    //
    // So k2 determines k1 and k3. We iterate k2 from 0 to x[2], derive
    //
    //     2*k1 = 2*k2 + (x[1] - x[2])
    //     2*k3 = 2*k2 + (x[3] - x[2])
    //
    // and skip anything that's negative, odd, or out of range. For each valid
    // triple, the number of towns is C(x[1], k1) * C(x[2], k2) * C(x[3], k3),
    // since we freely choose which coords in each group match the lone capital.
    // Multiply by 2^x[0] for the free coordinates. 

    int n = (int)cap[0].size();
    array<int, 4> x = {0, 0, 0, 0};
    for(int i = 0; i < n; i++) {
        int a = cap[0][i] - '0';
        int b = cap[1][i] - '0';
        int c = cap[2][i] - '0';
        if(a == b && b == c) {
            x[0]++;
        } else if(a != b && a != c) {
            x[1]++;
        } else if(b != a && b != c) {
            x[2]++;
        } else {
            x[3]++;
        }
    }

    int max_n = max({x[1], x[2], x[3]}) + 1;
    vector<int64_t> fact(max_n + 1), inv_fact(max_n + 1);
    fact[0] = 1;
    for(int i = 1; i <= max_n; i++) {
        fact[i] = fact[i - 1] * i % mod;
    }
    inv_fact[max_n] = pw(fact[max_n], mod - 2, mod);
    for(int i = max_n - 1; i >= 0; i--) {
        inv_fact[i] = inv_fact[i + 1] * (i + 1) % mod;
    }

    auto binom = [&](int n, int k) -> int64_t {
        if(k < 0 || k > n) {
            return 0;
        }
        return fact[n] * inv_fact[k] % mod * inv_fact[n - k] % mod;
    };

    int64_t ans = 0;
    for(int k2 = 0; k2 <= x[2]; k2++) {
        int two_k1 = 2 * k2 + x[1] - x[2];
        int two_k3 = 2 * k2 + x[3] - x[2];
        if(two_k1 < 0 || two_k3 < 0 || (two_k1 & 1) || (two_k3 & 1)) {
            continue;
        }
        int k1 = two_k1 / 2;
        int k3 = two_k3 / 2;
        if(k1 > x[1] || k3 > x[3]) {
            continue;
        }
        ans =
            (ans + binom(x[1], k1) * binom(x[2], k2) % mod * binom(x[3], k3)) %
            mod;
    }

    ans = ans * pw(2, x[0], mod) % mod;
    cout << ans << '\n';
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
