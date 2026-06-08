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

using V3 = array<int64_t, 3>;

V3 sub3(V3 a, V3 b) { return {a[0] - b[0], a[1] - b[1], a[2] - b[2]}; }
int64_t dot3(V3 a, V3 b) { return a[0] * b[0] + a[1] * b[1] + a[2] * b[2]; }

const int dvec[6][3] = {{1, 0, 0},  {-1, 0, 0}, {0, 1, 0},
                        {0, -1, 0}, {0, 0, 1},  {0, 0, -1}};

int opp(int d) { return d ^ 1; }

int n;
vector<V3> centers;
V3 src_pt, dst_pt;

vector<V3> face_center;
vector<int> face_dir;
vector<array<int, 3>> face_grid;
vector<array<V3, 4>> face_corner;
map<array<int, 4>, int> face_id;
vector<array<int, 4>> nb_face;
vector<array<int, 4>> nb_entry;
vector<int> sheet_of;
int num_sheets;
int face_of_src, face_of_dst;

set<array<int, 3>> grid_set;

bool present(int gx, int gy, int gz) {
    return grid_set.count({gx, gy, gz}) > 0;
}

void read() {
    cin >> n;
    centers.resize(n);
    for(int i = 0; i < n; i++) {
        cin >> centers[i][0] >> centers[i][1] >> centers[i][2];
    }

    cin >> src_pt[0] >> src_pt[1] >> src_pt[2];
    cin >> dst_pt[0] >> dst_pt[1] >> dst_pt[2];
}

void build_faces() {
    for(auto& c: centers) {
        grid_set.insert(
            {(int)(c[0] / 100), (int)(c[1] / 100), (int)(c[2] / 100)}
        );
    }

    for(auto& c: centers) {
        int gx = c[0] / 100, gy = c[1] / 100, gz = c[2] / 100;
        for(int d = 0; d < 6; d++) {
            if(present(gx + dvec[d][0], gy + dvec[d][1], gz + dvec[d][2])) {
                continue;
            }

            V3 fc = {
                c[0] + 50 * dvec[d][0], c[1] + 50 * dvec[d][1],
                c[2] + 50 * dvec[d][2]
            };
            int axis = d / 2;
            int la = -1, lb = -1;
            for(int a = 0; a < 3; a++) {
                if(a == axis) {
                    continue;
                }

                if(la < 0) {
                    la = a;
                } else {
                    lb = a;
                }
            }

            const int signs[4][2] = {{-1, -1}, {1, -1}, {1, 1}, {-1, 1}};
            array<V3, 4> corners;
            for(int k = 0; k < 4; k++) {
                V3 p = fc;
                p[la] += 50 * signs[k][0];
                p[lb] += 50 * signs[k][1];
                corners[k] = p;
            }

            face_id[{gx, gy, gz, d}] = face_center.size();
            face_center.push_back(fc);
            face_dir.push_back(d);
            face_grid.push_back({gx, gy, gz});
            face_corner.push_back(corners);
        }
    }
}

void build_adjacency() {
    int nf = face_center.size();
    nb_face.resize(nf);
    nb_entry.resize(nf);
    for(int f = 0; f < nf; f++) {
        int d = face_dir[f];
        auto g = face_grid[f];
        for(int e = 0; e < 4; e++) {
            V3 a = face_corner[f][e];
            V3 b = face_corner[f][(e + 1) % 4];
            V3 mid = {(a[0] + b[0]) / 2, (a[1] + b[1]) / 2, (a[2] + b[2]) / 2};
            V3 delta = sub3(mid, face_center[f]);
            int t = -1;
            for(int dd = 0; dd < 6; dd++) {
                if(delta[0] == 50 * dvec[dd][0] &&
                   delta[1] == 50 * dvec[dd][1] &&
                   delta[2] == 50 * dvec[dd][2]) {
                    t = dd;
                }
            }

            int ax = g[0] + dvec[t][0], ay = g[1] + dvec[t][1],
                az = g[2] + dvec[t][2];
            int cx = ax + dvec[d][0], cy = ay + dvec[d][1],
                cz = az + dvec[d][2];

            array<int, 4> key;
            if(!present(ax, ay, az)) {
                key = {g[0], g[1], g[2], t};
            } else if(!present(cx, cy, cz)) {
                key = {ax, ay, az, d};
            } else {
                key = {cx, cy, cz, opp(t)};
            }

            int gface = face_id[key];
            nb_face[f][e] = gface;

            int found = -1;
            for(int k = 0; k < 4; k++) {
                V3 p = face_corner[gface][k];
                V3 q = face_corner[gface][(k + 1) % 4];
                if((p == a && q == b) || (p == b && q == a)) {
                    found = k;
                }
            }

            nb_entry[f][e] = found;
        }
    }
}

