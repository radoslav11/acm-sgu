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

int N, Q;
vector<int> nxt, prv;

void read() { cin >> N >> Q; }

void solve() {
    nxt.resize(N + 1);
    prv.resize(N + 1);

    for(int i = 1; i <= N; i++) {
        nxt[i] = (i == N) ? 1 : i + 1;
        prv[i] = (i == 1) ? N : i - 1;
    }

    int current = 1;
    bool clockwise = true;
    int last_erased = -1;
    int remaining = N;

    while(remaining > 0) {
        int pos = current;
        for(int i = 1; i < Q; i++) {
            if(clockwise) {
                pos = nxt[pos];
            } else {
                pos = prv[pos];
            }
        }

        last_erased = pos;

        int next_pos = nxt[pos];
        int prev_pos = prv[pos];
        nxt[prev_pos] = next_pos;
        prv[next_pos] = prev_pos;

        remaining--;

        if(remaining == 0) {
            break;
        }

        current = next_pos;

        if(current % 2 == 1) {
            clockwise = true;
        } else {
            clockwise = false;
        }
    }

    cout << last_erased << endl;
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
