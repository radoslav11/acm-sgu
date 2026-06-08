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

long double W, H, D;
int n;
vector<array<long double, 4>> ghosts;
array<long double, 3> witness;

void read() {
    cin >> W >> H >> D >> n;
    ghosts.assign(n, {});
    for(auto& g: ghosts) {
        cin >> g[0] >> g[1] >> g[2] >> g[3];
    }
}

using vec = array<long double, 3>;

vec operator-(vec a, vec b) { return {a[0] - b[0], a[1] - b[1], a[2] - b[2]}; }

long double dot(vec a, vec b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

vec cross(vec a, vec b) {
    return {
        a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2],
        a[0] * b[1] - a[1] * b[0]
    };
}

bool valid_center(vec p, long double r) {
    long double eps = 1e-6L;
    if(p[0] < r - eps || p[0] > W - r + eps || p[1] < r - eps ||
       p[1] > H - r + eps || p[2] < r - eps || p[2] > D - r + eps) {
        return false;
    }
    for(auto& g: ghosts) {
        long double dx = p[0] - g[0], dy = p[1] - g[1], dz = p[2] - g[2];
        long double rho = r + g[3];
        if(sqrtl(dx * dx + dy * dy + dz * dz) < rho - eps) {
            return false;
        }
    }
    return true;
}

bool line_sphere(
    vec n1, long double d1, vec n2, long double d2, vec c, long double rho,
    long double r
) {
    vec u = cross(n1, n2);
    long double lu2 = dot(u, u);
    if(lu2 < 1e-18L) {
        return false;
    }

    vec p0;
    vec t1 = cross(n2, u), t2 = cross(u, n1);
    for(int a = 0; a < 3; a++) {
        p0[a] = (d1 * t1[a] + d2 * t2[a]) / lu2;
    }

    vec w = p0 - c;
    long double b = 2 * dot(u, w);
    long double cc = dot(w, w) - rho * rho;
    long double disc = b * b - 4 * lu2 * cc;
    if(disc < 0) {
        disc = 0;
    }
    long double sq = sqrtl(disc);
    for(int s = -1; s <= 1; s += 2) {
        long double t = (-b + s * sq) / (2 * lu2);
        vec p = {p0[0] + t * u[0], p0[1] + t * u[1], p0[2] + t * u[2]};
        if(valid_center(p, r)) {
            witness = p;
            return true;
        }
    }
    return false;
}

bool feasible(long double r) {
    if(2 * r > W || 2 * r > H || 2 * r > D) {
        return false;
    }

    int m = 6 + n;
    vec pn[6] = {{1, 0, 0}, {1, 0, 0}, {0, 1, 0},
                 {0, 1, 0}, {0, 0, 1}, {0, 0, 1}};
    long double pd[6] = {r, W - r, r, H - r, r, D - r};

    auto sphere_center = [&](int id) -> vec {
        return {ghosts[id - 6][0], ghosts[id - 6][1], ghosts[id - 6][2]};
    };

    auto sphere_rho = [&](int id) -> long double {
        return r + ghosts[id - 6][3];
    };

    for(int a = 0; a < m; a++) {
        for(int b = a + 1; b < m; b++) {
            for(int c = b + 1; c < m; c++) {
                int id[3] = {a, b, c};
                int sph = -1;
                for(int t = 0; t < 3; t++) {
                    if(id[t] >= 6) {
                        sph = id[t];
                    }
                }

                if(sph == -1) {
                    if(id[0] / 2 != id[1] / 2 && id[1] / 2 != id[2] / 2 &&
                       id[0] / 2 != id[2] / 2) {
                        vec p;
                        p[id[0] / 2] = pd[id[0]];
                        p[id[1] / 2] = pd[id[1]];
                        p[id[2] / 2] = pd[id[2]];
                        if(valid_center(p, r)) {
                            witness = p;
                            return true;
                        }
                    }

                    continue;
                }

                vec rc = sphere_center(sph);
                long double rrho = sphere_rho(sph);
                vec pl[2];
                long double pld[2];
                int cnt = 0;
                for(int t = 0; t < 3; t++) {
                    if(id[t] == sph) {
                        continue;
                    }

                    if(id[t] < 6) {
                        pl[cnt] = pn[id[t]];
                        pld[cnt] = pd[id[t]];
                    } else {
                        vec oc = sphere_center(id[t]);
                        long double orho = sphere_rho(id[t]);
                        pl[cnt] = {
                            2 * (oc[0] - rc[0]), 2 * (oc[1] - rc[1]),
                            2 * (oc[2] - rc[2])
                        };

                        pld[cnt] = rrho * rrho - orho * orho - dot(rc, rc) +
                                   dot(oc, oc);
                    }

                    cnt++;
                }

                if(line_sphere(pl[0], pld[0], pl[1], pld[1], rc, rrho, r)) {
                    return true;
                }
            }
        }
    }

    return false;
}

void solve() {
    // The achievable radius is monotone, so we binary search it: a candidate
    // radius r works iff some center can host a sphere of that radius. Fixing r
    // pins the placement region exactly: the center must sit in the shrunken
    // box [r, W - r] x [r, H - r] x [r, D - r] (so the sphere clears the walls)
    // and outside every old ghost inflated by r, i.e. at distance at least
    // R_i + r from ghost i's center. So r is feasible iff this region, an
    // intersection of six half-spaces and n ball-exteriors, is non-empty.
    //
    // With N <= 10 we decide that non-emptiness exactly rather than by
    // sampling. Write the region as F(r) = { p : p in the shrunken box and |p -
    // c_i| >= R_i + r for every i }; as a closed subset of the box it is
    // compact, so a linear functional attains a maximum on it. Maximize one
    // that is generic in the sense of ordering points lexicographically by (x,
    // then y, then z), and let p* be the maximizer. The constraints active at
    // p* are the bounding surfaces passing through it; if at most two were
    // active, their zero sets would meet in a curve (or larger set) through p*
    // along which p* could move a little while staying in F(r), and any such
    // move changes some coordinate, so it strictly raises the lexicographic
    // value, contradicting maximality. Hence p* lies on at least three surfaces
    // at once, and it suffices to test only the points where three surfaces
    // meet.
    //
    // The surfaces are the 6 box planes x = r, x = W - r, ..., z = D - r and
    // the n spheres |p - c_i| = R_i + r, at most 16 in all, so we enumerate all
    // C(16, 3) triples and solve each for its meeting point(s). Three planes
    // meet in a single point only when they use three distinct axes, giving one
    // box corner. A triple with at least one sphere is linearized: keep one
    // sphere S0 as the quadric and turn the other two surfaces into planes. A
    // plane stays itself, and a second sphere Sj is replaced by its radical
    // plane with S0, the plane |p - c0|^2 - (R0 + r)^2 = |p - cj|^2 - (Rj +
    // r)^2 that holds exactly on S0 ∩ Sj. The two planes meet in a line
    // (skipped if parallel), which meets S0 in at most two points through one
    // quadratic. A candidate is accepted when it lies in the box and at
    // distance >= R_i + r from every ghost; if any triple yields such a point
    // then r is feasible, and the center kept at the largest feasible r is the
    // answer.

    long double lo = 0, hi = min({W, H, D}) / 2;
    witness = {W / 2, H / 2, D / 2};
    for(int it = 0; it < 45; it++) {
        long double mid = (lo + hi) / 2;
        if(feasible(mid)) {
            lo = mid;
        } else {
            hi = mid;
        }
    }

    feasible(lo);
    cout << fixed << setprecision(12);
    cout << witness[0] << ' ' << witness[1] << ' ' << witness[2] << '\n';
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
