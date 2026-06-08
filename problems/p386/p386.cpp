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

int n;
int64_t p;
vector<vector<int64_t>> a;

void read() {
    cin >> n >> p;
    a.assign(n, vector<int64_t>(n));
    cin >> a;
}

int64_t mul_mod(int64_t x, int64_t y) { return (int64_t)((__int128)x * y % p); }

int64_t pow_mod(int64_t base, int64_t e) {
    int64_t r = 1 % p;
    base %= p;
    while(e > 0) {
        if(e & 1) {
            r = mul_mod(r, base);
        }

        base = mul_mod(base, base);
        e >>= 1;
    }

    return r;
}

void solve() {
    // Every point of the body can be written x = sum t_i v_i for one choice
    // of coefficients t_1..t_n; since the vectors are independent those
    // coefficients (call them the parameters of x) are unique. The point is
    // in the body exactly when every parameter is in [0, 1]: all-zero is the
    // origin corner, all-one is the opposite corner, and a parameter equal to
    // 0 or 1 means x sits on one of the flat walls of that direction.
    //
    // A facet is one of those flat boundary pieces: a corner (dimension 0),
    // an edge (dimension 1), a 2D face, and so on up to the solid body itself
    // (dimension n). To name a facet, pin some parameters to a wall (0 or 1)
    // and let the rest range over [0, 1]; the points you get form a facet
    // whose dimension is the number of parameters left free. The smallest
    // facet through a given x pins exactly the parameters that already sit at
    // 0 or 1 and frees the rest, so its dimension is the count of strictly
    // interior parameters
    // 
    //     k = #{ i : 0 < t_i < 1 }.
    //     
    // A corner has k = 0, a point in the solid interior has k = n, and the
    // document price 2^k is precisely the 2^0 .. 2^n scale in the statement.
    // The task is to add up 2^k over every integer point (every document)
    // whose parameters all lie in [0, 1].
    //
    // Adding 2^k over the whole body head-on is hard, because the points and
    // their prices both vary. So we compare against a region whose integer
    // points we can count exactly: the half-open box
    // 
    //     H = { sum t_i v_i : 0 <= t_i < 1 },
    //
    // i.e. the body with its far walls (the t_i = 1 ones) shaved off. Take a
    // copy of H and shift it by an integer combination of v1..vn. These
    // shifted copies cover space with no gaps or overlaps -- they tile it,
    // the way unit squares tile the plane -- and this works precisely because
    // v1..vn are independent: being a basis they give every point of space a
    // unique parameter vector, and peeling off the integer parts of those
    // parameters (which is an integer combination of the v_i) slides the
    // point into exactly one copy of H. Unique copy means total coverage with
    // no overlap, so it really is a tiling.
    //
    // Now count the grid points in one copy. Each copy is an integer
    // translate of H, and translating by an integer vector is a bijection of
    // the grid Z^n onto itself, so every copy holds the same number m of grid
    // points. Count the grid points of a large box two ways: directly it is
    // about (volume of the box), since the grid has one point per unit
    // volume; and it is also (number of copies inside) * m, with about
    // (volume of the box) / |det V| copies because each copy has volume
    // |det V|. Equating the two forces m = |det V|, so H holds exactly
    // |det V| grid points. (This is the geometry-of-numbers fact that a
    // sublattice's fundamental domain contains [Z^n : L] = |det V| grid
    // points; its two-dimensional area form is Pick's theorem.) The plan is
    // to charge the entire price sum to these |det V| points of H.
    //
    // Claim: group the body's integer points by the point of H they reduce
    // to, and every group sums to exactly 2^n in price. There are |det V|
    // groups, so the whole price sum is 2^n * |det V| -- the answer. The
    // per-group total is always the same because prices factor coordinate by
    // coordinate, and the only freedom inside a group is which wall a
    // boundary coordinate sits on.
    //
    // Reduce a body point x to its representative y in H like this: while some
    // parameter t_i equals 1, subtract v_i, which lowers that parameter to 0,
    // keeps x in the body, and ends at a y with every parameter in [0, 1).
    // Conversely, the body points that reduce to a fixed y are exactly those
    // built from y by taking each coordinate where y is 0 and setting it to 0
    // or 1 (setting it to 1 adds v_i); a coordinate of y already in (0, 1)
    // cannot move, since raising it would pass 1. So if y has z zero
    // coordinates, its group is the 2^z independent 0/1 choices on them.
    //
    // Now total the group. Write the price 2^k as one factor per coordinate:
    // 2 if that coordinate is strictly interior, 1 if it sits on a wall (0 or
    // 1). Summed over the group each coordinate contributes a factor of 2 -- a
    // coordinate with y in (0, 1) is interior in every member (factor 2), and
    // a coordinate with y = 0 runs over both walls 0 and 1 (factors 1 + 1 =
    // 2). The group therefore totals 2^n for every y. Concretely, the group
    // has 2^z members each of price 2^(n - z), and 2^z * 2^(n - z) = 2^n.
    //
    // With |det V| groups each worth 2^n, the answer is 2^n * |det V| mod p.
    //
    // Gaussian elimination over F_p gives det V mod p but only up to sign,
    // and the entries are too large to take an exact big-integer det, so the
    // true sign is read off a plain floating-point LU with partial pivoting
    // and used to turn the residue into the nonnegative |det V|. When det V
    // is a multiple of p the residue is already 0 and the sign does not
    // matter.

    vector<vector<int64_t>> mat = a;
    int64_t det = 1 % p;
    for(int col = 0; col < n; col++) {
        int piv = -1;
        for(int r = col; r < n; r++) {
            if(mat[r][col] % p != 0) {
                piv = r;
                break;
            }
        }

        if(piv == -1) {
            det = 0;
            break;
        }

        if(piv != col) {
            swap(mat[piv], mat[col]);
            det = (p - det) % p;
        }

        det = mul_mod(det, mat[col][col] % p);
        int64_t inv = pow_mod(mat[col][col], p - 2);
        for(int r = col + 1; r < n; r++) {
            int64_t factor = mul_mod(mat[r][col] % p, inv);
            for(int c = col; c < n; c++) {
                mat[r][c] =
                    (mat[r][c] - mul_mod(factor, mat[col][c]) % p + p) % p;
            }
        }
    }

    if(det != 0) {
        vector<vector<long double>> b(n, vector<long double>(n));
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < n; j++) {
                b[i][j] = (long double)a[i][j];
            }
        }

        int sign = 1;
        for(int col = 0; col < n; col++) {
            int piv = col;
            for(int r = col + 1; r < n; r++) {
                if(fabsl(b[r][col]) > fabsl(b[piv][col])) {
                    piv = r;
                }
            }

            if(piv != col) {
                swap(b[piv], b[col]);
                sign = -sign;
            }

            if(b[col][col] < 0) {
                sign = -sign;
            }

            for(int r = col + 1; r < n; r++) {
                long double factor = b[r][col] / b[col][col];
                for(int c = col; c < n; c++) {
                    b[r][c] -= factor * b[col][c];
                }
            }
        }

        if(sign < 0) {
            det = (p - det) % p;
        }
    }

    cout << mul_mod(det, pow_mod(2, n)) << '\n';
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
