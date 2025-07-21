#include <bits/stdc++.h>

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

int op(int a, int b) { return max(a, b); }

int id() { return 0; }

struct Point {
    int id, ry;
    int64_t x, y;
};

int N;
vector<Point> points;
vector<int64_t> y_coords;
vector<int> up_dp, down_dp;

void read() {
    cin >> N;
    points.resize(N);
    for(int i = 0; i < N; i++) {
        cin >> points[i].x >> points[i].y;
        points[i].id = i + 1;
        y_coords.push_back(points[i].y);
    }
}

void compress_coordinates() {
    sort(y_coords.begin(), y_coords.end());
    y_coords.erase(unique(y_coords.begin(), y_coords.end()), y_coords.end());

    for(auto& p: points) {
        p.ry = lower_bound(y_coords.begin(), y_coords.end(), p.y) -
               y_coords.begin();
    }
}

void compute_up_dp() {
    vector<Point> sorted_points = points;
    sort(
        sorted_points.begin(), sorted_points.end(),
        [](const Point& a, const Point& b) { return a.x < b.x; }
    );

    SegmentTree<int, &op, &id> seg_tree(y_coords.size());
    up_dp.assign(N + 1, 0);

    int idx = 0;
    while(idx < N) {
        int64_t current_x = sorted_points[idx].x;
        vector<pair<int, int>> group;

        while(idx < N && sorted_points[idx].x == current_x) {
            group.push_back({sorted_points[idx].ry, sorted_points[idx].id});
            idx++;
        }

        vector<int> dp_values(group.size());
        for(size_t j = 0; j < group.size(); j++) {
            int ry = group[j].first;
            int max_prev = (ry == 0 ? 0 : seg_tree.query(0, ry - 1));
            dp_values[j] = 1 + max_prev;
        }

        for(size_t j = 0; j < group.size(); j++) {
            int ry = group[j].first;
            int current_val = seg_tree.get_pos(ry);
            seg_tree.update(ry, max(current_val, dp_values[j]));
            up_dp[group[j].second] = dp_values[j];
        }
    }
}

void compute_down_dp() {
    vector<Point> sorted_points = points;
    sort(
        sorted_points.begin(), sorted_points.end(),
        [](const Point& a, const Point& b) { return a.x > b.x; }
    );

    SegmentTree<int, &op, &id> seg_tree(y_coords.size());
    down_dp.assign(N + 1, 0);

    int idx = 0;
    while(idx < N) {
        int64_t current_x = sorted_points[idx].x;
        vector<pair<int, int>> group;

        while(idx < N && sorted_points[idx].x == current_x) {
            group.push_back({sorted_points[idx].ry, sorted_points[idx].id});
            idx++;
        }

        vector<int> dp_values(group.size());
        for(size_t j = 0; j < group.size(); j++) {
            int ry = group[j].first;
            int max_prev =
                (ry + 1 < (int)y_coords.size()
                     ? seg_tree.query(ry + 1, y_coords.size() - 1)
                     : 0);
            dp_values[j] = 1 + max_prev;
        }

        for(size_t j = 0; j < group.size(); j++) {
            int ry = group[j].first;
            int current_val = seg_tree.get_pos(ry);
            seg_tree.update(ry, max(current_val, dp_values[j]));
            down_dp[group[j].second] = dp_values[j];
        }
    }
}

void solve() {
    compress_coordinates();
    compute_up_dp();
    compute_down_dp();

    int max_path_length = 0;
    for(int i = 1; i <= N; i++) {
        max_path_length = max(max_path_length, up_dp[i] + down_dp[i] - 1);
    }

    vector<int> critical_points;
    for(int i = 1; i <= N; i++) {
        if(up_dp[i] + down_dp[i] - 1 == max_path_length) {
            critical_points.push_back(i);
        }
    }

    sort(critical_points.begin(), critical_points.end());
    cout << critical_points.size();
    for(int point: critical_points) {
        cout << ' ' << point;
    }
    cout << '\n';

    map<int, vector<int>> groups_by_up;
    for(int id: critical_points) {
        groups_by_up[up_dp[id]].push_back(id);
    }

    vector<int> unique_critical_points;
    for(auto& p: groups_by_up) {
        if(p.second.size() == 1) {
            unique_critical_points.push_back(p.second[0]);
        }
    }

    sort(unique_critical_points.begin(), unique_critical_points.end());
    cout << unique_critical_points.size();
    for(int point: unique_critical_points) {
        cout << ' ' << point;
    }
    cout << '\n';
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T = 1;
    for(int test = 1; test <= T; test++) {
        read();
        solve();
    }

    return 0;
}
