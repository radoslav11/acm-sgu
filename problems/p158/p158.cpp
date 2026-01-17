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

int l, m, n;
vector<int> p;
vector<int> d;

void read() {
    cin >> l >> m;
    p.resize(m);
    cin >> p;
    cin >> n;
    d.resize(n);
    d[0] = 0;
    for(int i = 1; i < n; i++) {
        cin >> d[i];
    }
}

void solve() {
    // This is more of an implementation problem. As the L is fairly small, we
    // can try all possible starts S on either the integer, or .5 grid, and
    // simply evaluate linearly how much the utility would be.

    int l2 = l * 2;
    vector<int> p2(m), d2(n);
    for(int i = 0; i < m; i++) {
        p2[i] = p[i] * 2;
    }
    for(int i = 0; i < n; i++) {
        d2[i] = d[i] * 2;
    }

    int max_s = l2 - d2[n - 1];

    int64_t best_sum = -1;
    int best_s = 0;

    for(int s = 0; s <= max_s; s++) {
        int64_t sum = 0;
        int j = 0;
        for(int i = 0; i < m; i++) {
            int target = p2[i] - s;
            while(j + 1 < n && 2 * target > d2[j] + d2[j + 1]) {
                j++;
            }
            sum += abs(target - d2[j]);
        }
        if(sum > best_sum) {
            best_sum = sum;
            best_s = s;
        }
    }

    auto print_half = [](int64_t x) {
        if(x % 2 == 0) {
            cout << x / 2;
        } else {
            cout << x / 2 << ".5";
        }
    };

    print_half(best_s);
    cout << " ";
    print_half(best_sum);
    cout << "\n";
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
