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

int n, s;
int x, h, a, b;
int m;
vector<int> pl, pr, ph;

void read() {
    cin >> n >> s;
    cin >> x >> h >> a >> b;
    cin >> m;
    pl.assign(m, 0);
    pr.assign(m, 0);
    ph.assign(m, 0);
    for(int i = 0; i < m; i++) {
        cin >> pl[i] >> pr[i] >> ph[i];
    }
}

int next_below(int fx, int from_h) {
    int best = -1;
    for(int i = 0; i < m; i++) {
        if(ph[i] < from_h && pl[i] < fx && fx < pr[i]) {
            if(best == -1 || ph[i] > ph[best]) {
                best = i;
            }
        }
    }

    return best;
}

void solve() {
    // The statement says we may stop "only one" lemming, but the sample stops
    // the first two, so that phrase is a mistranslation: we may stop as many
    // lemmings as we like. A stopped lemming becomes a permanent point that
    // reverses anyone who touches it, so the real task is to pick the fewest
    // lemmings to sacrifice as reflectors so that every later (identical)
    // lemming is guided from A into the home (a, b). The first S lemmings build
    // the reflectors - lemming i runs with reflectors 1..i-1 already in place
    // and is stopped exactly at the i-th turn it would make - so any reflector
    // set lying on the target trajectory is realizable, and S sacrifices leave
    // K = N - S survivors that all reach home along one fixed path of length D,
    // the last of them appearing at (N-1)*s, so T = (N-1)*s + D.
    //
    // A lemming falls straight down (x fixed) until it lands on the highest
    // platform strictly below it, then walks until the platform's end or a
    // reflector. On a platform entered at ex with direction d there are only
    // two useful moves:
    //
    // - no turn (cost 0): walk to the forward end and drop off it keeping d,
    // - one turn (cost 1): drop a reflector right at ex, reverse, and walk to
    //   the far end, dropping off it with direction -d; cheapest walk is the
    //   full |ex - far_end|.
    //
    // A state is therefore (platform, entry x, direction): the exit end and
    // hence the next platform depend only on the direction and the turn choice,
    // while the walked distances depend on ex. Every fall strictly lowers the
    // height, so the state graph is a DAG whose topological order is just the
    // platforms by descending height; visiting them in that order means each
    // state is finalized before it is expanded, so a single relaxation pass on
    // the lexicographic cost (turns, distance) yields, for every state, the
    // fewest turns and then the shortest distance to reach it. Landing on the
    // home platform reaches home for free when a lies forward of ex and for one
    // extra turn when it lies behind, each adding the walk |a - ex|. The
    // lexicographic minimum over all home arrivals is (S, D); if home is never
    // reached, or S >= N, nobody makes it.

    int home_plat = -1;
    for(int i = 0; i < m; i++) {
        if(ph[i] == b && pl[i] <= a && a <= pr[i]) {
            home_plat = i;
        }
    }

    vector<int> order(m);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int i, int j) {
        return ph[i] > ph[j];
    });

    vector<map<pair<int, int>, pair<int, int>>> arrival(m);

    int start = next_below(x, h);
    int answer_s = INT_MAX, answer_d = INT_MAX;

    if(start != -1) {
        arrival[start][{x, 1}] = {0, h - ph[start]};
    }

    for(int plat: order) {
        for(auto& [state, cost]: arrival[plat]) {
            auto [ex, dir] = state;
            auto [turns, dist] = cost;

            if(plat == home_plat) {
                int diff = a - ex;
                int reach_turns = turns + ((int64_t)diff * dir >= 0 ? 0 : 1);
                int reach_dist = dist + abs(diff);
                if(make_pair(reach_turns, reach_dist) <
                   make_pair(answer_s, answer_d)) {
                    answer_s = reach_turns;
                    answer_d = reach_dist;
                }
            }

            int forward_end = dir > 0 ? pr[plat] : pl[plat];
            int back_end = dir > 0 ? pl[plat] : pr[plat];

            int moves[2][3] = {
                {forward_end, dir, turns}, {back_end, -dir, turns + 1}
            };

            for(auto& mv: moves) {
                int end_x = mv[0], new_dir = mv[1], new_turns = mv[2];
                int nb = next_below(end_x, ph[plat]);
                if(nb == -1) {
                    continue;
                }

                int new_dist = dist + abs(end_x - ex) + (ph[plat] - ph[nb]);
                auto key = make_pair(end_x, new_dir);
                auto jt = arrival[nb].find(key);
                if(jt == arrival[nb].end() ||
                   make_pair(new_turns, new_dist) < jt->second) {
                    arrival[nb][key] = {new_turns, new_dist};
                }
            }
        }
    }

    if(answer_s == INT_MAX || answer_s >= n) {
        cout << "0 0" << '\n';
        return;
    }

    cout << n - answer_s << ' ' << (int64_t)(n - 1) * s + answer_d << '\n';
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
