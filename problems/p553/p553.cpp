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

int n, m, k;
vector<pair<int, int>> a;

void read() {
    cin >> n >> m >> k;
    a.resize(n);
    cin >> a;
}

void solve() {
    vector<int> pref_w(n), pref_score(n);
    for(int i = 0; i < n; i++) {
        pref_w[i] = a[i].first;
        pref_score[i] = a[i].second;
        if(i > 0) {
            pref_w[i] += pref_w[i - 1];
            pref_score[i] += pref_score[i - 1];
        }
    }

    function<int(int, int)> get_weight = [&](int l, int r) -> int {
        if(l > r) {
            return 0;
        }
        return pref_w[r] - (l > 0 ? pref_w[l - 1] : 0);
    };
    function<int(int, int)> get_score = [&](int l, int r) -> int {
        if(l > r) {
            return 0;
        }
        return pref_score[r] - (l > 0 ? pref_score[l - 1] : 0);
    };

    function<int(int)> get_score_from_left = [&](int len) -> int {
        int low = 0, high = len - m - 1, mid, ans_pos = -1;
        int weight_hanging = get_weight(len - m, len - 1);
        while(low <= high) {
            mid = (low + high) / 2;
            if(get_weight(mid, len - m - 1) * k >= weight_hanging) {
                ans_pos = mid;
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }

        assert(ans_pos != -1);
        return get_score(0, ans_pos - 1);
    };

    int ans = 0, add = 0, best_len = 0;
    for(int len = n; len > m; len--) {
        if(get_weight(len - m, len - 1) > k * get_weight(0, len - m - 1)) {
            break;
        }
        int new_score = get_score_from_left(len) + add;
        if(new_score > ans) {
            ans = new_score;
            best_len = len;
        }

        add += a[len - 1].second;
    }

    if(ans == 0) {
        cout << 0 << " " << 0 << '\n';
        return;
    }

    string ans_str = "";
    for(int _ = best_len; _ < n; _++) {
        ans_str.push_back('H');
    }
    for(int l = 0; l < best_len; l++) {
        if(get_weight(best_len - m, best_len - 1) >
           k * get_weight(l + 1, best_len - m - 1)) {
            break;
        }

        ans_str.push_back('T');
    }

    cout << ans_str.size() << " " << ans << '\n';
    cout << ans_str << '\n';
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
