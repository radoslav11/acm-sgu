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

const int64_t BASE = 1000000000LL;

struct big_int {
    vector<int64_t> d;

    big_int(int64_t v = 0) {
        if(v == 0) {
            d.push_back(0);
            return;
        }
        while(v > 0) {
            d.push_back(v % BASE);
            v /= BASE;
        }
    }

    big_int& operator+=(const big_int& o) {
        d.resize(max(d.size(), o.d.size()) + 1, 0);
        for(size_t i = 0; i < o.d.size(); i++) {
            d[i] += o.d[i];
        }
        for(size_t i = 0; i + 1 < d.size(); i++) {
            d[i + 1] += d[i] / BASE;
            d[i] %= BASE;
        }
        while(d.size() > 1 && d.back() == 0) {
            d.pop_back();
        }
        return *this;
    }

    big_int& operator/=(int64_t v) {
        int64_t carry = 0;
        for(int i = (int)d.size() - 1; i >= 0; i--) {
            int64_t cur = d[i] + carry * BASE;
            d[i] = cur / v;
            carry = cur % v;
        }
        while(d.size() > 1 && d.back() == 0) {
            d.pop_back();
        }
        return *this;
    }

    friend ostream& operator<<(ostream& out, const big_int& b) {
        out << b.d.back();
        for(int i = (int)b.d.size() - 2; i >= 0; i--) {
            out << setfill('0') << setw(9) << b.d[i];
        }
        return out;
    }
};

int n, m;
big_int pw2[401];

void read() { cin >> n >> m; }

template<typename F>
int count_cycles(F&& perm) {
    vector<vector<bool>> vis(n, vector<bool>(m, false));
    int cnt = 0;
    for(int r = 0; r < n; r++) {
        for(int c = 0; c < m; c++) {
            if(!vis[r][c]) {
                cnt++;
                int cr = r, cc = c;
                while(!vis[cr][cc]) {
                    vis[cr][cc] = true;
                    auto [nr, nc] = perm(cr, cc);
                    cr = nr;
                    cc = nc;
                }
            }
        }
    }
    return cnt;
}

void solve() {
    // We want to count the distinct 2-colorings of an N x M torus up to its
    // orientation-preserving symmetries, which can be done using Burnside's
    // lemma. The torus Z_N x Z_M has the following symmetry group (inner/outer
    // distinguishable, so no reflections):
    //
    //     - Translations (a, b): (r, c) -> (r+a, c+b). There are N*M of these.
    //
    //     - 180-degree rotations composed with translations:
    //           (r, c) -> (-r+a, -c+b).
    //       Another N*M elements.
    //
    //     - If N = M, 90-degree and 270-degree rotations composed with
    //       translations:
    //           (r, c) -> (c+a, -r+b) and
    //           (r, c) -> (-c+a, r+b).
    //       Another 2*N^2 elements.
    //
    // Total group size: 2*N*M if N != M, 4*N^2 if N = M.
    //
    // By Burnside, the answer is (1/|G|) * sum over g in G of 2^(cycles(g)) -
    // for each of the group operations, we want to count the number of
    // configurations that are a fixed point under them. This can be done by
    // thinking of each N*M cell as a permutation element, going towards the
    // cell that should be the same. In other words we can choose exactly one
    // value per cycle, or cycles(g) is the number of orbits of the permutation
    // g on the N*M cells. Since N, M <= 20, the group has at most 1600
    // elements, and counting cycles per element is O(N*M), so the whole
    // computation is fast. The only subtlety is that the answer can be up to
    // 2^400, requiring big integer arithmetic.

    pw2[0] = big_int(1);
    for(int i = 1; i <= n * m; i++) {
        pw2[i] = pw2[i - 1];
        pw2[i] += pw2[i - 1];
    }

    int group_size = (n == m) ? 4 * n * m : 2 * n * m;
    big_int total(0);

    for(int a = 0; a < n; a++) {
        for(int b = 0; b < m; b++) {
            total += pw2[count_cycles([&](int r, int c) -> pair<int, int> {
                return {(r + a) % n, (c + b) % m};
            })];
            total += pw2[count_cycles([&](int r, int c) -> pair<int, int> {
                return {(n - r + a) % n, (m - c + b) % m};
            })];
        }
    }

    if(n == m) {
        for(int a = 0; a < n; a++) {
            for(int b = 0; b < n; b++) {
                total += pw2[count_cycles([&](int r, int c) -> pair<int, int> {
                    return {(c + a) % n, (n - r + b) % n};
                })];
                total += pw2[count_cycles([&](int r, int c) -> pair<int, int> {
                    return {(n - c + a) % n, (r + b) % n};
                })];
            }
        }
    }

    total /= group_size;
    cout << total << "\n";
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
