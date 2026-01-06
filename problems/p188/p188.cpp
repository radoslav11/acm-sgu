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

int n, t;
vector<int> l, v;

void read() {
    cin >> n >> t;
    l.resize(n);
    v.resize(n);
    cin >> l >> v;
}

int64_t first_c_geq(int64_t val) {
    if(val >= 0) {
        return (val + 999) / 1000;
    }
    return -((-val) / 1000);
}

int64_t last_c_leq(int64_t val) {
    if(val >= 0) {
        return val / 1000;
    }
    return -((-val + 999) / 1000);
}

int64_t count_meetings(int l1, int v1, int l2, int v2) {
    if((v1 > 0) == (v2 > 0)) {
        return 0;
    }

    int64_t dv = v1 - v2;
    int64_t dl = l2 - l1;

    int64_t c_low, c_high;
    if(dv > 0) {
        c_low = first_c_geq(-dl + 1);
        c_high = last_c_leq(dv * t - dl);
    } else {
        c_low = first_c_geq(dv * t - dl);
        c_high = last_c_leq(-dl - 1);
    }

    if(c_high < c_low) {
        return 0;
    }
    return c_high - c_low + 1;
}

void solve() {
    // Instead of having a circle, we can think of this as a plane with lines y
    // = Vi * t + Li for each of the soldiers. We will simply just make an
    // infinite number of copies starting at Li, Li + 1000, Li + 2000 and so on
    // (also same for negatives). Let's figure out how many intersections there
    // would be between each pair of lines. We know the form of one line is y =
    // Vi * t + Li, while we can consider all the lines y = Vi * t + Li + 1000 *
    // c. Particularly, we can binary search on c to figure out the first t > 0
    // and the last integer t <= T, or alternatively just do some simple math.
    // The answer being the length of this interval. The time complexity will be
    // O(N^2).

    vector<int64_t> ans(n, 0);
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if(i != j) {
                ans[i] += count_meetings(l[i], v[i], l[j], v[j]);
            }
        }
    }
    for(int i = 0; i < n; i++) {
        cout << ans[i];
        if(i < n - 1) {
            cout << ' ';
        }
    }
    cout << '\n';
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
