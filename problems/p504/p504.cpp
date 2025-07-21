#include <bits/stdc++.h>
// #include <coding_library/strings/hashing.hpp>

#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#pragma GCC target("avx2")

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

class HashMeta {
  private:
    void set_random_base() {
        seed_seq seed{
            (uint32_t)chrono::duration_cast<chrono::nanoseconds>(
                chrono::high_resolution_clock::now().time_since_epoch()
            )
                .count(),
            (uint32_t)random_device()(), (uint32_t)42
        };
        mt19937 rng(seed);
        // base = uniform_int_distribution<uint64_t>(0, mod - 1)(rng);
        base = uniform_int_distribution<uint64_t>()(rng);
    }

    void precompute_base_pow(size_t n) {
        base_pow.resize(n);
        base_pow[0] = 1;
        for(size_t i = 1; i < n; i++) {
            base_pow[i] = mul(base_pow[i - 1], base);
        }
    }

    static constexpr uint64_t add(uint64_t a, uint64_t b) {
        // a += b + 1;
        // a = (a & mod) + (a >> 61);
        // return a - 1;
        return (a + b);
    }

    static constexpr uint64_t sub(uint64_t a, uint64_t b) {
        return a - b;
        // return add(a, mod - b);
    }

    static constexpr uint64_t mul(uint64_t a, uint64_t b) {
        return a * b;
        // uint64_t l1 = (uint32_t)a, h1 = a >> 32, l2 = (uint32_t)b, h2 = b >> 32;
        // uint64_t l = l1 * l2, m = l1 * h2 + l2 * h1, h = h1 * h2;
        // uint64_t ret =
        //     (l & mod) + (l >> 61) + (h << 3) + (m >> 29) + (m << 35 >> 3) + 1;
        // ret = (ret & mod) + (ret >> 61);
        // ret = (ret & mod) + (ret >> 61);
        // return ret - 1;
    }

  public:
    class hash_t {
        uint64_t h;

      public:
        hash_t() : h(0) {}
        hash_t(uint64_t h) : h(h) {}
        operator uint64_t() const { return h; }

        hash_t& operator+=(const hash_t& other) {
            h = add(h, other.h);
            return *this;
        }

        hash_t& operator-=(const hash_t& other) {
            h = sub(h, other.h);
            return *this;
        }

        hash_t& operator*=(const hash_t& other) {
            h = mul(h, other.h);
            return *this;
        }

        hash_t operator+(const hash_t& other) const {
            return hash_t(*this) += other;
        }
        hash_t operator-(const hash_t& other) const {
            return hash_t(*this) -= other;
        }
        hash_t operator*(const hash_t& other) const {
            return hash_t(*this) *= other;
        }

        bool operator==(const hash_t& other) const { return h == other.h; }
        bool operator!=(const hash_t& other) const { return h != other.h; }

        // For use in std::map and std::set
        bool operator<(const hash_t& other) const { return h < other.h; }
    };

    uint64_t base;
    vector<hash_t> base_pow;
    // static constexpr uint64_t mod = (1ull << 61) - 1;

    void init(size_t n) {
        set_random_base();
        precompute_base_pow(n);
    }

    template<typename T>
    vector<hash_t> rabin_karp(const T& container) {
        vector<hash_t> h(container.size());
        for(size_t i = 0; i < container.size(); i++) {
            h[i] = (i ? h[i - 1] : hash_t(0)) * hash_t(base) +
                   hash_t(container[i]);
        }
        return h;
    }

    hash_t hash_range(int l, int r, const vector<hash_t>& h) {
        if(l == 0) {
            return h[r];
        }
        return h[r] - h[l - 1] * base_pow[r - l + 1];
    }
};

HashMeta hash_meta;
using hash_t = HashMeta::hash_t;

int n, m;
vector<string> tbl;

void read() {
    cin >> n >> m;
    tbl.resize(n);
    for(int i = 0; i < n; i++) {
        cin >> tbl[i];
    }
}

