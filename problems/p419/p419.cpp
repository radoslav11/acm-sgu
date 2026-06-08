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

int64_t x, y;
int d1, d2, b;

void read() { cin >> x >> y >> d2 >> d1 >> b; }

void solve() {
    // We need the fewest moves for a tank that can only turn left (heading
    // +1 mod 6), must step forward on every turn, and may not repeat a
    // direction more than b times in a row, to travel from (0,0) facing d1 to
    // (x,y) facing d2. Coordinates reach 1e12, so the answer can be huge and
    // searching is hopeless. Instead we describe the shape of every legal walk,
    // notice it has only two real degrees of freedom, and pin those down with a
    // bit of algebra and one binary search. The plan: (1) see that a walk is
    // forced to be a chain of direction "runs"; (2) reduce its freedom to a
    // start direction and a lap count m; (3) for a fixed m turn the best run
    // lengths into a one-variable convex minimisation g(m); (4) optimise m.
    //
    // Shape of a walk. Turning is one-way and every turn also steps forward, so
    // the heading never decreases: the directions we use, in order, form a
    // contiguous arc f, f+1, f+2, ... read mod 6. We can neither skip a
    // direction (each +1 costs a move) nor return to an earlier one without
    // going the long way around, so the walk breaks into runs: one maximal run
    // per direction along that arc, each run 1..b moves long. The first move is
    // the only special case, since at the very start we either step straight
    // away (first run direction f = d1) or spend our one allowed turn first
    // (f = d1+1). Beyond that, going once around the compass is a full lap of
    // six runs. So a whole walk is captured by two numbers: the start direction
    // f (two choices) and the run count R. The final run must face d2, which
    // fixes R modulo 6, hence R = R0 + 6*m with R0 = ((d2-f) mod 6) + 1 in
    // [1..6] and m >= 0 the number of extra laps. This is the natural "short
    // opening fragment plus m identical loops" split.
    //
    // Forced vs free moves. Direction k occurs in n_k = m + c_k runs, where c_k
    // counts its runs in the base R0. We may give it any S_k in [n_k, n_k*b]
    // total moves, the answer is T = sum S_k, and the displacement must satisfy
    // sum S_k*vec_k = (x, y). Split S_k = n_k + s_k: the n_k part (one move per
    // run, forced) contributes a displacement (Cx, Cy) = sum n_k*vec_k that is
    // independent of m, because a full lap visits every direction once and each
    // direction cancels its opposite. So all the free moves s_k in [0, cap_k],
    // cap_k = (m + c_k)*(b-1), have to do is cover the fixed leftover
    // D = (x - Cx, y - Cy).
    //
    // Where the free moves go. Opposite directions cancel, so the six headings
    // act as only three axes: dir 0 and 3 on one, 1 and 4 on the next, 2 and 5
    // on the last (vec3 = -vec0, vec4 = -vec1, vec5 = -vec2). Let A = s0 - s3,
    // B = s1 - s4, C = s2 - s5 be the net signed travel along each axis.
    // Reaching a net of A on an axis costs at least |A| moves, since stepping
    // one way and back only wastes moves, so the free moves total |A|+|B|+|C|.
    //
    // The three axes are redundant in the plane, since vec2 = vec1 - vec0, so a
    // step on axis 2 equals a step on axis 1 minus one on axis 0. That leaves a
    // single real choice: how much to route through the third axis, the value
    // C. Fixing C forces the other two, because the free displacement
    // A*vec0 + B*vec1 + C*vec2 = (A - C, B + C) must equal D = (Dx, Dy), giving
    // A = Dx + C and B = Dy - C. So for a fixed lap count m the minimal number
    // of free moves is:
    //
    //     g(m) = min over feasible C of |Dx + C| + |Dy - C| + |C|.
    //
    // This is a sum of absolute values: convex in C with kinks at C = -Dx,
    // C = Dy and C = 0, so ignoring the caps its minimum sits at the middle
    // kink C* = median(-Dx, Dy, 0), which does not depend on m. The caps only
    // forbid C outside an interval I(m) that widens as m grows (more laps means
    // more budget per direction), so g(m) is that expression at C* clamped into
    // I(m).
    //
    // Optimising m. T(m) = R0 + 6*m + g(m): each extra lap adds six forced
    // moves but loosens the caps, which can only lower g, so T is convex in m.
    // We binary-search the m where its slope first turns non-negative. Running
    // all of this for both start directions f and keeping the smaller total
    // gives the answer.

    const int vx[6] = {1, 0, -1, -1, 0, 1};
    const int vy[6] = {0, 1, 1, 0, -1, -1};

    if(x == 0 && y == 0 && d1 == d2) {
        cout << 0 << '\n';
        return;
    }

    int64_t best = LLONG_MAX;
    const int64_t MHI = (int64_t)6e12;

    for(int turn = 0; turn < 2; turn++) {
        int fd = (d1 + turn) % 6;
        int R0 = (((d2 - fd) % 6 + 6) % 6) + 1;

        int64_t c[6] = {0, 0, 0, 0, 0, 0};
        for(int i = 0; i < R0; i++) {
            c[(fd + i) % 6]++;
        }

        int64_t Cx = 0, Cy = 0;
        for(int k = 0; k < 6; k++) {
            Cx += c[k] * vx[k];
            Cy += c[k] * vy[k];
        }

        int64_t Dx = x - Cx, Dy = y - Cy;
        int64_t Cstar = (-Dx) + Dy - min({-Dx, Dy, (int64_t)0}) -
                        max({-Dx, Dy, (int64_t)0});

        auto cap = [&](int64_t m, int k) -> int64_t {
            return (m + c[k]) * (b - 1);
        };

        auto eval = [&](int64_t m) -> pair<bool, int64_t> {
            int64_t lo = max({-cap(m, 5), -cap(m, 3) - Dx, Dy - cap(m, 1)});
            int64_t hi = min({cap(m, 2), cap(m, 0) - Dx, Dy + cap(m, 4)});
            if(lo > hi) {
                return {false, 0};
            }

            int64_t C = max(lo, min(hi, Cstar));
            int64_t g = abs(Dx + C) + abs(Dy - C) + abs(C);
            return {true, R0 + 6 * m + g};
        };

        int64_t mlo = 0, mhi = MHI;
        while(mlo < mhi) {
            int64_t mid = mlo + (mhi - mlo) / 2;
            if(eval(mid).first) {
                mhi = mid;
            } else {
                mlo = mid + 1;
            }
        }

        int64_t lo = mlo, hi = MHI;
        while(lo < hi) {
            int64_t mid = lo + (hi - lo) / 2;
            if(eval(mid + 1).second >= eval(mid).second) {
                hi = mid;
            } else {
                lo = mid + 1;
            }
        }

        best = min(best, eval(lo).second);
    }

    cout << best << '\n';
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
