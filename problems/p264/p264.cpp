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
vector<string> man_name, woman_name;
map<string, int> man_id, woman_id;
vector<vector<int>> man_pref, woman_pref;

vector<vector<string>> man_pref_str, woman_pref_str;

void read() {
    cin >> n;
    man_name.resize(n);
    woman_name.resize(n);
    man_pref_str.resize(n, vector<string>(n));
    woman_pref_str.resize(n, vector<string>(n));

    for(int i = 0; i < n; i++) {
        cin >> man_name[i];
        man_id[man_name[i]] = i;
        for(int j = 0; j < n; j++) {
            cin >> man_pref_str[i][j];
        }
    }
    for(int i = 0; i < n; i++) {
        cin >> woman_name[i];
        woman_id[woman_name[i]] = i;
        for(int j = 0; j < n; j++) {
            cin >> woman_pref_str[i][j];
        }
    }
}

void solve() {
    // We should first start with some observations like the graph being
    // complete. This means that any permutation of length N corresponds to a
    // valid marriage, and the "hard" part of the problem is to find one that
    // is stable. Here we will present a very simple quadratic approach that
    // always guarantees this can be done.
    //
    // Let's incrementally add preferences, and try matching everyone who still
    // hasn't had a match. More precisely, in the first round we will match all
    // men to their top preference. This might give a case where more than one
    // man is matched to a women, so let's only keep the top match. Then we will
    // proceed to the next round that will only consider the unmatched men. It's
    // easy to implement this in O(N^2), and we can show that it will always
    // find a stable matching after N rounds. It's enough to show this via the
    // following two:
    //
    //     1) After N rounds, everyone will be matched. This is direct from the
    //        fact that at least one unmatched man/woman gets matched in a
    //        round.
    //
    //     2) We always keep the invariant that the subset of people already
    //        matched is stable. Particularly, we incrementally add preferences
    //        so on round K, all matches so far have a preference on position
    //        less than K in their preference list. This means that none of the
    //        pairs including the new people being matched at round K will break
    //        the stability and so the invariant is kept.
    //
    // A nice way of implementing this is to somewhat break the concept of
    // rounds and instead always keep a queue of unmatched people. Then each
    // operation will either match to someone unmatched, or replace the existing
    // match (which can only happen within the same round). As already
    // mentioned, this quadratic algorithm always finds a valid stable marriage.
    // It turns out that this is a popular algorithm in literature. It was
    // introduced in 1962 by Gale and Shapley, but later on in 1984 Alvin E.
    // Roth pointed out this approach had been used since the 1950 for matching
    // medical students with residency. Shapley and Roth then jointly won the
    // Nobel prize in Economics in 2012.
    //
    // https://en.wikipedia.org/wiki/Gale%E2%80%93Shapley_algorithm

    man_pref.resize(n, vector<int>(n));
    woman_pref.resize(n, vector<int>(n));

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            man_pref[i][j] = woman_id[man_pref_str[i][j]];
        }
    }
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            woman_pref[i][j] = man_id[woman_pref_str[i][j]];
        }
    }

    vector<vector<int>> woman_rank(n, vector<int>(n));
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            woman_rank[i][woman_pref[i][j]] = j;
        }
    }

    vector<int> man_match(n, -1), woman_match(n, -1);
    vector<int> man_next(n, 0);

    queue<int> free_men;
    for(int i = 0; i < n; i++) {
        free_men.push(i);
    }

    while(!free_men.empty()) {
        int m = free_men.front();
        free_men.pop();

        int w = man_pref[m][man_next[m]++];
        if(woman_match[w] == -1) {
            man_match[m] = w;
            woman_match[w] = m;
        } else {
            int curr = woman_match[w];
            if(woman_rank[w][m] < woman_rank[w][curr]) {
                man_match[m] = w;
                woman_match[w] = m;
                man_match[curr] = -1;
                free_men.push(curr);
            } else {
                free_men.push(m);
            }
        }
    }

    cout << "YES\n";
    for(int i = 0; i < n; i++) {
        cout << man_name[i] << " " << woman_name[man_match[i]] << "\n";
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
