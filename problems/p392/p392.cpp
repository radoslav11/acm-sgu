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

struct HashMonoid {
    hash_t h;
    hash_t pow;
    int64_t len;

    static HashMonoid identity() { return {hash_t(0), hash_t(1), 0}; }

    static HashMonoid single(char c, hash_t base) {
        return {hash_t((unsigned char)c), base, 1};
    }

    HashMonoid merge(const HashMonoid& other) const {
        return {h * other.pow + other.h, pow * other.pow, len + other.len};
    }
};

int r, c, q;
vector<string> arrows, letters;
vector<string> compressed_queries;
vector<vector<vector<pair<int, int>>>> jumps;
vector<HashMonoid> query_hashes;

void read() {
    cin >> r >> c;
    arrows.resize(r);
    letters.resize(r);
    for(int i = 0; i < r; i++) {
        cin >> arrows[i];
    }
    for(int i = 0; i < r; i++) {
        cin >> letters[i];
    }
    cin >> q;
    compressed_queries.resize(q);
    for(int i = 0; i < q; i++) {
        cin >> compressed_queries[i];
    }
}

pair<int, int> get_next_cell(int x, int y) {
    char arrow = arrows[x][y];
    int nx = x, ny = y;
    if(arrow == '<') {
        ny--;
    } else if(arrow == '>') {
        ny++;
    } else if(arrow == '^') {
        nx--;
    } else if(arrow == 'v') {
        nx++;
    }
    if(nx < 0 || nx >= r || ny < 0 || ny >= c) {
        return {-1, -1};
    }
    return {nx, ny};
}

pair<int, int> apply_jump(pair<int, int> pos, int level) {
    if(pos.first == -1) {
        return {-1, -1};
    }
    return jumps[level][pos.first][pos.second];
}

pair<HashMonoid, bool> compute_hash_from_pos(
    pair<int, int> pos, int64_t steps,
    const vector<vector<vector<HashMonoid>>>& dp
) {
    if(steps == 0) {
        return {HashMonoid::identity(), true};
    }
    if(pos.first == -1) {
        return {HashMonoid::identity(), false};
    }

    HashMonoid result = HashMonoid::identity();
    for(int i = 30; i >= 0; i--) {
        if(steps & (1LL << i)) {
            if(pos.first == -1) {
                return {HashMonoid::identity(), false};
            }

            const auto& m = dp[i][pos.first][pos.second];
            if(m.len < (1LL << i)) {
                return {HashMonoid::identity(), false};
            }

            result = result.merge(m);
            pos = apply_jump(pos, i);
        }
    }
    return {result, true};
}

hash_t geometric_series_sum(int64_t k, hash_t p) {
    if(k == 0) {
        return hash_t(0);
    }
    hash_t result = hash_t(0);
    hash_t subsum = hash_t(1);
    hash_t current_pow = p;
    while(k > 0) {
        if(k % 2 == 1) {
            result = result * current_pow + subsum;
        }
        subsum = subsum * (current_pow + hash_t(1));
        current_pow = current_pow * current_pow;
        k /= 2;
    }
    return result;
}

hash_t power(hash_t base, int64_t exp) {
    hash_t result = hash_t(1);
    while(exp > 0) {
        if(exp % 2 == 1) {
            result = result * base;
        }
        base = base * base;
        exp /= 2;
    }
    return result;
}

void solve() {
    // We should immediately think of hashes in this problem, as the pattern can
    // go up to 10^9 in length. This splits the problem in two - how to compute
    // the hash of the pattern, and how to compute the hash starting from some
    // cell (x, y).
    //
    // Note that after we know the length of the pattern, getting
    // the hash starting from cell (x, y) can be done with binary lifting, and
    // this is satisfactory because we only have R*C <= 900 cells. The binary
    // lifting is then also ~log(10^9) operations, and this is fast enough for Q
    // <= 50 queries. In terms of details for the binary lifting, we need to
    // hold the base power, the cell in 2^L jumps, and the actual hash. We have
    // to be careful about the case of going out of the table.
    //
    // In terms of computing the hash of the pattern, we need to de-compress the
    // pattern first. It's a fairly standard procedure which we can implement
    // with a stack holding the hash, base power, and length, similarly to what
    // we did in the binary power. The only part to be careful about is how to
    // handle the #repetitions, but this is essentially a sum of a geometric
    // series which we can either do with modular inverse, or in logarithmic
    // time with a procedure similar to binary exponentiation.

    hash_meta.init(1);
    hash_t base_hash = hash_t(hash_meta.base);

    int max_log = 31;
    jumps.resize(max_log);
    vector<vector<vector<HashMonoid>>> dp(
        max_log, vector<vector<HashMonoid>>(r, vector<HashMonoid>(c))
    );

    for(int l = 0; l < max_log; l++) {
        jumps[l].resize(r);
        for(int i = 0; i < r; i++) {
            jumps[l][i].resize(c, {-1, -1});
        }
    }

    for(int i = 0; i < r; i++) {
        for(int j = 0; j < c; j++) {
            jumps[0][i][j] = get_next_cell(i, j);
            dp[0][i][j] = HashMonoid::single(letters[i][j], base_hash);
        }
    }

    for(int l = 1; l < max_log; l++) {
        for(int i = 0; i < r; i++) {
            for(int j = 0; j < c; j++) {
                auto next = jumps[l - 1][i][j];

                if(next.first == -1) {
                    jumps[l][i][j] = {-1, -1};
                    dp[l][i][j] = dp[l - 1][i][j];
                } else {
                    dp[l][i][j] =
                        dp[l - 1][i][j].merge(dp[l - 1][next.first][next.second]
                        );
                    jumps[l][i][j] = jumps[l - 1][next.first][next.second];
                }
            }
        }
    }

    query_hashes.resize(q);
    for(int qi = 0; qi < q; qi++) {
        string query = "(" + compressed_queries[qi] + ")1";
        vector<HashMonoid> st;

        for(int idx = 0; idx < (int)query.size(); idx++) {
            char ch = query[idx];

            if(ch == '(') {
                st.push_back({hash_t(0), hash_t(1), -1});
            } else if(ch == ')') {
                HashMonoid combined = HashMonoid::identity();
                while(st.back().len != -1) {
                    combined = st.back().merge(combined);
                    st.pop_back();
                }
                st.pop_back();

                idx++;
                int64_t cnt = 0;
                while(idx < (int)query.size() && isdigit(query[idx])) {
                    cnt = cnt * 10ll + query[idx] - '0';
                    idx++;
                }
                idx--;

                hash_t pow_repeat = power(combined.pow, cnt);
                hash_t h_repeat =
                    combined.h * geometric_series_sum(cnt, combined.pow);

                st.push_back({h_repeat, pow_repeat, combined.len * cnt});
            } else {
                st.push_back(HashMonoid::single(ch, base_hash));
            }
        }

        query_hashes[qi] = st.back();
    }

    for(int qi = 0; qi < q; qi++) {
        HashMonoid target = query_hashes[qi];

        pair<int, int> answer = {-1, -1};

        for(int i = 0; i < r; i++) {
            for(int j = 0; j < c; j++) {
                auto [result, success] =
                    compute_hash_from_pos({i, j}, target.len, dp);
                if(success && result.h == target.h) {
                    if(answer.first == -1 || (i < answer.first) ||
                       (i == answer.first && j < answer.second)) {
                        answer = {i, j};
                    }
                }
            }
        }

        if(answer.first == -1) {
            cout << "NO\n";
        } else {
            cout << "YES (" << answer.first + 1 << "," << answer.second + 1
                 << ")\n";
        }
    }
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
