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

int n, k;
vector<int> d;

void read() {
    cin >> n >> k;
    string s;
    cin >> s;

    d.resize(n);
    for(int i = 0; i < n; i++) {
        d[i] = s[i] - '0';
    }
}

template<typename T>
int least_rotation(const vector<T>& s) {
    int n = s.size();
    if(n == 0) {
        return 0;
    }

    vector<int> f(2 * n, -1);
    int k_ = 0;
    for(int j = 1; j < 2 * n; ++j) {
        int i = f[j - k_ - 1];
        while(i != -1 && s[j % n] != s[(k_ + i + 1) % n]) {
            if(s[j % n] < s[(k_ + i + 1) % n]) {
                k_ = j - i - 1;
            }
            i = f[i];
        }
        if(i == -1 && s[j % n] != s[(k_ + i + 1) % n]) {
            if(s[j % n] < s[(k_ + i + 1) % n]) {
                k_ = j;
            }
            f[j - k_] = -1;
        } else {
            f[j - k_] = i + 1;
        }
    }
    return k_;
}

template<typename T>
vector<T> max_cyclic_shift(const vector<T>& v) {
    if(v.empty()) {
        return {};
    }
    int m = v.size();
    vector<T> t(m);
    for(int i = 0; i < m; i++) {
        t[i] = -v[i];
    }
    int kk = least_rotation(t);
    vector<T> ans(m);
    for(int i = 0; i < m; ++i) {
        ans[i] = v[(kk + i) % m];
    }
    return ans;
}

void solve() {
    vector<bool> used(n, false);
    vector<int8_t> ans;
    for(int i = 0; i < n; i++) {
        if(used[i]) {
            continue;
        }

        int pos = 0;
        vector<int8_t> cycle;
        for(int j = i; !used[j]; j = (j + k) % n) {
            used[j] = true;
            cycle.push_back(d[j]);
            pos++;
        }

        cycle = max_cyclic_shift(cycle);
        if(ans.empty() || cycle > ans) {
            ans = cycle;
        }
    }

    for(int i = 0; i < n; i++) {
        cout << (int)ans[i % ans.size()];
    }

    cout << '\n';
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
