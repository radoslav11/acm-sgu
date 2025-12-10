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
    static constexpr coord_t eps = 1e-6;

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

class Polygon {
  public:
    vector<Point> points;

    Polygon() {}
    Polygon(const vector<Point>& points) : points(points) {}

    int size() const { return points.size(); }

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
    int upper_hull_size;

    ConvexHull(const vector<Point>& points) {
        this->points = points;
        sort(this->points.begin(), this->points.end());
        this->points.erase(
            unique(this->points.begin(), this->points.end()), this->points.end()
        );

        if(this->points.size() <= 2) {
            this->upper_hull_size = this->points.size();
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

        upper_hull_size = hull.size();
        for(int i = (int)this->points.size() - 2; i >= 0; i--) {
            if(!used[i]) {
                expand_hull(i, upper_hull_size + 1);
            }
        }

        hull.pop_back();

        vector<Point> points_in_hull;
        for(int i: hull) {
            points_in_hull.push_back(this->points[i]);
        }
        this->points = std::move(points_in_hull);
    }
};

int n, m;
vector<Point> vertices;
ConvexHull* hull;
PointInConvexPolygon* pip;

bool segments_intersect(
    const Point& a1, const Point& a2, const Point& b1, const Point& b2
) {
    int d1 = ccw(b1, b2, a1);
    int d2 = ccw(b1, b2, a2);
    int d3 = ccw(a1, a2, b1);
    int d4 = ccw(a1, a2, b2);

    if(d1 * d2 < 0 && d3 * d4 < 0) {
        return true;
    }
    return false;
}

Point segment_intersection(
    const Point& a1, const Point& a2, const Point& b1, const Point& b2
) {
    return line_line_intersection(a1, a2, b1, b2);
}

bool strictly_inside(const Point& p) {
    if(!pip->contains(p)) {
        return false;
    }

    for(int i = 0; i < (int)hull->size(); i++) {
        Point p1 = hull->points[i];
        Point p2 = hull->points[(i + 1) % hull->size()];
        if(point_on_segment(p1, p2, p)) {
            return false;
        }
    }
    return true;
}

coord_t segment_length_inside(Point a, Point b) {
    const coord_t eps = Point::eps;
    vector<Point> points;

    points.push_back(a);
    points.push_back(b);

    for(int i = 0; i < hull->size(); i++) {
        Point p1 = hull->points[i];
        Point p2 = hull->points[(i + 1) % hull->size()];

        if(segments_intersect(a, b, p1, p2)) {
            Point intersection = segment_intersection(a, b, p1, p2);
            if(point_on_segment(a, b, intersection)) {
                points.push_back(intersection);
            }
        }

        if(point_on_segment(a, b, p1)) {
            points.push_back(p1);
        }

        if(point_on_segment(p1, p2, a)) {
            points.push_back(a);
        }
        if(point_on_segment(p1, p2, b)) {
            points.push_back(b);
        }
    }

    sort(points.begin(), points.end(), [&](const Point& x, const Point& y) {
        coord_t tx = abs(b.x - a.x) > eps ? (x.x - a.x) / (b.x - a.x)
                                          : (x.y - a.y) / (b.y - a.y);
        coord_t ty = abs(b.x - a.x) > eps ? (y.x - a.x) / (b.x - a.x)
                                          : (y.y - a.y) / (b.y - a.y);
        return tx < ty;
    });

    vector<Point> unique_points;
    for(auto& p: points) {
        if(unique_points.empty() || (p - unique_points.back()).norm() > eps) {
            unique_points.push_back(p);
        }
    }

    coord_t total = 0;
    for(int i = 0; i + 1 < (int)unique_points.size(); i++) {
        Point p1 = unique_points[i];
        Point p2 = unique_points[i + 1];
        Point mid = (p1 + p2) / 2.0;

        if(pip->contains(p1) && pip->contains(p2) && strictly_inside(mid)) {
            total += (p2 - p1).norm();
        }
    }

    return total;
}

void read() {
    cin >> n;
    vertices.resize(n);
    cin >> vertices;

    hull = new ConvexHull(vertices);
    pip = new PointInConvexPolygon(*hull);

    cin >> m;
}

void solve() {
    // The problem statement is a bit vague, but what it says about the middle
    // son's polygon is simply that it's convex "moving straight from any place
    // of this part to any other place of this part he will not cross the
    // boundary". Then we are asked for each mineral to find the length that is
    // fully inside of the boundary. This can be done by finding all
    // intersection points, and splitting each of the mineral zones into at most
    // 3 subsegments. We can then simply check if both endpoints of any of these
    // subsegments are within the polygon and if that's the case add +length to
    // the answer for that mineral zone. We can use PointInConvexPolygon for
    // that.

    for(int i = 0; i < m; i++) {
        Point a, b;
        cin >> a >> b;

        coord_t length = segment_length_inside(a, b);
        cout << fixed << setprecision(2) << length << '\n';
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
