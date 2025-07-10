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
vector<int> a;

void read() {
    cin >> n;
    a.resize(n);
    cin >> a;
}

void solve() {
    int sum_deg = accumulate(a.begin(), a.end(), 0);
    assert(sum_deg % 2 == 0);

    vector<int> perm(n);
    iota(perm.begin(), perm.end(), 0);
    sort(perm.begin(), perm.end(), [&](int i, int j) {
        return a[i] > a[j];
    });
    sort(a.rbegin(), a.rend());

    vector<pair<int, int>> matches;
    int pos = 0;
    for(int i = 0; i < sum_deg / 2; i++) {
        if(a[pos] == 1) {
            matches.emplace_back(pos + 1, pos);
            a[pos]--;
            a[pos + 1]--;
            pos++;
        } else {
            matches.emplace_back(pos, -1);
            a[pos]--;
        }
    }

    for(auto& match: matches) {
        if(match.second != -1) {
            continue;
        }
        while(a[pos] == 0) {
            pos++;
        }

        match.second = pos;
        a[pos]--;
    }

    cout << matches.size() << '\n';
    for(const auto& match: matches) {
        cout << perm[match.first] + 1 << ' ' << perm[match.second] + 1 << '\n';
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
