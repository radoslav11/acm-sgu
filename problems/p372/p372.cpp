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

int k, n;
vector<pair<int, int>> tea;

void read() {
    cin >> k >> n;
    tea.resize(n);
    cin >> tea;
}

void solve() {
    // The first observation is that in an optimal arrangement for a given type
    // of tea, is always going to be in increasing order of price - if this
    // wasn't the case, we can swap any inversion to achieve a better
    // configuration. If we didn't have the three consecutive rounds constraint,
    // the optimal order would have been true for all selected teas, but we
    // have cases like N=5, K=5 and (1, b), (2, b), (0, g), (3, b), (4, b): here
    // this is the only valid option and the (0, g) is out of order. However,
    // we can show that it's always optimal to keep a prefix of the black and
    // green teas when sorted - if we get some tea i we should also get all teas
    // before i of the same colour. This gives us a direct DP solution with
    // state dp[cnt_black][cnt_green][tail_state]. At each position we can
    // either take the next black, or the next green tea (with index
    // cnt_{black,green}+1). This works in O(K^2). In terms of implementation,
    // the tail states we use are: 0=start, 1=...G, 2=...B, 3=...GG, 4=...BB.

    vector<pair<int, int>> black_teas, green_teas;
    for(int i = 0; i < n; i++) {
        auto [cost, kind] = tea[i];
        if(kind == 1) {
            black_teas.emplace_back(cost, i + 1);
        } else {
            green_teas.emplace_back(cost, i + 1);
        }
    }
    ranges::sort(black_teas);
    ranges::sort(green_teas);

    int nb = (int)black_teas.size();
    int ng = (int)green_teas.size();

    constexpr int NUM_STATES = 5;
    constexpr int transition[NUM_STATES][2] = {
        {1, 2}, {3, 2}, {1, 4}, {-1, 2}, {1, -1},
    };

    auto fill_val = array<int64_t, 5>{};
    ranges::fill(fill_val, numeric_limits<int64_t>::max());
    vector dp(min(nb, k) + 1, vector(min(ng, k) + 1, fill_val));

    struct Parent {
        int cb, cg, state;
    };
    vector par(min(nb, k) + 1, vector(min(ng, k) + 1, array<Parent, 5>{{}}));

    dp[0][0][0] = 0;

    for(int cb = 0; cb <= min(nb, k); cb++) {
        for(int cg = 0; cg <= min(ng, k); cg++) {
            int pos = cb + cg;
            if(pos >= k) {
                continue;
            }
            int guests = k - pos;

            for(int s = 0; s < NUM_STATES; s++) {
                if(dp[cb][cg][s] == numeric_limits<int64_t>::max()) {
                    continue;
                }

                for(int choice: {0, 1}) {
                    int ns = transition[s][choice];
                    if(ns == -1) {
                        continue;
                    }

                    auto& teas = choice == 0 ? green_teas : black_teas;
                    int cnt = choice == 0 ? cg : cb;
                    int max_cnt = choice == 0 ? ng : nb;
                    if(cnt >= max_cnt || cnt >= k) {
                        continue;
                    }

                    int64_t new_cost =
                        dp[cb][cg][s] + (int64_t)guests * teas[cnt].first;
                    int ncb = cb + choice, ncg = cg + (1 - choice);

                    if(new_cost < dp[ncb][ncg][ns]) {
                        dp[ncb][ncg][ns] = new_cost;
                        par[ncb][ncg][ns] = {cb, cg, s};
                    }
                }
            }
        }
    }

    int64_t best = numeric_limits<int64_t>::max();
    int best_cb = -1, best_cg = -1, best_state = -1;
    for(int cb = 0; cb <= min(nb, k); cb++) {
        int cg = k - cb;
        if(cg < 0 || cg > ng) {
            continue;
        }
        for(int s = 0; s < NUM_STATES; s++) {
            if(dp[cb][cg][s] < best) {
                best = dp[cb][cg][s];
                best_cb = cb;
                best_cg = cg;
                best_state = s;
            }
        }
    }

    if(best == numeric_limits<int64_t>::max()) {
        cout << "Impossible" << '\n';
        return;
    }

    vector<int> result(k);
    int cb = best_cb, cg = best_cg, s = best_state;
    for(int pos = k - 1; pos >= 0; pos--) {
        auto [pcb, pcg, ps] = par[cb][cg][s];
        if(pcg < cg) {
            result[pos] = green_teas[cg - 1].second;
        } else {
            result[pos] = black_teas[cb - 1].second;
        }
        cb = pcb;
        cg = pcg;
        s = ps;
    }

    cout << result << '\n';
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
