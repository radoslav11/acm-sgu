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
const double INF = 1e7;

struct Point {
    static constexpr coord_t eps = 1e-9;

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

using Line = pair<Point, Point>;

class HalfPlaneIntersection {
  private:
    vector<Line> lines;
    deque<int> dq;
    bool empty_intersection = false;

    Point dir(int i) const { return lines[i].second - lines[i].first; }

    bool outside(int i, const Point& pt) const {
        return ccw(lines[i].first, lines[i].second, pt) < 0;
    }

    Point inter(int i, int j) const {
        return line_line_intersection(
            lines[i].first, lines[i].second, lines[j].first, lines[j].second
        );
    }

    bool is_parallel(int i, int j) const {
        return abs(dir(i) ^ dir(j)) < Point::eps;
    }

    bool same_direction(int i, int j) const { return (dir(i) * dir(j)) > 0; }

  public:
    static vector<Line> sort_by_angle(const vector<Line>& lines) {
        vector<Line> sorted = lines;
        sort(sorted.begin(), sorted.end(), [](const Line& a, const Line& b) {
            return (a.second - a.first).angle() < (b.second - b.first).angle();
        });
        return sorted;
    }

    HalfPlaneIntersection(const vector<Line>& lines, bool is_sorted = false)
        : lines(is_sorted ? lines : sort_by_angle(lines)) {
        int n = this->lines.size();

        for(int i = 0; i < n; i++) {
            while(dq.size() > 1 &&
                  outside(i, inter(dq.back(), dq[dq.size() - 2]))) {
                dq.pop_back();
            }
            while(dq.size() > 1 && outside(i, inter(dq.front(), dq[1]))) {
                dq.pop_front();
            }

            if(!dq.empty() && is_parallel(i, dq.back())) {
                if(!same_direction(i, dq.back())) {
                    empty_intersection = true;
                    return;
                }
                if(outside(i, this->lines[dq.back()].first)) {
                    dq.pop_back();
                } else {
                    continue;
                }
            }

            dq.push_back(i);
        }

        while(dq.size() > 2 &&
              outside(dq.front(), inter(dq.back(), dq[dq.size() - 2]))) {
            dq.pop_back();
        }
        while(dq.size() > 2 && outside(dq.back(), inter(dq.front(), dq[1]))) {
            dq.pop_front();
        }

        if(dq.size() < 3) {
            empty_intersection = true;
        }
    }

    bool is_non_empty() const { return !empty_intersection; }

    vector<Point> get_polygon() const {
        if(empty_intersection) {
            return {};
        }
        vector<Point> result(dq.size());
        for(size_t i = 0; i + 1 < dq.size(); i++) {
            result[i] = inter(dq[i], dq[i + 1]);
        }
        result.back() = inter(dq.back(), dq.front());
        return result;
    }
};

int n;
vector<Point> pts;

void read() {
    cin >> n;
    pts.resize(n);
    cin >> pts;
}

void solve() {
    // The is a direct application of half-plane intersection. We can binary
    // search for the answer R, and then shrink each of the polygon sides
    // towards their perpendiculars. The check is then to make sure that there
    // is a point inside of each of these N half-planes. There is a standard
    // randomized algorithm (add planes in random order and slice the polygon),
    // but there is also a simple algorithm introduced by Zeyuan Zhu in his
    // 2006 CTSC thesis. More details can be found here:
    //     https://cp-algorithms.com/geometry/halfplane-intersection.html

    vector<Line> edges(n);
    for(int i = 0; i < n; i++) {
        edges[i] = {pts[i], pts[(i + 1) % n]};
    }

    edges = HalfPlaneIntersection::sort_by_angle(edges);

    auto check = [&](double R) {
        vector<Line> pushed(n);
        for(int i = 0; i < n; i++) {
            Point offset = (edges[i].second - edges[i].first).normal() * R;
            pushed[i] = {edges[i].first + offset, edges[i].second + offset};
        }
        return HalfPlaneIntersection(pushed, true).is_non_empty();
    };

    double low = 0, high = 1e7;
    for(int ops = 0; ops < 100; ops++) {
        double mid = (low + high) / 2.0;
        if(check(mid)) {
            low = mid;
        } else {
            high = mid;
        }
    }

    cout << fixed << setprecision(4) << low << "\n";
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
