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

vector<int> button(13);
int X, Y;

void read() {
    cin >> button[1] >> button[2] >> button[3] >> button[10];
    cin >> button[4] >> button[5] >> button[6] >> button[11];
    cin >> button[7] >> button[8] >> button[9];
    cin >> button[12] >> button[0];
    cin >> X >> Y;
}

void solve() {
    // We can simply think of this as a graph, and then do a Dijkstra. This is
    // a small graph with 100 nodes and we can simply do 100*100 edges. We could
    // technically do a BFS as the weights are <= 3 (0-1 BFS style), but the
    // constraints are low so that anything would do, including Dijkstra.
    vector<int> dist(100, INT_MAX);
    priority_queue<
        pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>>
        pq;

    dist[X] = 0;
    pq.push({0, X});

    while(!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if(d > dist[u]) {
            continue;
        }

        if(button[10]) {
            int v = (u + 1) % 100;
            if(dist[v] > dist[u] + 1) {
                dist[v] = dist[u] + 1;
                pq.push({dist[v], v});
            }
        }

        if(button[11]) {
            int v = (u - 1 + 100) % 100;
            if(dist[v] > dist[u] + 1) {
                dist[v] = dist[u] + 1;
                pq.push({dist[v], v});
            }
        }

        for(int d = 0; d <= 9; d++) {
            if(button[d]) {
                int v = d;
                if(dist[v] > dist[u] + 1) {
                    dist[v] = dist[u] + 1;
                    pq.push({dist[v], v});
                }
            }
        }

        if(button[12]) {
            for(int d1 = 1; d1 <= 9; d1++) {
                if(!button[d1]) {
                    continue;
                }
                for(int d2 = 0; d2 <= 9; d2++) {
                    if(!button[d2]) {
                        continue;
                    }
                    int v = d1 * 10 + d2;
                    if(dist[v] > dist[u] + 3) {
                        dist[v] = dist[u] + 3;
                        pq.push({dist[v], v});
                    }
                }
            }
        }
    }

    if(dist[Y] == INT_MAX) {
        cout << -1 << '\n';
    } else {
        cout << dist[Y] << '\n';
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
