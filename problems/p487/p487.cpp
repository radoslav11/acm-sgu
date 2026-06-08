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
vector<int> road;

void read() {
    cin >> n;
    road.assign(4 * n + 1, 0);
    for(int i = 0; i < 2 * n; i++) {
        int a, b;
        cin >> a >> b;
        road[a] = b;
        road[b] = a;
    }
}

void solve() {
    // Gates are vertices. road[g] is the fixed partner of g across towns. We
    // additionally tie the 4 gates of every town into two internal pairs
    // (mate[g]), which means the courier enters one gate, walks inside to its
    // mate, and leaves through the road there. road + mate make every gate
    // degree 2, so the whole thing is a disjoint set of alternating cycles;
    // listing one such cycle in order is a valid courier route, so we want a
    // single cycle covering all gates.
    //
    // Start from the arbitrary pairing (g0,g1),(g2,g3) per town. To grow the
    // cycle that holds the capital: a town's two mate pairs each live entirely
    // in one cycle, so a town with exactly 2 gates in the capital's cycle has
    // its other pair in a different cycle. Re-pairing across the two pairs
    // splices those two cycles into one. If no such town exists while more than
    // one cycle remains, the towns split into groups with no road between them,
    // so the route is impossible.

    int g = 4 * n;
    vector<int> mate(g + 1);
    for(int town = 0; town < n; town++) {
        int base = 4 * town + 1;
        mate[base] = base + 1;
        mate[base + 1] = base;
        mate[base + 2] = base + 3;
        mate[base + 3] = base + 2;
    }

    vector<int> comp(g + 1);
    auto label = [&]() {
        fill(comp.begin(), comp.end(), -1);
        int c = 0;
        for(int s = 1; s <= g; s++) {
            if(comp[s] != -1) {
                continue;
            }

            int cur = s, prev = -1;
            do {
                comp[cur] = c;
                int nxt = (road[cur] != prev) ? road[cur] : mate[cur];
                prev = cur;
                cur = nxt;
            } while(cur != s);
            c++;
        }

        return c;
    };

    while(label() != 1) {
        int cap = comp[1];
        int base = -1;
        for(int town = 0; town < n && base == -1; town++) {
            int b = 4 * town + 1;
            int cnt = 0;
            for(int k = 0; k < 4; k++) {
                cnt += (comp[b + k] == cap);
            }
            if(cnt == 2) {
                base = b;
            }
        }

        if(base == -1) {
            cout << "No\n";
            return;
        }

        int in_gate[2], out_gate[2], ii = 0, oo = 0;
        for(int k = 0; k < 4; k++) {
            int gg = base + k;
            if(comp[gg] == cap) {
                in_gate[ii++] = gg;
            } else {
                out_gate[oo++] = gg;
            }
        }

        mate[in_gate[0]] = out_gate[0];
        mate[out_gate[0]] = in_gate[0];
        mate[in_gate[1]] = out_gate[1];
        mate[out_gate[1]] = in_gate[1];
    }

    vector<int> order;
    order.reserve(g);
    int cur = 1, prev = -1;
    do {
        order.push_back(cur);
        int nxt = (road[cur] != prev) ? road[cur] : mate[cur];
        prev = cur;
        cur = nxt;
    } while(cur != 1);

    cout << "Yes\n";
    cout << order << "\n";
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
