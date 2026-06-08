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
vector<string> g;

void read() {
    cin >> n;
    g.assign(n, "");
    cin >> g;
}

void solve() {
    // We need to split Q into A and B such that B is the image of A under a
    // rotation by a multiple of 90 degrees followed by a translation. Since
    // N <= 20, |Q| <= 400 and we can afford to brute force over the
    // transformation. There are only four rotations to try, and for each one
    // we can fix the lex-smallest cell c0 of Q to lie in A. Then f(c0) must
    // be some other cell of Q, which leaves only |Q| candidate shift vectors
    // per rotation - O(|Q|) transformations in total. If c0 actually belongs
    // to B in the true partition, the inverse rotation handles that case, so
    // anchoring c0 in A loses nothing.
    //
    // Fix a transformation f = R^k + v. The requirement Q = A U f(A) induces
    // a functional graph on Q: draw a directed edge c -> f(c) whenever both
    // c and f(c) are in Q. Each vertex has in- and out-degree at most one,
    // so the components are simple paths and simple cycles. Along every
    // component the labels A/B must alternate, because c in A forces f(c)
    // in B and vice versa. A path endpoint with no out-edge is forced into
    // B, an endpoint with no in-edge into A, so the path needs even length.
    // Cycles also need even length, but f has order dividing 4 and so the
    // cycles have length 2 or 4 - the parity is automatic. The only test
    // worth running per transformation is "are all path components even?".
    //
    // If some f passes the test, we recover A by taking the cells coloured
    // first along each component and print it. If nothing works, the answer
    // is "NO".

    vector<pair<int, int>> cells;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if(g[i][j] == '1') {
                cells.push_back({i, j});
            }
        }
    }

    int total = cells.size();
    if(total == 0) {
        cout << "YES" << '\n';
        for(int i = 0; i < n; i++) {
            cout << string(n, '0') << '\n';
        }
        return;
    }

    if(total % 2 != 0) {
        cout << "NO" << '\n';
        return;
    }

    auto rotate_k = [](int r, int c, int k) {
        for(int i = 0; i < k; i++) {
            int nr = c, nc = -r;
            r = nr;
            c = nc;
        }
        return make_pair(r, c);
    };

    auto in_q = [&](int r, int c) {
        return 0 <= r && r < n && 0 <= c && c < n && g[r][c] == '1';
    };

    auto [c0r, c0c] = cells[0];

    auto try_transform = [&](int k, int dr, int dc) -> bool {
        auto f = [&](int r, int c) {
            auto [nr, nc] = rotate_k(r, c, k);
            return make_pair(nr + dr, nc + dc);
        };
        auto f_inv = [&](int r, int c) {
            auto [nr, nc] = rotate_k(r - dr, c - dc, (4 - k) % 4);
            return make_pair(nr, nc);
        };

        vector<vector<int>> color(n, vector<int>(n, -1));

        for(int i = 0; i < n; i++) {
            for(int j = 0; j < n; j++) {
                if(g[i][j] != '1' || color[i][j] != -1) {
                    continue;
                }

                int sr = i, sc = j;
                while(true) {
                    auto [pr, pc] = f_inv(sr, sc);
                    if(!in_q(pr, pc) || (pr == i && pc == j)) {
                        break;
                    }
                    sr = pr;
                    sc = pc;
                }

                int col = 0, length = 0;
                int cr = sr, cc = sc;
                while(color[cr][cc] == -1) {
                    color[cr][cc] = col;
                    col ^= 1;
                    length++;
                    auto [nr, nc] = f(cr, cc);
                    if(!in_q(nr, nc)) {
                        break;
                    }
                    cr = nr;
                    cc = nc;
                }

                if(length % 2 != 0) {
                    return false;
                }
            }
        }

        cout << "YES" << '\n';
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < n; j++) {
                if(g[i][j] == '1' && color[i][j] == 0) {
                    cout << '1';
                } else {
                    cout << '0';
                }
            }
            cout << '\n';
        }
        return true;
    };

    for(int k = 0; k < 4; k++) {
        auto [rr, rc] = rotate_k(c0r, c0c, k);
        for(auto [tr, tc]: cells) {
            if(tr == c0r && tc == c0c) {
                continue;
            }
            int dr = tr - rr, dc = tc - rc;
            if(try_transform(k, dr, dc)) {
                return;
            }
        }
    }

    cout << "NO" << '\n';
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
