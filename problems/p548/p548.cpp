#include <bits/stdc++.h>
#include <functional>
#include <vector>

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
vector<pair<char, int>> a;

void read() {
    cin >> n;
    cin.ignore();
    a.resize(n - 1);
    for(int i = 0; i < n - 1; i++) {
        cin >> a[i].first >> a[i].second;
    }
}

void solve() {
    int sum = 0;
    priority_queue<
        pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>>
        q;

    for(int i = 0; i < n - 2; i++) {
        auto [c, x] = a[i];
        if(c == 'd') {
            sum += x;
            q.push({x, i});
        } else {
            while(q.size() >= x) {
                sum -= q.top().first;
                q.pop();
            }
        }
    }

    if(a.back().first != 'p' || q.size() < a.back().second) {
        cout << -1 << '\n';
        return;
    } else {
        cout << sum << '\n'; 
        vector<int> kills;
        while(!q.empty()) {
            kills.push_back(q.top().second + 2);
            q.pop();
        }

        cout << kills.size() << '\n';
        sort(kills.begin(), kills.end());
        cout << kills << '\n';
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
