#include <bits/stdc++.h>
// #include <coding_library/geometry/point.hpp>
// #include <coding_library/geometry/dynamic_convex_hull.hpp>

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

using coord_t = int64_t;

struct Point {
    static constexpr coord_t eps = 0;

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

class DynamicConvexHull {
  public:
    set<Point> lower;
    set<Point> upper;

  private:
    coord_t lower_sum = 0;
    coord_t upper_sum = 0;

    using Iter = set<Point>::iterator;

    void add_to_hull(set<Point>& hull, coord_t& sum, const Point& p, int sign) {
        if(hull.empty()) {
            hull.insert(p);
            return;
        }

        auto right = hull.lower_bound(p);

        if(right != hull.end() && right->x == p.x) {
            if(sign * (p.y - right->y) >= 0) {
                return;
            }
            Iter left = (right != hull.begin()) ? prev(right) : hull.end();
            Iter right_next = next(right);
            if(left != hull.end()) {
                sum -= (*left) ^ (*right);
            }
            if(right_next != hull.end()) {
                sum -= (*right) ^ (*right_next);
            }
            if(left != hull.end() && right_next != hull.end()) {
                sum += (*left) ^ (*right_next);
            }
            hull.erase(right);
            right = right_next;
        }

        Iter left = (right != hull.begin()) ? prev(right) : hull.end();

        if(left != hull.end() && right != hull.end()) {
            if(sign * ccw(*left, *right, p) >= 0) {
                return;
            }
            sum -= (*left) ^ (*right);
        }

        while(left != hull.end() && left != hull.begin()) {
            Iter left_left = prev(left);
            if(sign * ccw(*left_left, *left, p) > 0) {
                break;
            }
            sum -= (*left_left) ^ (*left);
            hull.erase(left);
            left = left_left;
        }

        while(right != hull.end()) {
            Iter right_next = next(right);
            if(right_next == hull.end()) {
                break;
            }
            if(sign * ccw(p, *right, *right_next) > 0) {
                break;
            }
            sum -= (*right) ^ (*right_next);
            hull.erase(right);
            right = right_next;
        }

        if(left != hull.end()) {
            sum += (*left) ^ p;
        }
        if(right != hull.end()) {
            sum += p ^ (*right);
        }

        hull.insert(p);
    }

  public:
    DynamicConvexHull() = default;

    void add(const Point& p) {
        add_to_hull(lower, lower_sum, p, 1);
        add_to_hull(upper, upper_sum, p, -1);
    }

    coord_t doubled_area() const {
        if(lower.empty() || upper.empty()) {
            return 0;
        }

        coord_t result = lower_sum - upper_sum;

        const Point& right_lower = *lower.rbegin();
        const Point& right_upper = *upper.rbegin();
        if(!(right_lower == right_upper)) {
            result += right_lower ^ right_upper;
        }

        const Point& left_lower = *lower.begin();
        const Point& left_upper = *upper.begin();
        if(!(left_lower == left_upper)) {
            result += left_upper ^ left_lower;
        }

        return result;
    }
};

Point a, b, c;
int q;

void read() {
    cin >> a >> b >> c;
    cin >> q;
}

DynamicConvexHull hull;

void solve() {
    // The idea is to do a dynamic convex hull after every operation. In
    // particular, we will maintain the lower and upper envelope and we will
    // maintain the area separately. The envelope will be maintained as a set of
    // points, sorted by X. We will then add points one by one, and binary
    // search in the set to find it's desired place. There are then two cases -
    // the point we are adding is inside of the polygon in which case we can
    // skip it, or outside meaning we need to potentially remove some of the
    // points already in the hull. Removing points can be done similarly to
    // Graham scan's algorithm and the complexity will be amortized O(log)
    // (because of the set).
    // To find the area, we can remember that the area of the polygon can be
    // given by the absolute of the sum of the determinants of the 2x2 matrices
    // given by each two consecutive points.

    hull.add(a);
    hull.add(b);
    hull.add(c);

    for(int i = 0; i < q; i++) {
        Point p;
        cin >> p;
        hull.add(p);
        cout << abs(hull.doubled_area()) << '\n';
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
