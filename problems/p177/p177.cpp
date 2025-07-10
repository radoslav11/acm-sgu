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

const int B = 1024;

int n, m;
int sq;
vector<bitset<B>> grid;
vector<bitset<B>> set_1_lazy, set_0_lazy;
bitset<B> full_m;

void apply_lazy(
    vector<bitset<B>>& grid, vector<bitset<B>>& set_1_lazy,
    vector<bitset<B>>& set_0_lazy, int bucket, int n, int sq
) {
    int start = bucket * sq;
    int end = min(n, (bucket + 1) * sq);
    for(int i = start; i < end; i++) {
        grid[i] |= set_1_lazy[bucket];
        grid[i] &= ~set_0_lazy[bucket];
    }
    set_1_lazy[bucket] = bitset<B>(0);
    set_0_lazy[bucket] = bitset<B>(0);
}

void print_table(
    vector<bitset<B>>& grid, vector<bitset<B>>& set_1_lazy,
    vector<bitset<B>>& set_0_lazy, int n, int sq
) {
    for(int bucket = 0; bucket * sq < n; bucket++) {
        apply_lazy(grid, set_1_lazy, set_0_lazy, bucket, n, sq);
    }
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            cout << (grid[i][j] ? 'W' : 'B');
        }
        cout << '\n';
    }
    cout << '\n';
}

void read() { cin >> n >> m; }

void solve() {
    sq = sqrt(n) + 1;
    grid.assign(n, bitset<B>());
    set_1_lazy.assign(sq, bitset<B>());
    set_0_lazy.assign(sq, bitset<B>());

    full_m = bitset<B>();
    for(int i = 0; i < n; i++) {
        full_m.set(i);
    }
    for(int i = 0; i < n; i++) {
        grid[i] = full_m;
    }

    while(m--) {
        int x1, y1, x2, y2;
        string c;
        cin >> x1 >> y1 >> x2 >> y2 >> c;
        x1--;
        y1--;
        x2--;
        y2--;

        if(x1 > x2) {
            swap(x1, x2);
        }
        if(y1 > y2) {
            swap(y1, y2);
        }

        bool color = (c == "w");
        bitset<B> mask = (full_m >> (n - (y2 - y1 + 1))) << y1;

        int r1 = x1 / sq, r2 = x2 / sq;

        if(r1 == r2) {
            apply_lazy(grid, set_1_lazy, set_0_lazy, r1, n, sq);
            for(int i = x1; i <= x2; i++) {
                if(color) {
                    grid[i] |= mask;
                } else {
                    grid[i] &= ~mask;
                }
            }
        } else {
            apply_lazy(grid, set_1_lazy, set_0_lazy, r1, n, sq);
            for(int i = x1; i < (r1 + 1) * sq && i <= x2; i++) {
                if(color) {
                    grid[i] |= mask;
                } else {
                    grid[i] &= ~mask;
                }
            }
            apply_lazy(grid, set_1_lazy, set_0_lazy, r2, n, sq);
            for(int i = r2 * sq; i <= x2; i++) {
                if(color) {
                    grid[i] |= mask;
                } else {
                    grid[i] &= ~mask;
                }
            }
            for(int i = r1 + 1; i < r2; i++) {
                if(color) {
                    set_1_lazy[i] |= mask;
                    set_0_lazy[i] &= ~mask;
                } else {
                    set_0_lazy[i] |= mask;
                    set_1_lazy[i] &= ~mask;
                }
            }
        }

        // print_table(grid, set_1_lazy, set_0_lazy, n, sq);
    }

    int ans = 0;
    for(int bucket = 0; bucket * sq < n; bucket++) {
        apply_lazy(grid, set_1_lazy, set_0_lazy, bucket, n, sq);
    }
    for(int i = 0; i < n; i++) {
        ans += grid[i].count();
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
        // cout << "Case #" << test << ": ";
        solve();
    }

    return 0;
}
