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

void read() { cin >> n; }

void solve() {
    // To maximize the risk function (sum of squared differences), we want to
    // pair numbers with maximum differences: (1,n), (2,n-1), (3,n-2), etc.
    // These pairs will be placed at adjacent positions (2i-1, 2i). The key is
    // how to arrange these pairs around the circle. We start with (1,n), then
    // cycle through 4 operations: inverted to back, normal to front, normal to
    // back, inverted to front. This creates large differences in both the
    // i -> i+2 direction and between the inner/outer layers. Something like:
    //
    // S5:   5   , n - 4
    // S3:   n-2 , 3
    // S1:   1   , n
    // S2:   n-1 , 2
    // S4:   4   , n - 3

    list<pair<int, int>> pairs;
    for(int i = 2; 2 * i <= n; i++) {
        pairs.push_back({i, n - i + 1});
    }

    list<int> ans = {1, n};
    for(int idx = 0; idx < n / 2 - 1; idx++) {
        auto [x, y] = pairs.front();
        pairs.pop_front();

        int op = idx % 4;
        if(op == 0) {
            ans.push_back(y);
            ans.push_back(x);
        } else if(op == 1) {
            ans.push_front(x);
            ans.push_front(y);
        } else if(op == 2) {
            ans.push_back(x);
            ans.push_back(y);
        } else {
            ans.push_front(y);
            ans.push_front(x);
        }
    }

    for(auto x: ans) {
        cout << x << " ";
    }
    cout << "\n";
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
