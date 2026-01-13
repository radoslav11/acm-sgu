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
vector<int> p;

void read() {
    cin >> n;
    p.resize(n);
    cin >> p;
}

pair<bool, vector<pair<int, int>>> check_1(
    const vector<int>& p, const vector<int>& q
) {
    vector<bool> visited(p.size(), false);
    vector<pair<int, int>> swaps;

    for(int i = 0; i < (int)p.size(); i++) {
        if(visited[i] || p[i] == q[i]) {
            continue;
        }

        int found = -1;
        for(int j = 0; j < (int)p.size(); j++) {
            if(!visited[j] && p[i] == q[j] && p[j] == q[i]) {
                found = j;
                break;
            }
        }

        if(found == -1) {
            return {false, {}};
        }

        visited[i] = true;
        visited[found] = true;
        swaps.push_back({i, found});
    }

    return {true, swaps};
}

void solve() {
    // To solve this, the most important observation is that we need at most 2
    // swaps. Let's first check if the answer is 0 or 1. Checking for 0 is
    // trivial by sorting and check if equal. For 1, we can greedily assign
    // swaps - let p be the initial array, and q the target, so we can find
    // pairs (i, j) such that p[i] = q[j] and p[j] = q[i] such that every
    // position is in at most one pair.
    //
    // Now let's say we can't solve this in one turn. We start by turning p
    // into a permutation (any should work in the case of duplicates). We would
    // like in two turns to fix all cycles of this new permutation. Consider an
    // arbitrary one v[1], ..., v[k]. We want to cyclically shift it to
    // v[k], v[1], ..., v[k-1]. In the first step, we will try to make
    // independent cycles of length 2. This can be done by swapping v[1] with
    // v[k-1], then v[2] with v[k-2], and so on. This results in v[k-1] (placed
    // at index 1) and v[k] being together in a cycle, v[k-2] (placed at index
    // 2) with v[k-1] (placed at index k-1) also being together, and so on. This
    // means we can fix all these cycles in one more step. Here is an example
    // for a cycle of length 5:
    //
    //         v[1] v[2] v[3] v[4] v[5]        (1, 4) (2, 3)
    //         v[4] v[3] v[2] v[1] v[5]        (1, 5) (2, 4)
    //         v[5] v[1] v[2] v[3] v[4]

    vector<int> q = p;
    sort(q.begin(), q.end());

    if(p == q) {
        cout << 0 << endl;
        return;
    }

    auto [ok_1, swaps_1] = check_1(p, q);

    if(ok_1) {
        cout << 1 << endl;
        cout << swaps_1.size() << " ";
        for(auto [i, j]: swaps_1) {
            cout << i + 1 << " " << j + 1 << " ";
        }
        cout << endl;
        return;
    }

    vector<int> perm;
    map<int, int> cnt;
    for(int i = 0; i < n; i++) {
        int pi = lower_bound(q.begin(), q.end(), p[i]) - q.begin() + cnt[p[i]];
        cnt[p[i]]++;
        perm.push_back(pi);
    }

    vector<pair<int, int>> turns[2];
    vector<bool> visited(n, false);
    for(int i = 0; i < n; i++) {
        if(!visited[i]) {
            int u = i;
            vector<int> cycle;
            while(!visited[u]) {
                cycle.push_back(u);
                visited[u] = true;
                u = perm[u];
            }

            if(cycle.size() == 1) {
                continue;
            } else if(cycle.size() == 2) {
                turns[0].push_back({cycle[0], cycle[1]});
            } else {
                int l = 0, r = (int)cycle.size() - 2;
                while(l < r) {
                    turns[0].push_back({cycle[l], cycle[r]});
                    turns[1].push_back({cycle[l], cycle[r + 1]});
                    l++;
                    r--;
                }

                if(r == l) {
                    turns[1].push_back({cycle[l], cycle[r + 1]});
                }
            }
        }
    }

    cout << 2 << endl;
    for(int t = 0; t < 2; t++) {
        cout << turns[t].size() << " ";
        for(auto [i, j]: turns[t]) {
            cout << i + 1 << " " << j + 1 << " ";
        }
        cout << endl;
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