int corner_in(int g, V3 v) {
    for(int k = 0; k < 4; k++) {
        if(face_corner[g][k] == v) {
            return k;
        }
    }

    return -1;
}

void build_sheets() {
    int nf = face_center.size();
    vector<int> par(nf * 4);
    iota(par.begin(), par.end(), 0);
    function<int(int)> find = [&](int x) {
        while(par[x] != x) {
            x = par[x] = par[par[x]];
        }

        return x;
    };

    auto uni = [&](int a, int b) { par[find(a)] = find(b); };

    for(int f = 0; f < nf; f++) {
        for(int k = 0; k < 4; k++) {
            V3 v = face_corner[f][k];
            for(int e: {k, (k + 3) % 4}) {
                int g = nb_face[f][e];
                int kk = corner_in(g, v);
                uni(f * 4 + k, g * 4 + kk);
            }
        }
    }

    sheet_of.assign(nf * 4, -1);
    num_sheets = 0;
    map<int, int> remap;
    for(int i = 0; i < nf * 4; i++) {
        int r = find(i);
        if(!remap.count(r)) {
            remap[r] = num_sheets++;
        }
        sheet_of[i] = remap[r];
    }
}

int find_face_of(V3 p) {
    int nf = face_center.size();
    for(int f = 0; f < nf; f++) {
        V3 fc = face_center[f];
        int d = face_dir[f];
        int axis = d / 2;
        if(p[axis] != fc[axis]) {
            continue;
        }

        bool ok = true;
        for(int a = 0; a < 3; a++) {
            if(a == axis) {
                continue;
            }
            if(llabs(p[a] - fc[a]) >= 50) {
                ok = false;
            }
        }
        if(ok) {
            return f;
        }
    }

    return -1;
}

const Point canon[4] = {{0, 0}, {100, 0}, {100, 100}, {0, 100}};

Point to_frame(V3 p, int f, const array<Point, 4>& c2) {
    V3 o = face_corner[f][0];
    coord_t alpha =
        (coord_t)dot3(sub3(p, o), sub3(face_corner[f][1], o)) / 10000.0;
    coord_t beta =
        (coord_t)dot3(sub3(p, o), sub3(face_corner[f][3], o)) / 10000.0;
    return c2[0] + (c2[1] - c2[0]) * alpha + (c2[3] - c2[0]) * beta;
}

bool in_sector(Point apex, Point lo, Point hi, Point pt) {
    Point dl = lo - apex, dr = hi - apex;
    if((dl ^ dr) < 0) {
        swap(dl, dr);
    }

    Point v = pt - apex;
    coord_t tol = 1e-9 * v.norm() + 1e-12;
    coord_t tl = tol * dl.norm();
    coord_t tr = tol * dr.norm();
    return (dl ^ v) >= -tl && (dr ^ v) <= tr;
}

void clip_half(coord_t a, coord_t b, coord_t& s0, coord_t& s1) {
    if(fabs(b) < 1e-15) {
        if(a < -1e-12) {
            s0 = 1;
            s1 = 0;
        }
        return;
    }

    coord_t r = -a / b;
    if(b > 0) {
        s0 = max(s0, r);
    } else {
        s1 = min(s1, r);
    }
}

bool clip_sector(
    Point apex, Point lo, Point hi, Point c, Point d, Point& q0, Point& q1
) {
    Point dl = lo - apex, dr = hi - apex;
    if((dl ^ dr) < 0) {
        swap(dl, dr);
    }

    coord_t s0 = 0, s1 = 1;
    clip_half(dl ^ (c - apex), dl ^ (d - c), s0, s1);
    clip_half(-(dr ^ (c - apex)), -(dr ^ (d - c)), s0, s1);
    if(s0 > s1 - 1e-12) {
        return false;
    }

    q0 = c + (d - c) * s0;
    q1 = c + (d - c) * s1;
    return true;
}

coord_t point_seg_dist(Point p, Point a, Point b) {
    Point ab = b - a;
    coord_t len = ab * ab;
    if(len < 1e-18) {
        return (p - a).norm();
    }

    coord_t t = ((p - a) * ab) / len;
    t = max((coord_t)0.0, min((coord_t)1.0, t));
    return (p - (a + ab * t)).norm();
}

Point place_corner(V3 p0, V3 p1, V3 q, Point m0, Point m1, Point apex) {
    coord_t along = (coord_t)dot3(sub3(q, p0), sub3(p1, p0)) / 100.0;
    coord_t dsq = (coord_t)dot3(sub3(q, p0), sub3(q, p0));
    coord_t h = sqrt(max((coord_t)0.0, dsq - along * along));
    Point u = (m1 - m0) * (1.0 / 100.0);
    Point base = m0 + u * along;
    Point cand1 = base + u.perp() * h;
    Point cand2 = base - u.perp() * h;
    coord_t side = (m1 - m0) ^ (apex - m0);
    coord_t s1 = (m1 - m0) ^ (cand1 - m0);
    if((side >= 0) == (s1 >= 0)) {
        return cand2;
    }
    return cand1;
}

