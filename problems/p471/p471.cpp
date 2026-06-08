#include <bits/stdc++.h>
// #include <coding_library/math/simplex.hpp>

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

template<class T = long double>
class Simplex {
  public:
    enum class Status { OPTIMAL, INFEASIBLE, UNBOUNDED };

    struct Result {
        Status status = Status::INFEASIBLE;
        T value = 0;
        vector<T> x;
    };

    static T eps() { return T(1e-9); }

    static T inf() {
        if constexpr(is_floating_point_v<T>) {
            return numeric_limits<T>::infinity();
        }
        return numeric_limits<T>::max() / T(4);
    }

    Result solve(
        const vector<vector<T>>& constraints, const vector<T>& rhs,
        const vector<T>& objective, bool recover_x = false
    ) {
        Result res;
        m = (int)rhs.size();
        n = (int)objective.size();
        if(m == 0) {
            res.status = Status::OPTIMAL;
            if(recover_x) {
                res.x.assign(n, 0);
            }
            return res;
        }

        const int slack = n + m;
        const int rhs_col = n + m + 1;
        const int width = n + m + 2;

        N.assign(width - 1, -1);
        B.assign(m + 1, 0);
        D.assign(m + 2, vector<T>(width, 0));

        for(int i = 0; i < m; i++) {
            for(int j = 0; j < n; j++) {
                D[i][j] = constraints[i][j];
            }
            D[i][n + i] = 1;
            D[i][rhs_col] = rhs[i];
            B[i] = n + i;
        }
        for(int j = 0; j < slack; j++) {
            N[j] = j;
        }
        for(int j = 0; j < n; j++) {
            D[m][j] = -objective[j];
        }
        N[slack] = -1;
        D[m + 1] = D[m];
        D[m + 1][slack] = 1;
        B[m] = m + 1;

        int x = n;
        for(int i = 0; i < m; i++) {
            if(D[i][rhs_col] < -eps()) {
                x = slack;
            }
        }

        if(!run_phase(1, x, slack, rhs_col)) {
            return res;
        }
        if(!run_phase(2, n, slack, rhs_col)) {
            res.status = Status::UNBOUNDED;
            return res;
        }

        res.status = Status::OPTIMAL;
        res.value = D[m][rhs_col];
        if(recover_x) {
            res.x.assign(n, 0);
            for(int i = 0; i < m; i++) {
                if(B[i] < n) {
                    res.x[B[i]] = D[i][rhs_col];
                }
            }
        }
        return res;
    }

  private:
    int m, n;
    vector<int> N, B;
    vector<vector<T>> D;

    void pivot(int r, int s, int width) {
        T pv = D[r][s];
        for(int j = 0; j < width; j++) {
            D[r][j] /= pv;
        }
        for(int i = 0; i < m + 2; i++) {
            if(i == r) {
                continue;
            }
            if(fabs(D[i][s]) <= eps()) {
                continue;
            }
            T f = D[i][s];
            for(int j = 0; j < width; j++) {
                D[i][j] -= f * D[r][j];
            }
        }
        B[r] = N[s];
        N[s] = -1;
    }

    bool run_phase(int phase, int x, int slack, int rhs_col) {
        int width = n + m + 2;
        int obj = phase == 1 ? m + 1 : m;
        for(int iter = 0; iter < 10000; iter++) {
            int s = -1;
            if(phase == 1) {
                for(int j = 0; j < slack; j++) {
                    if(N[j] == -1 && j != x && D[obj][j] > eps()) {
                        s = j;
                    }
                }
            } else {
                T best = -eps();
                for(int j = 0; j < slack; j++) {
                    if(D[obj][j] < best) {
                        best = D[obj][j];
                        s = j;
                    }
                }
            }
            if(s == -1) {
                if(phase == 1) {
                    for(int i = 0; i < m; i++) {
                        if(B[i] == slack) {
                            return false;
                        }
                    }
                }
                return true;
            }
            int r = -1;
            T best_ratio = inf();
            for(int i = 0; i < m; i++) {
                if(phase == 1) {
                    if(D[i][s] < -eps()) {
                        T df = -D[i][rhs_col] / D[i][s];
                        if(r == -1 || df < -D[r][rhs_col] / D[r][s] - eps()) {
                            r = i;
                        }
                    }
                } else if(D[i][s] > eps()) {
                    T ratio = D[i][rhs_col] / D[i][s];
                    if(ratio < best_ratio - eps()) {
                        best_ratio = ratio;
                        r = i;
                    }
                }
            }
            if(r == -1) {
                return false;
            }
            pivot(r, s, width);
        }
        return false;
    }
};

