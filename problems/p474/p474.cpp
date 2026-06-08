#include <bits/stdc++.h>
// #include <coding_library/geometry/point.hpp>

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

using coord_t = double;

struct Point {
    static constexpr coord_t eps = 1e-9;
    static inline const coord_t PI = acos((coord_t)-1.0);

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

    friend coord_t arc_area(
        const Point& center, coord_t r, const Point& p1, const Point& p2
    ) {
        coord_t theta1 = (p1 - center).angle();
        coord_t theta2 = (p2 - center).angle();
        if(theta2 < theta1 - eps) {
            theta2 += 2 * PI;
        }

        coord_t d_theta = theta2 - theta1;
        coord_t cx = center.x, cy = center.y;
        coord_t area = r * cx * (sin(theta2) - sin(theta1)) -
                       r * cy * (cos(theta2) - cos(theta1)) + r * r * d_theta;
        return area / 2.0;
    }

    friend vector<Point> intersect_circles(
        const Point& c1, coord_t r1, const Point& c2, coord_t r2
    ) {
        Point d = c2 - c1;
        coord_t dist = d.norm();

        if(dist > r1 + r2 + eps || dist < abs(r1 - r2) - eps || dist < eps) {
            return {};
        }

        coord_t a = (r1 * r1 - r2 * r2 + dist * dist) / (2 * dist);
        coord_t h_sq = r1 * r1 - a * a;
        if(h_sq < -eps) {
            return {};
        }
        if(h_sq < 0) {
            h_sq = 0;
        }
        coord_t h = sqrt(h_sq);

        Point mid = c1 + d.unit() * a;
        Point perp_dir = d.perp().unit();

        if(h < eps) {
            return {mid};
        }
        return {mid + perp_dir * h, mid - perp_dir * h};
    }

    friend optional<Point> intersect_ray_segment(
        const Point& ray_start, const Point& ray_through, const Point& seg_a,
        const Point& seg_b
    ) {
        Point ray_dir = ray_through - ray_start;
        if(ray_dir.norm2() < Point::eps) {
            return {};
        }
        Point seg_dir = seg_b - seg_a;
        coord_t denom = ray_dir ^ seg_dir;
        if(fabs(denom) < eps) {
            return {};
        }
        coord_t t = ((seg_a - ray_start) ^ seg_dir) / denom;
        if(t < eps) {
            return {};
        }
        coord_t s = ((seg_a - ray_start) ^ ray_dir) / denom;
        if(s < eps || s > 1 - eps) {
            return {};
        }
        return ray_start + ray_dir * t;
    }
};

namespace fast_in {
static char buf[1 << 22];
static int bp = 0, blen = 0;
static inline int gc() {
    if(bp == blen) {
        blen = (int)fread(buf, 1, sizeof(buf), stdin);
        bp = 0;
        if(blen == 0) {
            return -1;
        }
    }
    return buf[bp++];
}
static inline int ri() {
    int c = gc();
    while(c != -1 && c <= ' ') {
        c = gc();
    }
    int x = 0;
    while(c > ' ') {
        x = x * 10 + c - '0';
        c = gc();
    }
    return x;
}
}  // namespace fast_in

int n, m;
vector<array<Point, 3>> triangles;

int right_angle_index(const array<Point, 3>& t) {
    for(int i = 0; i < 3; i++) {
        int j = (i + 1) % 3, k = (i + 2) % 3;
        Point u = t[j] - t[i];
        Point v = t[k] - t[i];
        if(u * v == 0 && u.norm2() == v.norm2()) {
            bool axis_aligned =
                (u.x == 0 || u.y == 0) && (v.x == 0 || v.y == 0);
            if(axis_aligned) {
                return i;
            }
        }
    }
    return -1;
}

bool inside_large(const Point& p) {
    return p.x >= 0 && p.y >= 0 && p.x + p.y <= n;
}

void read() {
    n = fast_in::ri();
    m = fast_in::ri();
    triangles.assign(m, {});
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < 3; j++) {
            triangles[i][j].x = fast_in::ri();
            triangles[i][j].y = fast_in::ri();
        }
    }
}

