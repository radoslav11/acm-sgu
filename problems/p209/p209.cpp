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

struct Rational {
    int64_t n, d;
    Rational(int64_t nn = 0, int64_t dd = 1) : n(nn), d(dd) { normalize(); }
    void normalize() {
        if(d < 0) {
            n = -n;
            d = -d;
        }
        int64_t g = gcd(abs(n), abs(d));
        n /= g;
        d /= g;
    }
    bool operator==(const Rational& o) const { return n == o.n && d == o.d; }
    bool operator<(const Rational& o) const { return n * o.d < o.n * d; }
    Rational operator+(const Rational& o) const {
        return Rational(n * o.d + o.n * d, d * o.d);
    }
    Rational operator-(const Rational& o) const {
        return Rational(n * o.d - o.n * d, d * o.d);
    }
    Rational operator*(const Rational& o) const {
        return Rational(n * o.n, d * o.d);
    }
    Rational operator/(const Rational& o) const {
        return Rational(n * o.d, d * o.n);
    }
};

struct Point {
    Rational x, y;
    Point(Rational xx = 0, Rational yy = 0) : x(xx), y(yy) {}
    bool operator==(const Point& o) const { return x == o.x && y == o.y; }
    bool operator<(const Point& o) const {
        if(!(x == o.x)) {
            return x < o.x;
        }
        return y < o.y;
    }
};

struct Line {
    int64_t x1, y1, x2, y2;
};

int N;
vector<Line> lines;

void read() {
    cin >> N;
    lines.resize(N);
    for(int i = 0; i < N; i++) {
        cin >> lines[i].x1 >> lines[i].y1 >> lines[i].x2 >> lines[i].y2;
    }
}

void solve() {
    set<Point> all_vertices;
    for(int i = 0; i < N; i++) {
        for(int j = i + 1; j < N; j++) {
            int64_t dx1 = lines[i].x2 - lines[i].x1;
            int64_t dy1 = lines[i].y2 - lines[i].y1;
            int64_t dx2 = lines[j].x2 - lines[j].x1;
            int64_t dy2 = lines[j].y2 - lines[j].y1;
            int64_t den = dx1 * dy2 - dy1 * dx2;
            if(den == 0) {
                continue;
            }
            int64_t num_t = (lines[j].x1 - lines[i].x1) * dy2 -
                            (lines[j].y1 - lines[i].y1) * dx2;
            int64_t x_num = lines[i].x1 * den + num_t * dx1;
            int64_t y_num = lines[i].y1 * den + num_t * dy1;
            Point p(Rational(x_num, den), Rational(y_num, den));
            all_vertices.insert(p);
        }
    }
    vector<Point> verts(all_vertices.begin(), all_vertices.end());
    int n = verts.size();
    vector<vector<int>> adj(n);
    map<Point, int> point_to_id;
    for(int i = 0; i < n; i++) {
        point_to_id[verts[i]] = i;
    }
    for(int i = 0; i < N; i++) {
        set<Point> on_line;
        for(int j = 0; j < N; j++) {
            if(i == j) {
                continue;
            }
            int64_t dx1 = lines[i].x2 - lines[i].x1;
            int64_t dy1 = lines[i].y2 - lines[i].y1;
            int64_t dx2 = lines[j].x2 - lines[j].x1;
            int64_t dy2 = lines[j].y2 - lines[j].y1;
            int64_t den = dx1 * dy2 - dy1 * dx2;
            if(den == 0) {
                continue;
            }
            int64_t num_t = (lines[j].x1 - lines[i].x1) * dy2 -
                            (lines[j].y1 - lines[i].y1) * dx2;
            int64_t x_num = lines[i].x1 * den + num_t * dx1;
            int64_t y_num = lines[i].y1 * den + num_t * dy1;
            Point p(Rational(x_num, den), Rational(y_num, den));
            on_line.insert(p);
        }
        vector<Point> pts(on_line.begin(), on_line.end());
        for(size_t k = 0; k + 1 < pts.size(); k++) {
            int a = point_to_id[pts[k]];
            int b = point_to_id[pts[k + 1]];
            adj[a].push_back(b);
            adj[b].push_back(a);
        }
    }
    vector<vector<int>> sorted_neighbors(n);
    for(int i = 0; i < n; i++) {
        vector<int> nb = adj[i];
        sort(nb.begin(), nb.end(), [&](int a, int b) {
            Rational dx1 = verts[a].x - verts[i].x;
            Rational dy1 = verts[a].y - verts[i].y;
            double xx1 = (double)dx1.n / dx1.d;
            double yy1 = (double)dy1.n / dy1.d;
            double ang1 = atan2(yy1, xx1);
            Rational dx2 = verts[b].x - verts[i].x;
            Rational dy2 = verts[b].y - verts[i].y;
            double xx2 = (double)dx2.n / dx2.d;
            double yy2 = (double)dy2.n / dy2.d;
            double ang2 = atan2(yy2, xx2);
            return ang1 < ang2;
        });
        sorted_neighbors[i] = nb;
    }
    set<pair<int, int>> used_directed;
    vector<double> areas;
    for(int i = 0; i < n; i++) {
        for(int j: adj[i]) {
            if(used_directed.count({i, j})) {
                continue;
            }
            vector<int> cycle;
            int curr_u = i;
            int curr_v = j;
            used_directed.insert({curr_u, curr_v});
            cycle.push_back(curr_v);
            while(true) {
                int deg = sorted_neighbors[curr_v].size();
                int k = -1;
                for(int s = 0; s < deg; s++) {
                    if(sorted_neighbors[curr_v][s] == curr_u) {
                        k = s;
                        break;
                    }
                }
                int next_k = (k - 1 + deg) % deg;
                int next_v = sorted_neighbors[curr_v][next_k];
                used_directed.insert({curr_v, next_v});
                curr_u = curr_v;
                curr_v = next_v;
                cycle.push_back(curr_v);
                if(curr_v == j) {
                    break;
                }
            }
            cycle.pop_back();
            int m = cycle.size();
            if(m < 3) {
                continue;
            }
            double signed_area = 0.0;
            for(int kk = 0; kk < m; kk++) {
                int aa = cycle[kk];
                int bb = cycle[(kk + 1) % m];
                double x1 = (double)verts[aa].x.n / verts[aa].x.d;
                double y1 = (double)verts[aa].y.n / verts[aa].y.d;
                double x2 = (double)verts[bb].x.n / verts[bb].x.d;
                double y2 = (double)verts[bb].y.n / verts[bb].y.d;
                signed_area += x1 * y2 - x2 * y1;
            }
            if(signed_area > 1e-8) {
                areas.push_back(signed_area / 2.0);
            }
        }
    }
    sort(areas.begin(), areas.end());
    cout << areas.size() << '\n';
    cout << fixed << setprecision(4);
    for(double ar: areas) {
        cout << ar << '\n';
    }
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
