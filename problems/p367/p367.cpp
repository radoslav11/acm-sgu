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
int64_t total_time;
vector<int> t;
vector<vector<int>> adj;
vector<int> indeg;

void read() {
    cin >> n >> total_time;
    t.assign(n + 1, 0);
    for(int i = 1; i <= n; i++) {
        cin >> t[i];
    }

    adj.assign(n + 1, {});
    indeg.assign(n + 1, 0);
    int m;
    cin >> m;
    for(int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        adj[a].push_back(b);
        indeg[b]++;
    }
}

void solve() {
    // The penalty is the sum of completion times, so for a fixed set of solved
    // problems the penalty is minimized by solving them in non-decreasing order
    // of solving time (shortest first weights the cheap problems into the most
    // prefixes). The set we actually solve must also be closed under the
    // prerequisite relation: to solve b we must first solve every ancestor a.
    //
    // The crucial guarantee is that every constraint a -> b has ta <= tb, which
    // means the prerequisite order never contradicts the "easy problems first"
    // order. Sorting by solving time therefore already respects all
    // constraints, except between equally hard problems where an explicit
    // a -> b with ta = tb forces a before b. So we run Kahn's topological sort
    // but instead of an arbitrary queue we keep a min-heap of the currently
    // available problems keyed by solving time, always emitting the cheapest
    // one. This yields a single order that is simultaneously a valid
    // topological order and globally non-decreasing in solving time.
    //
    // Because the order is non-decreasing in time, each prefix is the cheapest
    // prerequisite-closed set of its size, so we just walk the order
    // accumulating time and stop the moment the next (and hence every
    // remaining) problem no longer fits in the training duration. That prefix
    // maximizes the count and, being increasing in time, also minimizes the
    // penalty. Problems trapped in a cycle never reach in-degree zero, so they
    // are naturally never emitted, which matches the fact that a cycle of
    // mutual prerequisites is unsolvable.

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    for(int i = 1; i <= n; i++) {
        if(indeg[i] == 0) {
            pq.push({t[i], i});
        }
    }

    vector<int> order;
    int64_t elapsed = 0, penalty = 0;
    while(!pq.empty()) {
        auto [ti, u] = pq.top();
        pq.pop();
        if(elapsed + ti > total_time) {
            break;
        }

        elapsed += ti;
        penalty += elapsed;
        order.push_back(u);
        for(int v: adj[u]) {
            if(--indeg[v] == 0) {
                pq.push({t[v], v});
            }
        }
    }

    cout << order.size() << ' ' << penalty << '\n';
    cout << order << '\n';
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
