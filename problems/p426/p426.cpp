#include <bits/stdc++.h>
// #include <coding_library/data_structures/sparse_table.hpp>
// #include <coding_library/strings/suffix_array.hpp>

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

template<class T, T (*merge)(T, T)>
class SparseTable {
  private:
    int n;
    vector<vector<T>> dp;
    vector<int> prec_lg2;

  public:
    SparseTable() {
        n = 0;
        dp.clear();
        prec_lg2.clear();
    }

    void init(const vector<T>& a) {
        n = a.size();
        prec_lg2.resize(n + 1);
        for(int i = 2; i <= n; i++) {
            prec_lg2[i] = prec_lg2[i >> 1] + 1;
        }

        dp.assign(prec_lg2[n] + 1, vector<T>(n));
        dp[0] = a;

        for(int j = 1; (1 << j) <= n; j++) {
            for(int i = 0; i + (1 << j) <= n; i++) {
                dp[j][i] = merge(dp[j - 1][i], dp[j - 1][i + (1 << (j - 1))]);
            }
        }
    }

    T query(int l, int r) {
        int k = prec_lg2[r - l + 1];
        return merge(dp[k][l], dp[k][r - (1 << k) + 1]);
    }
};

template<class T = string>
class SuffixArray {
  private:
    using G = conditional_t<
        is_same_v<T, const char*> || is_same_v<T, char*> || is_array_v<T>, char,
        typename T::value_type>;

    void build_sa(const T& s) {
        sa.resize(n);
        rnk.resize(n);
        vector<int> tmp(n);

        for(int i = 0; i < n; i++) {
            sa[i] = i;
            rnk[i] = (int)s[i];
        }

        for(int k = 1; k < n; k <<= 1) {
            auto cmp = [&](int x, int y) {
                if(rnk[x] != rnk[y]) {
                    return rnk[x] < rnk[y];
                }
                int rx = x + k < n ? rnk[x + k] : -1;
                int ry = y + k < n ? rnk[y + k] : -1;
                return rx < ry;
            };

            sort(sa.begin(), sa.end(), cmp);
            tmp[sa[0]] = 0;
            for(int i = 1; i < n; i++) {
                tmp[sa[i]] = tmp[sa[i - 1]] + (cmp(sa[i - 1], sa[i]) ? 1 : 0);
            }

            rnk = tmp;
            if(rnk[sa[n - 1]] == n - 1) {
                break;
            }
        }
    }

    void build_lcp(const T& s) {
        lcp.assign(n, 0);
        int h = 0;
        for(int i = 0; i < n; i++) {
            if(rnk[i] > 0) {
                int j = sa[rnk[i] - 1];
                while(i + h < n && j + h < n && s[i + h] == s[j + h]) {
                    h++;
                }

                lcp[rnk[i]] = h;
                if(h > 0) {
                    h--;
                }
            } else {
                h = 0;
            }
        }
    }

  public:
    int n;
    vector<int> sa, rnk, lcp;

    SuffixArray() : n(0) {}
    SuffixArray(const T& s) { init(s); }

    void clear() {
        n = 0;
        sa.clear();
        rnk.clear();
        lcp.clear();
    }

    void init(const T& s) {
        clear();
        n = (int)size(s);
        if(n == 0) {
            return;
        }

        build_sa(s);
        build_lcp(s);
    }
};

int min_custom(int a, int b) { return min(a, b); }

int n, m, k;
vector<int> a;

void read() {
    cin >> n >> m >> k;
    a.resize(n);
    cin >> a;
}

void solve() {
    // For each shift t we want the k-th element of the lex smallest cyclic
    // rotation of b_i(t) = (a_i + t) mod m. As t grows every b_i grows by 1
    // too, except whenever some b_i wraps from m-1 to 0. The wrap points are
    // t = (m - v) mod m for each distinct v in a, and sorted they split
    // [0, m) into phases.
    //
    // Inside a phase [L, R) nothing wraps, so every b_i moves in lockstep and
    // all pairwise inequalities among the b values are preserved. The lcp
    // lengths between any two rotations only see equalities in a so they
    // don't depend on t at all, and the tie-breaking character that picked
    // the optimal start at t = L stays the same. So the optimal start is
    // constant over the phase. The converse is also clear: for another
    // position to overtake the current best inside the phase it would have
    // to become 0 first (a 0 in the first position beats any non-zero
    // start), but that would make that t a wrap point and not interior to
    // the phase. So inside [L, R) we can fixate on a single starting index.
    //
    // For the phase of v = (m - L) mod m the rotations whose first b value
    // is 0 are exactly the ones starting at positions where a_i = v, so the
    // best start lives in C_v = { i : a_i = v }. To pick between two starts
    // in C_v in O(1) we build a suffix array + lcp on the doubled array
    // a + a and an RMQ over the lcp; the tie-breaking character is read as
    // (a[...] - v) mod m so the comparison matches the actual b(t) order.
    //
    // Once s_v is known, every t in the phase of v has answer
    // (a[(s_v + k - 1) mod n] + t) mod m, so we just walk the phase and write
    // the values out one by one. Overall O(n log n + m).

    vector<int> doubled(2 * n);
    for(int i = 0; i < 2 * n; i++) {
        doubled[i] = a[i % n];
    }

    SuffixArray<vector<int>> sa;
    sa.init(doubled);

    SparseTable<int, min_custom> rmq;
    rmq.init(sa.lcp);

    auto cyclic_lcp = [&](int p, int q) {
        if(p == q) {
            return n;
        }

        int rp = sa.rnk[p], rq = sa.rnk[q];
        if(rp > rq) {
            swap(rp, rq);
        }

        return min(n, rmq.query(rp + 1, rq));
    };

    vector<vector<int>> groups(m);
    for(int i = 0; i < n; i++) {
        groups[a[i]].push_back(i);
    }

    vector<int> base(m, 0);
    vector<int> wraps;
    wraps.reserve(min(n, m));

    for(int v = 0; v < m; v++) {
        if(groups[v].empty()) {
            continue;
        }

        wraps.push_back((m - v) % m);

        int best = groups[v][0];
        for(int idx = 1; idx < (int)groups[v].size(); idx++) {
            int c = groups[v][idx];
            int l = cyclic_lcp(c, best);
            if(l >= n) {
                continue;
            }

            int ca = (a[(c + l) % n] - v + m) % m;
            int cb = (a[(best + l) % n] - v + m) % m;
            if(ca < cb) {
                best = c;
            }
        }

        base[v] = a[(best + k - 1) % n];
    }

    sort(wraps.begin(), wraps.end());

    vector<int> ans(m);
    int w = (int)wraps.size();
    for(int i = 0; i < w; i++) {
        int L = wraps[i];
        int R = (i + 1 < w) ? wraps[i + 1] : wraps[0] + m;
        int v = (m - L) % m;
        int bv = base[v];
        for(int t = L; t < R; t++) {
            int tt = t % m;
            ans[tt] = (bv + tt) % m;
        }
    }

    for(int t = 0; t < m; t++) {
        cout << ans[t] << '\n';
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