int payoff(int i, int j) { return abs(i - j) == 1 ? 1 : -1; }

long double solve_game(int n) {
    int cols = max(n - 1, 0) + 2;
    int vp = n - 1;
    int vm = n;
    vector<vector<long double>> a;
    vector<long double> b, c(cols);
    c[vp] = 1;
    c[vm] = -1;

    for(int i = 0; i < n; i++) {
        vector<long double> row(cols);
        int pin = payoff(i, n - 1);
        for(int j = 0; j < n - 1; j++) {
            row[j] = -(payoff(i, j) - pin);
        }
        row[vp] = 1;
        row[vm] = -1;
        a.push_back(row);
        b.push_back(pin);
    }
    if(n > 1) {
        vector<long double> box(n - 1, 1);
        a.push_back(box);
        b.push_back(1);
    }

    auto res = Simplex<long double>().solve(a, b, c);
    if(res.status != Simplex<long double>::Status::OPTIMAL) {
        return 0;
    }
    return res.value;
}

pair<int64_t, int64_t> to_fraction(long double x) {
    for(int64_t den = 1; den <= 200; den++) {
        int64_t num = llround(x * den);
        if(fabsl(x - (long double)num / den) < 1e-8) {
            int64_t g = gcd(llabs(num), den);
            return {num / g, den / g};
        }
    }
    int64_t den = 1;
    int64_t num = llround(x);
    return {num, den};
}

int n;

bool read() { return bool(cin >> n); }

void solve() {
    // The opening story is a red herring. The game we care about is the second
    // one: vot picks a in [1, N], Goryinyich picks b in [1, N] without seeing
    // a, and if |a-b| = 1 then vot pays Goryinyich a dollar, otherwise
    // Goryinyich pays vot. Both players maximize expected gain, so this is a
    // two-player zero-sum matrix game. Rows are vot's choices, columns are
    // Goryinyich's, and the payoff is P[a][b] = +1 when |a-b| = 1 and -1
    // otherwise.
    //
    // Rational play means a Nash equilibrium: neither player can improve
    // expected gain by unilaterally changing their mixed strategy. In zero-sum
    // games, every Nash equilibrium is a minimax pair (sigma, tau), and the
    // expected payoff E(sigma, tau) is the same at every equilibrium; that
    // common number is the game value v we output.
    //
    // With sigma on rows and tau on columns,
    //
    //     E(sigma, tau) = sum_{a,b} sigma(a) tau(b) P[a][b].
    //
    // Von Neumann's minimax theorem says v is unique and
    //
    //     v = max_tau min_sigma E = min_sigma max_tau E.
    //
    // How do we find the game value? Build the N x N matrix P and write two
    // linear programs. Goryinyich is the column player (maximizer). He chooses
    // a mixed strategy tau and a number v with the meaning: "no matter which a
    // vot plays, my expected payoff is at least v." Formally,
    //
    //     maximize v
    //     subject to   sum_j tau_j * P[i][j] >= v   for i = 1..N
    //                sum_j tau_j = 1
    //                tau_j >= 0
    //
    // So v is the best payoff Goryinyich can guarantee for himself by picking
    // tau; it is a lower bound on what he actually earns at equilibrium. vot is
    // the row player (minimizer). He chooses sigma and a number u with the
    // meaning: "no matter which b Goryinyich plays, his expected payoff is at
    // most u." Formally,
    //
    //     minimize u
    //     subject to   sum_i sigma_i * P[i][j] <= u   for j = 1..N
    //                sum_i sigma_i = 1
    //                sigma_i >= 0
    //
    // So u is the most Goryinyich can be forced to accept once vot plays sigma;
    // it is an upper bound on Goryinyich's payoff. Any real play sits between
    // these two stories: Goryinyich pushes v up, vot pushes u down.
    //
    // Strong duality applies because both programs are feasible and bounded
    // (payoffs lie in [-1, 1]). Then the optimal v and optimal u coincide, and
    // that common value is exactly the Nash/minimax game value we print.
    //
    // We solve the primal directly: substitute tau_N = 1 - sum_{j<N} tau_j,
    // split v = v_plus - v_minus with both nonnegative, and run two-phase
    // simplex on that standard-form LP. N <= 50 keeps the tableau small. The
    // optimum is rational and we print it in lowest terms.

    long double val = solve_game(n);
    auto [num, den] = to_fraction(val);
    cout << num << "/" << den << "\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    for(int test = 1; read(); test++) {
        cout << "Case #" << test << ": ";
        solve();
    }

    return 0;
}
