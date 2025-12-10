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

void read() { cin >> n; }

void solve() {
    // For n <= 3 we can already see the solution in the problem statement. For
    // N >= 4, let's bound the largest number of edges we can have. Consider an
    // arbitrary graph G, and find a DFS spanning tree T of it. We are
    // particularly interested in DFS trees, because the additional edges are
    // only "back" (from some u to it's ancestor). The number of edges in T is
    // clearly n-1, as it's a tree, so let's now make an argument for the number
    // of back edges. The main observation is that if some back edge (u, v)
    // "covers" another back edge (x, y), we can select the two cycles given by
    // path(x, y) + (x, y) and path(u, x) + (x, y) + path(y, v) + (u, v), where
    // path(a, b) is the tree edges on the path from a to b. This means that if
    // we get the depths of the endpoints of the back edges, they should be
    // increasing. The lowest depth of the deeper endpoint of a back edge is 2,
    // while the largest depth is n - 1. This means there are at most n - 3 back
    // edges we can have, and so we get a total bound on the number of edges as
    // 2n-2.
    //
    // This bound is achievable: connect all vertices {1,...,n-2} to both
    // {n-1,n}. This gives 2(n-2) edges. Every cycle has length 4 and passes
    // through both n-1 and n, so any two cycles share 0 or 2 edges, never
    // exactly 1.

    if(n == 1) {
        cout << "1 0\n";
    } else if(n == 2) {
        cout << "2 1\n";
        cout << "1 2\n";
    } else if(n == 3) {
        cout << "3 3\n";
        cout << "1 2\n";
        cout << "2 3\n";
        cout << "1 3\n";
    } else {
        int edges = 2 * (n - 2);
        cout << n << " " << edges << "\n";
        for(int i = 1; i <= n - 2; i++) {
            cout << i << " " << (n - 1) << "\n";
            cout << i << " " << n << "\n";
        }
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
