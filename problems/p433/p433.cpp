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

int n, m, k;
vector<int> lengths;

void read() {
    cin >> n >> m >> k;
    lengths.resize(k);
    cin >> lengths;
}

void solve() {
    // The key idea is to stop thinking about individual bricks and instead
    // decide, for each cell, only the orientation of the brick covering it:
    // give the cell a 1 if it belongs to a horizontal brick and a 0 if it
    // belongs to a vertical one. Walk along a row: a maximal run of 1's is a
    // strip that is covered purely by horizontal bricks laid end to end, so
    // the run length must be a sum of brick lengths. Walk down a column: a
    // maximal run of 0's is covered purely by vertical bricks, so its length
    // must also be a sum of brick lengths. Conversely, any 0/1 matrix whose
    // horizontal 1-runs and vertical 0-runs all have such lengths can be cut
    // straight into bricks. So a tiling exists exactly when this orientation
    // matrix exists, and our whole job is to build one.
    //
    // Call a length "representable" if it is a non-negative combination of the
    // brick lengths. rep[s] is the coin-reachability of s computed bottom-up,
    // and par[s] remembers one brick length used to reach s, which lets us
    // later chop a run of representable length back into concrete bricks.
    //
    // Two cases are immediate. If the width is representable, every row on its
    // own is a horizontal strip of a representable length, so make the whole
    // matrix 1's (all bricks horizontal). If the width is not representable but
    // the height is, every column is a vertical strip, so leave the matrix all
    // 0's (all bricks vertical) - that is exactly the do-nothing branch below.
    //
    // The interesting case is when neither dimension is representable. We build
    // the orientation matrix one full column at a time, left to right. Write a
    // column as a bitmask x in {0,1}^h, where bit r is the orientation of cell
    // (r, current column): 1 horizontal, 0 vertical.
    //
    // The vertical constraint lives entirely inside one column and never
    // couples to its neighbours: x is "legal" iff every maximal run of 0-bits
    // in x has representable length. These legal masks are few here (if many
    // run lengths were representable the height itself would be), so we list
    // the whole set C = { legal x } up front and only ever place columns from
    // C. That leaves just the horizontal constraint, which does couple columns,
    // for the sweep to enforce. The DP is:
    //
    //   state    run[0..h-1], where run[r] is the length of the horizontal
    //            1-run in row r that is still open at the current column - i.e.
    //            the number of consecutive 1-bits in row r ending here, or 0 if
    //            the last placed column had a 0 in row r.
    //
    //   base     before any column is placed the state is run = (0, ..., 0).
    //
    //   step     to extend state run by a column x in C, look at each row r:
    //              x_r = 1  -> the run grows:      run'[r] = run[r] + 1
    //              x_r = 0  -> the run ends here, which is legal only if its
    //                          length is representable (rep[run[r]], or run[r]
    //                          was already 0); then run'[r] = 0.
    //            if any row's closing length is not representable, x is
    //            rejected from this state.
    //
    //   accept   after all w columns, a state is accepting iff every row's
    //            still-open run is representable (rep[run[r]] or run[r] = 0) -
    //            this is the right-border closing of the last bricks.
    //
    // A column extends a state iff a tiling-legal orientation matrix can have
    // those run lengths at that boundary, so reaching an accepting state after
    // w columns is exactly a valid matrix. Each DP node stores its predecessor
    // and the mask x that produced it, so from an accepting state we walk the
    // chain back to read off the column masks.
    //
    // On the number of states: each run[r] ranges over [0, w], so the naive
    // bound is (w + 1)^h distinct vectors, far too many to enumerate. What
    // saves us is that we only keep states that are prefixes of genuine
    // tilings - a run may stay open only while it can still close at a
    // representable length, so a column that strands some row at a length that
    // can never become representable is rejected on the spot and never spawns a
    // state. That decomposability requirement is what does the real pruning. On
    // top of it we fold equivalent run lengths together before using a state as
    // a dedup key: all that a run length L means for the future is the sequence
    // rep[L], rep[L + 1], ..., which is eventually periodic with period g (the
    // gcd of the lengths that fit), so canon[] maps L to a representative of its
    // class. For a dense set such as a single length 2 this collapses runs to
    // their parity; for a sparse set it changes nothing. Together these keep
    // the reachable states in the low thousands, so the sweep is fast in
    // practice.
    //
    // The board is always handled with its shorter side as the height h (and
    // the answer transposed back at the end when we flipped it), which keeps
    // both the 2^h mask enumeration and the per-row state vector small. Once
    // the orientation matrix is fixed, par[] cuts every horizontal and vertical
    // run into bricks with distinct ids, and the bricks are finally given
    // letters by a smallest-last greedy coloring. The brick adjacency graph is
    // planar, so this never needs more than six of the available letters.

    int dim = max(n, m);
    vector<char> rep(dim + 1, false);
    vector<int> par(dim + 1, -1);
    rep[0] = true;
    for(int s = 1; s <= dim; s++) {
        for(int len: lengths) {
            if(len <= s && rep[s - len]) {
                rep[s] = true;
                par[s] = len;
                break;
            }
        }
    }

    int g = 0;
    for(int len: lengths) {
        if(len <= dim) {
            g = gcd(g, len);
        }
    }

    vector<int> canon(dim + 1);
    if(g == 0) {
        for(int s = 0; s <= dim; s++) {
            canon[s] = s;
        }
    } else {
        int pre = 0;
        for(int s = 0; s <= dim; s++) {
            if((bool)rep[s] != (s % g == 0)) {
                pre = s + 1;
            }
        }
        for(int s = 0; s <= dim; s++) {
            canon[s] = s < pre ? s : pre + (s - pre) % g;
        }
    }

    bool transposed = n > m;
    int h = min(n, m), w = max(n, m);

    vector<vector<int>> horiz(h, vector<int>(w, 0));
    bool possible = true;

    if(rep[w]) {
        for(auto& row: horiz) {
            fill(row.begin(), row.end(), 1);
        }
    } else if(!rep[h]) {
        vector<int> valid_cols;
        for(int mask = 0; mask < (1 << h); mask++) {
            bool ok = true;
            int r = 0;
            while(r < h) {
                if(((mask >> r) & 1) == 0) {
                    int r2 = r;
                    while(r2 < h && ((mask >> r2) & 1) == 0) {
                        r2++;
                    }
                    if(!rep[r2 - r]) {
                        ok = false;
                        break;
                    }
                    r = r2;
                } else {
                    r++;
                }
            }
            if(ok) {
                valid_cols.push_back(mask);
            }
        }

        struct node {
            vector<int> runs;
            int parent;
            int mask;
        };

        vector<vector<node>> layers(w + 1);
        vector<map<vector<int>, int>> seen(w + 1);
        layers[0].push_back({vector<int>(h, 0), -1, -1});
        seen[0][vector<int>(h, 0)] = 0;

        auto key_of = [&](const vector<int>& runs) {
            vector<int> key(h);
            for(int r = 0; r < h; r++) {
                key[r] = canon[runs[r]];
            }
            return key;
        };

        for(int c = 0; c < w; c++) {
            for(int i = 0; i < (int)layers[c].size(); i++) {
                const vector<int>& runs = layers[c][i].runs;
                for(int mask: valid_cols) {
                    vector<int> nxt(h);
                    bool ok = true;
                    for(int r = 0; r < h; r++) {
                        if((mask >> r) & 1) {
                            nxt[r] = runs[r] + 1;
                        } else {
                            if(runs[r] > 0 && !rep[runs[r]]) {
                                ok = false;
                                break;
                            }
                            nxt[r] = 0;
                        }
                    }

                    if(!ok) {
                        continue;
                    }

                    vector<int> key = key_of(nxt);
                    if(seen[c + 1].find(key) == seen[c + 1].end()) {
                        seen[c + 1][key] = layers[c + 1].size();
                        layers[c + 1].push_back({nxt, i, mask});
                    }
                }
            }
        }

        int final_idx = -1;
        for(int i = 0; i < (int)layers[w].size(); i++) {
            bool ok = true;
            for(int r = 0; r < h; r++) {
                if(layers[w][i].runs[r] > 0 && !rep[layers[w][i].runs[r]]) {
                    ok = false;
                    break;
                }
            }
            if(ok) {
                final_idx = i;
                break;
            }
        }

        if(final_idx == -1) {
            possible = false;
        } else {
            int idx = final_idx;
            for(int c = w; c >= 1; c--) {
                int mask = layers[c][idx].mask;
                for(int r = 0; r < h; r++) {
                    horiz[r][c - 1] = (mask >> r) & 1;
                }
                idx = layers[c][idx].parent;
            }
        }
    }

    if(!possible) {
        cout << "NO\n";
        return;
    }

    vector<vector<int>> bid(h, vector<int>(w, -1));
    int next_id = 0;
    for(int r = 0; r < h; r++) {
        int c = 0;
        while(c < w) {
            if(horiz[r][c] == 1) {
                int c2 = c;
                while(c2 < w && horiz[r][c2] == 1) {
                    c2++;
                }

                int pos = c, rem = c2 - c;
                while(rem > 0) {
                    int len = par[rem];
                    for(int t = 0; t < len; t++) {
                        bid[r][pos + t] = next_id;
                    }
                    next_id++;
                    pos += len;
                    rem -= len;
                }

                c = c2;
            } else {
                c++;
            }
        }
    }

    for(int c = 0; c < w; c++) {
        int r = 0;
        while(r < h) {
            if(horiz[r][c] == 0) {
                int r2 = r;
                while(r2 < h && horiz[r2][c] == 0) {
                    r2++;
                }

                int pos = r, rem = r2 - r;
                while(rem > 0) {
                    int len = par[rem];
                    for(int t = 0; t < len; t++) {
                        bid[pos + t][c] = next_id;
                    }
                    next_id++;
                    pos += len;
                    rem -= len;
                }

                r = r2;
            } else {
                r++;
            }
        }
    }

    int b = next_id;
    vector<set<int>> nbr(b);
    for(int r = 0; r < h; r++) {
        for(int c = 0; c < w; c++) {
            if(c + 1 < w && bid[r][c] != bid[r][c + 1]) {
                nbr[bid[r][c]].insert(bid[r][c + 1]);
                nbr[bid[r][c + 1]].insert(bid[r][c]);
            }
            if(r + 1 < h && bid[r][c] != bid[r + 1][c]) {
                nbr[bid[r][c]].insert(bid[r + 1][c]);
                nbr[bid[r + 1][c]].insert(bid[r][c]);
            }
        }
    }

    vector<int> deg(b), order;
    vector<char> removed(b, false);
    for(int i = 0; i < b; i++) {
        deg[i] = nbr[i].size();
    }

    for(int step = 0; step < b; step++) {
        int u = -1;
        for(int i = 0; i < b; i++) {
            if(!removed[i] && (u == -1 || deg[i] < deg[u])) {
                u = i;
            }
        }

        removed[u] = true;
        order.push_back(u);
        for(int v: nbr[u]) {
            if(!removed[v]) {
                deg[v]--;
            }
        }
    }

    vector<int> color(b, -1);
    for(int i = b - 1; i >= 0; i--) {
        int u = order[i];
        set<int> used;
        for(int v: nbr[u]) {
            if(color[v] != -1) {
                used.insert(color[v]);
            }
        }

        int col = 0;
        while(used.count(col)) {
            col++;
        }
        color[u] = col;
    }

    vector<string> out(n, string(m, '?'));
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            int id = transposed ? bid[j][i] : bid[i][j];
            out[i][j] = 'a' + color[id];
        }
    }

    cout << "YES\n";
    for(auto& row: out) {
        cout << row << '\n';
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
