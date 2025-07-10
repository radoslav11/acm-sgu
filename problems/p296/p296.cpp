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

int k;
string n;

void read() { cin >> n >> k; }

vector<int> counting_sort(vector<int> a) {
    int n = a.size();
    int mx = *max_element(a.begin(), a.end());

    vector<int> cnt(mx + 1, 0);
    for(int i = 0; i < n; i++) {
        cnt[a[i]]++;
    }

    vector<int> res;
    for(int i = 0; i <= mx; i++) {
        for(int j = 0; j < cnt[i]; j++) {
            res.push_back(i);
        }
    }

    return res;
}

void solve() {
    string ans;
    vector<int> valid_states;

    char best = 0;
    for(int i = 0; i <= k; i++) {
        best = max(best, n[i]);
    }

    for(int i = 0; i <= k; i++) {
        if(n[i] == best) {
            valid_states.push_back(i);
        }
    }

    ans.push_back(best);

    int len = n.size() - k;
    for(int l = 1; l < len; l++) {
        vector<bool> can_visit(n.size(), false);
        for(int i: valid_states) {
            int ck = i - l - 1;
            for(int j = 0; j <= k - ck; j++) {
                int nxt = i + j + 1;
                if(nxt >= (int)n.size() || can_visit[nxt]) {
                    break;
                }
                can_visit[nxt] = true;
            }
        }

        char best = 0;
        for(int i = 0; i < (int)n.size(); i++) {
            if(can_visit[i]) {
                best = max(best, n[i]);
            }
        }

        vector<int> new_valid_states;
        for(int i = 0; i < (int)n.size(); i++) {
            if(n[i] == best) {
                new_valid_states.push_back(i);
            }
        }

        ans.push_back(best);
        valid_states = new_valid_states;
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
