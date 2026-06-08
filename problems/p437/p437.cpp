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

int k;
int64_t n;
array<int, 32> given;

int fwd[32][24], fwd_count[32];
int forbid_count[32][33];
unsigned avail[32];
int avail_count[32];
unsigned full_mask;
int64_t solutions;
int cur[32];
bool found;

void read() {
    cin >> k >> n;
    for(int i = 1; i <= 31; i++) {
        cin >> given[i];
    }
}

void search(int i) {
    if(i == 32) {
        if(++solutions == n) {
            found = true;
        }
        return;
    }
    if(given[i]) {
        search(i + 1);
        return;
    }

    unsigned cand = avail[i];
    const int* fw = fwd[i];
    int fn = fwd_count[i];
    while(cand && !found) {
        unsigned bit = cand & (~cand + 1);
        cand ^= bit;
        int v = __builtin_ctz(bit);

        bool dead = false;
        for(int t = 0; t < fn; t++) {
            int j = fw[t];
            if(forbid_count[j][v]++ == 0) {
                avail[j] &= ~bit;
                if(--avail_count[j] == 0) {
                    dead = true;
                }
            }
        }

        if(!dead) {
            cur[i] = v;
            search(i + 1);
        }

        for(int t = 0; t < fn; t++) {
            int j = fw[t];
            if(--forbid_count[j][v] == 0) {
                avail[j] |= bit;
                ++avail_count[j];
            }
        }
    }
}

void solve() {
    // All-different inside a group means every pair of its cells must differ,
    // so the board is a graph coloring instance: the numbers are colors and two
    // cells are adjacent when they share any of the 28 all-different groups -
    //
    // - the 7 rows running left to right (equal row_id),
    // - the 7 rows along the "/" direction (equal left_id),
    // - the 7 rows along the "\" direction (equal right_id),
    // - the 7 marked clusters, each a marked cell with its up to six
    //   neighbours, which is a clique of 7 and the reason 7 numbers are always
    //   needed.
    //
    // The N-th solution in lexicographic order comes from a depth-first search
    // that fills cells in index order 1..31 and at each cell tries the numbers
    // 1..K from small to large; this visits the complete colorings in exactly
    // lexicographic order, so we count leaves until the N-th. To keep each step
    // cheap we materialize every free cell's candidate set in avail[c] rather
    // than rebuilding it, backed by forbid_count[c][x] = how many filled
    // neighbours forbid number x on c (so x returns to avail[c] only once its
    // last blocker is undone). Placing a number at a cell removes it from every
    // later free neighbour's avail, and the branch is abandoned the instant one
    // of those neighbours is left with an empty avail (forward checking). The
    // per-node cost is proportional to a cell's not-yet-filled neighbours,
    // which shrinks with depth, so the search stays fast all the way down.
    //
    // Pre-filled cells are baked into the board before the walk starts (their
    // number is marked forbidden on every neighbour) and then skipped during
    // the walk; otherwise a given number on a late cell would only be
    // discovered at that depth, after the search had already wandered through
    // every free cell before it.

    static const int row_id[32] = {0, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3,
                                   3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5,
                                   5, 5, 5, 6, 6, 6, 6, 6, 7, 7};
    static const int left_id[32] = {0, 2, 3, 1, 2, 3, 4, 5, 1, 2, 3,
                                    4, 5, 6, 2, 3, 4, 5, 6, 2, 3, 4,
                                    5, 6, 7, 3, 4, 5, 6, 7, 5, 6};
    static const int right_id[32] = {0, 3, 2, 5, 4, 3, 2, 1, 6, 5, 4,
                                     3, 2, 1, 6, 5, 4, 3, 2, 7, 6, 5,
                                     4, 3, 2, 7, 6, 5, 4, 3, 6, 5};
    static const int clusters[7][7] = {
        {3, 4, 8, 9, 10, 14, 15},     {1, 2, 4, 5, 6, 10, 11},
        {6, 7, 11, 12, 13, 17, 18},   {10, 11, 15, 16, 17, 21, 22},
        {14, 15, 19, 20, 21, 25, 26}, {21, 22, 26, 27, 28, 30, 31},
        {17, 18, 22, 23, 24, 28, 29},
    };

    vector<vector<int>> members(28);
    for(int i = 1; i <= 31; i++) {
        members[row_id[i] - 1].push_back(i);
        members[7 + left_id[i] - 1].push_back(i);
        members[14 + right_id[i] - 1].push_back(i);
    }
    for(int c = 0; c < 7; c++) {
        for(int j = 0; j < 7; j++) {
            members[21 + c].push_back(clusters[c][j]);
        }
    }

    bool adj[32][32] = {};
    for(const auto& g: members) {
        for(int a: g) {
            for(int b: g) {
                adj[a][b] = (a != b);
            }
        }
    }

    for(int i = 1; i <= 31; i++) {
        fwd_count[i] = 0;
        for(int j = i + 1; j <= 31; j++) {
            if(adj[i][j] && !given[j]) {
                fwd[i][fwd_count[i]++] = j;
            }
        }
    }

    full_mask = ((1u << k) - 1) << 1;
    for(int i = 1; i <= 31; i++) {
        cur[i] = given[i];
        avail[i] = full_mask;
        avail_count[i] = k;
        for(int v = 0; v <= k; v++) {
            forbid_count[i][v] = 0;
        }
    }

    for(int i = 1; i <= 31; i++) {
        if(given[i]) {
            int v = given[i];
            for(int j = 1; j <= 31; j++) {
                if(adj[i][j] && forbid_count[j][v]++ == 0) {
                    avail[j] &= ~(1u << v);
                    avail_count[j]--;
                }
            }
        }
    }

    search(1);

    if(!found) {
        cout << "No way\n";
        return;
    }

    cout << "Found\n";
    for(int i = 1; i <= 31; i++) {
        cout << cur[i] << " \n"[i == 31];
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
