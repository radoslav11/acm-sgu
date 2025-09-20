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

int a, b, n;
vector<int> cnt;

void read() {
    cin >> a >> b;
    cin >> n;
    cnt.resize(n);
    cin >> cnt;
    cnt.insert(cnt.begin(), b);
}

void dfs(
    int pos, int cnt_boys, vector<vector<bool>>& visited,
    vector<vector<tuple<int, int, int>>>& par
) {

    int cnt_girls = cnt[pos] - cnt_boys;
    
    if(cnt_girls < 0 || cnt_boys < 0 || cnt_boys > a || cnt_girls > b) {
        return;
    }

    if(pos == n) {
        return;
    }

    for(int cnt_girls_leave = 0; cnt_girls_leave <= cnt_girls;
        cnt_girls_leave++) {
        int new_cnt_girls = cnt_girls - cnt_girls_leave;
        int new_cnt_boys = cnt[pos + 1] - new_cnt_girls;
        if(new_cnt_boys >= cnt_boys && new_cnt_boys <= a &&
           !visited[pos + 1][new_cnt_boys]) {
            par[pos + 1][new_cnt_boys] = {
                cnt_boys, new_cnt_boys - cnt_boys, cnt_girls_leave
            };
            visited[pos + 1][new_cnt_boys] = true;
            dfs(pos + 1, new_cnt_boys, visited, par);
        }
    }
}

void solve() {
    vector<vector<bool>> visited(n + 1, vector<bool>(a + 1, false));
    vector<vector<tuple<int, int, int>>> par(
        n + 1, vector<tuple<int, int, int>>(a + 1, {-1, -1, -1})
    );

    dfs(0, 0, visited, par);

    for(int cnt_boys = 0; cnt_boys <= a; cnt_boys++) {
        if(visited[n][cnt_boys]) {
            vector<pair<int, int>> ans;
            int pos = n, c = cnt_boys;
            while(pos > 0) {
                auto [prev_boys, move_boys, move_girls] = par[pos][c];
                ans.push_back({move_boys, move_girls});
                c = prev_boys;
                pos--;
            }

            reverse(ans.begin(), ans.end());
            for(auto [x, y]: ans) {
                cout << x << ' ' << y << '\n';
            }
            return;
        }
    }
    cout << "ERROR" << endl;
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