struct Win {
    int face, entry, depth;
    Point apex, lo, hi;
    array<Point, 4> c2;
};

coord_t ub;

void run_field(
    const vector<pair<int, Point>>& starts, int source_node,
    vector<coord_t>& dist
) {
    int total = num_sheets + 2;
    dist.assign(total, 1e18);
    dist[source_node] = 0;

    vector<Win> stk;
    for(auto& st: starts) {
        Win w;
        w.face = st.first;
        w.entry = -1;
        w.depth = 0;
        w.apex = st.second;
        w.lo = w.hi = Point(0, 0);
        w.c2 = {canon[0], canon[1], canon[2], canon[3]};
        stk.push_back(w);
    }

    while(!stk.empty()) {
        Win w = stk.back();
        stk.pop_back();
        int f = w.face;

        for(int k = 0; k < 4; k++) {
            Point pt = w.c2[k];
            if(w.entry == -1 || in_sector(w.apex, w.lo, w.hi, pt)) {
                coord_t dd = (pt - w.apex).norm();
                int node = sheet_of[f * 4 + k];
                dist[node] = min(dist[node], dd);
            }
        }

        if(face_of_src == f) {
            Point sp = to_frame(src_pt, f, w.c2);
            if(w.entry == -1 || in_sector(w.apex, w.lo, w.hi, sp)) {
                dist[num_sheets] = min(dist[num_sheets], (sp - w.apex).norm());
            }
        }

        if(face_of_dst == f) {
            Point dp = to_frame(dst_pt, f, w.c2);
            if(w.entry == -1 || in_sector(w.apex, w.lo, w.hi, dp)) {
                dist[num_sheets + 1] =
                    min(dist[num_sheets + 1], (dp - w.apex).norm());
            }
        }

        for(int e = 0; e < 4; e++) {
            if(e == w.entry) {
                continue;
            }

            Point c = w.c2[e], d = w.c2[(e + 1) % 4];
            Point q0, q1;
            if(w.entry == -1) {
                if((c - w.apex).norm() < 1e-9 || (d - w.apex).norm() < 1e-9) {
                    continue;
                }
                q0 = c;
                q1 = d;
            } else if(!clip_sector(w.apex, w.lo, w.hi, c, d, q0, q1)) {
                continue;
            }

            if(point_seg_dist(w.apex, q0, q1) > ub + 1e-6) {
                continue;
            }

            if(w.depth + 1 > 256) {
                continue;
            }

            int g = nb_face[f][e];
            int eg = nb_entry[f][e];
            V3 p0 = face_corner[f][e];
            V3 p1 = face_corner[f][(e + 1) % 4];
            Point m0 = c, m1 = d;

            array<Point, 4> c2g;
            for(int k = 0; k < 4; k++) {
                V3 cc = face_corner[g][k];
                if(cc == p0) {
                    c2g[k] = m0;
                } else if(cc == p1) {
                    c2g[k] = m1;
                } else {
                    c2g[k] = place_corner(p0, p1, cc, m0, m1, w.apex);
                }
            }

            Win nw;
            nw.face = g;
            nw.entry = eg;
            nw.depth = w.depth + 1;
            nw.apex = w.apex;
            nw.lo = q0;
            nw.hi = q1;
            nw.c2 = c2g;
            stk.push_back(nw);
        }
    }
}

