#include <bits/stdc++.h>
// #include <coding_library/geometry/point.hpp>
// #include <coding_library/geometry/voronoi.hpp>
// #include <coding_library/graph/dsu.hpp>

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

using coord_t = long double;

struct Point {
    static constexpr coord_t eps = 1e-12;

    coord_t x, y;
    Point(coord_t x = 0, coord_t y = 0) : x(x), y(y) {}

    Point operator+(const Point& p) const { return Point(x + p.x, y + p.y); }
    Point operator-(const Point& p) const { return Point(x - p.x, y - p.y); }
    Point operator*(coord_t c) const { return Point(x * c, y * c); }
    Point operator/(coord_t c) const { return Point(x / c, y / c); }

    coord_t operator*(const Point& p) const { return x * p.x + y * p.y; }
    coord_t operator^(const Point& p) const { return x * p.y - y * p.x; }

    bool operator==(const Point& p) const { return x == p.x && y == p.y; }
    bool operator!=(const Point& p) const { return x != p.x || y != p.y; }
    bool operator<(const Point& p) const {
        return x != p.x ? x < p.x : y < p.y;
    }
    bool operator>(const Point& p) const {
        return x != p.x ? x > p.x : y > p.y;
    }
    bool operator<=(const Point& p) const {
        return x != p.x ? x < p.x : y <= p.y;
    }
    bool operator>=(const Point& p) const {
        return x != p.x ? x > p.x : y >= p.y;
    }

    coord_t norm2() const { return x * x + y * y; }
    coord_t norm() const { return sqrt(norm2()); }
    coord_t angle() const { return atan2(y, x); }

    Point rotate(coord_t a) const {
        return Point(x * cos(a) - y * sin(a), x * sin(a) + y * cos(a));
    }

    Point perp() const { return Point(-y, x); }
    Point unit() const { return *this / norm(); }
    Point normal() const { return perp().unit(); }
    Point project(const Point& p) const {
        return *this * (*this * p) / norm2();
    }
    Point reflect(const Point& p) const {
        return *this * 2 * (*this * p) / norm2() - p;
    }

    friend ostream& operator<<(ostream& os, const Point& p) {
        return os << p.x << ' ' << p.y;
    }
    friend istream& operator>>(istream& is, Point& p) {
        return is >> p.x >> p.y;
    }

    friend int ccw(const Point& a, const Point& b, const Point& c) {
        coord_t v = (b - a) ^ (c - a);
        if(-eps <= v && v <= eps) {
            return 0;
        } else if(v > 0) {
            return 1;
        } else {
            return -1;
        }
    }

    friend bool point_on_segment(
        const Point& a, const Point& b, const Point& p
    ) {
        return ccw(a, b, p) == 0 && p.x >= min(a.x, b.x) - eps &&
               p.x <= max(a.x, b.x) + eps && p.y >= min(a.y, b.y) - eps &&
               p.y <= max(a.y, b.y) + eps;
    }

    friend bool point_in_triangle(
        const Point& a, const Point& b, const Point& c, const Point& p
    ) {
        int d1 = ccw(a, b, p);
        int d2 = ccw(b, c, p);
        int d3 = ccw(c, a, p);
        return (d1 >= 0 && d2 >= 0 && d3 >= 0) ||
               (d1 <= 0 && d2 <= 0 && d3 <= 0);
    }

    friend Point line_line_intersection(
        const Point& a1, const Point& b1, const Point& a2, const Point& b2
    ) {
        return a1 +
               (b1 - a1) * ((a2 - a1) ^ (b2 - a2)) / ((b1 - a1) ^ (b2 - a2));
    }

    friend bool collinear(const Point& a, const Point& b) {
        return abs(a ^ b) < eps;
    }

    friend Point circumcenter(const Point& a, const Point& b, const Point& c) {
        Point mid_ab = (a + b) / 2.0;
        Point mid_ac = (a + c) / 2.0;
        Point perp_ab = (b - a).perp();
        Point perp_ac = (c - a).perp();
        return line_line_intersection(
            mid_ab, mid_ab + perp_ab, mid_ac, mid_ac + perp_ac
        );
    }
};

