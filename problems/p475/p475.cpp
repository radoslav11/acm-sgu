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

const int64_t inf = (int64_t)1e18 + 42;

int n, m;
vector<int> w, t, s;
vector<int> c, D, d;

void read() {
    cin >> n >> m;
    w.resize(n);
    t.resize(n);
    s.resize(n);
    for(int j = 0; j < n; j++) {
        cin >> w[j] >> t[j] >> s[j];
    }
    c.resize(m);
    D.resize(m);
    d.resize(m);
    for(int i = 0; i < m; i++) {
        cin >> c[i] >> D[i] >> d[i];
    }
}

void solve() {
    // We first notice that n <= 10, meaning that something like 2^n is feasible
    // as one of the terms in the complexity. That matches the unique sets that
    // can be in the boat at any time, meaning we can try to encode the full
    // state. Particularly, the set is defined by the people currently in the
    // boat, and the current raft we are at. At every moment we can either
    // remove or add some people to the mask, or proceed to the next raft. This
    // is O(n) transitions which is sufficiently small, but the issue is that we
    // can't do DP as there are cycles (due to the mask transitions). However,
    // we can think of this as a graph problem instead, with states being the
    // same. Then running Dijkstra would be sufficiently fast for the given
    // constraints. There are just some details in terms of the transitions.
    // One small optimization is to do M independent runs of Dijkstra over the
    // 2^n dimension, as that's the only place where we could have cycles.

    int N = 1 << n;
    vector<int> sum_w(N), max_out(N);
    for(int mask = 0; mask < N; mask++) {
        int sw = 0, mo = 0;
        for(int j = 0; j < n; j++) {
            if(mask & (1 << j)) {
                sw += w[j];
            } else {
                mo = max(mo, t[j]);
            }
        }
        sum_w[mask] = sw;
        max_out[mask] = mo;
    }

    vector<int64_t> prev_dp(N, inf);
    prev_dp[0] = 0;

    auto run_dijkstra = [&](vector<int64_t>& dist) {
        priority_queue<
            pair<int64_t, int>, vector<pair<int64_t, int>>, greater<>>
            pq;
        for(int mask = 0; mask < N; mask++) {
            if(dist[mask] < inf) {
                pq.push({dist[mask], mask});
            }
        }
        while(!pq.empty()) {
            int64_t time = pq.top().first;
            int umask = pq.top().second;
            pq.pop();
            if(time > dist[umask]) {
                continue;
            }
            for(int j = 0; j < n; j++) {
                int vmask = umask ^ (1 << j);
                int64_t new_time = time + s[j];
                if(new_time < dist[vmask]) {
                    dist[vmask] = new_time;
                    pq.push({new_time, vmask});
                }
            }
        }
    };

    for(int riff = 0; riff < m; riff++) {
        vector<int64_t> dist(N, inf);
        for(int mask = 0; mask < N; mask++) {
            dist[mask] = prev_dp[mask];
        }
        run_dijkstra(dist);

        vector<int64_t> next_dp(N, inf);
        for(int mask = 1; mask < N; mask++) {
            if(dist[mask] < inf) {
                bool capsize = sum_w[mask] > c[riff];
                int raft_t = capsize ? D[riff] : d[riff];
                int seg_t = max(raft_t, max_out[mask]);
                next_dp[mask] = dist[mask] + seg_t;
            }
        }
        prev_dp = next_dp;
    }

    vector<int64_t> dist(N, inf);
    for(int mask = 0; mask < N; mask++) {
        dist[mask] = prev_dp[mask];
    }
    run_dijkstra(dist);

    cout << dist[0] << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T = 1;
    // cin >> T;
    for(int test = 1; test <= T; test++) {
        read();
        solve();
    }

    return 0;
}
