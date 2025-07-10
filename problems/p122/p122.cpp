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
vector<vector<int>> adj;

void read() {
    cin >> n;
    adj.assign(n, {});
    cin.ignore();
    for(int i = 0; i < n; i++) {
        string line;
        getline(cin, line);

        istringstream ss(line);
        int neighbor;
        while(ss >> neighbor) {
            adj[i].push_back(neighbor - 1);
        }
    }
}

void solve() {
    vector<int> pos(n, -1);
    vector<int> cycle = {0};
    pos[0] = 0;

    for(int i = 1; i < n; i++) {
        bool found = false;
        int u = cycle.back();
        for(int v: adj[u]) {
            if(pos[v] == -1) {
                pos[v] = i;
                cycle.push_back(v);
                found = true;
                break;
            }
        }

        if(found) {
            continue;
        }

        vector<bool> marked(n, false);
        for(int v: adj[u]) {
            marked[cycle[pos[v] + 1]] = true;
        }

        for(int new_v = 0; new_v < n; new_v++) {
            if(pos[new_v] == -1) {
                bool found = false;
                for(int v: adj[new_v]) {
                    if(marked[v]) {
                        found = true;
                        int j = pos[v];
                        reverse(cycle.begin() + pos[v], cycle.end());
                        break;
                    }
                }

                if(found) {
                    pos[new_v] = i;
                    cycle.push_back(new_v);
                    break;
                }
            }
        }
    }

    for(int i = 0; i < n; i++) {
        cout << cycle[i] + 1 << ' ';
    }
    cout << cycle[0] + 1;
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
