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

const int FX[5] = {0, 0, 0, 1, -1};
const int FY[5] = {0, -1, 1, 0, 0};

int n, m;
vector<string> grid;
int peter_x, peter_y, boat_x, boat_y;

void read() {
    cin >> n >> m;
    grid.assign(n, "");
    for(int r = 0; r < n; r++) {
        cin >> grid[r];
    }

    cin >> peter_x >> peter_y >> boat_x >> boat_y;
}

void solve() {
    // Cell (col c, row r) has its centre at the point (c, r); the lake occupies
    // x in [-0.5, m-0.5], y in [-0.5, n-0.5]. The flow digit picks one of five
    // vectors and Peter adds one of the same five, so a resulting velocity has
    // each component in {-2,-1,0,1,2}: along the flow axis he moves at speed 2
    // (next centre in 0.5 min), at speed 1 with no push (1 min), diagonally at
    // speed sqrt(2) to a diagonal neighbour (1 min), or stays put by cancelling
    // the flow. Every Peter-to-centre time is therefore a multiple of 0.5, so a
    // shortest path with edge weights 1 and 2 in half-minutes gives dist[c],
    // the earliest he can sit in each centre (he may then wait there
    // arbitrarily). Because the weights are only 1 and 2 the live frontier
    // spans at most three consecutive distances, so Dial's algorithm with
    // buckets keyed by distance % 3 settles every cell in order without a heap.
    //
    // The boat only drifts, so from any cell it has a single successor: it
    // steps one axis-aligned cell per minute. A deterministic walk with one
    // successor each must, within at most n*m steps, either stop on a
    // still-water cell, step off the board through the shore, or revisit a cell
    // it has already seen. The first repeated cell is where the trajectory
    // folds into a cycle: the prefix before it is walked once, and from there
    // the boat loops forever with period = (current time) - (time the repeated
    // cell was first seen). So a prefix cell is occupied at one fixed time,
    // while a cycle cell is occupied at base + k*period for every k >= 0; the
    // move from the last recorded cell back to the cycle's entry is itself an
    // edge of the loop and is added explicitly. Reaching the shore is always a
    // save, so one candidate answer is the min over cells of dist[c] plus the
    // time to push out of the nearest wall from c (half the usual cell-crossing
    // time, since a wall sits half a cell from the centre).
    //
    // To meet the boat there are three ways, each turned into the earliest
    // valid time and minimised together with the shore candidate. First, Peter
    // can sit in a centre the boat visits: he is fine as long as he arrives no
    // later than the boat, so the candidate is the earliest visit time >=
    // dist[c] (just the single time for a prefix cell, or base + k*period
    // rounded up past dist[c] for a cycle cell, or max(dist[c], stop_time) for
    // a stalled boat).
    //
    // Second, while the boat crosses an edge from A to A' during [t, t+1] Peter
    // can dart in from A' straight back toward A, head-on against it. To stay
    // on the edge his velocity must lie along it, so his speed sp there is 2 if
    // the flow at A' points from A' to A, 1 if A' is still water, and otherwise
    // this case does not apply. Leaving A' at his earliest time dist[A'] and
    // meeting the boat after he has covered sp*(t_m - dist[A']) while the boat
    // has covered (t_m - t) of the unit edge gives sp*(t_m - dist[A']) = 1 -
    // (t_m - t), i.e. t_m = (1 + t + sp*dist[A']) / (sp + 1). This only counts
    // when the contact point really falls inside the edge, which works out to t
    // lying in [dist[A'] - 1, dist[A'] + 1/sp]; for a cycle edge the smallest
    // qualifying t = base + k*period is used (this is the case the 2/3 sample
    // needs).
    //
    // Third, Peter can chase the boat from A in its own direction at speed 2
    // (only when the flow at A matches the boat's heading). Closing the unit
    // gap at relative speed 2 - 1 gives t_m = 2*dist[A] - t, valid when dist[A]
    // lies in [t, t + 0.5] (earlier arrival is just the centre-wait case at A).
    // If no candidate is ever finite Peter cannot be saved and the answer is
    // SOS.

    const double INF = 1e18;

    auto inside = [&](int r, int c) {
        return r >= 0 && r < n && c >= 0 && c < m;
    };

    auto flow_of = [&](int r, int c) {
        int d = grid[r][c] - '0';
        return make_pair(FX[d], FY[d]);
    };

    vector<int> dist_half(n * m, INT_MAX);
    int pr = peter_y - 1, pc = peter_x - 1;
    int br = boat_y - 1, bc = boat_x - 1;

    array<vector<int>, 3> bucket;
    dist_half[pr * m + pc] = 0;
    bucket[0].push_back(pr * m + pc);

    int max_dist = 0;
    for(int d = 0; d <= max_dist; d++) {
        auto& b = bucket[d % 3];
        while(!b.empty()) {
            int id = b.back();
            b.pop_back();
            if(dist_half[id] != d) {
                continue;
            }

            int r = id / m, c = id % m;
            auto [fx, fy] = flow_of(r, c);
            for(int k = 0; k < 5; k++) {
                int vx = fx + FX[k], vy = fy + FY[k];
                if(vx == 0 && vy == 0) {
                    continue;
                }

                int ox = (vx > 0) - (vx < 0), oy = (vy > 0) - (vy < 0);
                int nr = r + oy, nc = c + ox;
                if(!inside(nr, nc)) {
                    continue;
                }

                int w = (abs(vx) == 2 || abs(vy) == 2) ? 1 : 2;
                int nd = d + w;
                if(nd < dist_half[nr * m + nc]) {
                    dist_half[nr * m + nc] = nd;
                    bucket[nd % 3].push_back(nr * m + nc);
                    max_dist = max(max_dist, nd);
                }
            }
        }
    }

    vector<double> dist(n * m, INF);
    for(int i = 0; i < n * m; i++) {
        if(dist_half[i] != INT_MAX) {
            dist[i] = dist_half[i] * 0.5;
        }
    }

    double ans = INF;

    for(int r = 0; r < n; r++) {
        for(int c = 0; c < m; c++) {
            if(dist[r * m + c] >= INF) {
                continue;
            }

            auto [fx, fy] = flow_of(r, c);
            double best_exit = INF;
            for(int k = 0; k < 5; k++) {
                int vx = fx + FX[k], vy = fy + FY[k];
                if(vx == 0 && vy == 0) {
                    continue;
                }

                double tx = INF, ty = INF;
                if(vx > 0) {
                    tx = ((m - 1) + 0.5 - c) / vx;
                } else if(vx < 0) {
                    tx = (c + 0.5) / (-vx);
                }
                if(vy > 0) {
                    ty = ((n - 1) + 0.5 - r) / vy;
                } else if(vy < 0) {
                    ty = (r + 0.5) / (-vy);
                }

                double t_wall = min(tx, ty);
                int ox = (vx > 0) - (vx < 0), oy = (vy > 0) - (vy < 0);
                double t_center =
                    inside(r + oy, c + ox)
                        ? ((abs(vx) == 2 || abs(vy) == 2) ? 0.5 : 1.0)
                        : INF;
                if(t_wall <= t_center + 1e-12) {
                    best_exit = min(best_exit, t_wall);
                }
            }

            if(best_exit < INF) {
                ans = min(ans, dist[r * m + c] + best_exit);
            }
        }
    }

    vector<int> bcell;
    vector<int64_t> btime;
    vector<int> seen_idx(n * m, -1);
    int cr = br, cc = bc;
    int64_t bt = 0;
    int cycle_start = -1;
    int64_t period = 0;
    bool stationary = false;

    while(true) {
        int id = cr * m + cc;
        if(seen_idx[id] != -1) {
            cycle_start = seen_idx[id];
            period = bt - btime[cycle_start];
            break;
        }

        seen_idx[id] = (int)bcell.size();
        bcell.push_back(id);
        btime.push_back(bt);

        auto [fx, fy] = flow_of(cr, cc);
        if(fx == 0 && fy == 0) {
            stationary = true;
            break;
        }

        int nr = cr + fy, nc = cc + fx;
        if(!inside(nr, nc)) {
            break;
        }

        cr = nr;
        cc = nc;
        bt++;
    }

    int len = (int)bcell.size();
    auto is_cyclic = [&](int i) {
        return cycle_start != -1 && i >= cycle_start;
    };

    for(int i = 0; i < len; i++) {
        int id = bcell[i];
        if(dist[id] >= INF) {
            continue;
        }

        double base = (double)btime[i];
        if(stationary && i == len - 1) {
            ans = min(ans, max(dist[id], base));
        } else if(is_cyclic(i)) {
            double cand = base;
            if(dist[id] > base) {
                int64_t k =
                    (int64_t)ceil((dist[id] - base) / (double)period - 1e-9);
                cand = base + (double)max(0LL, k) * period;
            }
            ans = min(ans, cand);
        } else {
            if(dist[id] <= base + 1e-9) {
                ans = min(ans, base);
            }
        }
    }

    auto edge_meet = [&](int a_id, int db_idx, int64_t tb, bool cyc) {
        int ar = a_id / m, ac = a_id % m;
        int dx = FX[db_idx], dy = FY[db_idx];
        int a2r = ar + dy, a2c = ac + dx;
        if(!inside(a2r, a2c)) {
            return;
        }
        int a2 = a2r * m + a2c;

        if(dist[a2] < INF) {
            auto [f2x, f2y] = flow_of(a2r, a2c);
            int sp = 0;
            if(f2x == -dx && f2y == -dy) {
                sp = 2;
            } else if(f2x == 0 && f2y == 0) {
                sp = 1;
            }

            if(sp > 0) {
                double da = dist[a2];
                double lo = da - 1.0, hi = da + 1.0 / sp;
                double t = (double)tb;
                if(cyc && t < lo) {
                    int64_t k = (int64_t)ceil((lo - t) / (double)period - 1e-9);
                    t += (double)max(0LL, k) * period;
                }
                if(t >= lo - 1e-9 && t <= hi + 1e-9) {
                    ans = min(ans, (1.0 + t + sp * da) / (sp + 1));
                }
            }
        }

        if(dist[a_id] < INF) {
            auto [fax, fay] = flow_of(ar, ac);
            if(fax == dx && fay == dy) {
                double da = dist[a_id];
                double lo = da - 0.5, hi = da;
                double t = (double)tb;
                if(cyc && t < lo) {
                    int64_t k = (int64_t)ceil((lo - t) / (double)period - 1e-9);
                    t += (double)max(0LL, k) * period;
                }
                if(t >= lo - 1e-9 && t <= hi + 1e-9) {
                    ans = min(ans, 2.0 * da - t);
                }
            }
        }
    };

    for(int i = 0; i + 1 < len; i++) {
        int cellr = bcell[i] / m, cellc = bcell[i] % m;
        int d = grid[cellr][cellc] - '0';
        edge_meet(bcell[i], d, btime[i], is_cyclic(i));
    }

    if(cycle_start != -1) {
        int last = len - 1;
        int cellr = bcell[last] / m, cellc = bcell[last] % m;
        int d = grid[cellr][cellc] - '0';
        edge_meet(bcell[last], d, btime[last], true);
    }

    if(ans >= INF) {
        cout << "SOS\n";
        return;
    }

    cout << fixed << setprecision(2) << ans << '\n';
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
