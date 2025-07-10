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

    vector<hash_t> rabin_karp(const string& s) {
        vector<hash_t> h(s.size());
        for(size_t i = 0; i < s.size(); i++) {
            h[i] = (i ? h[i - 1] : hash_t(0)) * hash_t(base) + hash_t(s[i]);
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
const int B = 50;

int n;
vector<string> dictionary;

void read() {
    cin >> n;
    dictionary.resize(n);
    cin >> dictionary;
}

void solve() {
    vector<pair<hash_t, hash_t>> small_hashes;

    vector<int> large_strings;
    vector<vector<hash_t>> hashes(n);
    for(int k = 0; k < n; k++) {
        hashes[k] = hash_meta.rabin_karp(dictionary[k]);
        if(dictionary[k].size() >= B) {
            large_strings.push_back(k);
        } else {
            for(int i = 0; i < (int)hashes[k].size(); i++) {
                for(int j = 0; j < (int)hashes[k].size(); j++) {
                    hash_t phash = hash_meta.hash_range(0, i, hashes[k]);
                    hash_t shash = hash_meta.hash_range(
                        j, hashes[k].size() - 1, hashes[k]
                    );
                    small_hashes.push_back({phash, shash});
                }
            }
        }
    }

    sort(small_hashes.begin(), small_hashes.end());

    int q;
    cin >> q;
    while(q--) {
        string req_pref, req_suff;
        cin >> req_pref >> req_suff;
        auto hp = hash_meta.rabin_karp(req_pref);
        auto hs = hash_meta.rabin_karp(req_suff);

        int ans = upper_bound(
                      small_hashes.begin(), small_hashes.end(),
                      make_pair(hp.back(), hs.back())
                  ) -
                  lower_bound(
                      small_hashes.begin(), small_hashes.end(),
                      make_pair(hp.back(), hs.back())
                  );
        for(int k: large_strings) {
            if(dictionary[k].size() < max(req_pref.size(), req_suff.size())) {
                continue;
            }
            hash_t phash =
                hash_meta.hash_range(0, (int)req_pref.size() - 1, hashes[k]);
            hash_t shash = hash_meta.hash_range(
                (int)dictionary[k].size() - req_suff.size(),
                (int)dictionary[k].size() - 1, hashes[k]
            );
            if(phash == hp.back() && shash == hs.back()) {
                ans++;
            }
        }

        cout << ans << endl;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    hash_meta.init((int)1e6);

    int T = 1;
    // cin >> T;
    for(int test = 1; test <= T; test++) {
        read();
        // cout << "Case #" << test << ": ";
        solve();
    }

    return 0;
}
