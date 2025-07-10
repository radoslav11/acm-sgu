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

int n, f;
vector<int> a;

void read() {
    cin >> n >> f;
    a.resize(n);
    cin >> a;
}

void solve() {
    int tgt = a[0];
    vector<int> ans;
    while(!a.empty()) {
        bool added = false;
        while(true) {
            auto it = find(a.begin(), a.end(), f);
            if(it != a.end()) {
                if(!added) {
                    ans.push_back(f);
                }
                added = true;
                a.erase(it);
            } else {
                break;
            }
        }

        if(tgt == f) {
            if(a.empty()) {
                break;
            } else {
                tgt = a[0];
            }
        }

        if(tgt > f) {
            f++;
        } else {
            f--;
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