class VoronoiDiagram {
  private:
    static constexpr coord_t INF = 1e100;
    static inline coord_t sweep_x;

    struct Arc {
        mutable Point p, q;
        mutable int id = 0, i;

        Arc(const Point& p, const Point& q, int i) : p(p), q(q), i(i) {}

        coord_t get_y(coord_t x) const {
            if(q.y == INF) {
                return INF;
            }
            x += Point::eps;
            Point mid = (p + q) / 2.0;
            Point dir = (p - mid).perp();
            coord_t D = (x - p.x) * (x - q.x);
            if(D < 0) {
                return -INF;
            }

            if(abs(dir.y) < Point::eps) {
                return (x < mid.x) ? -INF : INF;
            }

            return mid.y +
                   ((mid.x - x) * dir.x + sqrtl(D) * dir.norm()) / dir.y;
        }

        bool operator<(const coord_t& y) const { return get_y(sweep_x) < y; }

        bool operator<(const Arc& o) const {
            return get_y(sweep_x) < o.get_y(sweep_x);
        }
    };

    using Beach = multiset<Arc, less<>>;

    struct Event {
        coord_t x;
        int id;
        Beach::iterator it;

        Event(coord_t x, int id, Beach::iterator it) : x(x), id(id), it(it) {}

        bool operator<(const Event& e) const { return x > e.x; }
    };

    Beach beach_line;
    vector<pair<Point, int>> vertices;
    priority_queue<Event> event_queue;
    vector<pair<int, int>> edges;
    vector<bool> valid;
    int n, next_vertex_id;

    void update_vertex_event(Beach::iterator it) {
        if(it->i == -1) {
            return;
        }

        valid[-it->id] = false;
        auto prev_it = prev(it);

        if(collinear(it->q - it->p, prev_it->p - it->p)) {
            return;
        }

        it->id = --next_vertex_id;
        valid.push_back(true);

        Point center = circumcenter(it->p, it->q, prev_it->p);
        coord_t event_x = center.x + (center - it->p).norm();

        bool valid_event =
            event_x > sweep_x - Point::eps &&
            prev_it->get_y(event_x) + Point::eps > it->get_y(event_x);
        if(valid_event) {
            event_queue.push(Event(event_x, it->id, it));
        }
    }

    void add_edge(int i, int j) {
        if(i == -1 || j == -1) {
            return;
        }
        edges.push_back({vertices[i].second, vertices[j].second});
    }

    void add_point(int i) {
        Point p = vertices[i].first;

        auto split_it = beach_line.lower_bound(p.y);
        auto new_it = beach_line.insert(split_it, Arc(p, split_it->p, i));
        auto prev_it =
            beach_line.insert(new_it, Arc(split_it->p, p, split_it->i));

        add_edge(i, split_it->i);

        update_vertex_event(prev_it);
        update_vertex_event(new_it);
        update_vertex_event(split_it);
    }

    void remove_arc(Beach::iterator it) {
        auto prev_it = prev(it);
        auto next_it = next(it);

        beach_line.erase(it);
        prev_it->q = next_it->p;

        add_edge(prev_it->i, next_it->i);

        update_vertex_event(prev_it);
        update_vertex_event(next_it);
    }

  public:
    VoronoiDiagram(const vector<Point>& points, bool fix_coordinates = true) {
        n = points.size();
        vertices.resize(n);

        for(int i = 0; i < n; i++) {
            vertices[i] = {points[i], i};
        }

        if(fix_coordinates && n > 0) {
            // Rotate around center by 1.0 radians.
            for(int i = 0; i < n; i++) {
                vertices[i].first = vertices[i].first.rotate(1.0);
            }
        }

        sort(vertices.begin(), vertices.end());
    }