void solve() {
    // For a valid tiling, at every unique vertex V the angles of the
    // triangles touching V must fill the locally-interior region exactly:
    // 360 deg strictly inside the large triangle, 180 deg along a boundary
    // edge, 90 deg at (0, 0), and 45 deg at (n, 0) and (0, n). Per triangle
    // we credit 90 deg at its right-angle vertex and 45 deg at each acute
    // vertex. A vertex that lies strictly in the interior of some other
    // triangle's edge (a T-junction) picks up another 180 deg from that
    // edge passing straight through it. So the entire check reduces to
    // computing this angle sum per unique vertex and matching it against
    // the expected value above.
    //
    // The only nontrivial part is detecting T-junctions efficiently. Edges
    // come in just four direction classes - horizontal, vertical, slope +1,
    // slope -1 - because catheti are axis-aligned and forces hypotenuses
    // onto a +/-45 deg diagonal. For each direction d we group everything
    // by the constant coordinate of the line supporting an edge in that
    // direction, called the "level":
    //
    //   d = 0 (horizontal):   level = y,      pos = x
    //   d = 1 (vertical):     level = x,      pos = y
    //   d = 2 (slope +1):     level = y - x,  pos = x
    //   d = 3 (slope -1):     level = x + y,  pos = x
    //
    // Within one level the problem becomes 1D: each edge is an interval
    // [lo, hi] in pos, each vertex is a point in pos, and we need to
    // know, for every vertex, how many edge intervals strictly contain it.
    //
    // We emit three event kinds, sorted by (level, pos, type):
    //
    //   type 2 - edge opens at its lower endpoint
    //   type 1 - vertex query
    //   type 0 - edge closes at its upper endpoint
    //
    // The tie-break order at equal (level, pos) is close < query < open,
    // so that an edge ending at a position is removed before any vertex
    // sitting on that position is queried, and an edge starting at a
    // position is added only after queries at the same position. Both
    // ensure a vertex coinciding with an edge endpoint is never counted
    // as being in that edge's interior.
    //
    // We sweep through the events one level at a time, maintaining a
    // counter of currently-open intervals. At each vertex query the
    // counter equals the number of edges in direction d whose interior
    // contains the vertex, and we credit 180 deg per such edge. Repeating
    // this for all four directions gives the full T-junction contribution.
    //
    // We also verify each triangle is right isosceles with axis-aligned
    // catheti, sits inside the large triangle, and that the total area
    // equals n^2 / 2.

    vector<int> right_idx(m);
    coord_t total_area_x2 = 0;
    for(int i = 0; i < m; i++) {
        int r = right_angle_index(triangles[i]);
        if(r < 0) {
            cout << "NO\n";
            return;
        }
        right_idx[i] = r;
        for(int j = 0; j < 3; j++) {
            if(!inside_large(triangles[i][j])) {
                cout << "NO\n";
                return;
            }
        }
        total_area_x2 +=
            abs((triangles[i][1] - triangles[i][0]) ^
                (triangles[i][2] - triangles[i][0]));
    }

    if(total_area_x2 != (coord_t)n * n) {
        cout << "NO\n";
        return;
    }

    vector<array<int, 6>> tri_int(m);
    vector<pair<int, int>> all_pts;
    all_pts.reserve(3 * m);
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < 3; j++) {
            int x = (int)triangles[i][j].x, y = (int)triangles[i][j].y;
            tri_int[i][2 * j] = x;
            tri_int[i][2 * j + 1] = y;
            all_pts.push_back({x, y});
        }
    }
    sort(all_pts.begin(), all_pts.end());
    all_pts.erase(unique(all_pts.begin(), all_pts.end()), all_pts.end());
    int V = (int)all_pts.size();

    vector<array<int, 3>> tri_vid(m);
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < 3; j++) {
            pair<int, int> key = {tri_int[i][2 * j], tri_int[i][2 * j + 1]};
            tri_vid[i][j] =
                (int)(lower_bound(all_pts.begin(), all_pts.end(), key) -
                      all_pts.begin());
        }
    }

    vector<int64_t> angle_at(V, 0);
    for(int i = 0; i < m; i++) {
        int r = right_idx[i];
        angle_at[tri_vid[i][r]] += 90;
        angle_at[tri_vid[i][(r + 1) % 3]] += 45;
        angle_at[tri_vid[i][(r + 2) % 3]] += 45;
    }

    // Packed event key, sortable as one uint64. Layout (high to low bits):
    //   level (signed, shifted): 18 bits  -> [-65536, 65535] range
    //   pos:                     17 bits  -> [0, 131071]
    //   type:                     2 bits  -> 0 = close, 1 = query, 2 = open
    //   vid + 1:                 20 bits  -> 0 means "no vid"
    auto pack = [](int level, int pos, int type, int vid) -> uint64_t {
        uint64_t lv = (uint64_t)(level + (1 << 17));
        return (lv << 39) | ((uint64_t)pos << 22) | ((uint64_t)type << 20) |
               (uint64_t)(vid + 1);
    };

    struct DirInfo {
        int level, pos;
    };
    auto level_pos = [](int d, int x, int y) -> DirInfo {
        if(d == 0) {
            return {y, x};
        }
        if(d == 1) {
            return {x, y};
        }
        if(d == 2) {
            return {y - x, x};
        }
        return {x + y, x};
    };

    auto edge_direction = [](int dx, int dy) -> int {
        if(dy == 0) {
            return 0;
        }
        if(dx == 0) {
            return 1;
        }
        if(dx == dy) {
            return 2;
        }
        return 3;
    };

    vector<uint64_t> events;
    events.reserve(6 * m + V);
    for(int d = 0; d < 4; d++) {
        events.clear();
        for(int i = 0; i < m; i++) {
            for(int j = 0; j < 3; j++) {
                int ax = tri_int[i][2 * j], ay = tri_int[i][2 * j + 1];
                int bx = tri_int[i][2 * ((j + 1) % 3)],
                    by = tri_int[i][2 * ((j + 1) % 3) + 1];
                if(edge_direction(bx - ax, by - ay) != d) {
                    continue;
                }
                DirInfo a = level_pos(d, ax, ay), b = level_pos(d, bx, by);
                int lo = min(a.pos, b.pos), hi = max(a.pos, b.pos);
                events.push_back(pack(a.level, lo, 2, -1));
                events.push_back(pack(a.level, hi, 0, -1));
            }
        }
        for(int v = 0; v < V; v++) {
            DirInfo lp = level_pos(d, all_pts[v].first, all_pts[v].second);
            events.push_back(pack(lp.level, lp.pos, 1, v));
        }

        sort(events.begin(), events.end());

        int prev_level = INT_MIN;
        int active = 0;
        for(uint64_t e: events) {
            int level = (int)(e >> 39) - (1 << 17);
            int type = (int)((e >> 20) & 3);
            int vid_plus_one = (int)(e & ((1u << 20) - 1));
            if(level != prev_level) {
                prev_level = level;
                active = 0;
            }
            if(type == 0) {
                active--;
            } else if(type == 1) {
                angle_at[vid_plus_one - 1] += 180LL * active;
            } else {
                active++;
            }
        }
    }

    for(int v = 0; v < V; v++) {
        int x = all_pts[v].first, y = all_pts[v].second;
        int64_t a = angle_at[v];
        int64_t expected;
        if(x == 0 && y == 0) {
            expected = 90;
        } else if((x == n && y == 0) || (x == 0 && y == n)) {
            expected = 45;
        } else if(x == 0 || y == 0 || x + y == n) {
            expected = 180;
        } else {
            expected = 360;
        }
        if(a != expected) {
            cout << "NO\n";
            return;
        }
    }

    cout << "YES\n";
}

int main() {
    int T = fast_in::ri();
    for(int test = 1; test <= T; test++) {
        read();
        solve();
    }

    return 0;
}
