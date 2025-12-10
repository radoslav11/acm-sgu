#include <bits/stdc++.h>
// #include <coding_library/graph/dsu.hpp>

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

const int MAXC = (int)1e4 + 42;

class DSU {
  public:
    int n;
    vector<int> par;
    vector<int> sz;

    DSU(int _n = 0) { init(_n); }

    void init(int _n) {
        n = _n;
        par.assign(n + 1, 0);
        sz.assign(n + 1, 0);
        for(int i = 0; i <= n; i++) {
            par[i] = i;
            sz[i] = 1;
        }
    }

    int root(int u) { return par[u] = ((u == par[u]) ? u : root(par[u])); }
    bool connected(int x, int y) { return root(x) == root(y); }

    int unite(int x, int y) {
        x = root(x), y = root(y);
        if(x == y) {
            return x;
        }
        if(sz[x] > sz[y]) {
            swap(x, y);
        }
        par[x] = y;
        sz[y] += sz[x];
        return y;
    }

    vector<vector<int>> components() {
        vector<vector<int>> comp(n + 1);
        for(int i = 0; i <= n; i++) {
            comp[root(i)].push_back(i);
        }
        return comp;
    }
};

int n;
vector<pair<int, int>> pnts;

void read() {
    cin >> n;
    pnts.resize(n);
    cin >> pnts;
}

bool segments_intersect(
    pair<int, int> a1, pair<int, int> a2, pair<int, int> b1, pair<int, int> b2
) {
    auto overlap = [](int a1, int a2, int b1, int b2) {
        return max(min(a1, a2), min(b1, b2)) < min(max(a1, a2), max(b1, b2));
    };

    if(a1.first == a2.first && b1.first == b2.first) {
        return a1.first == b1.first &&
               overlap(a1.second, a2.second, b1.second, b2.second);
    }

    if(a1.second == a2.second && b1.second == b2.second) {
        return a1.second == b1.second &&
               overlap(a1.first, a2.first, b1.first, b2.first);
    }

    if(a1.second == a2.second && b1.first == b2.first) {
        swap(a1, b1);
        swap(a2, b2);
    }

    if(a1.first == a2.first && b1.second == b2.second) {
        return min(a1.second, a2.second) < b1.second &&
               b1.second < max(a1.second, a2.second) &&
               min(b1.first, b2.first) < a1.first &&
               a1.first < max(b1.first, b2.first);
    }

    return false;
}

void solve() {
    // We will start this problem by making a few observations:
    //
    //     1) After each side, we change the angle by 90º. This
    //        together with the fact that the line should be closed
    //        implies that N should be even.
    //
    //     2) Using a similar argument we can also see that the number
    //        of points on the same X or Y should be even. In particular, say we
    //        make a horizontal move (equal X), meaning that we "block" two
    //        (even) points.
    //
    //     3) Let all points having some fixed X be Y[1],...,Y[2k]. Then we can
    //        only have edges Y[1]-Y[2], Y[3]-Y[4], ..., Y[2k-1]-Y[2k].
    //        Otherwise, we will have an intersection or touching. A similar
    //        argument can be applied for the same Y coordinate.
    //
    // The above 3 observations imply that we have exactly one polygon that can
    // be formed. We only want to make sure it's actually a valid "snake" line,
    // or that it's connected (trivial) and has no intersections (slightly more
    // complicated if we want to be efficient as we need to do a sweepline).
    // However, n isn't large, so we can just naively check for intersections.

    if(n % 2 != 0) {
        cout << 0 << endl;
        return;
    }

    map<int, vector<pair<int, int>>> pnts_on_x, pnts_on_y;
    for(int i = 0; i < n; i++) {
        pnts_on_x[pnts[i].first].push_back({pnts[i].second, i});
        pnts_on_y[pnts[i].second].push_back({pnts[i].first, i});
    }

    DSU dsu(n);
    long long total_length = 0;
    vector<pair<pair<int, int>, pair<int, int>>> edges;

    for(auto& it: pnts_on_x) {
        int x = it.first;
        auto& vec = it.second;
        sort(vec.begin(), vec.end());

        if(vec.size() % 2 != 0) {
            cout << 0 << endl;
            return;
        }

        for(int i = 0; i < (int)vec.size(); i += 2) {
            if(vec[i].first == vec[i + 1].first) {
                cout << 0 << endl;
                return;
            }

            int y1 = vec[i].first;
            int y2 = vec[i + 1].first;

            edges.push_back({{x, y1}, {x, y2}});
            total_length += abs(y2 - y1);
            dsu.unite(vec[i].second, vec[i + 1].second);
        }
    }

    for(auto& it: pnts_on_y) {
        int y = it.first;
        auto& vec = it.second;
        sort(vec.begin(), vec.end());

        if(vec.size() % 2 != 0) {
            cout << 0 << endl;
            return;
        }

        for(int i = 0; i < (int)vec.size(); i += 2) {
            if(vec[i].first == vec[i + 1].first) {
                cout << 0 << endl;
                return;
            }

            int x1 = vec[i].first;
            int x2 = vec[i + 1].first;

            edges.push_back({{x1, y}, {x2, y}});
            total_length += abs(x2 - x1);
            dsu.unite(vec[i].second, vec[i + 1].second);
        }
    }

    int root0 = dsu.root(0);
    for(int i = 1; i < n; i++) {
        if(dsu.root(i) != root0) {
            cout << 0 << endl;
            return;
        }
    }

    for(int i = 0; i < (int)edges.size(); i++) {
        for(int j = i + 1; j < (int)edges.size(); j++) {
            if(segments_intersect(
                   edges[i].first, edges[i].second, edges[j].first,
                   edges[j].second
               )) {
                cout << 0 << endl;
                return;
            }
        }
    }

    cout << total_length << endl;
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
