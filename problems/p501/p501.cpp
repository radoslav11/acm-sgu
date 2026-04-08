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
vector<string> layers;

void read() {
    cin >> n;
    layers.resize(2 * n);
    cin >> layers;
}

void solve() {
    // There are a ton of details in this problem, but in its core, it's a
    // dynamic programming. We simply need to avoid having too many states. In
    // particular, each of the 8 sides has n^2 cells, so for n=4 this is 128
    // cells and bitmask dp of 2^128 is clearly too slow.
    //
    // The octahedron has 8 triangular faces, 4 forming the top pyramid and 4
    // the bottom. Each face is divided into n^2 small triangles arranged in
    // rows of sizes 1, 3, 5, ..., 2n-1. Cells in each row alternate between
    // up-pointing and down-pointing. Adjacent cells (sharing an edge) can form
    // a domino, including across face boundaries and across consecutive rows.
    //
    // Key observation: between consecutive rows r and r+1, only up-pointing
    // cells in row r connect to down-pointing cells in row r+1. Per face there
    // are (r+1) such inter-row edges, giving 4*(r+1) "carry" cells total.
    // These carry cells are the only coupling between rows, so we can do a
    // layer-by-layer DP where the state is a bitmask of which carry cells are
    // still unmatched (need to be covered by a within-row domino in the next
    // row). The state size is 2^(4*r) which for n=4 is at most 2^16 = 65536.
    //
    // We solve the top and bottom pyramids independently. At the equator (the
    // boundary between the two halves), each pyramid's DP produces a count per
    // mask of unmatched equator cells. Two half-configurations are compatible
    // iff they have the exact same equator mask (an unmatched cell from above
    // pairs with the corresponding unmatched cell from below). So the answer
    // is the dot product of the two result vectors.

    vector<string> upper(n), lower(n);
    for(int i = 0; i < n; ++i) {
        upper[i] = layers[i];
    }
    for(int i = 0; i < n; ++i) {
        lower[i] = layers[2 * n - 1 - i];
    }

    auto solve_pyramid = [&](const vector<string>& rows) -> vector<int64_t> {
        int temp[1024 * 1024];
        int temp_size = 0;
        int first_row[16] = {};
        for(int mask = 0; mask < 16; ++mask) {
            for(int e = 0; e < 16; ++e) {
                int covered = mask;
                bool ok = true;
                for(int j = 0; j < 4 && ok; ++j) {
                    if(e & (1 << j)) {
                        int u = j, v = (j + 1) & 3;
                        if((covered >> u & 1) || (covered >> v & 1)) {
                            ok = false;
                        } else {
                            covered |= (1 << u) | (1 << v);
                        }
                    }
                }
                if(ok && covered == 15) {
                    first_row[mask]++;
                }
            }
        }

        int bit_mapping[5][32] = {};
        int bit_mapping_next[5][32];
        int bit_mapping_result[5][32];
        vector<int> next_row[4][1 << 12];
        int next_row_size[4][1 << 12] = {};
        int64_t dp[5][1 << 16] = {};

        memset(bit_mapping_next, -1, sizeof(bit_mapping_next));
        memset(bit_mapping_result, -1, sizeof(bit_mapping_result));

        for(int r = 0; r < n; ++r) {
            int cur_len = 2 * r + 1;
            int nxt_len = 2 * r + 3;
            int shift = 0, count = 0, current = 0;
            for(int g = 0; g < 4; ++g) {
                for(int i = 0; i < cur_len; i += 2) {
                    bit_mapping_next[r][current + i] = shift + (i + 1);
                    bit_mapping_result[r][current + i] = count++;
                }
                for(int i = 0; i < r; ++i) {
                    bit_mapping[r][g * r + i] = current + 2 * i + 1;
                }
                current += cur_len;
                shift += nxt_len;
            }
        }
        if(n > 0) {
            for(int i = 0; i < 32; ++i) {
                bit_mapping_next[n - 1][i] = 0;
            }
        }

        int row_mask[5] = {};
        for(int i = 0; i < n; ++i) {
            const string& a = rows[i];
            for(int j = 0; j < (int)a.size(); ++j) {
                if(a[j] == '*') {
                    row_mask[i] |= (1 << j);
                }
            }
        }

        auto carry_to_layer_mask = [&](int row, int cur_mask, int len) -> int {
            int res = 0;
            for(int i = 0; i < len; ++i) {
                if(cur_mask & (1 << i)) {
                    res |= (1 << bit_mapping[row][i]);
                }
            }
            return res;
        };

        auto enumerate_matchings = [&](auto&& self, int mask, int idx,
                                       int next_lvl_mask, int res, int len,
                                       int row) -> void {
            if(idx == len) {
                temp[temp_size++] = res;
                return;
            }
            if(mask & (1 << idx)) {
                self(self, mask, idx + 1, next_lvl_mask, res, len, row);
                return;
            }
            if(idx == 0 && ((mask & (1 << (len - 1))) == 0)) {
                self(
                    self, mask | (1 << (len - 1)), idx + 1, next_lvl_mask, res,
                    len, row
                );
            }
            if(bit_mapping_result[row][idx] != -1 &&
               ((next_lvl_mask & (1 << bit_mapping_next[row][idx])) == 0)) {
                self(
                    self, mask, idx + 1, next_lvl_mask,
                    res | (1 << bit_mapping_result[row][idx]), len, row
                );
            }
            if(idx < len - 1 && ((mask & (1 << (idx + 1))) == 0)) {
                self(self, mask, idx + 2, next_lvl_mask, res, len, row);
            }
        };

        for(int r = 1; r < n; ++r) {
            int prev_bits = 4 * r;
            for(int i = 0; i < (1 << prev_bits); ++i) {
                temp_size = 0;
                int cur_mask = carry_to_layer_mask(r, i, 4 * r);
                if((cur_mask & row_mask[r]) != 0) {
                    continue;
                }
                enumerate_matchings(
                    enumerate_matchings, cur_mask | row_mask[r], 0,
                    row_mask[r + 1], 0, 4 * (2 * r + 1), r
                );
                next_row[r][i].resize(temp_size);
                next_row_size[r][i] = temp_size;
                for(int j = 0; j < temp_size; ++j) {
                    next_row[r][i][j] = temp[j];
                }
            }
        }

        for(int i = 0; i < (1 << 4); ++i) {
            if((i & row_mask[0]) != 0) {
                continue;
            }
            if(n > 1 && (carry_to_layer_mask(1, i, 4) & row_mask[1]) != 0) {
                continue;
            }
            dp[1][i] += first_row[i | row_mask[0]];
        }

        for(int r = 1; r < n; ++r) {
            int prev_bits = 4 * r;
            for(int mask = 0; mask < (1 << prev_bits); ++mask) {
                if(dp[r][mask] == 0) {
                    continue;
                }
                for(int k = 0; k < next_row_size[r][mask]; ++k) {
                    int nxt = next_row[r][mask][k];
                    dp[r + 1][nxt] += dp[r][mask];
                }
            }
        }

        vector<int64_t> result(1 << (4 * n), 0LL);
        if(n == 0) {
            result[0] = 1;
        } else {
            for(int i = 0; i < (1 << (4 * n)); ++i) {
                if(i < (1 << 16)) {
                    result[i] = dp[n][i];
                }
            }
        }
        return result;
    };

    vector<int64_t> dp_up = solve_pyramid(upper);
    vector<int64_t> dp_down = solve_pyramid(lower);

    int64_t ans = 0;
    int final_size = 1 << (4 * n);
    for(int i = 0; i < final_size; ++i) {
        ans += dp_up[i] * dp_down[i];
    }
    cout << ans << '\n';
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
