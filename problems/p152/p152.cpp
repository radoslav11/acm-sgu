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
    int sum = 0;
    for(int x: a) {
        sum += x;
    }

    vector<int> p(n);
    int sum_p = 100;
    for(int i = 0; i < n; i++) {
        p[i] = (a[i] * 100) / sum;
        sum_p -= p[i];
    }

    for(int i = 0; i < n; i++) {
        if(p[i] * 1ll * sum != a[i] * 100ll && sum_p) {
            p[i]++; 
            sum_p--;
        }
    }

    cout << p << endl;
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
