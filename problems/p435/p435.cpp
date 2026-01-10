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
};

int n;
vector<Point> centers;
vector<coord_t> radii;

void read() {
    cin >> n;
    centers.resize(n);
    radii.resize(n);
    for(int i = 0; i < n; i++) {
        cin >> centers[i] >> radii[i];
    }
}

void solve() {
    // We will start by building a planar graph of the circles, where the arcs
    // are the edges. We will have a vertex for every intersection point, and
    // also add two additional arbitrary points per circle (this is for
    // convenience). It's clear that there are O(N^2) vertices and edges, and
    // trivially there would be O(N^2) faces too (due to Euler, or V - E + F = 1
    // + C). We are interested in the area of each face, and if it's covered by
    // an even or odd number of circles.
    //
    //    1) For the area, we can use Green's theorem, or for an arc from θ₁ to
    //       θ₂ on a circle with center (cx, cy) and radius r, the signed area
    //       contribution is:
    //
    //           0.5 * (r * cx * (sin θ₂ - sin θ₁) -
    //                  r * cy * (cos θ₂ - cos θ₁) +
    //                  r^2 * (θ₂ - θ₁))
    //
    //       More details in:
    //           https://en.wikipedia.org/wiki/Green%27s_theorem
    //
    //    2) For the parity, we can choose some point inside of the face. A
    //       simple example would be to choose an arbitrary vertex in the face,
    //       get the two adjacent edges, sum the vectors (get director in the
    //       middle of them), and then move with "eps" in that direction. There
    //       are other approaches too. We can then iterate through all circles
    //       and see how many include this particular point.
    //
    // To build the graph, we will initially for every circle find all
    // intersection points, and two arbitrary points (say center-(R,0), and
    // center+(R,0)), then sort by angle and add edges between the adjacent
    // vertices. Then finding faces is also not hard - we create an embedding of
    // the planar graph (sort the neighbours by angle), and do a walk from each
    // point clockwise. All in all, the complexity is O(N^3).
    //
    // However, this solution is a bit complicated due to all the planar graph
    // logic. Instead, we can notice that we don't actually have to explicitly
    // build the faces - we can just go through the arcs and figure out how they
    // contribute to the two answers (burned and inverse grains). Particularly,
    // we are doing Green's theorem which already computes the area as signed
    // contribution from every, so instead for a given arc we can compute it's
    // contribution on both sides. For every arc we will choose an inner and an
    // outer point, and count the parity of the number of circles that cover the
    // inner point. This means that we will get CW contribution for that parity
    // and CCW for the opposite (or we can do the inverse, doesn't matter). We
    // should only be careful for the case when the count is equal, as we don't
    // want the contribution from the outer face. The complexity is still the
    // same, but we avoid a lot of the more complicated logic.

    vector<vector<pair<coord_t, Point>>> events(n);

    for(int i = 0; i < n; i++) {
        Point right = centers[i] + Point(radii[i], 0);
        Point left = centers[i] - Point(radii[i], 0);
        events[i].emplace_back((right - centers[i]).angle(), right);
        events[i].emplace_back((left - centers[i]).angle(), left);

        for(int j = 0; j < n; j++) {
            if(i == j) {
                continue;
            }
            vector<Point> pts =
                intersect_circles(centers[i], radii[i], centers[j], radii[j]);
            for(auto& p: pts) {
                coord_t ang = (p - centers[i]).angle();
                events[i].emplace_back(ang, p);
            }
        }

        sort(events[i].begin(), events[i].end());

        vector<pair<coord_t, Point>> unique_events;
        for(auto& e: events[i]) {
            if(unique_events.empty() ||
               abs(unique_events.back().first - e.first) > Point::eps) {
                unique_events.push_back(e);
            }
        }
        events[i] = unique_events;
    }

    coord_t burned = 0.0;
    coord_t inverse = 0.0;

    for(int i = 0; i < n; i++) {
        int m = events[i].size();
        if(m < 2) {
            continue;
        }

        for(int j = 0; j < m; j++) {
            Point p1 = events[i][j].second;
            Point p2 = events[i][(j + 1) % m].second;
            coord_t ang1 = events[i][j].first;
            coord_t ang2 = events[i][(j + 1) % m].first;
            if(ang2 < ang1) {
                ang2 += 2 * Point::PI;
            }

            coord_t mid_ang = (ang1 + ang2) / 2.0;
            Point mid_dir(cos(mid_ang), sin(mid_ang));
            Point test_inside = centers[i] + mid_dir * (radii[i] - 1e-6);
            Point test_outside = centers[i] + mid_dir * (radii[i] + 1e-6);

            auto count_circles = [&](Point p) {
                int cnt = 0;
                for(int k = 0; k < n; k++) {
                    if((p - centers[k]).norm() < radii[k] - Point::eps) {
                        cnt++;
                    }
                }
                return cnt;
            };

            int cnt_inside = count_circles(test_inside);
            int cnt_outside = count_circles(test_outside);

            coord_t piece = arc_area(centers[i], radii[i], p1, p2);

            if(cnt_inside % 2 == 1) {
                burned += piece;
            } else if(cnt_inside > 0) {
                inverse += piece;
            }

            if(cnt_outside % 2 == 1) {
                burned -= piece;
            } else if(cnt_outside > 0) {
                inverse -= piece;
            }
        }
    }

    cout << fixed << setprecision(5);
    cout << burned << " " << inverse << "\n";
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
