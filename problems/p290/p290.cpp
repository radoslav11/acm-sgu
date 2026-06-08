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

using coord_t = int64_t;

struct Point3D {
    coord_t x, y, z;

    Point3D(coord_t x = 0, coord_t y = 0, coord_t z = 0) : x(x), y(y), z(z) {}

    Point3D operator+(const Point3D& p) const {
        return {x + p.x, y + p.y, z + p.z};
    }
    Point3D operator-(const Point3D& p) const {
        return {x - p.x, y - p.y, z - p.z};
    }
    Point3D cross(const Point3D& p) const {
        return {y * p.z - z * p.y, z * p.x - x * p.z, x * p.y - y * p.x};
    }
    coord_t dot(const Point3D& p) const { return x * p.x + y * p.y + z * p.z; }
    bool operator==(const Point3D& p) const {
        return x == p.x && y == p.y && z == p.z;
    }

    friend coord_t signed_volume(
        const Point3D& a, const Point3D& b, const Point3D& c, const Point3D& d
    ) {
        return (b - a).cross(c - a).dot(d - a);
    }
};

class QuickHull3D {
  public:
    struct Face {
        array<int, 3> v;
        array<int, 3> nbr;
        vector<int> outside;
        bool alive;
    };

    vector<Point3D> points;
    vector<Face> faces;
    bool degenerate;

    QuickHull3D(const vector<Point3D>& pts) : points(pts), degenerate(false) {
        build();
    }

    bool on_boundary(const Point3D& p) const {
        if(degenerate) {
            return true;
        }
        for(const auto& f: faces) {
            if(!f.alive) {
                continue;
            }
            if(signed_volume(
                   points[f.v[0]], points[f.v[1]], points[f.v[2]], p
               ) == 0) {
                return true;
            }
        }
        return false;
    }

  private:
    coord_t above(int fi, int pi) const {
        const auto& f = faces[fi];
        return signed_volume(
            points[f.v[0]], points[f.v[1]], points[f.v[2]], points[pi]
        );
    }

    int add_face(int a, int b, int c) {
        faces.push_back({{a, b, c}, {-1, -1, -1}, {}, true});
        return (int)faces.size() - 1;
    }

    int find_edge(int fi, int u, int v) const {
        for(int e = 0; e < 3; e++) {
            if(faces[fi].v[e] == u && faces[fi].v[(e + 1) % 3] == v) {
                return e;
            }
        }
        return -1;
    }

    void build() {
        int n = (int)points.size();
        if(n < 4) {
            degenerate = true;
            return;
        }

        int i0 = 0, i1 = -1, i2 = -1, i3 = -1;
        for(int i = 1; i < n; i++) {
            if(!(points[i] == points[i0])) {
                i1 = i;
                break;
            }
        }
        if(i1 < 0) {
            degenerate = true;
            return;
        }

        for(int i = 1; i < n; i++) {
            if(i == i1) {
                continue;
            }
            Point3D c = (points[i1] - points[i0]).cross(points[i] - points[i0]);
            if(c.dot(c) > 0) {
                i2 = i;
                break;
            }
        }

        if(i2 < 0) {
            degenerate = true;
            return;
        }
        for(int i = 0; i < n; i++) {
            if(i == i0 || i == i1 || i == i2) {
                continue;
            }

            if(signed_volume(points[i0], points[i1], points[i2], points[i]) !=
               0) {
                i3 = i;
                break;
            }
        }

        if(i3 < 0) {
            degenerate = true;
            return;
        }

        if(signed_volume(points[i0], points[i1], points[i2], points[i3]) < 0) {
            swap(i1, i2);
        }

        int f0 = add_face(i0, i2, i1);
        int f1 = add_face(i1, i2, i3);
        int f2 = add_face(i0, i3, i2);
        int f3 = add_face(i0, i1, i3);

        faces[f0].nbr = {f2, f1, f3};
        faces[f1].nbr = {f0, f2, f3};
        faces[f2].nbr = {f3, f1, f0};
        faces[f3].nbr = {f0, f1, f2};

        for(int p = 0; p < n; p++) {
            if(p == i0 || p == i1 || p == i2 || p == i3) {
                continue;
            }

            for(int fi = 0; fi < 4; fi++) {
                if(above(fi, p) > 0) {
                    faces[fi].outside.push_back(p);
                    break;
                }
            }
        }

        queue<int> q;
        for(int fi = 0; fi < 4; fi++) {
            if(!faces[fi].outside.empty()) {
                q.push(fi);
            }
        }

        while(!q.empty()) {
            int fi = q.front();
            q.pop();
            if(!faces[fi].alive || faces[fi].outside.empty()) {
                continue;
            }

            int apex = faces[fi].outside[0];
            coord_t best = above(fi, apex);
            for(int p: faces[fi].outside) {
                coord_t d = above(fi, p);
                if(d > best) {
                    best = d;
                    apex = p;
                }
            }

            vector<int> visible = {fi};
            vector<char> seen(faces.size(), 0);
            seen[fi] = 1;
            vector<array<int, 4>> horizon;

            for(int idx = 0; idx < (int)visible.size(); idx++) {
                int cur = visible[idx];
                for(int e = 0; e < 3; e++) {
                    int nb = faces[cur].nbr[e];
                    if(nb < 0 || seen[nb]) {
                        continue;
                    }

                    if(above(nb, apex) > 0) {
                        seen[nb] = 1;
                        visible.push_back(nb);
                    } else {
                        int u = faces[cur].v[e];
                        int v = faces[cur].v[(e + 1) % 3];
                        int ne = find_edge(nb, v, u);
                        horizon.push_back({u, v, nb, ne});
                    }
                }
            }

            vector<int> orphans;
            for(int vf: visible) {
                for(int p: faces[vf].outside) {
                    if(p != apex) {
                        orphans.push_back(p);
                    }
                }

                faces[vf].outside.clear();
                faces[vf].alive = false;
            }

            vector<int> new_faces;
            map<pair<int, int>, int> apex_edge;
            for(auto& h: horizon) {
                int u = h[0], v = h[1], nb = h[2], ne = h[3];
                int nf = add_face(apex, u, v);
                faces[nf].nbr[1] = nb;
                faces[nb].nbr[ne] = nf;
                apex_edge[{apex, u}] = nf;
                apex_edge[{v, apex}] = nf;
                new_faces.push_back(nf);
            }

            for(int nf: new_faces) {
                int u = faces[nf].v[1];
                int v = faces[nf].v[2];
                auto it1 = apex_edge.find({u, apex});
                if(it1 != apex_edge.end()) {
                    faces[nf].nbr[0] = it1->second;
                }

                auto it2 = apex_edge.find({apex, v});
                if(it2 != apex_edge.end()) {
                    faces[nf].nbr[2] = it2->second;
                }
            }

            for(int p: orphans) {
                for(int nf: new_faces) {
                    if(above(nf, p) > 0) {
                        faces[nf].outside.push_back(p);
                        break;
                    }
                }
            }

            for(int nf: new_faces) {
                if(!faces[nf].outside.empty()) {
                    q.push(nf);
                }
            }
        }
    }
};

