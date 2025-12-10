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

const double EPS = 1e-6;

struct Vec3 {
    double x, y, z;
    Vec3(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}
    Vec3 operator+(const Vec3& o) const {
        return Vec3(x + o.x, y + o.y, z + o.z);
    }
    Vec3 operator-(const Vec3& o) const {
        return Vec3(x - o.x, y - o.y, z - o.z);
    }
    Vec3 operator*(double k) const { return Vec3(x * k, y * k, z * k); }
    double dot(const Vec3& o) const { return x * o.x + y * o.y + z * o.z; }
    double norm() const { return sqrt(dot(*this)); }
    Vec3 normalize() const {
        double n = norm();
        return Vec3(x / n, y / n, z / n);
    }
};

struct Sphere {
    Vec3 center;
    double r;
};

int n;
vector<Sphere> spheres;
Vec3 origin, dir;

double ray_sphere_intersect(Vec3 O, Vec3 D, const Sphere& s) {
    // Find parameter t where ray P(t) = O + t*D intersects sphere:
    //
    //     |P - C|^2 = r^2
    //     |O + t*D - C|^2 = r^2
    //
    // Let V = O - C, then |V + t*D|^2 = r^2.
    //
    //     (V + t*D) . (V + t*D) = r^2
    //     V . V + 2t(V . D) + t^2(D . D) = r^2
    //
    // This gives quadratic equation:
    //   a*t^2 + b*t + c = 0
    //   where a = D . D, b = 2(V . D), c = V . V - r^2
    //
    // This function returns smallest positive t, or -1 if no valid
    // intersection.

    Vec3 V = O - s.center;
    double a = D.dot(D);
    double b = 2 * V.dot(D);
    double c = V.dot(V) - s.r * s.r;
    double disc = b * b - 4 * a * c;
    if(disc < -EPS) {
        return -1;
    }
    disc = sqrt(max(0.0, disc));
    double t1 = (-b - disc) / (2 * a);
    double t2 = (-b + disc) / (2 * a);
    if(t1 > EPS) {
        return t1;
    }
    if(t2 > EPS) {
        return t2;
    }
    return -1;
}

Vec3 reflect(Vec3 D, Vec3 N) {
    // Reflect incident direction D about normal N.
    // Decompose D into parallel and perpendicular components to N:
    //   D_parallel = (D . N)N, D_perp = D - D_parallel
    // Reflection reverses the parallel component:
    //   R = D_perp - D_parallel = D - 2*D_parallel = D - 2(D . N)N

    return D - N * (2 * D.dot(N));
}

void read() {
    cin >> n;
    spheres.resize(n);
    for(int i = 0; i < n; i++) {
        cin >> spheres[i].center.x >> spheres[i].center.y >>
            spheres[i].center.z >> spheres[i].r;
    }
    double x1, y1, z1, x2, y2, z2;
    cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
    origin = Vec3(x1, y1, z1);
    dir = Vec3(x2 - x1, y2 - y1, z2 - z1).normalize();
}

void solve() {
    // We can solve this problem in a straight forward way - we just need to
    // implement some 3D geometry functions. In particular, we want to be able
    // to get intersections, tangents and reflection vectors, all of which have
    // closed forms. Then we can simulate the first 10 hits.

    vector<int> hits;
    int last_hit = -1;
    for(int step = 0; step < 11; step++) {
        double best_t = 1e18;
        int best_sphere = -1;
        for(int i = 0; i < n; i++) {
            if(i == last_hit) {
                continue;
            }
            double t = ray_sphere_intersect(origin, dir, spheres[i]);
            if(t > 0 && t < best_t) {
                best_t = t;
                best_sphere = i;
            }
        }
        if(best_sphere == -1) {
            break;
        }

        hits.push_back(best_sphere + 1);
        last_hit = best_sphere;

        Vec3 P = origin + dir * best_t;
        Vec3 N = (P - spheres[best_sphere].center).normalize();
        dir = reflect(dir, N).normalize();
        origin = P;
    }

    int output_count = min((int)hits.size(), 10);
    for(int i = 0; i < output_count; i++) {
        if(i > 0) {
            cout << " ";
        }
        cout << hits[i];
    }
    if(hits.size() > 10) {
        cout << " etc.";
    }
    cout << "\n";
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
