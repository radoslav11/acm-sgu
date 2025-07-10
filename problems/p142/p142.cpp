#include <bits/stdc++.h>
// #include <coding_library/strings/suffix_automaton.hpp>

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
string s;

void read() { cin >> n >> s; }

void solve() {
    vector<int> cnt;
    for(int len = 1;; len++) {
        cnt.assign(1 << len, 0);
        int mask = 0;
        for(int i = 0; i < len; i++) {
            mask = (mask << 1) | (s[i] - 'a');
        }
        cnt[mask]++;
        for(int i = len; i < n; i++) {
            mask = ((mask << 1) & ((1 << len) - 1)) | (s[i] - 'a');
            cnt[mask]++;
        }

        for(int i = 0; i < (1 << len); i++) {
            if(cnt[i] == 0) {
                cout << len << '\n';
                for(int j = len - 1; j >= 0; j--) {
                    cout << (char)((i >> j & 1) + 'a');
                }
                return;
            }
        }
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
