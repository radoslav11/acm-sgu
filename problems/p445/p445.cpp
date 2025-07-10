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
long double vw, vc;
vector<pair<long double, long double>> points;

void read() {
    cin >> n >> vw >> vc;
    points.resize(n);
    for(int i = 0; i < n; i++) {
        cin >> points[i];
    }
}

void solve() {
    vector<long double> dp(n, 1e12);
    dp[0] = 0.0;

    for(int i = 0; i < n; i++) {
        auto [x, y] = points[i];
        if(i > 0) {
            auto [x_prev, y_prev] = points[i - 1];
            dp[i] =
                min(dp[i], dp[i - 1] + sqrt(
                                           (x - x_prev) * (x - x_prev) +
                                           (y - y_prev) * (y - y_prev)
                                       ) / vw);
        }

        for(int j = i - 1; j >= 0; j--) {
            auto [x_prev, y_prev] = points[j];
            auto [x_prev_next, y_prev_next] = points[j + 1];

            long double up = max(y_prev, y_prev_next);
            long double down = min(y_prev, y_prev_next);

            if(down <= y && y <= up) {
                long double slope_x =
                    y_prev_next == y_prev
                        ? 0.0
                        : (x_prev_next - x_prev) / (y_prev_next - y_prev);
                long double intersection_x = x_prev + slope_x * (y - y_prev);
                if(y_prev_next == y_prev) {
                    intersection_x = x_prev_next;
                }

                long double dist_intersection_to_prev = sqrt(
                    (x_prev - intersection_x) * (x_prev - intersection_x) +
                    (y_prev - y) * (y_prev - y)
                );

                dp[i] =
                    min(dp[i], dp[j] + dist_intersection_to_prev / vw +
                                   (x - intersection_x) / vc);
            }

            if(y_prev <= y) {
                break;
            }
        }

        for(int j = i + 1; j < n; j++) {
            auto [x_next, y_next] = points[j];
            auto [x_next_prev, y_next_prev] = points[j - 1];

            long double up = max(y_next, y_next_prev);
            long double down = min(y_next, y_next_prev);

            if(down <= y && y <= up) {
                long double slope_x =
                    y_next_prev == y_next
                        ? 0.0
                        : (x_next - x_next_prev) / (y_next - y_next_prev);
                long double intersection_x =
                    x_next_prev + slope_x * (y - y_next_prev);
                long double dist_intersection_to_next = sqrt(
                    (x_next - intersection_x) * (x_next - intersection_x) +
                    (y_next - y) * (y_next - y)
                );

                dp[j] =
                    min(dp[j], dp[i] + dist_intersection_to_next / vw +
                                   (intersection_x - x) / vc);
            }

            if(y_next <= y) {
                break;
            }
        }
    }

    cout << setprecision(10) << fixed << dp.back() << '\n';
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
