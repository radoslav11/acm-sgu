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

int n, m;
vector<int> arrival, service;

void read() {
    cin >> n >> m;
    arrival.resize(n);
    service.resize(n);
    for(int i = 0; i < n; i++) {
        cin >> arrival[i] >> service[i];
    }
}

void solve() {
    // We can directly implement this problem by simulation the queue process -
    // we just need to keep when everyone entered and then go in increasing
    // order of time. We don't actually need this, but we could use a priority
    // queue of "events" to make this more efficient.

    vector<vector<int>> finish_times(m);

    for(int i = 0; i < n; i++) {
        int k = arrival[i];
        int t = service[i];

        vector<int> queue_count(m, 0);
        for(int w = 0; w < m; w++) {
            for(int finish: finish_times[w]) {
                if(finish > k) {
                    queue_count[w]++;
                }
            }
        }

        int best_window = 0;
        for(int w = 1; w < m; w++) {
            if(queue_count[w] < queue_count[best_window]) {
                best_window = w;
            }
        }

        int start_time = k;
        if(!finish_times[best_window].empty()) {
            start_time = max(start_time, finish_times[best_window].back());
        }
        int end_time = start_time + t;

        finish_times[best_window].push_back(end_time);

        cout << (best_window + 1) << ' ' << end_time << '\n';
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
