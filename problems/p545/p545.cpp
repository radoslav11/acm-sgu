#include <bits/stdc++.h>

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
    static constexpr coord_t eps = 1e-12L;
    static inline const coord_t PI = acosl((coord_t)-1.0L);

    coord_t x, y;
    Point(coord_t x = 0, coord_t y = 0) : x(x), y(y) {}

    Point operator+(const Point& p) const { return Point(x + p.x, y + p.y); }
    Point operator-(const Point& p) const { return Point(x - p.x, y - p.y); }
    Point operator*(coord_t c) const { return Point(x * c, y * c); }
    Point operator/(coord_t c) const { return Point(x / c, y / c); }

    coord_t dot(const Point& p) const { return x * p.x + y * p.y; }
    coord_t norm2() const { return x * x + y * y; }
    coord_t norm() const { return sqrtl(norm2()); }

    friend ostream& operator<<(ostream& os, const Point& p) {
        return os << p.x << ' ' << p.y;
    }
    friend istream& operator>>(istream& is, Point& p) {
        return is >> p.x >> p.y;
    }
};

int n, m;
vector<coord_t> xs, ls;
vector<int> cut_seq;
vector<coord_t> ts;

bool inside_disks(const Point& p, const vector<int>& S, coord_t tol = 1e-7L) {
    for(int i: S) {
        coord_t dx = p.x - xs[i], dy = p.y;
        if(dx * dx + dy * dy > ls[i] * ls[i] + tol) {
            return false;
        }
    }

    return true;
}

vector<Point> circle_pair_inters(int i, int j) {
    coord_t dx = xs[j] - xs[i];
    coord_t d = fabsl(dx);
    if(d < 1e-14L) {
        return {};
    }

    coord_t r1 = ls[i], r2 = ls[j];
    if(d > r1 + r2 + 1e-12L || d < fabsl(r1 - r2) - 1e-12L) {
        return {};
    }

    coord_t a = (r1 * r1 - r2 * r2 + d * d) / (2 * d);
    coord_t h2 = r1 * r1 - a * a;
    if(h2 < 0) {
        h2 = 0;
    }

    coord_t h = sqrtl(h2);
    coord_t sgn = (xs[j] > xs[i]) ? 1.0L : -1.0L;
    coord_t mx = xs[i] + a * sgn;
    return {Point(mx, h), Point(mx, -h)};
}

Point equilibrium(const vector<int>& S) {
    if(S.empty()) {
        return Point(0, -1e18L);
    }

    vector<Point> cands;
    for(int i: S) {
        cands.push_back(Point(xs[i], -ls[i]));
    }

    for(size_t a = 0; a < S.size(); ++a) {
        for(size_t b = a + 1; b < S.size(); ++b) {
            for(auto& q: circle_pair_inters(S[a], S[b])) {
                cands.push_back(q);
            }
        }
    }

    Point best(0, 1e18L);
    for(auto& c: cands) {
        if(inside_disks(c, S) && c.y < best.y) {
            best = c;
        }
    }

    return best;
}

vector<int> tight_set(const Point& p, const vector<int>& S) {
    vector<int> tight;
    for(int i: S) {
        coord_t dx = p.x - xs[i], dy = p.y;
        coord_t d = sqrtl(dx * dx + dy * dy);
        if(fabsl(d - ls[i]) < 1e-6L) {
            tight.push_back(i);
        }
    }

    return tight;
}

struct DirResult {
    Point dir;
    int arc_circle;
};

DirResult descent_dir(const Point& p, const vector<int>& tight) {
    Point g(0, -1);
    if(tight.empty()) {
        return {g, -1};
    }

    vector<Point> normals;
    for(int i: tight) {
        Point r(p.x - xs[i], p.y);
        coord_t rn = r.norm();
        if(rn < 1e-14L) {
            normals.push_back(Point(0, -1));
        } else {
            normals.push_back(r / rn);
        }
    }

    bool g_ok = true;
    for(auto& nk: normals) {
        if(g.dot(nk) > 1e-9L) {
            g_ok = false;
            break;
        }
    }
    if(g_ok) {
        int tangent_circle = -1;
        for(size_t k = 0; k < tight.size(); ++k) {
            if(fabsl(g.dot(normals[k])) < 1e-9L) {
                tangent_circle = tight[k];
                break;
            }
        }

        return {g, tangent_circle};
    }

    DirResult best;
    best.dir = Point(0, 1);
    best.arc_circle = -1;
    bool found = false;
    for(size_t k = 0; k < tight.size(); ++k) {
        const Point& nk = normals[k];
        coord_t gn = g.dot(nk);
        Point d(-gn * nk.x, -1 - gn * nk.y);
        coord_t dn = d.norm();
        if(dn < 1e-14L) {
            continue;
        }

        d = d / dn;
        if(d.y >= -1e-12L) {
            continue;
        }

        bool ok = true;
        for(size_t kk = 0; kk < tight.size(); ++kk) {
            if(kk == k) {
                continue;
            }
            if(d.dot(normals[kk]) > 1e-9L) {
                ok = false;
                break;
            }
        }
        if(!ok) {
            continue;
        }

        if(!found || d.y < best.dir.y) {
            best.dir = d;
            best.arc_circle = tight[k];
            found = true;
        }
    }

    if(!found) {
        return {Point(0, 0), -1};
    }

    return best;
}

