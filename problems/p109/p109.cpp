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

void read() {
    cin >> n;
}

void solve() {
    vector<vector<int>> ans;

    ans.push_back({n});
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if(i + j > n) { 
                ans.back().push_back(i * n + j + 1);
            }
        }
    }

    int q = n + 1;
    if(q % 2 == 0) {
        q++;
    }

    for(int steps = 0; steps < n; steps++) {
        ans.push_back({q});
        int diag_x = 0, diag_y = n - steps;
        if(steps == 0) {
            diag_x = 1;
            diag_y = n - 1; 
        }

        while(diag_y >= 0 && diag_x < n) {
            ans.back().push_back(diag_x * n + diag_y + 1);
            diag_x++;
            diag_y--;
        }
        q += 2;
    }

    
    for(auto it: ans) {
        for(auto x: it) {
            cout << x << ' ';
        }
        cout << '\n';
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
