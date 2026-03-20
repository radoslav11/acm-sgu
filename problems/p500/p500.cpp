#include <bits/stdc++.h>
// #include <coding_library/geometry/point.hpp>
// #include <coding_library/geometry/polygon.hpp>

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

class Polygon {
  public:
    vector<Point> points;

    Polygon() {}
    Polygon(const vector<Point>& points) : points(points) {}

    int size() const { return points.size(); }
    const Point& operator[](int i) const { return points[i]; }
    Point& operator[](int i) { return points[i]; }

    coord_t area() const {
        coord_t a = 0;
        for(int i = 0; i < size(); i++) {
            a += points[i] ^ points[(i + 1) % size()];
        }
        return a / 2.0;
    }
};

class PointInConvexPolygon {
  private:
    Point min_point;
    vector<Point> points_by_angle;

    void prepare() {
        points_by_angle = polygon.points;
        vector<Point>::iterator min_point_it =
            min_element(points_by_angle.begin(), points_by_angle.end());
        min_point = *min_point_it;

        points_by_angle.erase(min_point_it);
        sort(
            points_by_angle.begin(), points_by_angle.end(),
            [&](const Point& a, const Point& b) {
                int d = ccw(min_point, a, b);
                if(d != 0) {
                    return d > 0;
                }
                return (a - min_point).norm2() < (b - min_point).norm2();
            }
        );
    }

  public:
    Polygon polygon;
    PointInConvexPolygon(const Polygon& polygon) : polygon(polygon) {
        prepare();
    }

    bool contains(const Point& p) const {
        int l = 0, r = (int)points_by_angle.size() - 1;
        while(r - l > 1) {
            int m = (l + r) / 2;
            if(ccw(min_point, points_by_angle[m], p) >= 0) {
                l = m;
            } else {
                r = m;
            }
        }

        return point_in_triangle(
            min_point, points_by_angle[l], points_by_angle[r], p
        );
    }
};

class ConvexHull : public Polygon {
  public:
    int lower_end;
    vector<Point> lower, upper;

    ConvexHull(const vector<Point>& points) {
        this->points = points;
        sort(this->points.begin(), this->points.end());
        this->points.erase(
            unique(this->points.begin(), this->points.end()), this->points.end()
        );

        if(this->points.size() <= 2) {
            lower_end = (int)this->points.size() - 1;
            lower = this->points;
            upper = {this->points.back()};
            if(this->points.size() > 1) {
                upper.push_back(this->points.front());
            }
            return;
        }

        vector<int> hull = {0};
        vector<bool> used(this->points.size());

        function<void(int, int)> expand_hull = [&](int i, int min_hull_size) {
            while((int)hull.size() >= min_hull_size &&
                  ccw(this->points[hull[hull.size() - 2]],
                      this->points[hull.back()], this->points[i]) >= 0) {
                used[hull.back()] = false;
                hull.pop_back();
            }
            hull.push_back(i);
            used[i] = true;
        };

        for(int i = 1; i < (int)this->points.size(); i++) {
            expand_hull(i, 2);
        }

        int uhs = hull.size();
        for(int i = (int)this->points.size() - 2; i >= 0; i--) {
            if(!used[i]) {
                expand_hull(i, uhs + 1);
            }
        }

        hull.pop_back();

        vector<Point> pts;
        for(int i: hull) {
            pts.push_back(this->points[i]);
        }
        reverse(pts.begin(), pts.end());
        this->points = std::move(pts);

        lower_end = size() - uhs;
        lower.assign(
            this->points.begin(), this->points.begin() + lower_end + 1
        );
        upper.assign(this->points.begin() + lower_end, this->points.end());
        upper.push_back(this->points[0]);
    }

    pair<int, int> tangents_from(const Point& p) const {
        int n = size();
        if(n <= 1) {
            return {0, 0};
        }

        int a = 0, b = 0;
        auto update = [&](int id) {
            id %= n;
            if(ccw(p, points[a], points[id]) > 0) {
                a = id;
            }
            if(ccw(p, points[b], points[id]) < 0) {
                b = id;
            }
        };

        auto bin_search = [&](int low, int high) {
            if(low >= high) {
                return;
            }
            update(low);
            int sl = ccw(p, points[low % n], points[(low + 1) % n]);
            while(low + 1 < high) {
                int mid = (low + high) / 2;
                if(ccw(p, points[mid % n], points[(mid + 1) % n]) == sl) {
                    low = mid;
                } else {
                    high = mid;
                }
            }
            update(high);
        };

        int lid =
            (int)(lower_bound(lower.begin(), lower.end(), p) - lower.begin());
        bin_search(0, lid);
        bin_search(lid, (int)lower.size() - 1);

        int uid =
            (int)(lower_bound(upper.begin(), upper.end(), p, greater<Point>()) -
                  upper.begin());
        int base = lower_end;
        bin_search(base, base + uid);
        bin_search(base + uid, base + (int)upper.size() - 1);
        return {a, b};
    }
};

