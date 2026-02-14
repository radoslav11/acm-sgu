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

const long double eps = 1e-8l;

int n;
vector<array<int, 3>> circles;

void read() {
    cin >> n;
    circles.resize(n);
    for(int i = 0; i < n; i++) {
        cin >> circles[i][0] >> circles[i][1] >> circles[i][2];
    }
}

void solve() {
    // We only want to find 3 reals as the answer. Let's treat them as
    // variables, and each of the circles as constraints. We effectively want to
    // solve this system with 3 variables and N constraints. There are a few
    // possibilities - there is exactly one solution, no solutions, or
    // infinitely many. The only tricky bit is that the systems are non-linear,
    // so we can't directly use Gauss or another popular approach. But we will
    // start with defining the constraints and show that we can actually convert
    // this to a linear system.
    //
    // We can notice two circles are orthogonal if the square of the distance
    // between the centers equals the sum of the squares of the radii:
    //
    //     (xi-x)^2 + (yi-y)^2 = ri^2 + r^2
    //     -2*xi*x + -2*yi*y + x^2 + y^2 - r^2 = -xi^2 - yi^2 + ri^2
    //     A*x + B*y + (x^2 + y^2 - r^2) = C
    //
    // where (x, y, r) are our variables. Let's treat z = x^2 + y^2 - r^2,
    // making the system linear which we can solve with a simple Gauss in O(n)
    // as we have only 3 variables. We will have one of the below:
    //
    //     1) A unique solution for (x, y, z), in which case we set the radius
    //        r = sqrt(x^2 + y^2 - z).
    //
    //     2) No solutions in which case we can't really set r.
    //
    //     3) Infinitely many solution. Similarly to (1), any value given by
    //        r = sqrt(x^2 + y^2 - z) will satisfy the system.
    //
    // This means it's enough to solve the system after the transformation of
    // introducing z. Complexity as already mentioned is linear.

    vector<array<long double, 4>> matrix;
    for(auto& circ: circles) {
        long long xi = circ[0], yi = circ[1], ri = circ[2];
        long double a = -2.0l * xi;
        long double b = -2.0l * yi;
        long double c = 1.0l;
        long double d = -(xi * xi + yi * yi - ri * ri);
        matrix.push_back({a, b, c, d});
    }

    int rows = matrix.size();
    int cols = 3;

    int pivot_row = 0;
    for(int col = 0; col < cols; col++) {
        int best = -1;
        for(int row = pivot_row; row < rows; row++) {
            if(abs(matrix[row][col]) > eps) {
                if(best == -1 ||
                   abs(matrix[row][col]) > abs(matrix[best][col])) {
                    best = row;
                }
            }
        }

        if(best == -1) {
            continue;
        }

        swap(matrix[pivot_row], matrix[best]);

        long double divisor = matrix[pivot_row][col];
        for(int c = 0; c <= cols; c++) {
            matrix[pivot_row][c] /= divisor;
        }

        for(int row = 0; row < rows; row++) {
            if(row != pivot_row && abs(matrix[row][col]) > eps) {
                long double factor = matrix[row][col];
                for(int c = 0; c <= cols; c++) {
                    matrix[row][c] -= factor * matrix[pivot_row][c];
                }
            }
        }

        pivot_row++;
    }

    for(int row = pivot_row; row < rows; row++) {
        if(abs(matrix[row][3]) > eps) {
            cout << -1 << '\n';
            return;
        }
    }

    if(pivot_row < 3) {
        cout << -2 << '\n';
        return;
    }

    vector<long double> solution(3);
    for(int i = 0; i < 3; i++) {
        solution[i] = matrix[i][3];
    }

    long double x = solution[0];
    long double y = solution[1];
    long double z = solution[2];
    long double r_squared = x * x + y * y - z;
    if(r_squared <= eps) {
        cout << -1 << '\n';
        return;
    }

    long double r = sqrt(r_squared);
    cout << fixed << setprecision(10) << x << ' ' << y << ' ' << r << '\n';
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
