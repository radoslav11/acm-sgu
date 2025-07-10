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

int n, d;
vector<pair<int, int>> a;

void read() {
    cin >> n >> d;
    a.resize(n);
    cin >> a;
}

void solve() {
    vector<pair<int, int>> pref(n), suff(n);
    for(int i = 0; i < n; i++) {
        pref[i] = {a[i].second, i};
        if(i > 0) {
            pref[i] = max(pref[i], pref[i - 1]);
        }
    }

    for(int i = n - 1; i >= 0; i--) {
        suff[i] = {a[i].second, i};
        if(i + 1 < n) {
            suff[i] = max(suff[i], suff[i + 1]);
        }
    }

    int ans = 0;
    pair<int, int> ans_pos = {-1, -1};

    for(int i = 0; i < n; i++) {
        auto it = lower_bound(a.begin(), a.end(), make_pair(a[i].first + d, 0));
        if(it == a.end()) {
            continue;
        }

        int j = it - a.begin();
        int cand = pref[i].first + suff[j].first;
        if(cand >= ans) {
            ans = cand;
            ans_pos = {pref[i].second + 1, suff[j].second + 1};
        }
    }

    cout << ans_pos << '\n';
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
