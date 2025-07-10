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

string s;

void read() { cin >> s; }

void backtracking(
    vector<int>& mapping, int64_t& g, int digit_mask, int pos, int64_t curr,
    int& steps
) {
    if(g == 1 || steps <= 0) {
        return;
    }

    if(pos == s.size()) {
        g = gcd(g, curr);
        steps--;
        return;
    }

    int c = s[pos] - 'a';
    if(mapping[c] != -1) {
        backtracking(
            mapping, g, digit_mask, pos + 1, curr * 10 + mapping[c], steps
        );
    } else {
        for(int d = (pos == 0); d < 10; d++) {
            if(digit_mask & (1 << d)) {
                continue;
            }
            mapping[c] = d;
            backtracking(
                mapping, g, digit_mask | (1 << d), pos + 1, curr * 10 + d, steps
            );
            mapping[c] = -1;
        }
    }
}

void solve() {
    int n = s.size();
    vector<int> mapping(26, -1);
    int64_t g = 0;
    int steps = 200;
    backtracking(mapping, g, 0, 0, 0, steps);

    map<char, int64_t> masks;
    int64_t mask = 1;
    for(int i = n - 1; i >= 0; i--) {
        masks[s[i]] += mask;
        mask = mask * 10;
    }

    for(auto [xl, mask_x]: masks) {
        for(auto [yl, mask_y]: masks) {
            for(int x = 0; x < 10; x++) {
                for(int y = max(x - 1, 0); y < min(x + 2, 10); y++) {
                    if(xl == yl || x == y || (xl == s[0] && x == 0) ||
                       (yl == s[0] && y == 0) || (xl == s[0] && y == 0) ||
                       (yl == s[0] && x == 0)) {
                        continue;
                    }
                    g =
                        gcd(g, -mask_x * (int64_t)(x - y) -
                                   mask_y * (int64_t)(y - x));
                }
            }
        }
    }

    assert(g > 0);

    vector<pair<int64_t, int>> prime_divs;
    for(int x = 2; x * 1ll * x <= g; x++) {
        if(g % x != 0) {
            continue;
        }

        int cnt = 0;
        while(g % x == 0) {
            cnt++;
            g /= x;
        }
        prime_divs.push_back({x, cnt});
    }

    if(g > 1) {
        prime_divs.push_back({g, 1});
    }

    vector<int64_t> all_divs = {1};
    for(auto [x, cnt]: prime_divs) {
        int sz = all_divs.size();
        for(int i = 0; i < sz; i++) {
            int64_t y = all_divs[i];
            for(int j = 1; j <= cnt; j++) {
                all_divs.push_back(y * x);
                y *= x;
            }
        }
    }

    sort(all_divs.begin(), all_divs.end());
    cout << all_divs << '\n';
}

/*
    gcd(a, a - mask_x * x - mask_y * y + mask_x * y + mask_y * x) = gcd(a, a -
   mask_x * (x - y) - mask_y * (y - x)) = gcd(a, a - (mask_x + mask_y) * (x -
   y)) = gcd(a, (mask_x + mask_y) * (y - x))
*/

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T = 1;
    cin >> T;
    for(int test = 1; test <= T; test++) {
        read();
        cout << "Case " << test << ": ";
        solve();
    }

    return 0;
}
