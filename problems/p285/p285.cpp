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
vector<double> sector_prob;
vector<double> internet_prob;

void read() {
    cin >> n;
    sector_prob.resize(12);
    cin >> sector_prob;
    internet_prob.resize(n);
    cin >> internet_prob;
}

void solve() {
    // This is a classic problem with dynamic programming, where we need to
    // define a state space that is small enough. Each of the 12 normal sectors
    // can be chosen at most once, so this is 2^12 states. Also, we can clearly
    // choose the internet at most 11 times as by pigeonhole principle there
    // will be at least one winning team. We also need to maintain the 6x6
    // states for the current score between the teams. Overall this is 2^12 * 12
    // * 6 * 6 ~= 1.77M states, which fits conveniently. In practice, we
    // actually have fewer states that will actually visit, as we know that
    // bitcount(mask) + cnt_13 <= 11. For each state, we can go through the 13
    // chances of where the arrow will land, and always keep the closest
    // non-chosen envelope while iterating through the 13 options (this is to
    // avoid making a 13 * 13 nested loop in the transitions which is slow).

    double avg_internet = 0;
    for(auto x: internet_prob) {
        avg_internet += x;
    }
    avg_internet /= n;

    vector<vector<vector<double>>> dp(
        8192, vector<vector<double>>(6, vector<double>(6, 0.0))
    );
    dp[0][0][0] = 1.0;

    vector<double> ans(12, 0.0);

    for(int mask = 0; mask < 8192; mask++) {
        for(int cs = 0; cs < 6; cs++) {
            for(int vs = 0; vs < 6; vs++) {
                double cur = dp[mask][cs][vs];
                if(cur < 1e-18) {
                    continue;
                }

                auto is_avail = [&](int s) {
                    return (s == 13) ? !(mask & (1 << 12))
                                     : !(mask & (1 << (s - 1)));
                };

                int wrap_target = -1;
                for(int s = 1; s <= 13; s++) {
                    if(is_avail(s)) {
                        wrap_target = s;
                        break;
                    }
                }
                if(wrap_target < 0) {
                    continue;
                }

                int freq[14] = {};
                int target = wrap_target;
                for(int s = 13; s >= 1; s--) {
                    if(is_avail(s)) {
                        target = s;
                    }
                    freq[target]++;
                }

                for(int s = 1; s <= 13; s++) {
                    if(!freq[s]) {
                        continue;
                    }
                    double land = cur * freq[s] / 13.0;
                    double p = (s <= 12) ? sector_prob[s - 1] : avg_internet;
                    int new_mask = mask | (1 << (s <= 12 ? s - 1 : 12));

                    if(cs + 1 == 6) {
                        ans[vs] += land * p;
                    } else {
                        dp[new_mask][cs + 1][vs] += land * p;
                    }

                    if(vs + 1 == 6) {
                        ans[6 + 5 - cs] += land * (1.0 - p);
                    } else {
                        dp[new_mask][cs][vs + 1] += land * (1.0 - p);
                    }
                }
            }
        }
    }

    cout << fixed << setprecision(3);
    for(int i = 0; i < 12; i++) {
        cout << ans[i] << "\n";
    }
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
