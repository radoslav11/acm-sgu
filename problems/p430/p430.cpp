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

const long double EPS_EDGE = 1e-7;
const long double EPS_NON_EDGE = 1e-2;
const long double EPS_DISTINCT = 1e-2;
const long double MAX_COORD = 7.0;
const long double TIME_LIMIT = 0.6;
const long double LR_START = 0.05;
const long double LR_DECAY = 0.999;
const int MAX_ITERS = 10000;

int n, m;
vector<pair<int, int>> edges;
vector<vector<bool>> adj;
vector<long double> x, y;

void read() {
    cin >> n >> m;
    adj.assign(n, vector<bool>(n, false));
    x.resize(n);
    y.resize(n);
    edges.clear();
    for(int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        edges.emplace_back(u, v);
        adj[u][v] = adj[v][u] = true;
    }
}

bool verify_solution() {
    for(int i = 0; i < n; i++) {
        for(int j = i + 1; j < n; j++) {
            long double dx = x[i] - x[j];
            long double dy = y[i] - y[j];
            long double d = sqrt(dx * dx + dy * dy);

            if(d < EPS_DISTINCT) {
                return false;
            }
            if(adj[i][j]) {
                if(fabsl(d - 1.0) > EPS_EDGE) {
                    return false;
                }
            } else {
                if(fabsl(d - 1.0) < EPS_NON_EDGE) {
                    return false;
                }
            }
        }
    }
    return true;
}

void solve() {
    // We can solve the unit-distance graph problem by formulating it as a
    // continuous optimization problem and applying gradient descent with random
    // restarts. For each vertex i (0 ≤ i < n), we assign two variables for the
    // coordinates (x_i, y_i). Let d_ij = sqrt((x_i - x_j)^2 + (y_i - y_j)^2)
    // be the Euclidean distance between vertices i and j. The we can minimize a
    // total energy E consisting of three parts:
    //
    //     1) Edge length constraints. For every edge (i, j), the distance must
    //        be exactly 1. We penalize squared deviation from 1:
    //            E_edge = sum_{(i,j) in edges} (d_ij - 1)^2
    //
    //     2) Non-edge forbidden band. For non-edges, the distance must NOT be
    //        close to 1. We introduce a forbidden band of width 1e-2 around 1.
    //        Only when a non-edge distance enters this band do we penalize it:
    //            For (i, j) not an edge:
    //                if |d_ij - 1| < 1e-2:
    //                    target = 1 - 1e-2           if d_ij < 1
    //                             1 + 1e-2           if d_ij > 1
    //                    E_nonedge += (d_ij - target)^2
    //
    //     3) All points must remain distinct. If two points get closer than
    //        1e-2, we push them apart:
    //
    //            E_distinct += max(0, 1e-2 - d_ij)^2
    //
    // The total energy is then:
    //     E = E_edge + E_nonedge + E_distinct
    //
    // Let's optimize this with gradient descent.
    // For the (d_ij - T)^2 terms, the gradient for point i is the below:
    //
    //     d/d(x_i, y_i) = 2 (d_ij - T) * ((x_i - x_j), (y_i - y_j)) / d_ij
    //
    // While for point j, we directly have the negative. This can be shown from:
    //
    //     d_ij = |p_i - p_j|
    //         and
    //     grad{p_i} d_ij = (p_i - p_j) / d_ij
    //
    // To optimize this, we might have to tune the learning rate a bit, and make
    // some restarts, but it's doable for the given constraints. We should note
    // that we have branching in this solution, so the initialization does
    // matter and the function isn't convex and we might end up in a state where
    // it's impossible to satisfy the (2) constraints. This lack of convexity is
    // why we end up making multiple restarts. Alternatively, we could have went
    // for a SGD-like approach of considering smaller batches instead of the
    // full loss.

    auto start = clock();
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    uniform_real_distribution<long double> init(-MAX_COORD, MAX_COORD);

    while((long double)(clock() - start) / CLOCKS_PER_SEC < TIME_LIMIT) {
        for(int i = 0; i < n; i++) {
            x[i] = init(rng);
            y[i] = init(rng);
        }

        long double lr = LR_START;
        for(int iter = 0; iter < MAX_ITERS; iter++) {
            vector<long double> gx(n, 0), gy(n, 0);

            for(int i = 0; i < n; i++) {
                for(int j = i + 1; j < n; j++) {
                    long double dx = x[i] - x[j];
                    long double dy = y[i] - y[j];
                    long double d = sqrt(dx * dx + dy * dy) + 1e-12;

                    if(adj[i][j]) {
                        long double diff = d - 1.0;
                        long double g = 2 * diff / d;
                        gx[i] += g * dx;
                        gy[i] += g * dy;
                        gx[j] -= g * dx;
                        gy[j] -= g * dy;
                    } else {
                        if(abs(d - 1.0) < EPS_NON_EDGE) {
                            long double target =
                                (d < 1.0 ? 1.0 - EPS_NON_EDGE
                                         : 1.0 + EPS_NON_EDGE);
                            long double diff = d - target;
                            long double g = 2 * diff / d;
                            gx[i] += g * dx;
                            gy[i] += g * dy;
                            gx[j] -= g * dx;
                            gy[j] -= g * dy;
                        }
                    }

                    if(d < EPS_DISTINCT) {
                        long double diff = d - EPS_DISTINCT;
                        long double g = 2 * diff / d;
                        gx[i] += g * dx;
                        gy[i] += g * dy;
                        gx[j] -= g * dx;
                        gy[j] -= g * dy;
                    }
                }
            }

            for(int i = 0; i < n; i++) {
                x[i] -= lr * gx[i];
                y[i] -= lr * gy[i];
            }

            lr *= LR_DECAY;
        }

        if(verify_solution()) {
            cout << "Yes\n";
            cout << fixed << setprecision(12);
            for(int i = 0; i < n; i++) {
                cout << x[i] << " " << y[i] << "\n";
            }
            return;
        }
    }

    cout << "No\n";
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
