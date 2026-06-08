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

int N;
double u, T;
vector<double> w, v;

void read() {
    cin >> N >> u >> T;
    w.resize(N);
    v.resize(N);
    for(int i = 0; i < N; i++) {
        cin >> w[i] >> v[i];
    }
}

void solve() {
    // We can think of this as an optimization problem. Let t[1], ..., t[n] be
    // the times we spend at each river. Clearly T = t[1] + ... + t[n], because
    // there is no point in wasting time waiting. Also, we know the smallest
    // amount of time t[i] >= w[i] / u. For a given t[i], we can always show
    // that it's optimal to swim in a constant direction vector (x[i], y[i])
    // with a fairly simple convexity argument (if we change directions at some
    // point, we can consider the average between the two vectors for the same
    // amount of time). The constraint is that u^2 = x[i]^2 + (y[i] + v[i])^2.
    //
    // For fixed t[i] we must have x[i] = w[i] / t[i] (exact crossing). To
    // maximize total northward displacement we take the root:
    //
    //     y[i] = -v[i] + sqrt(u^2 - x[i]^2)
    //
    // The total eastward displacement W = sum w[i] is fixed. The total
    // northward displacement becomes:
    //
    //     Y = sum g_i(t[i])
    //       = sum y[i]*t[i]
    //       = sum v[i]*t[i] + sum sqrt((u*t[i])^2 - w[i]^2).
    //
    // We therefore need to choose t[1]..t[n] (t[i] >= w[i]/u, sum t[i] = t)
    // that maximize Y. The final answer distance is sqrt(W*W + Y*Y). If sum
    // (w[i]/u) > t it is impossible (-1). Otherwise the optimum always uses
    // exactly t seconds (extra time always gives extra drift).
    //
    // To optimize this, we can do a gradient descent like approach, but we can
    // also do Lagrange multipliers. Being more precise, the equality constraint
    // is a single one in T = t[1] + ... + t[n], so the partial derivative of
    // this is 1 for each t[i]. However, we have the inequality constraints t[i]
    // >= w[i] / u for which we also need a Lagrange multiplier, which we denote
    // my \mu_i. By KKT, we simply know that g_i'(t) = \lambda - \mu_i. However,
    // g_i(t) is concave, or g_i'(t) is strictly decreasing and t -> w[i]/u
    // has g_i'(t) -> +inf, meaning the optimum we always has t[i] > w[i]/u
    // because g_i'(t) is undefined before w[i]/u. In other words, any t[:] \in
    // R^n we find will have t[i] > w[i]/u simply because the g_i'(t) term
    // requires this. This means that \mu_i will be inactive and we can set
    // \mu_i = 0. Therefore, all g_i'(t) = \lambda for the same \lambda. The
    // derivative is:
    //
    //     g_i'(t) = v[i] + (u*u*t) / sqrt((u*t)^2 - w[i]*w[i])
    //
    // For a candidate \lambda we solve for each t[i] in closed form:
    //
    //     a = \lambda - v[i]
    //     t[i] = w[i] * a / (u * sqrt(a*a - u*u))     (valid when a > u)
    //
    // We binary-search \lambda (approx 100-200 iterations suffice for 1e-12
    // precision) until sum t[i] equals the given t (larger \lambda makes every
    // t[i] smaller). This gives us both the optimal Y and the t[1], ..., t[n]
    // (and so answer to the problem). It works in O(N log eps^-1), ignoring the
    // complexity of explicitly solving the t[i] that requires computing a
    // square root.

    double min_time = 0.0;
    for(int i = 0; i < N; i++) {
        min_time += w[i] / u;
    }

    if(min_time > T + 1e-12) {
        cout << -1 << endl;
        return;
    }

    double max_asympt = 0.0;
    for(int i = 0; i < N; i++) {
        max_asympt = max(max_asympt, v[i] + u);
    }

    auto compute_times = [&](double lam) -> vector<double> {
        vector<double> res(N);
        for(int i = 0; i < N; i++) {
            double a = lam - v[i];
            double sqrt_term = sqrt(a * a - u * u);
            res[i] = w[i] * a / (u * sqrt_term);
        }
        return res;
    };

    double low = max_asympt + 1e-9;
    double high = 1e18;
    for(int iter = 0; iter < 200; iter++) {
        double mid = (low + high) / 2.0;
        auto ts = compute_times(mid);
        double sumt = 0.0;
        for(double t: ts) {
            sumt += t;
        }
        if(sumt > T) {
            low = mid;
        } else {
            high = mid;
        }
    }

    double lambda_val = (low + high) / 2.0;
    auto times = compute_times(lambda_val);

    double Y = 0.0;
    double W = 0.0;
    for(int i = 0; i < N; i++) {
        W += w[i];
        double ti = times[i];
        Y += v[i] * ti + sqrt(u * ti * ti * u - w[i] * w[i]);
    }
    double dist = sqrt(W * W + Y * Y);

    cout << fixed << setprecision(10) << dist << endl;
    cout << fixed << setprecision(10);
    for(int i = 0; i < N; i++) {
        if(i > 0) {
            cout << ' ';
        }
        cout << times[i];
    }
    cout << endl;
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