struct Segment {
    bool is_vert;
    coord_t x, y0, y1;
    coord_t cx, r, theta0, theta1;
    coord_t len;
};

struct Phase {
    coord_t start_t;
    coord_t total_len;
    Point start_p, end_p;
    vector<Segment> segs;
};

coord_t fwd_angle(coord_t from, coord_t to, coord_t s) {
    coord_t two_pi = 2 * Point::PI;
    coord_t dt = s * (to - from);
    dt = fmodl(dt, two_pi);
    if(dt < 0) {
        dt += two_pi;
    }
    return dt;
}

vector<Segment> trace_path(Point start, Point target, const vector<int>& S) {
    vector<Segment> segs;
    Point cur = start;
    coord_t two_pi = 2 * Point::PI;
    for(int iter = 0; iter < 20000; ++iter) {
        if((cur - target).norm() < 1e-9L) {
            break;
        }

        vector<int> tight = tight_set(cur, S);
        DirResult dr = descent_dir(cur, tight);
        if(dr.dir.norm() < 1e-9L) {
            break;
        }

        if(dr.arc_circle == -1) {
            coord_t y_end = -1e18L;
            for(int i: S) {
                bool is_t = false;
                for(int t: tight) {
                    if(t == i) {
                        is_t = true;
                        break;
                    }
                }
                if(is_t) {
                    continue;
                }

                coord_t dx = cur.x - xs[i];
                coord_t disc = ls[i] * ls[i] - dx * dx;
                if(disc < 0) {
                    continue;
                }

                coord_t y_b = -sqrtl(disc);
                if(y_b < cur.y - 1e-12L && y_b > y_end) {
                    y_end = y_b;
                }
            }

            if(y_end < -1e17L) {
                y_end = target.y;
            }

            if(fabsl(cur.x - target.x) < 1e-9L && y_end < target.y - 1e-12L) {
                y_end = target.y;
            }

            Segment sg;
            sg.is_vert = true;
            sg.x = cur.x;
            sg.y0 = cur.y;
            sg.y1 = y_end;
            sg.len = cur.y - y_end;
            if(sg.len < 1e-12L) {
                break;
            }

            segs.push_back(sg);
            cur = Point(cur.x, y_end);
        } else {
            int arc_i = dr.arc_circle;
            coord_t theta0 = atan2l(cur.y, cur.x - xs[arc_i]);
            Point tang(-sinl(theta0), cosl(theta0));
            coord_t s = (tang.dot(dr.dir) > 0) ? 1.0L : -1.0L;
            coord_t best_dt = two_pi;
            Point best_pt = cur;
            bool have_event = false;
            for(int j: S) {
                if(j == arc_i) {
                    continue;
                }

                for(auto& q: circle_pair_inters(arc_i, j)) {
                    coord_t tq = atan2l(q.y, q.x - xs[arc_i]);
                    coord_t dt = fwd_angle(theta0, tq, s);
                    if(dt < 1e-9L) {
                        continue;
                    }
                    if(dt < best_dt) {
                        best_dt = dt;
                        best_pt = q;
                        have_event = true;
                    }
                }
            }

            {
                coord_t tq = -Point::PI / 2;
                coord_t dt = fwd_angle(theta0, tq, s);
                if(dt > 1e-9L && dt < best_dt) {
                    best_dt = dt;
                    best_pt = Point(xs[arc_i], -ls[arc_i]);
                    have_event = true;
                }
            }

            if(!have_event) {
                break;
            }

            coord_t theta1 = theta0 + s * best_dt;
            Segment sg;
            sg.is_vert = false;
            sg.cx = xs[arc_i];
            sg.r = ls[arc_i];
            sg.theta0 = theta0;
            sg.theta1 = theta1;
            sg.len = ls[arc_i] * best_dt;
            if(sg.len < 1e-12L) {
                break;
            }

            segs.push_back(sg);
            cur = best_pt;
        }

        if((cur - target).norm() < 1e-7L) {
            cur = target;
            break;
        }
    }

    return segs;
}

Point interpolate(const Phase& ph, coord_t s) {
    if(s <= 0) {
        return ph.start_p;
    }

    if(s >= ph.total_len - 1e-12L) {
        return ph.end_p;
    }

    coord_t acc = 0;
    for(const auto& sg: ph.segs) {
        if(s <= acc + sg.len + 1e-12L) {
            coord_t local = s - acc;
            coord_t frac = (sg.len > 1e-14L) ? local / sg.len : 0;
            if(sg.is_vert) {
                return Point(sg.x, sg.y0 + (sg.y1 - sg.y0) * frac);
            } else {
                coord_t theta = sg.theta0 + (sg.theta1 - sg.theta0) * frac;
                return Point(sg.cx + sg.r * cosl(theta), sg.r * sinl(theta));
            }
        }
        acc += sg.len;
    }

    return ph.end_p;
}

