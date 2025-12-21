#include <bits/stdc++.h>
// #include <coding_library/data_structures/segment_tree.hpp>

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

template<class T, T (*merge)(T, T), T (*e)()>
class SegmentTree {
  private:
    int n, size;
    vector<T> tr;

    void pull(int x) { tr[x] = merge(tr[2 * x], tr[2 * x + 1]); }

  public:
    SegmentTree() { init(vector<T>()); }
    SegmentTree(int _n) { init(vector<T>(_n, e())); }
    SegmentTree(const vector<T>& _a) { init(_a); }

    void init(const vector<T>& _a) {
        n = _a.size();
        size = 1;
        while(size < n) {
            size <<= 1;
        }

        tr.assign(2 * size, e());
        for(int i = 0; i < n; i++) {
            tr[size + i] = _a[i];
        }
        for(int i = size - 1; i > 0; i--) {
            pull(i);
        }
    }

    void update(int pos, T val) {
        pos += size;
        tr[pos] = val;
        for(pos >>= 1; pos > 0; pos >>= 1) {
            pull(pos);
        }
    }

    T get_pos(int pos) { return tr[pos + size]; }

    T query(int l, int r) {
        T ansl = e(), ansr = e();
        for(l += size, r += size + 1; l < r; l >>= 1, r >>= 1) {
            if(l & 1) {
                ansl = merge(ansl, tr[l++]);
            }
            if(r & 1) {
                ansr = merge(tr[--r], ansr);
            }
        }
        return merge(ansl, ansr);
    }

    T query_all() { return tr[1]; }

    template<bool (*f)(T)>
    int max_right(int l) const {
        return max_right(l, [](T x) { return f(x); });
    }
    template<class F>
    int max_right(int l, F f) const {
        if(l == n) {
            return n;
        }

        l += size;
        T sm = e();
        do {
            while(l % 2 == 0) {
                l >>= 1;
            }
            if(!f(merge(sm, tr[l]))) {
                while(l < size) {
                    l = (2 * l);
                    if(f(merge(sm, tr[l]))) {
                        sm = merge(sm, tr[l]);
                        l++;
                    }
                }
                return l - size;
            }
            sm = merge(sm, tr[l]);
            l++;
        } while((l & -l) != l);
        return n;
    }

    template<bool (*f)(T)>
    int min_left(int r) const {
        return min_left(r, [](T x) { return f(x); });
    }
    template<class F>
    int min_left(int r, F f) const {
        if(r == -1) {
            return 0;
        }

        r += size + 1;
        T sm = e();
        do {
            r--;
            while(r > 1 && (r % 2)) {
                r >>= 1;
            }
            if(!f(merge(tr[r], sm))) {
                while(r < size) {
                    r = (2 * r + 1);
                    if(f(merge(tr[r], sm))) {
                        sm = merge(tr[r], sm);
                        r--;
                    }
                }
                return r + 1 - size;
            }
            sm = merge(tr[r], sm);
        } while((r & -r) != r);
        return 0;
    }
};

pair<int, int> max_custom(pair<int, int> a, pair<int, int> b) {
    return max(a, b);
}
pair<int, int> max_e() { return {INT_MIN, -1}; }

const int MAXV = (int)1e6 + 42;

int n;
vector<int> c;

void read() {
    cin >> n;
    c.resize(n);
    for(int i = 0; i < n; i++) {
        cin >> c[i];
    }
}

void solve() {
    // We can solve this problem with DP - there is a direct N^2 solution that
    // keeps a dp[pos] as a state, and tries to select the largest dp[last],
    // such that last < pos and |c[pos] - c[last]| != 1. The cost of (pos - last
    // - 1), so overall:
    //
    //    dp[pos] = max dp[last] + pos - last - 1, over the eligible last
    //    positions
    //
    // We do the classic approach of separating what depends on last, and what
    // depends on pos, which in this case makes it easy as there are no terms
    // depending on both.
    //
    //    dp[pos] = pos - 1 + max (dp[last] - last)
    //
    // Essentially, for some pos, we are interested in the largest value of
    // dp[last] - last, excluding such positions that c[last] = c[pos]. We can
    // notice that this is two ranges: [0;c[pos]-2], [c[pos];c[pos]] and [c[pos]
    // + 2;MAXV], and so we can use a max segment tree to query this
    // efficiently. The problem does ask to recover the solution, so at index
    // c[last] in the segment tree, we will keep the largest value of {dp[last]
    // - last, last}.
    //
    // The overall complexity is O(N log MAXV).

    auto t = SegmentTree<pair<int, int>, max_custom, max_e>(MAXV + 1);

    vector<int> dp(n);
    vector<int> parent(n, -1);

    dp[0] = 1;
    t.update(c[0], {1, 0});

    for(int i = 1; i < n; i++) {
        pair<int, int> best = {0, -1};

        if(c[i] >= 2) {
            best = max(best, t.query(0, c[i] - 2));
        }
        best = max(best, t.query(c[i], c[i]));
        if(c[i] + 2 <= MAXV) {
            best = max(best, t.query(c[i] + 2, MAXV));
        }

        dp[i] = best.first + 1;
        parent[i] = best.second;

        auto current_val = make_pair(dp[i], i);
        t.update(c[i], max(t.get_pos(c[i]), current_val));
    }

    int max_len = *max_element(dp.begin(), dp.end());
    int end_pos = max_element(dp.begin(), dp.end()) - dp.begin();

    vector<int> path;
    int curr = end_pos;
    while(curr != -1) {
        path.push_back(curr);
        curr = parent[curr];
    }
    reverse(path.begin(), path.end());

    cout << n - max_len << '\n';
    for(int p: path) {
        cout << c[p] << ' ';
    }
    cout << '\n';
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
