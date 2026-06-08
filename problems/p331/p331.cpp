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
double dist_total, c_change;
double a_arr[5], b_arr[5], delta_arr[5];

void read() {
    cin >> n >> dist_total >> c_change;
    for(int i = 0; i < n; i++) {
        cin >> a_arr[i] >> b_arr[i] >> delta_arr[i];
    }
}

double drive_time_exact(int lane, double t_start, double dx) {
    double a = a_arr[lane], b = b_arr[lane], d = delta_arr[lane],
           cos_t0 = cos(t_start + d), lo = (dx <= 0) ? 0.0 : dx / (b + a),
           hi = (a > 0 && dx > 0) ? dx / (b - a) : lo;
    if(hi < lo) {
        hi = lo;
    }

    for(int it = 0; it < 100; it++) {
        double mid = 0.5 * (lo + hi),
               f = b * mid - a * (cos(t_start + mid + d) - cos_t0) - dx;
        if(f > 0) {
            hi = mid;
        } else {
            lo = mid;
        }
    }
    return 0.5 * (lo + hi);
}

double drive_time(int lane, double t_start, double dx) {
    double a = a_arr[lane], b = b_arr[lane], d = delta_arr[lane],
           cos_t = cos(t_start + d), sin_t = sin(t_start + d),
           dt = dx / (b + a * sin_t);
    for(int it = 0; it < 4; it++) {
        double dt2 = dt * dt,
               cos_dt = 1.0 -
                        dt2 * 0.5 *
                            (1.0 - dt2 / 12.0 *
                                       (1.0 - dt2 / 30.0 * (1.0 - dt2 / 56.0))),
               sin_dt =
                   dt * (1.0 - dt2 / 6.0 *
                                   (1.0 - dt2 / 20.0 *
                                              (1.0 - dt2 / 42.0 *
                                                         (1.0 - dt2 / 72.0)))),
               cos_a = cos_t * cos_dt - sin_t * sin_dt,
               sin_a = sin_t * cos_dt + cos_t * sin_dt,
               f = b * dt - a * (cos_a - cos_t) - dx, fp = b + a * sin_a;
        dt -= f / fp;
    }
    return dt;
}

const int M = 200000;
double time_arr[M + 1][5];
int par[M + 1][5];

