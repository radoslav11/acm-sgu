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

const coord_t INF = 1e9 + 9;

int n;
vector<Point> poly;
vector<Point> board;

Point now;
vector<Point> shadow_poly;

bool polar_less(const Point& a, const Point& b) {
    return (a.y - now.y) * (b.x - now.x) < (a.x - now.x) * (b.y - now.y);
}

coord_t compute_shadow_area(coord_t px, coord_t py) {
    now = Point(px, py);
    shadow_poly.clear();
    int min_idx = 0, max_idx = 0;
    for(int i = 0; i < n; i++) {
        if(polar_less(poly[i], poly[min_idx])) {
            min_idx = i;
        }
        if(polar_less(poly[max_idx], poly[i])) {
            max_idx = i;
        }
    }
    shadow_poly.push_back(poly[max_idx]);
    for(int j = (max_idx + 1) % n; j != min_idx; j = (j + 1) % n) {
        shadow_poly.push_back(poly[j]);
    }
    shadow_poly.push_back(poly[min_idx]);
    int start_i = -1;
    for(int i = 0; i < 4; i++) {
        Point a = board[i];
        Point b = board[(i + 1) % 4];
        Point thru = poly[min_idx];
        auto temp = intersect_ray_segment(now, thru, a, b);
        if(temp) {
            shadow_poly.push_back(*temp);
            start_i = i;
            break;
        }
        coord_t cross_val =
            (a.x - now.x) * (thru.y - now.y) - (a.y - now.y) * (thru.x - now.x);
        coord_t dist = fabs(a.x - now.x) + fabs(a.y - now.y);
        if(fabs(cross_val) < Point::eps && dist > Point::eps) {
            shadow_poly.push_back(a);
            start_i = i;
            break;
        }
    }
    if(start_i == -1) {
        return 0;
    }
    for(int j = start_i; j < start_i + 4; j++) {
        int idx = j % 4;
        if(j != start_i) {
            shadow_poly.push_back(board[idx]);
        }
        Point thru = poly[max_idx];
        Point a = board[idx];
        Point b = board[(idx + 1) % 4];
        auto temp = intersect_ray_segment(now, thru, a, b);
        if(temp) {
            shadow_poly.push_back(*temp);
            break;
        }
        coord_t cross_val = (board[idx].x - now.x) * (thru.y - now.y) -
                            (board[idx].y - now.y) * (thru.x - now.x);
        coord_t dist = fabs(board[idx].x - now.x) + fabs(board[idx].y - now.y);
        if(fabs(cross_val) < Point::eps && dist > Point::eps) {
            break;
        }
    }
    coord_t ret = 0;
    size_t cnt = shadow_poly.size();
    for(size_t i = 1; i < cnt - 1; i++) {
        ret += 0.5 * ((shadow_poly[i].x - shadow_poly[0].x) *
                          (shadow_poly[i + 1].y - shadow_poly[0].y) -
                      (shadow_poly[i + 1].x - shadow_poly[0].x) *
                          (shadow_poly[i].y - shadow_poly[0].y));
    }
    return ret;
}

template<typename F>
coord_t adaptive_simpson(
    coord_t a, coord_t b, F f, coord_t tol,
    coord_t int_whole = numeric_limits<coord_t>::quiet_NaN()
) {
    if(isnan(int_whole)) {
        coord_t m = 0.5 * (a + b);
        int_whole = (b - a) / 6.0 * (f(a) + 4 * f(m) + f(b));
    }
    coord_t m = 0.5 * (a + b);
    coord_t fm = f(m);
    coord_t int_l = (m - a) / 6.0 * (f(a) + 4 * f(0.5 * (a + m)) + fm);
    coord_t int_r = (b - m) / 6.0 * (fm + 4 * f(0.5 * (m + b)) + f(b));
    if(fabs(int_l + int_r - int_whole) < tol) {
        return 0.5 * (int_l + int_r + int_whole);
    }
    return adaptive_simpson(a, m, f, tol, int_l) +
           adaptive_simpson(m, b, f, tol, int_r);
}

coord_t integrand_x(coord_t x) {
    coord_t ymin = INF, ymax = -INF;
    for(int i = 0; i < n; i++) {
        Point seg_a = poly[i];
        Point seg_b = poly[(i + 1) % n];
        Point vert_start(x, 0);
        Point vert_through(x, 100);
        auto temp =
            intersect_ray_segment(vert_start, vert_through, seg_a, seg_b);
        if(temp) {
            if(ymin > temp->y) {
                ymin = temp->y;
            }
            if(ymax < temp->y) {
                ymax = temp->y;
            }
        }
        if(fabs(poly[i].x - x) <= Point::eps) {
            if(ymin > poly[i].y) {
                ymin = poly[i].y;
            }
            if(ymax < poly[i].y) {
                ymax = poly[i].y;
            }
        }
    }
    auto fy = [x](coord_t y) { return compute_shadow_area(x, y); };
    if(ymin > ymax) {
        return adaptive_simpson(0, 100, fy, 1e-4);
    }
    coord_t low_contrib = 0;
    coord_t low_b = ymin - Point::eps;
    if(low_b > 0) {
        low_contrib = adaptive_simpson(0, low_b, fy, 1e-4);
    }
    coord_t high_contrib = 0;
    coord_t high_a = ymax + Point::eps;
    if(high_a < 100) {
        high_contrib = adaptive_simpson(high_a, 100, fy, 1e-4);
    }
    return (ymax - ymin) * 10000 + low_contrib + high_contrib;
}

void read() {
    cin >> n;
    poly.resize(n);
    cin >> poly;
    board = {Point(0, 0), Point(100, 0), Point(100, 100), Point(0, 100)};
}

void solve() {
    // This is a somewhat challenging problem in terms of getting it accepted.
    // This solution computes the probability that at least one of two random
    // shots hits the target or their connecting segment intersects it. We
    // integrate over the position of the first shot P=(x,y) the area of winning
    // positions for the second shot Q. The total possible is 100^4 = 1e8, so
    // the probability is the integral divided by 1e8. We use adaptive Simpson's
    // rule for double integration: outer over x=0 to 100, inner over y=0 to
    // 100. For each fixed P, if P is inside the target, winning area is entire
    // board, or 10000. To check if P inside, for each x, find the y-interval
    // [ymin,ymax] where vertical line intersects target. If no intersection, P
    // always outside; else inside for y in [ymin,ymax]. For P outside, the
    // winning Q are: target union the region where segment PQ intersects
    // target, which is the "shadow" cast by the target from P, clipped to
    // board. Since target convex, find the two extremal vertices min_idx,
    // max_idx from P (tangent points). Construct the polygon: the chain from
    // max_idx to min_idx (assuming ccw), then ray from P through min_idx
    // intersects board boundary, then along board ccw to ray through max_idx
    // intersection. Compute area of this constructed polygon using shoelace
    // triangulated from first point. There are a fair bit of details and
    // special handling if rays hit board corners exactly.

    auto integrand = [](coord_t x) { return integrand_x(x); };
    coord_t integral = adaptive_simpson(0, 100, integrand, 1e-1);
    cout << fixed << setprecision(10) << integral / 1e8 << '\n';
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
