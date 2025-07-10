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

string a, b;

void read() { cin >> a >> b; }

pair<vector<int>, vector<int>> get_pos_plus_minus(const string& s) {
    vector<int> pos, neg;
    for(int i = 0; i < (int)s.size(); i++) {
        if(s[i] == '+') {
            pos.push_back(i);
        } else {
            neg.push_back(i);
        }
    }
    return {pos, neg};
}

int64_t count_inversions(vector<int> a) {
    function<int64_t(vector<int>&, int, int)> merge_sort = [&](vector<int>& a,
                                                               int l, int r) {
        if(l == r) {
            return 0LL;
        }
        int m = (l + r) / 2;
        int64_t ans = merge_sort(a, l, m) + merge_sort(a, m + 1, r);
        vector<int> b;
        int i = l, j = m + 1;
        while(i <= m && j <= r) {
            if(a[i] <= a[j]) {
                b.push_back(a[i++]);
            } else {
                b.push_back(a[j++]);
                ans += m - i + 1;
            }
        }
        while(i <= m) {
            b.push_back(a[i++]);
        }
        while(j <= r) {
            b.push_back(a[j++]);
        }
        for(int i = l; i <= r; i++) {
            a[i] = b[i - l];
        }
        return ans;
    };

    return merge_sort(a, 0, a.size() - 1);
}

void solve() {
    auto [pos_a, neg_a] = get_pos_plus_minus(a);
    auto [pos_b, neg_b] = get_pos_plus_minus(b);

    if(pos_a.size() != pos_b.size() || neg_a.size() != neg_b.size()) {
        cout << -1 << '\n';
        return;
    }

    reverse(pos_b.begin(), pos_b.end());
    reverse(neg_b.begin(), neg_b.end());
    vector<int> perm;
    for(int i = 0; i < (int)a.size(); i++) {
        if(a[i] == '+') {
            perm.push_back(pos_b.back());
            pos_b.pop_back();
        } else {
            perm.push_back(neg_b.back());
            neg_b.pop_back();
        }
    }

    cout << count_inversions(perm) << '\n';
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
