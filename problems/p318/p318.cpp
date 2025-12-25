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

int n, m;
vector<vector<int>> resource_users;

void read() {
    cin >> n >> m;
    resource_users.resize(n + 1);

    for(int u = 1; u <= m; u++) {
        int k;
        cin >> k;
        for(int j = 0; j < k; j++) {
            int r;
            cin >> r;
            resource_users[r].push_back(u);
        }
    }
}

void solve() {
    // The solution is fairly simple - we can just check for every file the set
    // of people that should have access to it. We don't want any other person
    // to have access to the same file, so this corresponds to a group. The only
    // way to share groups is if there is another file with the same set of
    // people. We can simply sort the ids of the people that should have access
    // to every file, and then sort all of these - the number of groups will be
    // the number of unique vectors.

    set<vector<int>> unique_groups;
    for(int r = 1; r <= n; r++) {
        if(!resource_users[r].empty()) {
            sort(resource_users[r].begin(), resource_users[r].end());
            unique_groups.insert(resource_users[r]);
        }
    }

    cout << unique_groups.size() << "\n";
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
