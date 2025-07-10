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

int N, K, P;
vector<int> B;
vector<vector<pair<int, int>>> gums;

void read() {
    cin >> N >> K >> P;
    B.resize(K);
    cin >> B;
    gums = vector<vector<pair<int, int>>>(K);
    for(int i = 0; i < N; ++i) {
        int cost, gum;
        cin >> cost >> gum;
        gums[gum - 1].push_back({cost, i + 1});
    }
    for(auto& gum: gums) {
        sort(gum.begin(), gum.end());
    }
}

void solve() {
    vector<vector<pair<int, int>>> dp(
        K + 1, vector<pair<int, int>>(N + 1, {INT_MAX, 0})
    );
    dp[0][0] = {0, 0};

    for(int g = 0; g < K; ++g) {
        for(int t = 0; t <= N; ++t) {
            if(dp[g][t].first == INT_MAX) {
                continue;
            }
            int cost = B[g];
            if(dp[g][t].first < dp[g + 1][t].first) {
                dp[g + 1][t] = {dp[g][t].first, -1};
            }
            for(int p = 0; p < min((int)gums[g].size(), N - t); ++p) {
                cost += gums[g][p].first;
                if(dp[g][t].first + cost < dp[g + 1][t + p + 1].first) {
                    dp[g + 1][t + p + 1] = {dp[g][t].first + cost, p + 1};
                }
            }
        }
    }

    int max_teeth = 0;
    while(max_teeth < N && dp[K][max_teeth + 1].first <= P) {
        max_teeth++;
    }

    cout << max_teeth << '\n';

    vector<int> solution;
    int current_teeth = max_teeth;
    for(int g = K; g > 0; --g) {
        int selected = dp[g][current_teeth].second;
        if(selected != -1) {
            for(int i = 0; i < selected; ++i) {
                solution.push_back(gums[g - 1][i].second);
            }

            current_teeth -= selected;
        }
    }

    sort(solution.begin(), solution.end());
    cout << solution << '\n';
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    // cin >> T;
    int T = 1;
    for(int test = 1; test <= T; test++) {
        read();
        solve();
    }

    return 0;
}
