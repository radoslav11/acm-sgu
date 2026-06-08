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
double f1, f2, profit;

void read() { cin >> n >> f1 >> f2 >> profit; }

void solve() {
    // The race is a stochastic game over states (k1, k2), the number of
    // approvals each businessman already holds (0 <= ki < N while the game is
    // live; reaching N is absorbing). On a given day each player i privately
    // picks a bribe bi >= 0, succeeds with probability pi = 1 - 0.99*(1-Fi)^bi
    // (so bi = 0 still yields the base 1% chance and a larger Fi saturates
    // faster), and pays bi whether or not the bribe works. The day's four
    // outcomes move the state to (k1+1,k2+1), (k1+1,k2), (k1,k2+1) or back to
    // (k1,k2) when both fail; a player who reaches N first takes the whole
    // profit V, a same-day double finish is split by a coin toss.
    //
    // Solve the states by backward induction on remaining approvals: every
    // transition either advances a coordinate or stalls in place, so once the
    // three strictly-more-advanced neighbours are known the only self-reference
    // left is the "both fail" loop. Let V1/V2 be the equilibrium net payoff
    // (expected profit minus own future bribes) of the live state and let A, B,
    // C be player 1's payoff at the resulting states (k1+1,k2+1), (k1+1,k2),
    // (k1,k2+1), reading off V, 0 or V/2 at any neighbour that is already
    // terminal. Geometric summation over the repeated stalls gives, with
    // leave = p1 + p2 - p1*p2,
    //
    //     V1 = (-b1 + p1*p2*A + p1*(1-p2)*B + (1-p1)*p2*C) / leave,
    //
    // and the same form for V2 with player 2's A2, B2, C2.
    //
    // Writing the bribe in terms of the success probability it buys,
    // bi = ln((1-pi)/0.99)/ln(1-Fi) for pi in [0.01, 1), turns a state's
    // strategy into a point (p1, p2) and makes V1, V2 explicit functions of it.
    // Let BR1(q) be player 1's best reply when player 2 plays q, that is the p1
    // maximising V1(p1, q), and BR2(q) the p2 maximising V2(q, p2). Each value
    // is single-peaked in that player's own probability (the bribe cost is
    // convex, the reward linear), so BR1 and BR2 are located by ternary search
    // on the maximiser itself, not by rooting the first-order condition, which
    // carries extra solutions that are not the true optimum. The state's
    // equilibrium is the (unique) pair that are mutual best replies, p1 =
    // BR1(p2) and p2 = BR2(p1); substituting the second into the first
    // collapses it to the scalar fixed point p1 = BR1(BR2(p1)), solved by
    // bisection. Iterating the best replies directly is avoided because it can
    // land on a profile that is not individually rational.
    //
    // There is no linear-programming shortcut for this per-state game, which is
    // why the two structural facts above are needed. A simplex solves a
    // zero-sum matrix game, but this game is general-sum (V1 + V2 = V minus the
    // total bribes paid, which varies with play) and its payoffs are
    // transcendental in the action (the bribe enters as a log), so neither the
    // best replies nor the equilibrium are linear. What makes it tractable
    // anyway is shape: single-peakedness reduces each best reply to one concave
    // maximisation a search can solve, and because a best reply always lands in
    // [0.01, 1) the map BR1(BR2(.)) satisfies gap(0.01) >= 0 >= gap(1), so its
    // unique root is bracketed by the endpoints and a plain bisection reaches
    // it.
    //
    // The same backward sweep, replaying the equilibrium (p1, p2), propagates
    // the win probability W1 (terminal value 1, 0 or 1/2):
    //
    //     W1 = (p1*p2*Wa + p1*(1-p2)*Wb + (1-p1)*p2*Wc) / leave,
    //
    // and the answer is W1 at (0,0) against 1 - W1, since some player finishes
    // with probability 1.

    vector<vector<double>> val1(n + 1, vector<double>(n + 1));
    vector<vector<double>> val2(n + 1, vector<double>(n + 1));
    vector<vector<double>> win1(n + 1, vector<double>(n + 1));

    double l1 = log(1 - f1), l2 = log(1 - f2);
    const double lo = 0.01, hi = 1 - 1e-12;

    auto resolve = [&](const vector<vector<double>>& board, int x, int y,
                       double won, double lost, double tie) {
        if(x == n && y == n) {
            return tie;
        }
        if(x == n) {
            return won;
        }
        if(y == n) {
            return lost;
        }

        return board[x][y];
    };

    auto bribe = [](double p, double l) { return log((1 - p) / 0.99) / l; };

    auto expected = [](double p1, double p2, double both, double only1,
                       double only2) {
        return p1 * p2 * both + p1 * (1 - p2) * only1 + (1 - p1) * p2 * only2;
    };

    auto argmax = [&](auto value) {
        double x = lo, y = hi;
        for(int it = 0; it < 80; it++) {
            double m1 = x + (y - x) / 3, m2 = y - (y - x) / 3;
            if(value(m1) < value(m2)) {
                x = m1;
            } else {
                y = m2;
            }
        }

        return (x + y) / 2;
    };

    for(int a = n - 1; a >= 0; a--) {
        for(int b = n - 1; b >= 0; b--) {
            double a1 = resolve(val1, a + 1, b + 1, profit, 0.0, profit / 2);
            double b1 = resolve(val1, a + 1, b, profit, 0.0, profit / 2);
            double c1 = resolve(val1, a, b + 1, profit, 0.0, profit / 2);
            double a2 = resolve(val2, a + 1, b + 1, 0.0, profit, profit / 2);
            double b2 = resolve(val2, a + 1, b, 0.0, profit, profit / 2);
            double c2 = resolve(val2, a, b + 1, 0.0, profit, profit / 2);

            auto v1 = [&](double p1, double p2) {
                return (-bribe(p1, l1) + expected(p1, p2, a1, b1, c1)) /
                       (p1 + p2 - p1 * p2);
            };
            auto v2 = [&](double p1, double p2) {
                return (-bribe(p2, l2) + expected(p1, p2, a2, b2, c2)) /
                       (p1 + p2 - p1 * p2);
            };
            auto br1 = [&](double p2) {
                return argmax([&](double q) { return v1(q, p2); });
            };
            auto br2 = [&](double p1) {
                return argmax([&](double q) { return v2(p1, q); });
            };
            auto gap = [&](double p1) { return br1(br2(p1)) - p1; };

            double x = lo, y = hi;
            for(int it = 0; it < 100; it++) {
                double m = (x + y) / 2;
                if(gap(m) > 0) {
                    x = m;
                } else {
                    y = m;
                }
            }

            double p1 = (x + y) / 2, p2 = br2(p1);
            val1[a][b] = v1(p1, p2);
            val2[a][b] = v2(p1, p2);
            win1[a][b] = expected(
                             p1, p2, resolve(win1, a + 1, b + 1, 1.0, 0.0, 0.5),
                             resolve(win1, a + 1, b, 1.0, 0.0, 0.5),
                             resolve(win1, a, b + 1, 1.0, 0.0, 0.5)
                         ) /
                         (p1 + p2 - p1 * p2);
        }
    }

    cout << setprecision(9) << fixed << win1[0][0] << ' ' << 1 - win1[0][0]
         << '\n';
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
