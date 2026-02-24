#include <bits/stdc++.h>
// #include <coding_library/data_structures/fenwick.hpp>

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

template<class T>
class Fenwick {
  private:
    int sz, log_size;
    vector<T> tr;

  public:
    void init(int n) {
        sz = n + 1;
        log_size = 31 - __builtin_clz(sz);
        tr.assign(sz + 1, 0);
    }

    void update(int idx, T val) {
        if(idx <= 0) {
            assert(false);
            return;
        }
        for(; idx <= sz; idx += (idx & -idx)) {
            tr[idx] += val;
        }
    }

    T query(int idx) {
        T ans = 0;
        for(; idx >= 1; idx -= (idx & -idx)) {
            ans += tr[idx];
        }

        return ans;
    }

    T query(int l, int r) { return query(r) - query(l - 1); }

    int find_kth(T k) {
        int idx = 0;
        for(int i = log_size; i >= 0; i--) {
            if(idx + (1 << i) < sz && tr[idx + (1 << i)] < k) {
                k -= tr[idx + (1 << i)];
                idx += (1 << i);
            }
        }
        return idx + 1;
    }
};

int n, k;
vector<int64_t> x, y, w;

void read() {
    cin >> n >> k;
    x.resize(n);
    y.resize(n);
    w.resize(n);
    for(int i = 0; i < n; i++) {
        cin >> x[i] >> y[i] >> w[i];
    }
}

void solve() {
    // We will start with the standard trick of rotating everything by 45%
    // degrees and scaling by two with (x, y) goes into (x + y, x - y). The area
    // of each pizza hut is now a rectangle with corners (x - w, y - w) and (x +
    // w, y + w), where this is the transformed (x, y). We want to figure out
    // which centers are covered by at least K rectangles (excluding the given
    // one). This can be done by treating each rectangle as two events (low_y,
    // high_y, low_x, +1), and (low_y, high_y, high_x, -1), and maintaining a
    // segment or fenwick tree over the Ys. We will add +delta at low_y and
    // subtract it at (high_y+1). Then we will also have each center as a query,
    // and we can simply query the fenwick tree. For convenince, we can compress
    // all Y coordinates before processing the events.

    vector<int64_t> tx(n), ty(n);
    for(int i = 0; i < n; i++) {
        tx[i] = x[i] + y[i];
        ty[i] = x[i] - y[i];
    }

    vector<int64_t> all_ys;
    for(int i = 0; i < n; i++) {
        all_ys.push_back(ty[i] - w[i]);
        all_ys.push_back(ty[i] + w[i] + 1);
        all_ys.push_back(ty[i]);
    }
    sort(all_ys.begin(), all_ys.end());
    all_ys.erase(unique(all_ys.begin(), all_ys.end()), all_ys.end());

    auto compress = [&](int64_t v) -> int {
        return lower_bound(all_ys.begin(), all_ys.end(), v) - all_ys.begin() +
               1;
    };

    struct Event {
        int64_t x_coord;
        int type;
        int idx;
        int delta;
    };

    vector<Event> events;
    for(int i = 0; i < n; i++) {
        events.push_back({tx[i] - w[i], 0, i, 1});
        events.push_back({tx[i] + w[i] + 1, 0, i, -1});
        events.push_back({tx[i], 1, i, 0});
    }

    sort(events.begin(), events.end(), [](const Event& a, const Event& b) {
        if(a.x_coord != b.x_coord) {
            return a.x_coord < b.x_coord;
        }
        return a.type < b.type;
    });

    Fenwick<int> fw;
    fw.init(all_ys.size());

    vector<int> cnt(n);
    for(auto& e: events) {
        if(e.type == 0) {
            int lo = compress(ty[e.idx] - w[e.idx]);
            int hi = compress(ty[e.idx] + w[e.idx] + 1);
            fw.update(lo, e.delta);
            fw.update(hi, -e.delta);
        } else {
            cnt[e.idx] = fw.query(compress(ty[e.idx]));
        }
    }

    vector<int> result;
    for(int i = 0; i < n; i++) {
        if(cnt[i] - 1 >= k) {
            result.push_back(i + 1);
        }
    }

    cout << result.size() << "\n";
    for(int i = 0; i < (int)result.size(); i++) {
        if(i) {
            cout << " ";
        }
        cout << result[i];
    }
    cout << "\n";
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
