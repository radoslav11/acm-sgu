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

int c, k;
vector<vector<int>> a;

void read() {
    cin >> c >> k;
    a.assign(c, vector<int>(k));
    for(int i = 0; i < c; i++) {
        for(int j = 0; j < k; j++) {
            cin >> a[i][j];
        }
    }
}

vector<int> normalize_base8(const vector<int>& coeffs) {
    vector<int> digits;
    digits.reserve(coeffs.size() + 4);
    int64_t carry = 0;
    for(int v: coeffs) {
        int64_t cur = (int64_t)v + carry;
        digits.push_back((int)(cur % 8));
        carry = cur / 8;
    }

    while(carry > 0) {
        digits.push_back((int)(carry % 8));
        carry /= 8;
    }

    while(!digits.empty() && digits.back() == 0) {
        digits.pop_back();
    }

    return digits;
}

int cmp_digits(const vector<int>& x, const vector<int>& y) {
    if(x.size() != y.size()) {
        return x.size() < y.size() ? -1 : 1;
    }

    for(int i = (int)x.size() - 1; i >= 0; i--) {
        if(x[i] != y[i]) {
            return x[i] < y[i] ? -1 : 1;
        }
    }

    return 0;
}

void solve() {
    // A "corner" of size 2^j sits inside a bounding cube of side 2^{j+1} and
    // occupies 4 of the 8 unit sub-cubes meeting at one of its corners. The
    // opposite four sub-cubes form another corner, so two corners of size 2^j
    // tile a cube of side 2^{j+1}. Hence the big corner of size 2^k splits
    // into 4 cubes of side 2^k = 8 corners of size 2^{k-1}, and recursively
    // any corner of size 2^j can be replaced by 8 corners of size 2^{j-1}.
    //
    // Because of that recursion, the only constraint on which multiset of
    // small corners fills the big corner is the total volume: if x_j corners
    // of size 2^j are chosen with sum x_j * 8^j = 8^k, the tiling is always
    // realizable. The per-level cap x_j <= N_j (demand cascading from above)
    // is automatic, since sum_{i >= j} x_i * 8^i <= 8^k forces x_j to fit
    // the remaining demand. So feasibility for a chosen set S of colors is
    // just sum_{c in S, j} a[c][j] * 8^j >= 8^k.
    //
    // Minimizing the number of colors is then "smallest prefix of largest
    // volumes that reaches the target". Volumes are up to 8^1000, so each
    // color's volume is stored as a base-8 digit vector (after normalizing
    // a[c][.] by carry propagation), and colors are sorted by lexicographic
    // comparison of those digit vectors. We add colors one at a time in that
    // order and after each addition run the top-down greedy on the combined
    // per-size capacities T_j to test feasibility.
    //
    // The top-down greedy itself uses 64-bit arithmetic with a clamp on the
    // unmet demand N: as long as N stays below ~4e9 we propagate it
    // exactly; once it exceeds that bound it can no longer be absorbed by
    // any T_j (capped at c * 1000 = 1e6), so we clamp and treat the run as
    // infeasible. When feasibility holds, the loop terminates at the first
    // level where T_j >= N_j, giving the totals x_j; these are then split
    // across the chosen colors in any order, respecting a[c][j].

    vector<vector<int>> vol_digits(c);
    for(int i = 0; i < c; i++) {
        vol_digits[i] = normalize_base8(a[i]);
    }

    vector<int> order(c);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int u, int v) {
        return cmp_digits(vol_digits[u], vol_digits[v]) > 0;
    });

    vector<int64_t> sum_caps(k, 0);
    vector<int64_t> x(k, 0);
    vector<int> chosen;
    bool feasible = false;

    auto try_greedy = [&]() -> bool {
        fill(x.begin(), x.end(), (int64_t)0);
        int64_t N = 8;
        const int64_t CLAMP = (int64_t)4e9;
        for(int j = k - 1; j >= 0; j--) {
            if(sum_caps[j] >= N) {
                x[j] = N;
                N = 0;
                break;
            }

            x[j] = sum_caps[j];
            int64_t rem = N - sum_caps[j];
            if(rem > CLAMP / 8) {
                N = CLAMP;
            } else {
                N = rem * 8;
            }
        }
        return N == 0;
    };

    for(int idx = 0; idx < c; idx++) {
        int cc = order[idx];
        chosen.push_back(cc);
        for(int j = 0; j < k; j++) {
            sum_caps[j] += a[cc][j];
        }

        if(try_greedy()) {
            feasible = true;
            break;
        }
    }

    if(!feasible) {
        cout << "NO SOLUTION\n";
        return;
    }

    vector<tuple<int, int, int>> result;
    for(int j = 0; j < k; j++) {
        int64_t need = x[j];
        for(int cc: chosen) {
            if(need == 0) {
                break;
            }

            int64_t take = min((int64_t)a[cc][j], need);
            if(take > 0) {
                result.push_back({cc + 1, j, (int)take});
                need -= take;
            }
        }
    }

    cout << result.size() << "\n";
    for(auto& [col, sz, cnt]: result) {
        cout << col << ' ' << sz << ' ' << cnt << "\n";
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