    vector<pair<int, int>> compute(coord_t X = 1e9) {
        edges.clear();
        beach_line.clear();
        event_queue = priority_queue<Event>();

        X *= 3;
        beach_line.insert(Arc(Point(-X, -X), Point(-X, X), -1));
        beach_line.insert(Arc(Point(-X, X), Point(INF, INF), -1));

        for(int i = 0; i < n; i++) {
            event_queue.push(Event(vertices[i].first.x, i, beach_line.end()));
        }

        next_vertex_id = 0;
        valid.assign(1, false);

        while(!event_queue.empty()) {
            Event e = event_queue.top();
            event_queue.pop();
            sweep_x = e.x;

            if(e.id >= 0) {
                add_point(e.id);
            } else if(valid[-e.id]) {
                remove_arc(e.it);
            }
        }

        return edges;
    }

    const vector<pair<int, int>>& get_edges() const { return edges; }
};

class DSU {
  public:
    int n;
    vector<int> par;
    vector<int> sz;

    DSU(int _n = 0) { init(_n); }

    void init(int _n) {
        n = _n;
        par.assign(n + 1, 0);
        sz.assign(n + 1, 0);
        for(int i = 0; i <= n; i++) {
            par[i] = i;
            sz[i] = 1;
        }
    }

    int root(int u) { return par[u] = ((u == par[u]) ? u : root(par[u])); }
    bool connected(int x, int y) { return root(x) == root(y); }

    int unite(int x, int y) {
        x = root(x), y = root(y);
        if(x == y) {
            return x;
        }
        if(sz[x] > sz[y]) {
            swap(x, y);
        }
        par[x] = y;
        sz[y] += sz[x];
        return y;
    }

    vector<vector<int>> components() {
        vector<vector<int>> comp(n + 1);
        for(int i = 0; i <= n; i++) {
            comp[root(i)].push_back(i);
        }
        return comp;
    }
};

int n, q;
vector<Point> pnts;
vector<pair<int, int>> queries;

void read() {
    cin >> n;
    pnts.resize(n);
    for(auto& p: pnts) {
        cin >> p.x >> p.y;
    }

    cin >> q;
    queries.resize(q);
    for(auto& query: queries) {
        cin >> query;
        query.first--;
        query.second--;
    }
}

void solve() {
    // The key to this problem is that we need to build the Euclidean spanning
    // tree, and then the problem ends up being computing the largest edge
    // between the two endpoints. This can be done in different ways, easiest
    // probably being parallel binary search as part of the Kruskal which we
    // anyways need to do. One corner case that isn't well described in the
    // problem statement is s[i] = t[i], where the answer should be 0.0.

    auto voronoi = VoronoiDiagram(pnts);
    auto edges = voronoi.compute();

    sort(
        edges.begin(), edges.end(),
        [&](pair<int, int> edge1, pair<int, int> edge2) {
            return (pnts[edge1.first] - pnts[edge1.second]).norm2() <
                   (pnts[edge2.first] - pnts[edge2.second]).norm2();
        }
    );

    vector<int> low(q, 0), high(q, edges.size() - 1);
    while(true) {
        bool has_queries = false;
        vector<vector<int>> queries_at_pos(edges.size());
        for(int i = 0; i < q; i++) {
            if(low[i] <= high[i]) {
                has_queries = true;

                int mid = (low[i] + high[i]) / 2;
                queries_at_pos[mid].push_back(i);
            }
        }

        if(!has_queries) {
            break;
        }

        DSU dsu(n);
        for(int i = 0; i < (int)edges.size(); i++) {
            dsu.unite(edges[i].first, edges[i].second);

            for(int query: queries_at_pos[i]) {
                if(dsu.connected(queries[query].first, queries[query].second)) {
                    high[query] = i - 1;
                } else {
                    low[query] = i + 1;
                }
            }
        }
    }

    for(int i = 0; i < q; i++) {
        if(queries[i].first == queries[i].second) {
            cout << setprecision(10) << fixed << 0.0 << endl;
        } else {
            auto edge = edges[high[i] + 1];
            cout << setprecision(10) << fixed
                 << (pnts[edge.first] - pnts[edge.second]).norm() << endl;
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
