#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

struct Edge {
    int u, v, c, t;
};

int n, m;
vector<Edge> edges;
vector<double> dist;
vector<int> par;

pair<bool, vector<int>> check(double x) {
    dist.assign(n + 2, 1e18);
    par.assign(n + 2, -1);

    int source = n + 1;
    dist[source] = 0;

    for(int i = 0; i < n; i++) {
        for(auto& e: edges) {
            double weight = e.t * x - e.c;
            if(dist[e.u] < 1e17 && dist[e.u] + weight < dist[e.v]) {
                dist[e.v] = dist[e.u] + weight;
                par[e.v] = e.u;
            }
        }

        for(int v = 1; v <= n; v++) {
            if(dist[source] < dist[v]) {
                dist[v] = dist[source];
                par[v] = source;
            }
        }
    }

    int cycle_node = -1;
    for(auto& e: edges) {
        double weight = e.t * x - e.c;
        if(dist[e.u] < 1e17 && dist[e.u] + weight < dist[e.v]) {
            cycle_node = e.v;
            break;
        }
    }

    if(cycle_node == -1) {
        return {false, {}};
    }

    for(int i = 0; i < n; i++) {
        cycle_node = par[cycle_node];
    }

    vector<int> cycle;
    int curr = cycle_node;
    do {
        cycle.push_back(curr);
        curr = par[curr];
    } while(curr != cycle_node);

    reverse(cycle.begin(), cycle.end());
    return {true, cycle};
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n >> m;
    edges.resize(m);

    for(int i = 0; i < m; i++) {
        cin >> edges[i].u >> edges[i].v >> edges[i].c >> edges[i].t;
    }

    double l = 0.0, r = 200.0;

    for(int i = 0; i < 100; i++) {
        double mid = (l + r) / 2;
        auto [has_cycle, cycle] = check(mid);
        if(has_cycle) {
            l = mid;
        } else {
            r = mid;
        }
    }

    auto [has_cycle, cycle] = check(l);

    if(!has_cycle) {
        cout << "0\n";
    } else {
        cout << cycle.size() << "\n";
        for(int i = 0; i < cycle.size(); i++) {
            if(i > 0) {
                cout << " ";
            }
            cout << cycle[i];
        }
        cout << "\n";
    }

    return 0;
}
