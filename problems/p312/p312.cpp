#include <bits/stdc++.h>
#include <vector>
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
};

int n;
vector<Point> pnts;
string k_ratios;

vector<int> parse_ratios(const string& s) {
    vector<int> res;
    int cur = 0;
    for(char c: s) {
        if(c == ':') {
            res.push_back(cur);
            cur = 0;
        } else {
            cur = cur * 10 + (c - '0');
        }
    }
    res.push_back(cur);
    return res;
}

double tri_area(const Point& a, const Point& b, const Point& c) {
    return 0.5 * ((b - a) ^ (c - a));
}

struct Region {
    int cnt;
    Point p[4];
};

Region make_tri(Point a, Point b, Point c) {
    Region r;
    r.cnt = 3;
    r.p[0] = a;
    r.p[1] = b;
    r.p[2] = c;
    return r;
}

Region make_quad(Point a, Point b, Point c, Point d) {
    Region r;
    r.cnt = 4;
    r.p[0] = a;
    r.p[1] = b;
    r.p[2] = c;
    r.p[3] = d;
    return r;
}

void print_region(const Region& r) {
    cout << r.cnt;
    for(int i = 0; i < r.cnt; i++) {
        cout << ' ' << r.p[i];
    }
    cout << '\n';
}

pair<Region, Region> split_quad(
    Point v0, Point v1, Point v2, Point v3, double r1, double r2
) {
    double a1 = tri_area(v0, v1, v2);
    double a2 = tri_area(v0, v2, v3);
    double total = a1 + a2;
    double target = r1 / (r1 + r2) * total;

    double s = target / a1;
    if(s >= -1e-9 && s <= 1 + 1e-9) {
        s = max(0.0, min(1.0, s));
        Point P = v1 + (v2 - v1) * s;
        return {make_tri(v0, v1, P), make_quad(v0, P, v2, v3)};
    } else {
        double rem = target - a1;
        double s2 = rem / a2;
        s2 = max(0.0, min(1.0, s2));
        Point P = v2 + (v3 - v2) * s2;
        return {make_quad(v0, v1, v2, P), make_tri(v0, P, v3)};
    }
}

void read() {
    cin >> n;
    pnts.resize(n);
    cin >> pnts >> k_ratios;
}

void solve() {
    // On first glance the problem looks very complicated, but it's actually a
    // combination of some fairly simple ideas. Let us solve the N=3 and N=4
    // cases separately.
    //
    // For N=3, it turns out it's always possible. Let's try to make the 3 areas
    // fully match with one side of the original triangle. We will denote the 3
    // vertices of the original area as A, B and C. Take the AB side. Then
    // project C onto AB to get H. The area of the triangle is CH*AB/2. We know
    // K1:K2:K3, so it's not hard to see that we would like the "height" to be
    // h1 = K1:(K2+K3) * CH. This gives us a line for potential candidate of
    // where the last point of the AB region would be (parallel line to AB at
    // distance h1). Let's repeat absolutely the same for BC. This gives us a
    // another line at distance h2 = K2:(K1+K3) * AQ, where Q is the projection
    // of A onto BC. We now have two lines, and we would like to take a point
    // that is on both so we can just intersect them (denote as O). We could do
    // a third line in the same style for CA, but two lines are already enough
    // to find O, and they guarantee that the third area will have K3:(K1+K2) as
    // the proportion.
    //
    // The N=4 case is a bit harder. Note that the polygon could be non-convex
    // (arrow-like case). Having solved N=3, we should think of a similar style
    // approach that isn't over-complicated. We will use A, B, C, and D as the
    // points. The key is that one of the two diagonals (either AC or BD), is
    // inside of the polygon. Let's use AC as the diagonal, and choose a point Q
    // on the diagonal such that the ABQD polygon compared to BCDQ has ratio
    // (K1+K4):(K2+K3). This works because area scales linearly along the
    // diagonal. Now we have two independent and simpler problems - given a
    // quadrangle, split it into two parts given two ratios. For ABQD, sides AB
    // and DA share vertex A, so we draw a line from A to a point P on the
    // opposite chain B->Q->D. We compute the target area for the AB region as
    // K1/(K1+K4) * area(ABQD). If P lies on segment BQ at fraction s, then
    // area(ABP) = s * area(ABQ) (since the cross product scales linearly in P).
    // So if target <= area(ABQ), we get s = target/area(ABQ) and P = B+s*(Q-B),
    // giving triangle ABP for K1 and quad APQD for K4. Otherwise P is on QD,
    // and we solve s = (target - area(ABQ)) / area(AQD) analogously, giving
    // quad ABQP for K1 and triangle APD for K4. Similarly for BCDQ, sides BC
    // and CD share vertex C, so we split from C to a point on chain D->Q->B.

    vector<int> K = parse_ratios(k_ratios);
    cout << fixed << setprecision(10);

    if(n == 3) {
        Point A = pnts[0], B = pnts[1], C = pnts[2];
        double sum = K[0] + K[1] + K[2];
        Point O = (A * K[1] + B * K[2] + C * K[0]) / sum;
        cout << "3 " << A << ' ' << B << ' ' << O << '\n';
        cout << "3 " << B << ' ' << C << ' ' << O << '\n';
        cout << "3 " << C << ' ' << A << ' ' << O << '\n';
    } else {
        Point A = pnts[0], B = pnts[1], C = pnts[2], D = pnts[3];
        double sum = K[0] + K[1] + K[2] + K[3];

        int s1 = ccw(A, C, B), s2 = ccw(A, C, D);

        Region r[4];
        if(s1 * s2 < 0) {
            double t = (K[0] + K[3]) / sum;
            Point Q = A + (C - A) * t;
            auto [r0, r3] = split_quad(A, B, Q, D, K[0], K[3]);
            auto [r2, r1] = split_quad(C, D, Q, B, K[2], K[1]);
            r[0] = r0;
            r[1] = r1;
            r[2] = r2;
            r[3] = r3;
        } else {
            double t = (K[0] + K[1]) / sum;
            Point Q = B + (D - B) * t;
            auto [r1, r0] = split_quad(B, C, Q, A, K[1], K[0]);
            auto [r3, r2] = split_quad(D, A, Q, C, K[3], K[2]);
            r[0] = r0;
            r[1] = r1;
            r[2] = r2;
            r[3] = r3;
        }

        for(int i = 0; i < 4; i++) {
            print_region(r[i]);
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
        solve();
    }

    return 0;
}
