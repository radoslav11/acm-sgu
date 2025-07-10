#include <bits/stdc++.h>

using namespace std;

template<typename T1, typename T2>
ostream &operator<<(ostream &out, const pair<T1, T2> &x) {
    return out << x.first << ' ' << x.second;
}

template<typename T1, typename T2>
istream &operator>>(istream &in, pair<T1, T2> &x) {
    return in >> x.first >> x.second;
}

template<typename T>
istream &operator>>(istream &in, vector<T> &a) {
    for(auto &x: a) {
        in >> x;
    }
    return in;
};

template<typename T>
ostream &operator<<(ostream &out, const vector<T> &a) {
    for(auto x: a) {
        out << x << ' ';
    }
    return out;
};

int n;

void read() {
    cin >> n;
}

void solve() {
    if(n % 2 == 0) {
        cout << "No solution\n";
    } else {
        vector<int> ans;
        while(n > 1) {
            int x = (n - 1) / 2;
            int y = (n + 1) / 2;
            if(x % 2 == 0) {
                ans.push_back(1);
                n = y;
            } else {
                ans.push_back(2);
                n = x;
            }
        }

        reverse(ans.begin(), ans.end());
        cout << ans.size() << '\n';
        cout << ans << '\n';
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
