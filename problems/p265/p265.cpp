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

const double PI = acos(-1.0);

namespace fastio {
constexpr int BUF_SIZE = 1 << 16;
char buf[BUF_SIZE];
int buf_pos = 0, buf_len = 0;

int get_char() {
    if(buf_pos == buf_len) {
        buf_len = (int)fread(buf, 1, BUF_SIZE, stdin);
        buf_pos = 0;
    }
    return buf_pos < buf_len ? (unsigned char)buf[buf_pos++] : EOF;
}

int skip_ws() {
    int c = get_char();
    while(c == ' ' || c == '\n' || c == '\r' || c == '\t') {
        c = get_char();
    }
    return c;
}

int read_int() {
    int c = skip_ws();
    int sign = 1;
    if(c == '-') {
        sign = -1;
        c = get_char();
    } else if(c == '+') {
        c = get_char();
    }
    int x = 0;
    while(c >= '0' && c <= '9') {
        x = x * 10 + (c - '0');
        c = get_char();
    }
    return sign * x;
}

double read_double() {
    int c = skip_ws();
    int sign = 1;
    if(c == '-') {
        sign = -1;
        c = get_char();
    } else if(c == '+') {
        c = get_char();
    }
    double x = 0.0;
    while(c >= '0' && c <= '9') {
        x = x * 10.0 + (c - '0');
        c = get_char();
    }
    if(c == '.') {
        c = get_char();
        double f = 0.1;
        while(c >= '0' && c <= '9') {
            x += (c - '0') * f;
            f *= 0.1;
            c = get_char();
        }
    }
    return sign * x;
}

char read_char() { return (char)skip_ws(); }
}  // namespace fastio

int n, m;
vector<tuple<char, array<double, 4>>> spells;
vector<array<double, 3>> points;

void read() {
    n = fastio::read_int();
    spells.assign(n, {});
    for(auto& [c, params]: spells) {
        c = fastio::read_char();
        int k = (c == 'R') ? 4 : 3;
        for(int i = 0; i < k; i++) {
            params[i] = fastio::read_double();
        }
    }

    m = fastio::read_int();
    points.assign(m, {});
    for(auto& p: points) {
        p[0] = fastio::read_double();
        p[1] = fastio::read_double();
        p[2] = fastio::read_double();
    }
}

void solve() {
    // Each basic spell is an affine map on R^3, so on the homogeneous vector
    // (x, y, z, 1) it becomes a plain linear map - a 4x4 matrix. Concretely:
    //
    //   Translation(tx,ty,tz): identity 4x4 with (tx,ty,tz) in the last column.
    //   Scaling(sx,sy,sz):     diag(sx, sy, sz, 1).
    //   Rotation(axis, angle): Rodrigues' formula in the upper-left 3x3, with
    //                          the last row and column kept as (0,0,0,1).
    //
    // Composing the whole spell list is one big 4x4 product M = T_n * ... * T_1
    // (each new spell multiplies on the left because it is applied after all
    // earlier ones), and producing the final coordinates for every vertex is
    // just M * (x, y, z, 1)^T. So we pay O(N) for the composition and O(M)
    // for the application, independently of each other.
    //
    // Translation and scaling are immediate from the definitions above - a
    // translation adds the vector to (x,y,z) and a scaling multiplies each
    // coordinate by its factor, which is exactly what the matrices encode.
    // Rotation is the interesting one because the axis is arbitrary, not one
    // of the coordinate axes. We first normalize the axis to a unit vector
    // u = (ux, uy, uz) (the input only guarantees x^2 + y^2 + z^2 > 0). Then
    // Rodrigues' formula gives the 3x3 rotation matrix R (what we drop into
    // the upper-left of t) by angle a around u as
    //
    //   R = I + sin(a) * K + (1 - cos(a)) * K^2,
    //
    // where I is the 3x3 identity and K is the cross-product matrix of u
    // (so K*v = u x v for every vector v):
    //
    //          [  0  -uz   uy ]
    //      K = [ uz    0  -ux ].
    //          [-uy   ux    0 ]
    //
    // Geometrically: any vector v splits into a part parallel to u (which is
    // fixed by the rotation) and a part perpendicular to u (which lives in
    // the plane orthogonal to u and is rotated there by angle a using the
    // 2D rotation written in the orthonormal pair (v_perp, u x v_perp));
    // expanding that gives exactly the R above. We hardcode the resulting
    // nine entries of R directly into the 3x3 block of t below to avoid
    // building K and K^2 explicitly.
    //
    // The problem says rotation is "clockwise about the ray from origin
    // through the point". Standard Rodrigues with a right-handed axis already
    // matches this when you look along the ray from the origin outward (it
    // matches the sample with R about +y by 90 sending (1,0,0) to (0,0,-1)).

    auto id = []() {
        array<array<double, 4>, 4> r{};
        for(int i = 0; i < 4; i++) {
            r[i][i] = 1.0;
        }
        return r;
    };

    auto mul = [](const array<array<double, 4>, 4>& a,
                  const array<array<double, 4>, 4>& b) {
        array<array<double, 4>, 4> r{};
        for(int i = 0; i < 4; i++) {
            for(int j = 0; j < 4; j++) {
                double s = 0.0;
                for(int k = 0; k < 4; k++) {
                    s += a[i][k] * b[k][j];
                }
                r[i][j] = s;
            }
        }
        return r;
    };

    auto M = id();
    for(auto& [c, p]: spells) {
        auto t = id();
        if(c == 'T') {
            t[0][3] = p[0];
            t[1][3] = p[1];
            t[2][3] = p[2];
        } else if(c == 'S') {
            t[0][0] = p[0];
            t[1][1] = p[1];
            t[2][2] = p[2];
        } else {
            double x = p[0], y = p[1], z = p[2];
            double len = sqrt(x * x + y * y + z * z);
            x /= len;
            y /= len;
            z /= len;

            double rad = p[3] * PI / 180.0;
            double co = cos(rad), si = sin(rad);
            t[0][0] = co + x * x * (1 - co);
            t[0][1] = x * y * (1 - co) - z * si;
            t[0][2] = x * z * (1 - co) + y * si;
            t[1][0] = y * x * (1 - co) + z * si;
            t[1][1] = co + y * y * (1 - co);
            t[1][2] = y * z * (1 - co) - x * si;
            t[2][0] = z * x * (1 - co) - y * si;
            t[2][1] = z * y * (1 - co) + x * si;
            t[2][2] = co + z * z * (1 - co);
        }

        M = mul(t, M);
    }

    cout << fixed << setprecision(2);
    for(auto& v: points) {
        double x = M[0][0] * v[0] + M[0][1] * v[1] + M[0][2] * v[2] + M[0][3];
        double y = M[1][0] * v[0] + M[1][1] * v[1] + M[1][2] * v[2] + M[1][3];
        double z = M[2][0] * v[0] + M[2][1] * v[1] + M[2][2] * v[2] + M[2][3];
        cout << x << ' ' << y << ' ' << z << '\n';
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
