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
vector<pair<int, int>> a;

void read() {
    cin >> n;
    a.resize(n);
    cin >> a;
}

void solve() {
    vector<int> order(n);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int i, int j) {
        return a[i].first < a[j].first;
    });

    vector<int> par(n, -1);
    vector<int> l(n, -1), r(n, -1);
    vector<int> st;

    function<int(int)> collapse = [&](int last) {
        int prev_top = st.back();
        st.pop_back();

        if(last != -1) {
            par[last] = prev_top;
            r[prev_top] = last;
        }

        return prev_top;
    };

    for(int i: order) {
        int last = -1;
        while(!st.empty() && a[st.back()].second > a[i].second) {
            last = collapse(last);
        }

        if(last != -1) {
            par[last] = i;
            l[i] = last;
        }

        st.push_back(i);
    }

    cout << "YES\n";

    int last = -1;
    while(!st.empty()) {
        last = collapse(last);
    }

    for(int i = 0; i < n; i++) {
        cout << par[i] + 1 << ' ' << l[i] + 1 << ' ' << r[i] + 1 << '\n';
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