void compute_ub() {
    int nf = face_center.size();
    int total = nf + 2;
    vector<vector<pair<int, coord_t>>> g(total);
    for(int f = 0; f < nf; f++) {
        for(int e = 0; e < 4; e++) {
            g[f].push_back({nb_face[f][e], 100.0});
        }
    }

    auto dist3 = [&](V3 a, V3 b) {
        V3 dd = sub3(a, b);
        return (coord_t)sqrt((coord_t)dot3(dd, dd));
    };

    g[nf].push_back({face_of_src, dist3(src_pt, face_center[face_of_src])});
    g[face_of_src].push_back({nf, dist3(src_pt, face_center[face_of_src])});
    g[nf + 1].push_back({face_of_dst, dist3(dst_pt, face_center[face_of_dst])});
    g[face_of_dst].push_back({nf + 1, dist3(dst_pt, face_center[face_of_dst])});

    vector<coord_t> dist(total, 1e18);
    priority_queue<pair<coord_t, int>, vector<pair<coord_t, int>>, greater<>>
        pq;
    dist[nf] = 0;
    pq.push({0, nf});
    while(!pq.empty()) {
        auto [c, u] = pq.top();
        pq.pop();
        if(c > dist[u] + 1e-9) {
            continue;
        }

        for(auto& [v, w]: g[u]) {
            if(dist[v] > dist[u] + w + 1e-9) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }

    ub = dist[nf + 1];
}

void solve() {
    // The robot lives on the surface of the polycube, which is a closed
    // manifold tiled by square faces of side 100 - exactly the cube faces that
    // are not glued to another cube. Two faces that share an edge are walkable
    // across that edge, and which face you arrive on is decided locally by the
    // three cubes around the edge. For a face on cube C with outward normal d,
    // step across the edge lying towards a tangent direction t and look at the
    // side cube A = C + t and the diagonal cube B = C + t + d:
    //
    //     - if A is missing you fold over C's own edge onto face (C, t), a
    //       convex 90 degree turn (rule a);
    //
    //     - else if B is missing you cross flat onto the coplanar face (A, d),
    //       a straight 180 degree step (rule b);
    //
    //     - else you turn into the concave corner onto face (B, -t), a 270
    //       degree step (rule c).
    //
    // Every surface edge falls into exactly one of these cases, so the gluing
    // is unambiguous; in particular two cubes that touch only along an edge are
    // never joined, because each of their faces folds back onto its own cube.
    //
    // A shortest route on such a surface is a geodesic: unfold the strip of
    // faces it crosses into the plane and it becomes a straight segment, and it
    // can change direction only at a vertex of the polycube (a cube corner). We
    // therefore turn the problem into a graph whose nodes are the surface
    // vertices together with the start S and the goal D, and whose edges carry
    // the length of the straight geodesic that joins two nodes while passing
    // through face interiors only. Dijkstra over this graph reconstructs any
    // geodesic by chaining these straight pieces between consecutive bends.
    //
    // A vertex is not always a single node. Around a corner the incident faces
    // may split into several sheets that meet only at the point itself - the
    // touching tips of two cubes glued along an edge are the simplest example -
    // and the robot may walk within one sheet but cannot cross between sheets
    // through the bare point. We group the incident face-corners into connected
    // sheets using the edge gluing above and make each sheet its own node,
    // which stops such a pinch from leaking a free shortcut across the gap.
    //
    // The edge lengths come from a single-source sweep that unfolds the surface
    // on the fly. Starting from a point (or from a vertex sheet, which radiates
    // into each of its incident faces) we carry the unfolded image of the
    // source in one common plane and walk outward face by face, keeping for the
    // current face the visible angular cone seen from that image. Crossing into
    // a neighbour we place its far corners from their known distances to the
    // shared edge, then clip the cone against each onward edge; whenever the
    // cone covers a vertex or the target we record the Euclidean distance to
    // its unfolded image. The cone only narrows as it advances, so a window
    // whose nearest point already lies past an upper bound on the answer -
    // taken from a coarse path through the face centres - can never improve any
    // node and is dropped, which is what keeps the otherwise unbounded sweep
    // finite.

    build_faces();
    build_adjacency();
    build_sheets();

    face_of_src = find_face_of(src_pt);
    face_of_dst = find_face_of(dst_pt);

    compute_ub();

    int nf = face_center.size();
    vector<vector<pair<int, int>>> incident(num_sheets);
    for(int f = 0; f < nf; f++) {
        for(int k = 0; k < 4; k++) {
            incident[sheet_of[f * 4 + k]].push_back({f, k});
        }
    }

    int total = num_sheets + 2;
    int src_node = num_sheets, dst_node = num_sheets + 1;

    vector<vector<coord_t>> field(total);
    for(int v = 0; v < num_sheets; v++) {
        vector<pair<int, Point>> starts;
        for(auto& [f, k]: incident[v]) {
            starts.push_back({f, canon[k]});
        }
        run_field(starts, v, field[v]);
    }

    {
        array<Point, 4> c2 = {canon[0], canon[1], canon[2], canon[3]};
        Point sp = to_frame(src_pt, face_of_src, c2);
        run_field({{face_of_src, sp}}, src_node, field[src_node]);
    }

    vector<coord_t> dist(total, 1e18);
    priority_queue<pair<coord_t, int>, vector<pair<coord_t, int>>, greater<>>
        pq;
    dist[src_node] = 0;
    pq.push({0, src_node});
    while(!pq.empty()) {
        auto [c, u] = pq.top();
        pq.pop();
        if(c > dist[u] + 1e-12) {
            continue;
        }

        if(u == dst_node) {
            continue;
        }

        for(int v = 0; v < total; v++) {
            coord_t w = field[u][v];
            if(w > 1e17) {
                continue;
            }

            if(dist[v] > dist[u] + w + 1e-12) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }

    printf("%.10f\n", dist[dst_node]);
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
