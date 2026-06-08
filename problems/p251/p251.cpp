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
vector<double> r_vals;

void read() {
    cin >> n;
    r_vals.assign(n, 0.0);
    cin >> r_vals;
}

void solve() {
    // The constraint for N is fairly small. We can try a numerical approach -
    // let's create a system for this, with 2*n variables (x[:], y[:]). We then
    // have C(n, 3) constraints, that are given by:
    //
    //     v[i] + v[j] + v[k] = abs((x[i] - x[j]) * (y[k] - y[j]) -
    //                              (x[k] - x[j]) * (y[i] - y[j])).
    //
    // By squaring both sides, we get rid of the absolute value and the
    // branching.
    //
    //     Const = a((x[i] - x[j]) * (y[k] - y[j]) -
    //               (x[k] - x[j]) * (y[i] - y[j]))^2
    //
    // We can now solve this with gradient descent by instead minimizing the sum
    // of squares. The term itself will be of high order as we have products of
    // variables and then we ^4, but it's still a polynomial. After some
    // tuning, we can get a solution that runs in a reasonable amount of time.
    // Here I implement Adam with random restarts, and do the gradient with
    // chain rule as it's easier.

    vector<double> x(n), y(n), gx(n), gy(n);
    vector<double> m_x(n), m_y(n), v_x(n), v_y(n);

    mt19937 rng(987654321);
    uniform_real_distribution<double> init_dist(-15.0, 15.0);

    auto compute = [&](bool need_grad) -> double {
        if(need_grad) {
            fill(gx.begin(), gx.end(), 0.0);
            fill(gy.begin(), gy.end(), 0.0);
        }
        double loss = 0.0;
        for(int i = 0; i < n; i++) {
            for(int j = i + 1; j < n; j++) {
                for(int k = j + 1; k < n; k++) {
                    double cross = x[i] * (y[k] - y[j]) + x[j] * (y[i] - y[k]) +
                                   x[k] * (y[j] - y[i]);
                    double target = 2.0 * (r_vals[i] + r_vals[j] + r_vals[k]);
                    double f = cross * cross - target * target;
                    loss += f * f;
                    if(need_grad) {
                        double coef = 4.0 * f * cross;
                        gx[i] += coef * (y[k] - y[j]);
                        gx[j] += coef * (y[i] - y[k]);
                        gx[k] += coef * (y[j] - y[i]);
                        gy[i] += coef * (x[j] - x[k]);
                        gy[j] += coef * (x[k] - x[i]);
                        gy[k] += coef * (x[i] - x[j]);
                    }
                }
            }
        }
        return loss;
    };

    double best_loss = numeric_limits<double>::infinity();
    vector<double> best_x(n), best_y(n);

    const double beta1 = 0.9, beta2 = 0.999, adam_eps = 1e-8;
    const double lr = 0.1;
    const int max_restarts = 80;
    const int max_steps = 2500;

    for(int restart = 0; restart < max_restarts; restart++) {
        for(int i = 0; i < n; i++) {
            x[i] = init_dist(rng);
            y[i] = init_dist(rng);
        }
        fill(m_x.begin(), m_x.end(), 0.0);
        fill(m_y.begin(), m_y.end(), 0.0);
        fill(v_x.begin(), v_x.end(), 0.0);
        fill(v_y.begin(), v_y.end(), 0.0);

        double b1t = 1.0, b2t = 1.0;
        for(int step = 1; step <= max_steps; step++) {
            double loss = compute(true);
            if(loss < 1e-20) {
                break;
            }
            b1t *= beta1;
            b2t *= beta2;
            double bc1 = 1.0 - b1t;
            double bc2 = 1.0 - b2t;
            for(int i = 0; i < n; i++) {
                m_x[i] = beta1 * m_x[i] + (1.0 - beta1) * gx[i];
                m_y[i] = beta1 * m_y[i] + (1.0 - beta1) * gy[i];
                v_x[i] = beta2 * v_x[i] + (1.0 - beta2) * gx[i] * gx[i];
                v_y[i] = beta2 * v_y[i] + (1.0 - beta2) * gy[i] * gy[i];
                x[i] -= lr * (m_x[i] / bc1) / (sqrt(v_x[i] / bc2) + adam_eps);
                y[i] -= lr * (m_y[i] / bc1) / (sqrt(v_y[i] / bc2) + adam_eps);
            }
        }

        double final_loss = compute(false);
        if(final_loss < best_loss) {
            best_loss = final_loss;
            best_x = x;
            best_y = y;
        }
        if(best_loss < 1e-12) {
            break;
        }
    }

    if(best_loss < 1e-4) {
        cout << "YES\n";
        cout << fixed << setprecision(4);
        for(int i = 0; i < n; i++) {
            cout << best_x[i] << ' ' << best_y[i] << '\n';
        }
    } else {
        cout << "NO\n";
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
