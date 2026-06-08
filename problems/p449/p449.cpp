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

int n, m, q;
vector<pair<int, vector<int>>> lines;
vector<int> queries;

void read() {
    cin >> n >> m >> q;
    lines.resize(m);
    for(auto& [y, members]: lines) {
        int len;
        cin >> y >> len;
        members.resize(len);
        cin >> members;
    }
    queries.resize(q);
    cin >> queries;
}

void solve() {
    // Each horizontal line is an internal node whose children are the
    // subgroups it joins, leaves 1..n are leaf nodes. A larger Y sits lower in
    // the picture (more similar), so it is a deeper merge; processing lines by
    // decreasing Y and unioning each line's members lets a DSU map every member
    // id to the current top node of its group, and a fresh internal node for
    // the line collects those tops as children. What is left when M < N - 1 is
    // a forest of such trees plus untouched leaves.
    //
    // A clean diagram means every subtree occupies a contiguous block, so the
    // only freedom is the left-to-right order of each node's children, chosen
    // for the lexicographically smallest leaf sequence. The lex-smallest
    // arrangement of a subtree always begins with that subtree's minimum leaf
    // (the child holding the min sorts first, by induction). Sibling subtrees
    // have disjoint leaves, so two sibling sequences already differ at their
    // first element: ordering children by their full sequence is identical to
    // ordering them by their subtree minimum. So we sort children (and the
    // forest roots) by subtree min and read the leaves off in a pre-order walk.
    //
    // Internal nodes get ids n+1.. in creation order, and every child has a
    // smaller id than its parent, so subtree mins fill in by a single sweep in
    // increasing id. The final walk and the DSU find are iterative to stay safe
    // on chains of depth up to n.

    sort(lines.begin(), lines.end(), [](const auto& a, const auto& b) {
        return a.first > b.first;
    });

    int total = n + m;
    vector<int> par(n + 1);
    vector<int> node_of(n + 1);
    for(int i = 1; i <= n; i++) {
        par[i] = i;
        node_of[i] = i;
    }

    auto find = [&](int x) {
        while(par[x] != x) {
            par[x] = par[par[x]];
            x = par[x];
        }
        return x;
    };

    vector<vector<int>> children(total + 1);
    int next_id = n;
    for(const auto& [y, members]: lines) {
        int cur = ++next_id;
        int base = find(members[0]);
        for(int v: members) {
            int r = find(v);
            children[cur].push_back(node_of[r]);
            par[r] = base;
        }

        base = find(base);
        node_of[base] = cur;
    }

    vector<int> subtree_min(total + 1);
    for(int i = 1; i <= n; i++) {
        subtree_min[i] = i;
    }
    for(int id = n + 1; id <= total; id++) {
        int best = INT_MAX;
        for(int c: children[id]) {
            best = min(best, subtree_min[c]);
        }

        subtree_min[id] = best;
    }

    auto by_min = [&](int a, int b) { return subtree_min[a] < subtree_min[b]; };
    for(int id = n + 1; id <= total; id++) {
        sort(children[id].begin(), children[id].end(), by_min);
    }

    vector<int> roots;
    for(int i = 1; i <= n; i++) {
        if(find(i) == i) {
            roots.push_back(node_of[i]);
        }
    }

    sort(roots.begin(), roots.end(), by_min);

    vector<int> order;
    order.reserve(n);
    vector<int> stack;
    for(auto it = roots.rbegin(); it != roots.rend(); ++it) {
        stack.push_back(*it);
    }

    while(!stack.empty()) {
        int u = stack.back();
        stack.pop_back();
        if(u <= n) {
            order.push_back(u);
            continue;
        }

        const auto& ch = children[u];
        for(auto it = ch.rbegin(); it != ch.rend(); ++it) {
            stack.push_back(*it);
        }
    }

    for(int pos: queries) {
        cout << order[pos - 1] << '\n';
    }
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
