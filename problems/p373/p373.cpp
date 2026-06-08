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

void read() {}

void solve() {
    // There are N <= 4 cuts, meaning that there would be at most O(N^2) pizza
    // slices corresponding to faces in the "planar" graph. We build a DCEL of
    // the arrangement: vertices are line-line and line-circle intersections,
    // edges are either straight chord pieces or circular arcs between
    // consecutive vertices on the circle. Each edge becomes two twin
    // half-edges; at every vertex we sort the outgoing half-edges CCW by
    // tangent angle and set next(h) = the outgoing half-edge immediately
    // clockwise from twin(h). Walking next-pointers yields every face exactly
    // once; areas come from Green's theorem over the boundary (straight
    // line pieces are direct, (1/2) r^2 dtheta for arcs). The unbounded
    // outer face shows up with negative area and gets dropped.
    //
    // For the game itself greedy is NOT optimal: picking a large piece keeps
    // you busy and lets the opponent grab several small pieces in a row. E.g.
    // with pieces {6,5,5,5,5}, Carlsson gets 11 by taking 6 first but 15 by
    // taking a 5 first (letting Pooh commit to the slow 6). So we run a real
    // minimax with alpha-beta, where the next mover is whoever currently has
    // the smaller cumulative eating time (ties go to Carlsson).

    int n;
    double r;
    cin >> n >> r;
    vector<array<double, 3>> lines(n);
    for(auto& l: lines) {
        cin >> l[0] >> l[1] >> l[2];
    }

    auto line_circle_pts = [&](int i) -> array<Point, 2> {
        double a = lines[i][0], b = lines[i][1], c = lines[i][2];
        double n2 = a * a + b * b;
        Point foot(-a * c / n2, -b * c / n2);
        double h2 = r * r - c * c / n2;
        double h = sqrt(max(0.0, h2));
        Point dir = Point(-b, a) / sqrt(n2);
        return {foot + dir * h, foot - dir * h};
    };

    auto line_line_pt = [&](int i, int j) -> optional<Point> {
        double a1 = lines[i][0], b1 = lines[i][1], c1 = lines[i][2];
        double a2 = lines[j][0], b2 = lines[j][1], c2 = lines[j][2];
        double d = a1 * b2 - a2 * b1;
        if(fabs(d) < Point::eps) {
            return nullopt;
        }
        Point p((-c1 * b2 + b1 * c2) / d, (-a1 * c2 + a2 * c1) / d);
        if(p.norm() > r + Point::eps) {
            return nullopt;
        }
        return p;
    };

    vector<Point> verts;
    auto add_vertex = [&](Point p) {
        for(int i = 0; i < (int)verts.size(); i++) {
            if((verts[i] - p).norm() < 1e-7) {
                return i;
            }
        }
        verts.push_back(p);
        return (int)verts.size() - 1;
    };

    struct HalfEdge {
        int from, to;
        bool is_arc;
        bool ccw;
        int twin;
        int next;
    };
    vector<HalfEdge> hes;
    auto add_edge = [&](int u, int v, bool is_arc, bool ccw_uv) {
        int h1 = (int)hes.size();
        hes.push_back({u, v, is_arc, ccw_uv, h1 + 1, -1});
        hes.push_back({v, u, is_arc, !ccw_uv, h1, -1});
    };

    for(int i = 0; i < n; i++) {
        vector<int> on_line;
        auto cp = line_circle_pts(i);
        on_line.push_back(add_vertex(cp[0]));
        on_line.push_back(add_vertex(cp[1]));
        for(int j = 0; j < n; j++) {
            if(j == i) {
                continue;
            }
            auto pt = line_line_pt(i, j);
            if(pt) {
                on_line.push_back(add_vertex(*pt));
            }
        }
        double a = lines[i][0], b = lines[i][1];
        sort(on_line.begin(), on_line.end(), [&](int u, int v) {
            return -b * verts[u].x + a * verts[u].y <
                   -b * verts[v].x + a * verts[v].y;
        });
        on_line.erase(unique(on_line.begin(), on_line.end()), on_line.end());
        for(int k = 0; k + 1 < (int)on_line.size(); k++) {
            add_edge(on_line[k], on_line[k + 1], false, false);
        }
    }

    vector<int> circle_verts;
    for(int i = 0; i < n; i++) {
        auto cp = line_circle_pts(i);
        circle_verts.push_back(add_vertex(cp[0]));
        circle_verts.push_back(add_vertex(cp[1]));
    }
    auto angle_of = [&](int v) {
        double a = atan2(verts[v].y, verts[v].x);
        if(a < 0) {
            a += 2 * Point::PI;
        }
        return a;
    };
    sort(circle_verts.begin(), circle_verts.end(), [&](int u, int v) {
        return angle_of(u) < angle_of(v);
    });
    circle_verts.erase(
        unique(circle_verts.begin(), circle_verts.end()), circle_verts.end()
    );
    int cm = (int)circle_verts.size();
    for(int k = 0; k < cm; k++) {
        add_edge(circle_verts[k], circle_verts[(k + 1) % cm], true, true);
    }

    auto outgoing_dir = [&](int h) -> Point {
        Point p = verts[hes[h].from];
        if(!hes[h].is_arc) {
            return verts[hes[h].to] - p;
        }
        return hes[h].ccw ? Point(-p.y, p.x) : Point(p.y, -p.x);
    };

    int vtotal = (int)verts.size();
    vector<vector<int>> out_at(vtotal);
    for(int h = 0; h < (int)hes.size(); h++) {
        out_at[hes[h].from].push_back(h);
    }
    for(auto& lst: out_at) {
        sort(lst.begin(), lst.end(), [&](int a, int b) {
            Point da = outgoing_dir(a);
            Point db = outgoing_dir(b);
            return atan2(da.y, da.x) < atan2(db.y, db.x);
        });
    }
    vector<int> pos_of(hes.size());
    for(int v = 0; v < vtotal; v++) {
        for(int k = 0; k < (int)out_at[v].size(); k++) {
            pos_of[out_at[v][k]] = k;
        }
    }
    for(int h = 0; h < (int)hes.size(); h++) {
        int t = hes[h].twin;
        int v = hes[t].from;
        int k = (int)out_at[v].size();
        hes[h].next = out_at[v][(pos_of[t] - 1 + k) % k];
    }

    vector<bool> visited(hes.size(), false);
    vector<double> areas;
    for(int h0 = 0; h0 < (int)hes.size(); h0++) {
        if(visited[h0]) {
            continue;
        }
        double area2 = 0;
        int cur = h0;
        while(!visited[cur]) {
            visited[cur] = true;
            Point pf = verts[hes[cur].from];
            Point pt = verts[hes[cur].to];
            if(!hes[cur].is_arc) {
                area2 += pf.x * pt.y - pt.x * pf.y;
            } else {
                double th1 = atan2(pf.y, pf.x);
                double th2 = atan2(pt.y, pt.x);
                double dth = th2 - th1;
                if(hes[cur].ccw) {
                    if(dth <= 1e-12) {
                        dth += 2 * Point::PI;
                    }
                } else {
                    if(dth >= -1e-12) {
                        dth -= 2 * Point::PI;
                    }
                }
                area2 += r * r * dth;
            }
            cur = hes[cur].next;
        }
        double a = area2 / 2.0;
        if(a > 1e-9) {
            areas.push_back(a);
        }
    }

    int p = (int)areas.size();
    function<double(int, double, double, double, double, double)>
        best_carlsson = [&](int mask, double tc, double tp, double csf,
                            double alpha, double beta) -> double {
        if(mask == (1 << p) - 1) {
            return csf;
        }
        bool c_turn = (tc <= tp + Point::eps);
        if(c_turn) {
            double best = -1e18;
            for(int i = 0; i < p; i++) {
                if(mask & (1 << i)) {
                    continue;
                }
                double v = best_carlsson(
                    mask | (1 << i), tc + areas[i], tp, csf + areas[i], alpha,
                    beta
                );
                best = max(best, v);
                alpha = max(alpha, best);
                if(alpha >= beta) {
                    break;
                }
            }
            return best;
        } else {
            double best = 1e18;
            for(int i = 0; i < p; i++) {
                if(mask & (1 << i)) {
                    continue;
                }
                double v = best_carlsson(
                    mask | (1 << i), tc, tp + areas[i], csf, alpha, beta
                );
                best = min(best, v);
                beta = min(beta, best);
                if(alpha >= beta) {
                    break;
                }
            }
            return best;
        }
    };

    double total = 0;
    for(double a: areas) {
        total += a;
    }
    double carlsson = best_carlsson(0, 0, 0, 0, -1e18, 1e18);
    double pooh = total - carlsson;
    cout << fixed << setprecision(10) << carlsson << ' ' << pooh << '\n';
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
