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

int n, m;
vector<pair<int, int>> actual;
vector<vector<pair<int, int>>> predictions;

void read() {
    cin >> n >> m;
    actual.resize(m);
    predictions.resize(m);

    for(int match = 0; match < m; match++) {
        cin >> actual[match];
        predictions[match].resize(n);
        cin >> predictions[match];
    }
}

void solve() {
    // The solution here is to directly implement what's described in the
    // problem. There is no real trick or insight but rather following the rules
    // in the problem statement.

    vector<int> scores(n, 0);

    for(int match = 0; match < m; match++) {
        int a = actual[match].first;
        int b = actual[match].second;

        for(int i = 0; i < n; i++) {
            int p = predictions[match][i].first;
            int q = predictions[match][i].second;

            int score = 0;

            // Check if guessed the winner (or tie)
            if((a > b && p > q) || (a < b && p < q) || (a == b && p == q)) {
                score += 2;
            }

            // Check if guessed the difference
            if(a - b == p - q) {
                score += 3;
            }

            // Check if guessed first team's goals
            if(a == p) {
                score += 1;
            }

            // Check if guessed second team's goals
            if(b == q) {
                score += 1;
            }

            scores[i] += score;
        }
    }

    cout << scores << '\n';
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
