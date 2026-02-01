#include <bits/stdc++.h>
#include <immintrin.h>

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
}

template<typename T>
ostream& operator<<(ostream& out, const vector<T>& a) {
    for(auto x: a) {
        out << x << ' ';
    }
    return out;
};

vector<tuple<bool, int, int>> queries;

void read() {
    char type;
    int l, r;
    multiset<pair<int, int>> segments;
    vector<int> all_coords;
    while(cin >> type >> l >> r) {
        if(type == '+') {
            segments.insert({l, r});
            queries.push_back({true, l, r});
        } else {
            auto it = segments.find({l, r});
            if(it == segments.end()) {
                continue;
            }
            segments.erase(it);
            queries.push_back({false, l, r});
        }

        all_coords.push_back(l);
        all_coords.push_back(r);
    }

    sort(all_coords.begin(), all_coords.end());
    all_coords.erase(
        unique(all_coords.begin(), all_coords.end()), all_coords.end()
    );

    for(auto& [add, l, r]: queries) {
        l = (int)(lower_bound(all_coords.begin(), all_coords.end(), l) -
                  all_coords.begin() + 1);
        r = (int)(lower_bound(all_coords.begin(), all_coords.end(), r) -
                  all_coords.begin() + 1);
    }
}

#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2")

class ActiveSegments {
  private:
    vector<int> Ls;
    vector<int> Rs;

  public:
    ActiveSegments() {
        Ls.reserve(1024);
        Rs.reserve(1024);
    }

    void add(int l, int r) {
        Ls.push_back(l);
        Rs.push_back(r);
    }

    bool remove(int ql, int qr) {
        size_t n = Ls.size();
        size_t i = 0;

        __m256i qlv = _mm256_set1_epi32(ql);
        __m256i qrv = _mm256_set1_epi32(qr);

        for(; i + 7 < n; i += 8) {
            __m256i lv = _mm256_loadu_si256((const __m256i*)(Ls.data() + i));
            __m256i rv = _mm256_loadu_si256((const __m256i*)(Rs.data() + i));

            __m256i matchL = _mm256_cmpeq_epi32(lv, qlv);
            __m256i matchR = _mm256_cmpeq_epi32(rv, qrv);
            __m256i both = _mm256_and_si256(matchL, matchR);

            int mask = _mm256_movemask_epi8(both);
            if(mask != 0) {
                int bit = __builtin_ctz(mask);
                size_t lane = bit >> 2;
                size_t pos = i + lane;

                Ls[pos] = Ls.back();
                Ls.pop_back();
                Rs[pos] = Rs.back();
                Rs.pop_back();
                return true;
            }
        }

        for(; i < n; ++i) {
            if(Ls[i] == ql && Rs[i] == qr) {
                Ls[i] = Ls.back();
                Ls.pop_back();
                Rs[i] = Rs.back();
                Rs.pop_back();
                return true;
            }
        }
        return false;
    }

    int count_contained(int ql, int qr) const {
        int cnt = 0;
        size_t n = Ls.size();
        size_t i = 0;

        __m256i qlv = _mm256_set1_epi32(ql - 1);
        __m256i qrv = _mm256_set1_epi32(qr + 1);

        for(; i + 7 < n; i += 8) {
            __m256i lv = _mm256_loadu_si256((const __m256i*)&Ls[i]);
            __m256i rv = _mm256_loadu_si256((const __m256i*)&Rs[i]);

            __m256i ge = _mm256_cmpgt_epi32(lv, qlv);
            __m256i le = _mm256_cmpgt_epi32(qrv, rv);

            __m256i both = _mm256_and_si256(ge, le);

            int mask = _mm256_movemask_epi8(both);
            cnt += __builtin_popcount(mask) >> 2;
        }

        for(; i < n; ++i) {
            if(Ls[i] >= ql && Rs[i] <= qr) {
                ++cnt;
            }
        }

        return cnt;
    }
};

void solve() {
    // One approach for solving this in O(Q log^2 Q) is to build a segment tree
    // on the L coordinates, and then keep all R coordinates in a sorted manner.
    // Then the update (addition / removal), ends up being a walk down to the
    // corresponding leaf of the segment tree for L, and inserting / erasing R
    // from all nodes we visit. The query ends up being just decomposing the [L;
    // R] range with the O(log Q) segment tree nodes, and figuring out how many
    // R coordinates fall within [L; R]. For the inner structure we can use some
    // BST or offline build all unique values that could appear in the segment
    // tree node, and do a segment tree / BIT over them. One easy approach is
    // to use the Policy-Based Data Structures (PBDS) from GNU C++ which is
    // widely accepted on online judges. Another simple approach that ends up
    // being fast in practice is to implement everything with a 2D fenwick tree,
    // by keeping the nodes as an unordered_map. However, after trying to
    // implement this it's a bit hard to fit in the memory limit.
    //
    // Instead, here we will use the fact that there are at most ~1000-2000
    // active segments at any moment and do a brute-force with a vector. This is
    // fast enough to pass, but we can further speed it up with AVX2 SIMD.

    ActiveSegments active;

    for(auto [is_add, l, r]: queries) {
        if(is_add) {
            int cnt = active.count_contained(l, r);
            cout << cnt << '\n';
            active.add(l, r);
        } else {
            active.remove(l, r);
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