tuple<bool, pair<int, int>, pair<int, int>> check(
    const vector<vector<hash_t>>& h, const vector<vector<hash_t>>& rh,
    const vector<vector<hash_t>>& v, const vector<vector<hash_t>>& rv, int k
) {
    if(k > n || k > m) {
        return {false, {-1, -1}, {-1, -1}};
    }

    if(k <= 1) {
        return {true, {0, 0}, {0, 0}};
    }

    vector<vector<int>> cnt_right(n, vector<int>(m, 0));
    vector<vector<int>> cnt_down(n, vector<int>(m, 0));
    for(int i = n - 1; i >= 0; i--) {
        for(int j = m - 1; j >= 0; j--) {
            bool is_row_palindrome = false, is_col_palindrome = false;
            if(j + k <= m) {
                hash_t h1 = hash_meta.hash_range(j, j + k - 1, h[i]);
                hash_t rh1 =
                    hash_meta.hash_range(m - 1 - (j + k - 1), m - 1 - j, rh[i]);
                is_row_palindrome = (h1 == rh1);
                cnt_down[i][j] = (int)is_row_palindrome *
                                 (1 + (i + 1 < n ? cnt_down[i + 1][j] : 0));
            }

            if(i + k <= n) {
                hash_t v1 = hash_meta.hash_range(i, i + k - 1, v[j]);
                hash_t rv1 =
                    hash_meta.hash_range(n - 1 - (i + k - 1), n - 1 - i, rv[j]);
                is_col_palindrome = (v1 == rv1);
                cnt_right[i][j] = (int)is_col_palindrome *
                                  (1 + (j + 1 < m ? cnt_right[i][j + 1] : 0));
            }

            if(cnt_right[i][j] >= k && cnt_down[i][j] >= k) {
                return {true, {i, j}, {i + k - 1, j + k - 1}};
            }
        }
    }

    return {false, {-1, -1}, {-1, -1}};
}

void solve() {
    hash_meta.init(max(n, m) + 1);
    vector<vector<hash_t>> h(n, vector<hash_t>(m)), rh(n, vector<hash_t>(m));
    for(int i = 0; i < n; i++) {
        h[i] = hash_meta.rabin_karp(tbl[i]);
        rh[i] = hash_meta.rabin_karp(string(tbl[i].rbegin(), tbl[i].rend()));
    }

    vector<vector<hash_t>> v(m, vector<hash_t>(n)), rv(m, vector<hash_t>(n));
    for(int j = 0; j < m; j++) {
        string col;
        for(int i = 0; i < n; i++) {
            col.push_back(tbl[i][j]);
        }
        v[j] = hash_meta.rabin_karp(col);
        rv[j] = hash_meta.rabin_karp(string(col.rbegin(), col.rend()));
    }

    tuple<int, pair<int, int>, pair<int, int>> ans = {-1, {-1, -1}, {-1, -1}};
    int low = (get<0>(ans) + 1) / 2, high = min(n, m) / 2, mid;
    int only_parity = -1;
    while(low <= high) {
        mid = (low + high) / 2;

        int parity = only_parity == -1 ? 1 : only_parity;
        int k = mid * 2 + parity;
        auto [state, p1, p2] = check(h, rh, v, rv, k);
        if(state) {
            ans = max(ans, make_tuple(k, p1, p2));
            low = mid + 1;
        } else {
            if(only_parity == -1) {
                k = mid * 2 + (1 - parity);
                auto [even_state, q1, q2] = check(h, rh, v, rv, k);
                if(even_state) {
                    only_parity = 1 - parity;
                    ans = max(ans, make_tuple(k, q1, q2));
                    low = mid + 1;
                } else {
                    high = mid - 1;
                }
            } else {
                high = mid - 1;
            }
        }
    }

    get<1>(ans).first++;
    get<1>(ans).second++;
    get<2>(ans).first++;
    get<2>(ans).second++;

    cout << get<1>(ans) << ' ' << get<2>(ans) << '\n';
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