int n;
vector<string> names;
vector<Point3D> pts;

void read() {
    string line;
    getline(cin, line);
    n = stoi(line);
    names.assign(n, "");
    pts.assign(n, {});
    for(int i = 0; i < n; i++) {
        getline(cin, line);
        while(!line.empty() && (line.back() == '\r' || line.back() == ' ')) {
            line.pop_back();
        }

        auto sp = line.find_last_of(' ');
        coord_t z = stoll(line.substr(sp + 1));
        line.resize(sp);
        sp = line.find_last_of(' ');
        coord_t y = stoll(line.substr(sp + 1));
        line.resize(sp);
        sp = line.find_last_of(' ');
        coord_t x = stoll(line.substr(sp + 1));
        names[i] = line.substr(0, sp);
        pts[i] = {x, y, z};
    }
}

void solve() {
    // A star needs a laser control tower iff it lies on the boundary of the
    // 3D convex hull of all stars: a star strictly inside the hull is shielded
    // by the laser triangles, while any star on a face, edge or vertex of the
    // hull is touched by some laser triangle and must host its own tower.
    //
    // We build the hull with a 3D Quickhull over integer coordinates. The hull
    // is stored as triangular faces with outward normals and edge-neighbor
    // pointers; the initial tetrahedron uses the first four non-coplanar
    // input points, and at each iteration we pop a face with a non-empty
    // outside set, pick its furthest point, BFS the connected region of
    // visible faces, collect the horizon edges, and replace the visible
    // region by a fan of new triangles incident to the chosen apex. Orphan
    // outside points of the deleted faces are redistributed to the new ones.
    //
    // A point P lies on the boundary iff the signed tetrahedron volume of
    // some hull face together with P vanishes. Coplanar hull faces (when the
    // input is non-generic) are kept by the algorithm; they share a plane,
    // so the same zero-volume test still recognises every coplanar boundary
    // point. Degenerate inputs (fewer than four non-coplanar points) are
    // 0-, 1- or 2-dimensional, so every star is on the boundary.
    //
    // More details can be found in the Quickhull algorithm from Barber, Dobkin,
    // Huhdanpaa, "The Quickhull Algorithm for Convex Hulls", ACM TOMS 22(4),
    // 1996: https://doi.org/10.1145/235815.235821.

    QuickHull3D hull(pts);
    vector<string> answer;
    for(int i = 0; i < n; i++) {
        if(hull.on_boundary(pts[i])) {
            answer.push_back(names[i]);
        }
    }

    sort(answer.begin(), answer.end());
    cout << answer.size() << '\n';
    for(auto& s: answer) {
        cout << s << '\n';
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
