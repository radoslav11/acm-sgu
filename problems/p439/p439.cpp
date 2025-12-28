#include <bits/stdc++.h>
// #include <coding_library/strings/hashing.hpp>

#pragma GCC optimize("O3,unroll-loops")
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

int n, m;
string A, B;

void read() {
    cin >> n >> m;
    cin >> A >> B;
    A = A + A;
    B = B + B;
}

void solve() {
    // We could potentially solve this with suffix trees or arrays, but the "1
    // character difference" condition makes this a bit more annoying. Instead,
    // there is a fairly direct way with hashing. We can recall the O(n log^2 n)
    // algorithm for suffix arrays that is based on hashing: we define a lcp(i,
    // j) function that finds the longest common prefix of the suffix starting
    // at i and j using binary search and hashing in log(n). Then the full
    // suffix array algorithm is just using this lcp(i, j) in the cmp function
    // of a std::sort.
    //
    // The idea of using hashes and binary search to find the first mismatch can
    // also be generalized - it doesn't have to be for building the suffix
    // array. In particular, in this problem we first need to find the
    // lexicographically smallest cycle shift, which can be done in O(n) calls
    // of the lcp(i, j) oracle, by just thinking of it as finding the minimum in
    // an array, but with just a slightly more complicated compare function.
    // Alternatively, we could use Booth's algorithm which is heavily based on
    // Knuth-Morris-Pratt (KMP) and is a bit faster (overall O(n)), but in the
    // second part of the problem we will anyways need hashes so an O(n log n)
    // approach is fine.
    //
    // After we have found the minimal cyclic shift (we will call this the
    // pattern), we want to find a substring in the first string that matches
    // the pattern with 1 character error allowed. Let's rephrase this instead
    // as finding the first <= 2 mismatches. This can be done with two calls of
    // lcp(i, j) which we already know are in O(log). We have to be careful
    // about the indices when we implement this.
    //
    // In terms of implementation, we start with A = A + A and B = B + B. This
    // is so that we only look at substrings of length m, and don't care about
    // the actual cycle shifts.
    //
    // Overall, the time complexity is O(n log n).

    hash_meta.init(2 * n);
    vector<hash_t> ph_a = hash_meta.rabin_karp(A);
    vector<hash_t> ph_b = hash_meta.rabin_karp(B);

    auto lcp = [&](int i, int j, vector<hash_t>& ph_i, vector<hash_t>& ph_j,
                   int max_len) {
        int lo = 0, hi = max_len;
        while(lo < hi) {
            int mid = (lo + hi + 1) / 2;
            if(hash_meta.hash_range(i, i + mid - 1, ph_i) ==
               hash_meta.hash_range(j, j + mid - 1, ph_j)) {
                lo = mid;
            } else {
                hi = mid - 1;
            }
        }
        return lo;
    };

    auto cmp_cyclic = [&](int i, int j) {
        int l = lcp(i, j, ph_b, ph_b, m);
        if(l == m) {
            return false;
        }
        return B[i + l] < B[j + l];
    };

    int best_b = 0;
    for(int i = 1; i < m; i++) {
        if(cmp_cyclic(i, best_b)) {
            best_b = i;
        }
    }

    auto count_mismatches = [&](int pos_a) {
        int l1 = lcp(pos_a, best_b, ph_a, ph_b, m);
        if(l1 == m) {
            return 0;
        }
        if(l1 == m - 1) {
            return 1;
        }
        int l2 = lcp(pos_a + l1 + 1, best_b + l1 + 1, ph_a, ph_b, m - l1 - 1);
        if(l1 + 1 + l2 == m) {
            return 1;
        }
        return 2;
    };

    int best_a = -1;
    for(int d = 0; d <= n / 2 && best_a == -1; d++) {
        if(count_mismatches(d) <= 1) {
            best_a = d;
        } else if(d > 0 && n - d != d && count_mismatches(n - d) <= 1) {
            best_a = n - d;
        }
    }

    string result_b = B.substr(best_b, m);
    string result_a = A.substr(best_a, n);

    cout << result_b << "\n";
    cout << result_a << "\n";
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
