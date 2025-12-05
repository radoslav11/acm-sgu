#include <bits/stdc++.h>
// #include <coding_library/strings/hashing.hpp>

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
        base = uniform_int_distribution<uint64_t>(0, mod - 1)(rng);
    }

    void precompute_base_pow(size_t n) {
        base_pow.resize(n);
        base_pow[0] = 1;
        for(size_t i = 1; i < n; i++) {
            base_pow[i] = mul(base_pow[i - 1], base);
        }
    }

    static constexpr uint64_t add(uint64_t a, uint64_t b) {
        a += b + 1;
        a = (a & mod) + (a >> 61);
        return a - 1;
    }

    static constexpr uint64_t sub(uint64_t a, uint64_t b) {
        return add(a, mod - b);
    }

    static constexpr uint64_t mul(uint64_t a, uint64_t b) {
        uint64_t l1 = (uint32_t)a, h1 = a >> 32, l2 = (uint32_t)b, h2 = b >> 32;
        uint64_t l = l1 * l2, m = l1 * h2 + l2 * h1, h = h1 * h2;
        uint64_t ret =
            (l & mod) + (l >> 61) + (h << 3) + (m >> 29) + (m << 35 >> 3) + 1;
        ret = (ret & mod) + (ret >> 61);
        ret = (ret & mod) + (ret >> 61);
        return ret - 1;
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
    static constexpr uint64_t mod = (1ull << 61) - 1;

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

string a, b;

void read() { cin >> a >> b; }

vector<hash_t> ha, ha_rev, hb;

pair<int, int> check(int len) {
    map<hash_t, pair<int, int>> vis;

    for(int i = len - 1; i < (int)a.size(); i++) {
        int l = i - len + 1, r = i;
        int rev_l = (int)a.size() - r - 1;
        int rev_r = (int)a.size() - l - 1;

        hash_t fh = hash_meta.hash_range(l, r, ha);
        hash_t rh = hash_meta.hash_range(rev_l, rev_r, ha_rev);
        if(fh == rh) {
            vis[fh] = {l, r};
        }
    }

    for(int i = len - 1; i < (int)b.size(); i++) {
        hash_t fh = hash_meta.hash_range(i - len + 1, i, hb);
        auto it = vis.find(fh);

        if(it != vis.end()) {
            return it->second;
        }
    }

    return {-1, -1};
}

void solve() {
    // We can solve this with binary search and hashing. We need to consider two
    // cases - odd and even length of the palindrome and do two separate binary
    // searches, because the maximal odd length might be quite different from
    // the maximal even. The complexity will be O(S log^2 S). To check for a
    // given length, we will do backward and forward hash to identify the
    // palindromes in A of length L, then mark them in a map or as set (this is
    // the second log), and go through the substrings of length L in B, checking
    // if they appear in the map.

    hash_meta.init(a.size() + b.size());

    ha = hash_meta.rabin_karp(a);
    ha_rev = hash_meta.rabin_karp(string(a.rbegin(), a.rend()));
    hb = hash_meta.rabin_karp(b);

    pair<int, int> ans = {-1, -1};

    int low = 1, high = min(a.size(), b.size());
    while(low <= high) {
        int mid = (low + high) / 2;
        int len = 2 * mid - 1;
        auto res = check(len);
        if(res.first != -1) {
            if(ans.first == -1 ||
               res.second - res.first + 1 > ans.second - ans.first + 1) {
                ans = res;
            }
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    low = 1, high = min(a.size(), b.size());
    while(low <= high) {
        int mid = (low + high) / 2;
        int len = 2 * mid;
        auto res = check(len);
        if(res.first != -1) {
            if(ans.first == -1 ||
               res.second - res.first + 1 > ans.second - ans.first + 1) {
                ans = res;
            }
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    assert(ans.first != -1);
    cout << a.substr(ans.first, ans.second - ans.first + 1) << '\n';
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
