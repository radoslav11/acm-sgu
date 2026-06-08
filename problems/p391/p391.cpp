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
}

template<typename T>
ostream& operator<<(ostream& out, const vector<T>& a) {
    for(auto x: a) {
        out << x << ' ';
    }
    return out;
}

int n, m, k;
vector<pair<int, int>> pts;

void read() {
    cin >> n >> m >> k;
    pts.resize(k);
    cin >> pts;
}

bool foldable_1d(vector<int>& v, int lim) {
    int s = (int)v.size();
    if(s < 2) {
        return true;
    }

    for(int i = 1; i < s; i++) {
        if(((v[i] - v[i - 1]) & 1) == 0) {
            return false;
        }
    }

    int low = 0, high = s;
    int left_border = 1, right_border = lim;
    while(low < high && v[low] == left_border) {
        low++;
        left_border++;
    }

    while(low < high && v[high - 1] == right_border) {
        high--;
        right_border--;
    }

    vector<int> seq;
    for(int i = low; i < high;) {
        int j = i;
        while(j + 1 < high && v[j + 1] == v[j] + 1) {
            j++;
        }

        int run_len = j - i + 1;
        if(run_len > 1 && (run_len & 1)) {
            return false;
        }

        seq.push_back(run_len);
        if(j + 1 < high) {
            seq.push_back(v[j + 1] - v[j]);
        }

        i = j + 1;
    }

    int prev = -1;
    for(int k = 0; k < (int)seq.size(); k += 2) {
        if(seq[k] != 1) {
            continue;
        }

        if(prev >= 0) {
            for(int l = prev + 1, r = k - 1; l < r; l++, r--) {
                if(seq[l] != seq[r]) {
                    return false;
                }
            }
        }

        prev = k;
    }

    return true;
}

bool is_full_grid(vector<pair<int, int>>& cells, vector<int>& cols) {
    int n_cells = (int)cells.size();
    int n_cols = (int)cols.size();
    if(n_cells == 0) {
        return true;
    }
    if(n_cells % n_cols != 0) {
        return false;
    }
    int n_rows = n_cells / n_cols;
    for(int row_idx = 0; row_idx < n_rows; row_idx++) {
        int expected_row = cells[row_idx * n_cols].first;
        for(int col_idx = 0; col_idx < n_cols; col_idx++) {
            const auto& cell = cells[row_idx * n_cols + col_idx];
            if(cell.first != expected_row || cell.second != cols[col_idx]) {
                return false;
            }
        }
    }
    return true;
}

void solve() {
    // The core idea is that folds act independently on rows and columns.
    // Each fold is a reflection across a line that sits exactly between
    // two adjacent cells (a half-integer position). Also, any two positions
    // that can ever be stacked on top of each other must have an odd difference
    // (their sum is always odd).
    //
    // Therefore the set of distinct row indices (and separately the distinct
    // column indices) must satisfy several intuitive folding rules:
    //
    //     - All gaps between consecutive marked positions are odd
    //       (reflection invariant).
    //
    //     - Runs that touch the border (position 1 or n) can be peeled
    //       away one cell at a time.
    //
    //     - Any internal run of length > 1 must be even. Only then is the
    //       fold-line at the centre of the run between two cells, and folding
    //       there pairs marks with marks and the unmarked neighbours of the
    //       run with each other. The result is a half-length run flush against
    //       the new border, which the previous rule then peels.
    //       An odd-length internal run has no central fold line and
    //       inevitably drags an unmarked cell onto a marked one.
    //
    //     - For each pair of consecutive isolated single marks, the
    //       run-and-gap pattern strictly between them must be a palindrome.
    //       The only fold that merges two isolated marks at positions p
    //       and q is along the line equidistant from both (valid because
    //       the gap is odd), and that fold reflects the cells between
    //       them pairwise from the outside in. For the reflection to map
    //       mark onto mark and gap onto gap, the pattern in between has
    //       to mirror around the midpoint.
    //
    // Some intuition for the exact construction on how to do the folding is
    // that we can look at the left side and try to fold it by going to the
    // right. We can fold both a marked and unmarked groups that touch the
    // border into size one: say we start with unmarked and convert it to a one
    // size unmarked, followed by some K marked cells, and then B marked. We
    // will fold the K marked in the middle so now the resulting sequence starts
    // with K/2 marked, followed by B >= 1 unmarked. We can continue folding by
    // 1, getting to a 1 marked cell as the start, followed by B unmarked. Then
    // to get rid of the 1 marked, we fold the B unmarked in two. We can notice
    // that this logic breaks only when K = 1, as then we can't fold the marked
    // cells. We can collapse everything before the first one-size group to the
    // left and to the right of the last one. In the middle, the only thing we
    // could do is potentially fold in half, which requires us having a
    // palindrome.
    //
    // Once we know the row set and column set are both foldable,
    // we only need to check that the given marked cells form exactly
    // the Cartesian product of these two sets (a complete rectangle
    // with no holes and no extra cells).

    vector<int> rows(k), cols(k);
    for(int i = 0; i < k; i++) {
        rows[i] = pts[i].first;
        cols[i] = pts[i].second;
    }

    sort(rows.begin(), rows.end());
    rows.erase(unique(rows.begin(), rows.end()), rows.end());
    sort(cols.begin(), cols.end());
    cols.erase(unique(cols.begin(), cols.end()), cols.end());

    if(!foldable_1d(rows, n) || !foldable_1d(cols, m)) {
        cout << "NO\n";
        return;
    }

    sort(pts.begin(), pts.end());
    if(!is_full_grid(pts, cols)) {
        cout << "NO\n";
        return;
    }

    cout << "YES\n";
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
