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

int N, M, is, js;

void read() { cin >> N >> M >> is >> js; }

void solve() {
    // This problem is a direct implementation of the problem statement.
    // We simply maintain the list of buildings that still exist, and the
    // current position of the robot.

    vector<vector<bool>> destroyed(N + 1, vector<bool>(M + 1, false));

    int i = is, j = js;
    string result = "";

    while(true) {
        destroyed[i][j] = true;

        int di[] = {1, 0, -1, 0};
        int dj[] = {0, -1, 0, 1};
        char dir[] = {'D', 'L', 'U', 'R'};

        bool moved = false;
        for(int k = 0; k < 4; k++) {
            int ni = i + di[k];
            int nj = j + dj[k];

            if(ni >= 1 && ni <= N && nj >= 1 && nj <= M && !destroyed[ni][nj]) {
                result += dir[k];
                i = ni;
                j = nj;
                moved = true;
                break;
            }
        }

        if(!moved) {
            break;
        }
    }

    cout << result << endl;
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
