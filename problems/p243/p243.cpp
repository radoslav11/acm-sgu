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
vector<string> grid;

void read() {
    cin >> n;
    grid.assign(20, "");
    cin >> grid;
}

void solve() {
    // We need to tile the N x N square (N <= 5) with the given pieces, where
    // each piece may be used in any of its 4 rotations. This is exact-cover
    // backtracking: always look at the first empty cell in row-major order, and
    // try to cover it with some still-unused piece in some orientation. Because
    // that cell is the row-major minimum among all empty cells, the piece's own
    // row-major minimum cell must land exactly on it, so each (piece,
    // orientation) gives a single placement to test, which keeps recursion
    // shallow and branchless per cell.
    //
    // The key pruning is collapsing isomorphic pieces. For every piece we
    // generate the (de-duplicated) set of its rotation shapes and take the
    // smallest of them as a canonical key. Pieces sharing a key are
    // interchangeable, so we store them as one group holding the shared list of
    // orientations plus a stack of the actual letters still available. The
    // branching factor at each step is then "number of distinct shapes still
    // present" times "orientations", not "number of pieces" - e.g. 25 unit
    // squares become a single group instead of a 25! explosion.
    //
    // Pieces are represented as sorted vectors of relative (row, col) cells,
    // normalized so the minimum row and column are zero; orient[0] is then the
    // anchor cell we align onto the target empty cell.
    //
    // For a loose complexity bound, let the distinct shapes have multiplicities
    // m_1..m_S and orientation counts o_i <= 4, with sum(m_i) = K pieces and
    // sum(m_i * size_i) = N^2 <= 25. A root-to-leaf path is a sequence of
    // (shape, orientation) placements, so the number of leaves is at most
    // K! / (m_1! * ... * m_S!) * product(o_i ^ m_i): the multinomial counts the
    // orderings of non-isomorphic pieces (identical pieces collapse, which is
    // why 25 unit squares stay linear instead of 25!), and the product counts
    // the orientation freedom. This is maximized when all shapes are distinct,
    // giving at most K! * 4^K; feasibility caps K at 8 for N = 5 (the smallest
    // eight distinct one-sided polyominoes already sum to 25 cells), so the
    // worst case is about 8! * 4^8 ~ 2.6e9. That is a wild over-count because
    // it ignores geometry: the first empty cell that no remaining piece can
    // cover prunes an entire subtree, so the real search is microscopic in
    // practice.

    map<char, vector<pair<int, int>>> piece_cells;
    for(int i = 0; i < 20; i++) {
        for(int j = 0; j < 20; j++) {
            if(grid[i][j] != '.') {
                piece_cells[grid[i][j]].push_back({i, j});
            }
        }
    }

    auto normalize = [](vector<pair<int, int>> cells) {
        int mr = INT_MAX, mc = INT_MAX;
        for(auto& [r, c]: cells) {
            mr = min(mr, r);
            mc = min(mc, c);
        }
        for(auto& [r, c]: cells) {
            r -= mr;
            c -= mc;
        }
        sort(cells.begin(), cells.end());
        return cells;
    };

    auto rotate = [&](const vector<pair<int, int>>& cells) {
        vector<pair<int, int>> rotated;
        for(auto& [r, c]: cells) {
            rotated.push_back({c, -r});
        }
        return normalize(rotated);
    };

    auto orientations = [&](vector<pair<int, int>> base) {
        base = normalize(base);
        vector<vector<pair<int, int>>> orients;
        for(int k = 0; k < 4; k++) {
            if(find(orients.begin(), orients.end(), base) == orients.end()) {
                orients.push_back(base);
            }
            base = rotate(base);
        }
        return orients;
    };

    struct group_t {
        vector<vector<pair<int, int>>> orients;
        vector<char> letters;
    };

    map<vector<pair<int, int>>, group_t> grouped;
    for(auto& [letter, cells]: piece_cells) {
        auto orients = orientations(cells);
        auto key = *min_element(orients.begin(), orients.end());
        auto& g = grouped[key];
        if(g.orients.empty()) {
            g.orients = orients;
        }
        g.letters.push_back(letter);
    }

    vector<group_t> groups;
    for(auto& [key, g]: grouped) {
        groups.push_back(g);
    }

    vector<string> res(n, string(n, '.'));

    function<bool()> backtrack = [&]() -> bool {
        int tr = -1, tc = -1;
        for(int i = 0; i < n && tr < 0; i++) {
            for(int j = 0; j < n; j++) {
                if(res[i][j] == '.') {
                    tr = i;
                    tc = j;
                    break;
                }
            }
        }

        if(tr < 0) {
            return true;
        }

        for(auto& g: groups) {
            if(g.letters.empty()) {
                continue;
            }

            for(auto& orient: g.orients) {
                int ar = orient[0].first, ac = orient[0].second;
                bool ok = true;
                for(auto& [r, c]: orient) {
                    int nr = tr + r - ar, nc = tc + c - ac;
                    if(nr < 0 || nr >= n || nc < 0 || nc >= n ||
                       res[nr][nc] != '.') {
                        ok = false;
                        break;
                    }
                }

                if(!ok) {
                    continue;
                }

                char letter = g.letters.back();
                g.letters.pop_back();
                for(auto& [r, c]: orient) {
                    res[tr + r - ar][tc + c - ac] = letter;
                }

                if(backtrack()) {
                    return true;
                }

                for(auto& [r, c]: orient) {
                    res[tr + r - ar][tc + c - ac] = '.';
                }
                g.letters.push_back(letter);
            }
        }

        return false;
    };

    backtrack();

    for(auto& row: res) {
        cout << row << "\n";
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
