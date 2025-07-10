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
vector<pair<pair<int, int>, pair<int, int>>> segments;
pair<int, int> tgt;

void read() {
    cin >> n;
    segments.resize(n);
    cin >> segments;
    cin >> tgt;
}


void solve() {
    int cnt = 0;
    for(int i = 0; i < (int)segments.size(); i++) {
        auto [p1, p2] = segments[i];
        auto [x1, y1] = p1;
        auto [x2, y2] = p2;

        if(x1 == x2 && tgt.second >= min(y1, y2) && tgt.second <= max(y1, y2) &&
           tgt.first == x1) {
            cout << "BORDER" << endl;
            return;
        }

        if(y1 == y2 && tgt.second == y1 && min(x1, x2) <= tgt.first &&
           tgt.first <= max(x1, x2)) {
            cout << "BORDER" << endl;
            return;
        }

        if(y1 == y2 && tgt.second < y1 && min(x1, x2) < tgt.first &&
           tgt.first <= max(x1, x2)) {
            cnt++;
        }
    }

    if(cnt % 2 == 0) {
        cout << "OUTSIDE" << endl;
    } else {
        cout << "INSIDE" << endl;
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