coord_t R;

coord_t normalize(coord_t x) {
    while(x < 0) {
        x += 2 * Point::PI;
    }
    while(x >= 2 * Point::PI) {
        x -= 2 * Point::PI;
    }
    return x;
}

coord_t cap_area(const Point& o, coord_t alpha) {
    Point dir(cos(alpha), sin(alpha));

    coord_t b = o * dir;
    coord_t c = o.norm2() - R * R;

    coord_t D = b * b - c;
    if(D < -Point::eps) {
        return 0;
    }
    D = max(D, (coord_t)0);
    coord_t sd = sqrt(D);

    Point q1 = o + dir * (-b - sd);
    Point q2 = o + dir * (-b + sd);

    coord_t dt = normalize(atan2(q2 ^ q1, q2 * q1));

    return 0.5 * R * R * dt + 0.5 * (q1 ^ q2);
}

coord_t solve_side(const ConvexHull& A, const ConvexHull& B) {
    coord_t ans = numeric_limits<coord_t>::max();
    int n = A.size();

    vector<coord_t> edge_angle(n);
    for(int i = 0; i < n; ++i) {
        edge_angle[i] = (A[(i + 1) % n] - A[i]).angle();
    }

    vector<pair<coord_t, int>> ev;
    for(int i = 0; i < n; ++i) {
        Point p = A[i];

        ev.clear();
        ev.emplace_back(0, 0);

        if(p.norm2() > Point::eps) {
            coord_t a = p.perp().angle();
            ev.emplace_back(normalize(a), 0);
            ev.emplace_back(normalize(a + Point::PI), 0);
        }

        int cnt = 0;

        auto ban = [&](coord_t l, coord_t r) {
            l = normalize(l + Point::eps);
            r = normalize(r - Point::eps);
            if(l > r) {
                ++cnt;
            }
            ev.emplace_back(l, 1);
            ev.emplace_back(r, -1);
        };

        if(n > 1) {
            ban(edge_angle[i], edge_angle[(i + n - 1) % n] + 2 * Point::PI);
        }

        auto [ta, tb] = B.tangents_from(p);
        coord_t angL = (B[tb] - p).angle() + Point::PI;
        coord_t angR = (B[ta] - p).angle();
        ban(angL, angR);

        sort(ev.begin(), ev.end());

        for(auto [ang, d]: ev) {
            int was = cnt;
            cnt += d;
            if(was == 0 || cnt == 0) {
                ans = min(ans, cap_area(p, ang));
            }
        }
    }
    return ans;
}

int n_ja, n_se;
vector<Point> ja, se;

void read() {
    int r;
    cin >> r;
    R = r;
    cin >> n_ja;
    ja.resize(n_ja);
    for(auto& p: ja) {
        int x, y;
        cin >> x >> y;
        p.x = x;
        p.y = y;
    }
    cin >> n_se;
    se.resize(n_se);
    for(auto& p: se) {
        int x, y;
        cin >> x >> y;
        p.x = x;
        p.y = y;
    }
}

void solve() {
    // We want to find the min and max area of one tribe's land on a circular
    // island of radius R, separated by a straight line from the other tribe.
    // The key observation is that the optimal separating line must pass through
    // a vertex of one of the two convex hulls.
    //
    // Let's fix a vertex v of hull A and consider all lines through v. Each
    // such line cuts the circle into two caps. We want the minimum cap area on
    // A's side, over all lines that validly separate A from B. As the line
    // rotates around v, the cap area changes, and we can compute it for any
    // given angle: the line hits the circle at two points q1, q2, and the cap
    // area is sector(q1, q2) + triangle(q1, q2) using the directed (CCW) angle.
    //
    // Not all angles are valid though. The line must not cut through either
    // hull. This gives us two forbidden angle intervals:
    //   1) The line can't cut through A itself. Since the hull is CCW, the
    //      forbidden directions are determined by the two edges adjacent to v.
    //   2) The line can't cut through B. We find the two tangent lines from v
    //      to hull B, and the directions between them (on the B side) are
    //      forbidden. Finding these tangent points can be done in O(log n) by
    //      binary searching on the lower and upper parts of the hull.
    //
    // We also add as candidate angles the perpendicular directions from v to
    // the center (where the cap area has local extrema). After collecting all
    // critical angles and sweeping with a ban counter, we evaluate the cap area
    // at every angle where no ban is active.
    //
    // Taking the minimum over all vertices of A gives the minimum area for A's
    // side. By symmetry, the maximum area is pi*R^2 minus the minimum area
    // when we swap the roles of A and B. Total complexity is O((n+m) log(n+m)).

    ConvexHull hja(ja);
    ConvexHull hse(se);

    coord_t min_area = solve_side(hja, hse);
    coord_t max_area = Point::PI * R * R - solve_side(hse, hja);

    cout << fixed << setprecision(15) << min_area << " " << max_area << "\n";
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
