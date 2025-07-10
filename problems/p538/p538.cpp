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

string s;

void read() { getline(cin, s); }

void solve() {
    int n = s.size();
    vector<int> prev_bracket(n, -1);
    vector<int> next_bracket(n, -1);
    vector<int> cnt_non_letters(n, 0);
    for(int i = 0; i < n; i++) {
        if(s[i] == '(') {
            prev_bracket[i] = i;
        } else {
            prev_bracket[i] = i ? prev_bracket[i - 1] : -1;
        }

        if(!isalpha(s[i]) && s[i] != ' ' && s[i] != '(' && s[i] != ')') {
            cnt_non_letters[i]++;
        }
        cnt_non_letters[i] += i ? cnt_non_letters[i - 1] : 0;
    }

    function<int(int, int)> get_non_letters = [&](int l, int r) {
        return cnt_non_letters[r] - (l ? cnt_non_letters[l - 1] : 0);
    };

    for(int i = n - 1; i >= 0; i--) {
        if(s[i] == ')') {
            next_bracket[i] = i;
        } else {
            next_bracket[i] = i < n - 1 ? next_bracket[i + 1] : -1;
        }
    }

    int ans = 0;
    for(int i = 0; i < n; i++) {
        if(s[i] == '(' || s[i] == ')') {
            ans++;
        }

        if(s[i] == '(' && next_bracket[i] != -1 &&
           prev_bracket[next_bracket[i]] == i &&
           get_non_letters(i, next_bracket[i]) == 0) {
            ans--;
        }

        if(s[i] == ')' && prev_bracket[i] != -1 &&
           next_bracket[prev_bracket[i]] == i &&
           get_non_letters(prev_bracket[i], i) == 0) {
            ans--;
        }
    }

    cout << ans << '\n';
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
