#include <bits/stdc++.h>
// #include <coding_library/dp_optimizations/convex_hull_trick.hpp>

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

template<class T, bool maximum = true>
class ConvexHullTrick {
  public:
    static constexpr T inf = is_integral_v<T> ? numeric_limits<T>::max()
                                              : numeric_limits<T>::infinity();

    T div(T a, T b) {
        if constexpr(is_integral_v<T>) {
            return a / b - ((a ^ b) < 0 && a % b);
        } else {
            return a / b;
        }
    }

    struct Line {
        mutable T k, m, p;
        bool operator<(const Line& o) const { return k < o.k; }
        bool operator<(T x) const { return p < x; }
    };

    multiset<Line, less<>> hull;

    using Iter = typename multiset<Line, less<>>::iterator;

    bool intersect(Iter x, Iter y) {
        if(y == hull.end()) {
            return x->p = inf, 0;
        }
        if(x->k == y->k) {
            x->p = x->m > y->m ? inf : -inf;
        } else {
            x->p = div(y->m - x->m, x->k - y->k);
        }
        return x->p >= y->p;
    }

    void add(T k, T m) {
        if constexpr(!maximum) {
            k = -k;
            m = -m;
        }
        auto z = hull.insert({k, m, 0}), y = z++, x = y;
        while(intersect(y, z)) {
            z = hull.erase(z);
        }
        if(x != hull.begin() && intersect(--x, y)) {
            intersect(x, y = hull.erase(y));
        }
        while((y = x) != hull.begin() && (--x)->p >= y->p) {
            intersect(x, hull.erase(y));
        }
    }

    T query(T x) {
        assert(!hull.empty());
        auto l = *hull.lower_bound(x);
        T res = l.k * x + l.m;
        if constexpr(!maximum) {
            res = -res;
        }
        return res;
    }
};

long long B;
int N;

struct Stable {
    long long x;
    vector<pair<long long, long long>> horses;
};

struct SegTree {
    int n;
    vector<ConvexHullTrick<double, false>> tree;

    SegTree() : n(0) {}
    SegTree(int n) : n(n), tree(4 * n) {}

    void update(int ql, int qr, double k, double m, int idx, int lo, int hi) {
        if(ql > hi || qr < lo) {
            return;
        }
        if(ql <= lo && hi <= qr) {
            tree[idx].add(k, m);
            return;
        }
        int mid = (lo + hi) / 2;
        update(ql, qr, k, m, 2 * idx, lo, mid);
        update(ql, qr, k, m, 2 * idx + 1, mid + 1, hi);
    }

    double query(int pos, double x, int idx, int lo, int hi) {
        double res = 1e18;
        if(!tree[idx].hull.empty()) {
            res = tree[idx].query(x);
        }
        if(lo == hi) {
            return res;
        }
        int mid = (lo + hi) / 2;
        if(pos <= mid) {
            return min(res, query(pos, x, 2 * idx, lo, mid));
        } else {
            return min(res, query(pos, x, 2 * idx + 1, mid + 1, hi));
        }
    }
};

vector<Stable> stables;

void read() {
    cin >> B >> N;
    stables.resize(N);
    for(int i = 0; i < N; i++) {
        int m;
        cin >> stables[i].x >> m;
        stables[i].horses.resize(m);
        for(int j = 0; j < m; j++) {
            long long v, d;
            cin >> v >> d;
            stables[i].horses[j] = {d, v};
        }
    }
}

void solve() {
    // We should ideally aim for something that is quicker than O(N * M) which
    // can be done with a simple DP but might be a bit too tight to pass. We
    // know that every horse can get us from some Xi to some other Xj and all
    // stables between these (we can do a binary search to find the relevant
    // ranges). Afterwards, if these two are [i;r], we would like to update
    // every j > i in this using DP[j] := min(DP[j], (Xj - Xi) / vk + DP[j]) for
    // horse k that can get us from i to j. Implementing this naively by going
    // through all horses (M) and the O(N) reachable stables, results in O(N *
    // M).
    //
    // To make this quicker, we can notice that the second term in the minimum
    // above is actually a linear function. This should immediately lead us to
    // thinking about convex hull trick. Let us build a segment tree like
    // structure over the N stables, with a convex hull trick container in each
    // node. Then after processing some stable i, we would like to add Mi lines
    // to various ranges [i+1, rk]. We can decompose these ranges recursively
    // into O(log N) intervals, and then add line individually to each of the
    // containers covered. This update ends up being in O(log N * log M). The
    // query is then done whenever we are processing some stable, and is simply
    // going through the nodes from the root until a leaf, which is again the
    // same complexity of O(log N * log M). We have O(M) updates and queries,
    // which overall is quicker than O(N * M).

    sort(stables.begin(), stables.end(), [](const Stable& a, const Stable& b) {
        return a.x < b.x;
    });

    while(!stables.empty() && stables.back().x > B) {
        stables.pop_back();
    }
    N = stables.size();

    if(N == 0 || stables[0].x != 0) {
        cout << -1 << endl;
        return;
    }

    SegTree seg(N);
    vector<double> dp(N, 1e18);
    double ans = 1e18;

    for(int i = 0; i < N; i++) {
        if(stables[i].x == 0) {
            dp[i] = 0;
        }

        if(i > 0) {
            double q = seg.query(i, (double)stables[i].x, 1, 0, N - 1);
            dp[i] = min(dp[i], q);
        }

        if(dp[i] >= 1e18) {
            continue;
        }

        for(auto& [d, v]: stables[i].horses) {
            long long reach = stables[i].x + d;

            if(reach >= B) {
                ans = min(ans, dp[i] + (double)(B - stables[i].x) / v);
            }

            int r = (int)(upper_bound(
                              stables.begin(), stables.end(), reach,
                              [](long long val, const Stable& s) {
                                  return val < s.x;
                              }
                          ) -
                          stables.begin()) -
                    1;

            if(r > i) {
                double slope = 1.0 / v;
                double intercept = dp[i] - (double)stables[i].x / v;
                seg.update(i + 1, r, slope, intercept, 1, 0, N - 1);
            }
        }
    }

    if(ans >= 1e18) {
        cout << -1 << endl;
    } else {
        cout << fixed << setprecision(8) << ans << endl;
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
