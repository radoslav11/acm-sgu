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

const double eps = 1e-6;

int n;
vector<double> mx, my;

void read() {
    cin >> n;
    mx.resize(n);
    my.resize(n);

    for(int i = 0; i < n; i++) {
        cin >> mx[i] >> my[i];
    }
}

vector<double> solve_system(vector<double> mids) {
    int n = mids.size();

    double x1;
    if(n % 2 == 1) {
        x1 = 0;
        for(int i = 0; i < n; i++) {
            if(i % 2 == 0) {
                x1 += mids[i];
            } else {
                x1 -= mids[i];
            }
        }
    } else {
        x1 = 0;
    }

    vector<double> x(n);
    x[0] = x1;
    for(int i = 0; i < n - 1; i++) {
        x[i + 1] = 2 * mids[i] - x[i];
    }

    if(abs(x[n - 1] + x[0] - 2 * mids[n - 1]) > eps) {
        return {};
    }

    return x;
}

void solve() {
    // The two main observations are:
    //   (1) The hint in the problem statement is that we don't care about
    //       edge intersections.
    //   (2) The midpoint of (x1, y1) and (x2, y2) is ((x1+x2)/2,
    //       (y1+y2)/2), and so the X and Y dimensions are independent.
    //
    // Therefore, we can solve the following system:
    //   x1 + x2 = 2 * mx1
    //   x2 + x3 = 2 * mx2
    //     ...
    //   xn + x1 = 2 * mxn
    //
    // (and the analogous one for y).
    //
    // To solve this system, we can notice that we can split into two cases by
    // parity:
    //   (1) N is odd. Then we have either 0 or 1 solutions. If there is a
    //       single solution, we can get x1 and then recover everything. To
    //       get x1, we will get:
    //          2 * x1 = 2 * (mx1 - mx2 + mx3 - mx4 + ... + mxn).
    //          x1 = (mx1 - mx2 + mx3 - mx4 + ... + mxn)
    //   (2) N is even. Then we have either 0 or infinitely many solutions. If
    //   that's the case, we will try x1 = 0.
    //
    // After we have decided on x1, recovering is simple by x{i+1} = 2 * mxi -
    // xi. Now we have the x1...xn candidate. We want to re-validate that this
    // is a solution to this system.

    vector<double> x = solve_system(mx);
    vector<double> y = solve_system(my);

    if(x.empty() || y.empty()) {
        cout << "NO" << endl;
        return;
    }

    cout << "YES" << endl;
    for(int i = 0; i < n; i++) {
        cout << setprecision(3) << fixed << x[i] << " " << y[i] << endl;
    }
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
