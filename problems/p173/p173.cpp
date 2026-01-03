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

int n, m, k, l;
vector<pair<int, int>> ops;
vector<pair<uint64_t, uint64_t>> constraints;
uint64_t final_row;

void read() {
    cin >> n >> m >> k >> l;
    ops.resize(m);
    for(auto& [s, d]: ops) {
        cin >> s >> d;
        s--;
    }
    constraints.resize(l);
    for(auto& [before, after]: constraints) {
        string sb, sa;
        cin >> sb >> sa;
        before = 0;
        after = 0;
        for(int i = 0; i < k; i++) {
            if(sb[i] == '1') {
                before |= (1ull << i);
            }
            if(sa[i] == '1') {
                after |= (1ull << i);
            }
        }
    }
    string s;
    cin >> s;
    final_row = 0;
    for(int i = 0; i < n; i++) {
        if(s[i] == '1') {
            final_row |= (1ull << i);
        }
    }
}

void solve() {
    // The problem statement is a bit convoluted, but the key is that there is a
    // single transformation X that is valid based on the L constraints.
    // Particularly, each of the L constraint can be formalized as a linear
    // equation in Z2 - whenever A and X[1:K] match, we flip X[0] (which will
    // later be placed at X[-1] due to the cyclic shift). The variables are
    // given by A, while we already have the coefficients.
    //
    // The constraint is that L <= 200, so we can do an initial Gauss
    // elimination to find A. We don't have to, but we can use bitsets / int64_t
    // for it too to make it quicker. Afterwards, we simply need to simulate the
    // operations.
    //
    // Simulating might be a bit slow if done naively as Di <= 10^6 and we have
    // M <= 10 of these, so we should try to make each individual update
    // quickly. This can be done quickly if we keep the initial sequence and the
    // A as a bitmask / int64_t too, as then the cyclic shift and X
    // transformation ends up being a few bit operations plus a popcount which
    // is quick.

    vector<pair<uint64_t, int>> eqs(l);
    for(int i = 0; i < l; i++) {
        auto [before, after] = constraints[i];
        uint64_t coeffs = (before >> 1);
        int rhs = ((after >> (k - 1)) ^ (before & 1)) & 1;
        eqs[i] = {coeffs, rhs};
    }

    uint64_t a = 0;
    int pivot_row = 0;
    for(int col = 0; col < k - 1 && pivot_row < l; col++) {
        int found = -1;
        for(int row = pivot_row; row < l; row++) {
            if(eqs[row].first & (1ull << col)) {
                found = row;
                break;
            }
        }
        if(found == -1) {
            continue;
        }
        swap(eqs[pivot_row], eqs[found]);
        for(int row = 0; row < l; row++) {
            if(row != pivot_row && (eqs[row].first & (1ull << col))) {
                eqs[row].first ^= eqs[pivot_row].first;
                eqs[row].second ^= eqs[pivot_row].second;
            }
        }
        pivot_row++;
    }

    for(int i = 0; i < l; i++) {
        if(eqs[i].first == 0) {
            continue;
        }
        int col = __builtin_ctzll(eqs[i].first);
        if(eqs[i].second) {
            a |= (1ull << col);
        }
    }

    uint64_t mask = (1ull << k) - 1;
    uint64_t row = final_row;
    uint64_t lower_bits = (1ull << (k - 1)) - 1;
    for(int op = m - 1; op >= 0; op--) {
        int s = ops[op].first;
        int d = ops[op].second;
        for(int t = 0; t < d; t++) {
            uint64_t sub = (row >> s) & mask;
            int flip_bit = __builtin_popcountll((sub & a) & lower_bits) & 1;
            uint64_t shifted = sub ^ ((uint64_t)flip_bit << (k - 1));
            uint64_t original = ((shifted << 1) | (shifted >> (k - 1))) & mask;
            row = (row & ~(mask << s)) | (original << s);
        }
    }

    for(int i = 0; i < n; i++) {
        cout << ((row >> i) & 1);
    }
    cout << '\n';
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
