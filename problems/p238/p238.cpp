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
    // The solution to this problem is surprisingly simple, while the
    // constraints might direct us in more complex or incorrect solutions. As a
    // start, we can think of the relations as a labeled tree. Let's add a
    // "fake" node with an (n+1)-st color representing the floor. We will root
    // the tree from that node, meaning that the operation represents choosing
    // some child u of the root, re-rooting the tree at u, and swapping the
    // color of u and the previous root. It's fairly simple to see that the
    // operations reversible, and each composition is strictly defined by the
    // current root (there are no to compositions that can have the same root).
    // Therefore, trivially the answer is always n+1. Apart from a small corner
    // case for n=1 which would make the solution fail otherwise. The complexity
    // is O(1).

    if(n == 1) {
        cout << 1 << endl;
    } else {
        cout << n + 1 << endl;
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