void solve() {
    // The solution roughly goes as finding a discretized path from the start
    // until distance D, and then locally adjusting it to smoothly do the
    // transitions. For the first part, we do a position-discretized DP on
    // states being (lane L, position-grid i) with step = d / M. We do
    // transitions (L, i-1) -> (L, i) the cost of which can be found by solving
    // the closed-form integral
    //
    //     b*dt - a*(cos(t+dt+d) - cos(t+d)) = step
    //
    // for dt by using Newton. v_L > 0 makes the LHS strictly monotone, and dt
    // <= step <= 5e-3 lets us replace cos(dt), sin(dt) with a 4-term
    // Taylor series so the inner loop is trig-free.
    //
    // Switch transition (Lp, i) -> (L, i) costs c * |L - Lp|; only direct
    // switches matter because c * |.| is a metric on the lane-index line.
    // par[i][L] is -1 at the start, L for a drive parent, or lp != L for a
    // switch parent.
    //
    // For the second stage of refinement, we have to do it because the DP only
    // places switches on grid points, leaving each switch time off by up to
    // step/(2*v). This is small per switch, but with up to ~100 switches in
    // tight tests the cumulative error exceeds the 1e-6 tolerance (without this
    // step we got WA on test 4). For each switch k from lane A to
    // lane B with switching duration c_k = c*|A-B|, the optimal switch
    // moment t_k is the one where
    //
    //     v_A(t_k) = v_B(t_k + c_k).
    //
    // Intuitively, at the optimum the speed we'd leave A with equals
    // the speed we'd arrive on B with, so delaying or advancing the
    // switch trades equal distance between the two segments and the
    // total drive needed on the final lane is unchanged. The condition
    // is independent across switches (it follows from a Lagrangian
    // argument: perturbing t_k by delta shifts adjacent-segment
    // distance by (v_A(t_k) - v_B(t_k + c_k))*delta, which must vanish
    // at a minimum of T_end), so a 1D Newton per switch suffices.
    // Steps are clamped to <= 0.3 to stay in the basin of the root the
    // DP identified; if Newton still lands on the wrong branch
    // (f' >= 0 at convergence marks a max of T_end rather than a min)
    // we revert that switch to its DP estimate. t_k is clamped
    // monotone so the schedule remains valid.
    //
    // The final segment's end time is solved by bisecting the same
    // integral (dt can now be large, so the Taylor version no longer
    // converges). As a safety net we compute T_end for both the
    // refined and the DP schedule and emit the smaller one: the DP
    // schedule covers exactly d by construction, so refinement can
    // only help.

    double step = dist_total / M;
    for(int L = 0; L < n; L++) {
        for(int i = 0; i <= M; i++) {
            time_arr[i][L] = 1e18;
            par[i][L] = -1;
        }
    }

    time_arr[0][0] = 0;

    double switch_cost[5][5];
    for(int L = 0; L < n; L++) {
        for(int lp = 0; lp < n; lp++) {
            switch_cost[L][lp] = c_change * abs(L - lp);
        }
    }

    for(int i = 0; i <= M; i++) {
        double drive_arrival[5];
        for(int L = 0; L < n; L++) {
            drive_arrival[L] = time_arr[i][L];
        }

        for(int L = 0; L < n; L++) {
            for(int lp = 0; lp < n; lp++) {
                if(lp == L) {
                    continue;
                }

                double cand = drive_arrival[lp] + switch_cost[L][lp];
                if(cand < time_arr[i][L]) {
                    time_arr[i][L] = cand;
                    par[i][L] = lp;
                }
            }
        }

        if(i < M) {
            for(int L = 0; L < n; L++) {
                double t = time_arr[i][L], dt = drive_time(L, t, step);
                time_arr[i + 1][L] = t + dt;
                par[i + 1][L] = L;
            }
        }
    }

    int best_lane = 0;
    for(int L = 1; L < n; L++) {
        if(time_arr[M][L] < time_arr[M][best_lane]) {
            best_lane = L;
        }
    }

    vector<int> lane_seq{best_lane};
    vector<double> switch_times;
    int cur_l = best_lane, cur_i = M;
    while(true) {
        int p = par[cur_i][cur_l];
        if(p == -1) {
            break;
        }
        if(p == cur_l) {
            cur_i--;
        } else {
            switch_times.push_back(time_arr[cur_i][p]);
            cur_l = p;
            lane_seq.push_back(cur_l);
        }
    }

    reverse(lane_seq.begin(), lane_seq.end());
    reverse(switch_times.begin(), switch_times.end());

    vector<double> dp_times = switch_times;
    for(size_t i = 0; i < switch_times.size(); i++) {
        int prev = lane_seq[i], next = lane_seq[i + 1];
        double c_dur = c_change * abs(next - prev), a_p = a_arr[prev],
               b_p = b_arr[prev], d_p = delta_arr[prev], a_n = a_arr[next],
               b_n = b_arr[next], d_n = delta_arr[next], t_dp = switch_times[i],
               t = t_dp;
        for(int it = 0; it < 60; it++) {
            double f = (b_p + a_p * sin(t + d_p)) -
                       (b_n + a_n * sin(t + c_dur + d_n)),
                   fp = a_p * cos(t + d_p) - a_n * cos(t + c_dur + d_n);
            if(fabs(fp) < 1e-13) {
                break;
            }

            double step_t = max(-0.3, min(0.3, f / fp));
            t -= step_t;
            if(fabs(step_t) < 1e-16) {
                break;
            }
        }

        if(a_p * cos(t + d_p) - a_n * cos(t + c_dur + d_n) >= 0) {
            t = t_dp;
        }

        double min_t = (i == 0)
                           ? 0.0
                           : switch_times[i - 1] +
                                 c_change * abs(lane_seq[i] - lane_seq[i - 1]);
        switch_times[i] = max(t, min_t);
    }

    auto compute = [&](const vector<double>& times) {
        double total_dist = 0, cur_time = 0;
        size_t len = times.size();
        for(size_t i = 0; i < times.size(); i++) {
            int prev = lane_seq[i], next = lane_seq[i + 1];
            double t_sw = times[i], a = a_arr[prev], b = b_arr[prev],
                   d = delta_arr[prev],
                   seg = b * (t_sw - cur_time) -
                         a * (cos(t_sw + d) - cos(cur_time + d));
            if(total_dist + seg >= dist_total) {
                len = i;
                break;
            }
            total_dist += seg;
            cur_time = t_sw + c_change * abs(next - prev);
        }

        double rem = max(0.0, dist_total - total_dist);
        double t = cur_time + drive_time_exact(lane_seq[len], cur_time, rem);
        return make_pair(t, len);
    };

    auto [t_refined, len_refined] = compute(switch_times);
    auto [t_dp, len_dp] = compute(dp_times);

    const vector<double>& chosen = (t_dp < t_refined) ? dp_times : switch_times;
    double t_end = min(t_refined, t_dp);
    size_t schedule_len = (t_dp < t_refined) ? len_dp : len_refined;

    cout << fixed << setprecision(17);
    cout << t_end << "\n";
    cout << schedule_len << "\n";
    for(size_t i = 0; i < schedule_len; i++) {
        cout << (lane_seq[i + 1] + 1) << " " << chosen[i] << "\n";
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