void read() {
    cin >> n >> m;
    xs.resize(n);
    ls.resize(n);
    for(auto& v: xs) {
        cin >> v;
    }

    for(auto& v: ls) {
        cin >> v;
    }

    cut_seq.resize(n - 1);
    for(auto& v: cut_seq) {
        cin >> v;
        --v;
    }

    ts.resize(m);
    for(auto& v: ts) {
        cin >> v;
    }
}

void solve() {
    // Each remaining rope i is the closed disk D_i = { p : (p.x - xs[i])^2
    // + p.y^2 <= ls[i]^2 } centered at (xs[i], 0). For any non-empty active
    // set S the stable position of the ball is the unique point of minimum
    // y inside the intersection of those disks. The KKT conditions of
    // minimising y over that intersection say at least one constraint is
    // active at the optimum, and at most two are needed in 2D; the
    // candidates are therefore the |S| circle bottoms (xs[i], -ls[i]) and
    // the at most two intersections of every pair of circles. We enumerate
    // all of them, keep those that lie inside every disk of S, and pick the
    // one with the smallest y.
    //
    // Cuts are processed in the given order. Cut 1 happens at t = 0; cut j
    // (j >= 2) happens at the instant the ball first becomes stationary
    // after cut j - 1. If a cut does not change the stable point it is
    // instantaneous.
    //
    // Between two consecutive equilibria the ball follows a perfectly
    // viscous gradient descent: its velocity is the projection of gravity
    // g = (0, -1) onto the feasible tangent cone at the current point.
    // Let T be the set of currently tight constraints (circles where
    // |p - c_i| = ls[i]); the tangent cone is { d : d . n_i <= 0 for i in T }
    // with outward normals n_i = (p - c_i) / ls[i]. The projection is
    // computed analytically:
    //   - if g satisfies all g . n_i <= 0, the velocity is g itself and the
    //     motion is straight down;
    //   - otherwise for each k in T we compute d_k = g - (g . n_k) n_k,
    //     which is the projection of g onto the hyperplane n_k = 0
    //     (i.e. the unit tangent to circle k at p that lowers y), and keep
    //     the one whose y-component is the smallest among those that still
    //     satisfy d . n_l <= 0 for every other l in T;
    //   - if no such d exists the cone has only the zero descent direction
    //     and the ball is at equilibrium.
    //
    // Each piece of the trajectory is therefore either a vertical segment
    // (interior motion until we touch the first circle from inside) or a
    // circular arc on some currently-tight rope. While on the arc of circle
    // i we follow it until the first angular event: either an intersection
    // with another active circle j or the bottom of circle i at theta =
    // -pi/2. Hitting an intersection turns it into a vertex and the next
    // iteration's projection chooses whether to transfer to the new circle,
    // re-enter the interior, or stop. The trajectory is built event by
    // event with at most O(n^2) events total.
    //
    // Each phase stores its starting time, total arc length, the precise
    // (start, end) equilibrium points, and the ordered list of vertical /
    // arc segments with their Euclidean lengths. Because the ball travels
    // at unit speed, the time spent in a phase equals its total length.
    // A query at time t locates the unique phase whose interval contains t,
    // walks the segment list by the local distance s = t - start_t,
    // interpolates linearly along the active piece (vertical or arc), and
    // returns the resulting point. After the final cut the ball stays at
    // the bottom of the last rope for all future times.

    vector<int> active(n);
    iota(active.begin(), active.end(), 0);

    Point cur = equilibrium(active);
    coord_t cur_time = 0;
    vector<Phase> phases;
    Point final_pos = cur;

    for(int ci = 0; ci < n - 1; ++ci) {
        int cut = cut_seq[ci];
        active.erase(find(active.begin(), active.end(), cut));

        Point new_eq = equilibrium(active);
        if((new_eq - cur).norm() < 1e-9L) {
            cur = new_eq;
            final_pos = cur;
            continue;
        }

        vector<Segment> segs = trace_path(cur, new_eq, active);
        coord_t total = 0;
        for(auto& sg: segs) {
            total += sg.len;
        }

        if(total < 1e-9L) {
            cur = new_eq;
            final_pos = cur;
            continue;
        }

        Phase ph;
        ph.start_t = cur_time;
        ph.total_len = total;
        ph.start_p = cur;
        ph.end_p = new_eq;
        ph.segs = std::move(segs);
        phases.push_back(std::move(ph));

        cur = new_eq;
        cur_time += total;
        final_pos = cur;
    }

    cout << fixed << setprecision(10);
    for(coord_t t: ts) {
        Point pos = final_pos;
        for(const auto& ph: phases) {
            if(t <= ph.start_t + ph.total_len + 1e-9L) {
                coord_t s = t - ph.start_t;
                if(s < 0) {
                    s = 0;
                }
                if(s > ph.total_len) {
                    s = ph.total_len;
                }
                pos = interpolate(ph, s);
                break;
            }
        }

        cout << pos.x << ' ' << pos.y << '\n';
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
