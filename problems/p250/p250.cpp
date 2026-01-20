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
vector<vector<int>> grid;
vector<vector<int>> dist;

struct Result {
    int area;
    int col;
    int r1_start, r1_end, w1;
    int r2_start, r2_end, w2;
    int r3_start, r3_end, w3;

    Result()
        : area(-1),
          col(-1),
          r1_start(-1),
          r1_end(-1),
          w1(-1),
          r2_start(-1),
          r2_end(-1),
          w2(-1),
          r3_start(-1),
          r3_end(-1),
          w3(-1) {}

    Result(
        int a, int c, int rs1, int re1, int ww1, int rs2, int re2, int ww2,
        int rs3, int re3, int ww3
    )
        : area(a),
          col(c),
          r1_start(rs1),
          r1_end(re1),
          w1(ww1),
          r2_start(rs2),
          r2_end(re2),
          w2(ww2),
          r3_start(rs3),
          r3_end(re3),
          w3(ww3) {}
};

void read() {
    cin >> n >> m;
    grid.assign(n, vector<int>(m));
    cin >> grid;
    dist.assign(n, vector<int>(m));
    for(int row = 0; row < n; row++) {
        for(int col = m - 1; col >= 0; col--) {
            if(grid[row][col] == 1) {
                dist[row][col] = 0;
            } else {
                dist[row][col] = (col == m - 1) ? 1 : dist[row][col + 1] + 1;
            }
        }
    }
}

Result solve_histogram(int col) {
    vector<int> height(n);
    for(int row = 0; row < n; row++) {
        height[row] = dist[row][col];
    }

    vector<int> l(n), r(n);
    stack<int> s;

    for(int i = 0; i < n; i++) {
        while(!s.empty() && height[s.top()] >= height[i]) {
            s.pop();
        }
        l[i] = s.empty() ? -1 : s.top();
        s.push(i);
    }
    while(!s.empty()) {
        s.pop();
    }

    for(int i = n - 1; i >= 0; i--) {
        while(!s.empty() && height[s.top()] >= height[i]) {
            s.pop();
        }
        r[i] = s.empty() ? n : s.top();
        s.push(i);
    }

    vector<vector<int>> min_h(n, vector<int>(n, INT_MAX));
    for(int i = 0; i < n; i++) {
        min_h[i][i] = height[i];
        for(int j = i + 1; j < n; j++) {
            min_h[i][j] = min(min_h[i][j - 1], height[j]);
        }
    }

    Result best;
    for(int i = 0; i < n; i++) {
        if(height[i] == 0) {
            continue;
        }

        int top_start = l[i] + 1, top_end;
        for(int j = i + 2; j < n; j++) {
            if(height[j] == 0) {
                continue;
            }

            int bot_start;
            int bot_end = r[j] - 1;
            int w1 = height[i];
            int w3 = height[j];

            if(r[i] >= j) {
                int mid_row = max(i + 1, l[j]);
                top_end = mid_row - 1;
                bot_start = mid_row + 1;

                int w2 = min(w1, w3) - 1;
                int area = (top_end - top_start + 1) * w1 + 1 * w2 +
                           (bot_end - bot_start + 1) * w3;
                if(w2 > 0 && area > best.area) {
                    best = Result(
                        area, col, top_start, top_end, w1, mid_row, mid_row, w2,
                        bot_start, bot_end, w3
                    );
                }
            }

            if(l[j] <= i) {
                int mid_row = min(j - 1, r[i]);
                top_end = mid_row - 1;
                bot_start = mid_row + 1;

                int w2 = min(w1, w3) - 1;
                int area = (top_end - top_start + 1) * w1 + 1 * w2 +
                           (bot_end - bot_start + 1) * w3;
                if(w2 > 0 && area > best.area) {
                    best = Result(
                        area, col, top_start, top_end, w1, mid_row, mid_row, w2,
                        bot_start, bot_end, w3
                    );
                }
            }

            if(r[i] <= l[j]) {
                top_end = r[i] - 1;
                bot_start = l[j] + 1;

                int mid_start = r[i];
                int mid_end = l[j];

                int min_middle = min_h[r[i]][l[j]];
                int w2 = min({w1 - 1, w3 - 1, min_middle});
                int area = (top_end - top_start + 1) * w1 +
                           (mid_end - mid_start + 1) * w2 +
                           (bot_end - bot_start + 1) * w3;
                if(w2 > 0 && area > best.area) {
                    best = Result(
                        area, col, top_start, top_end, w1, mid_start, mid_end,
                        w2, bot_start, bot_end, w3
                    );
                }
            }
        }
    }

    return best;
}

void solve() {
    // Implementing this problem is non-trivial, but the core idea is to think
    // about how the largest sub-rectangle problem is solved, and adapt the
    // solution. More precisely, there is a popular approach with building a
    // "histogram" of the heights of empty places, and then using stacks find
    // the closest columns that are smaller as they define how far we can extend
    // the current column. There is a Geeks-for-Geeks page describing the idea
    // further:
    //
    //     www.geeksforgeeks.org/dsa/maximum-size-rectangle-binary-sub-matrix-1s
    //
    // In this problem we have to choose 3 adjacent rectangles. We know that
    // their leftmost points should fall on the same vertical line. Let's try to
    // independently solve (find best possible answer) for every column in O(M).
    // We can build a histogram with the distances to the right from each cell
    // in this column. Let's try to fix i + 1 < j - the "critical" column in the
    // top and bottom rectangles out of the three. We can easily precompute with
    // a stack l[i] and r[i] as the closest positions smaller than height[i],
    // and we should trivially extend the top and bottom rectangles to l[i] and
    // r[j]. The area between i and j is a bit trickier as we have a few cases:
    //
    //     1) r[i] >= j. In that case the height of the middle column should be
    //        simply min(height[i], height[j])-1, and we can have a rectangle of
    //        length 1 directly adjacent to the larger of i and j. To be more
    //        precise, we should first try to extend j as far as possible to the
    //        left, then put the middle with width 1, and finally have the rest
    //        for i.
    //
    //     2) l[j] <= i. This is symmetric to the (1) case.
    //
    //     3) If it's neither (1) or (2), we have r[i] <= l[j]. In this case, we
    //        are bounded by min(height[k] for r[i] <= k <= l[j]) for the middle
    //        part. We can do a quadratic precompute of min[L][R] with a simple
    //        DP or a sparse table, so that this case can also be handled in
    //        constant time.
    //
    // As we already mentioned, we are looking at the O(M) histogram, finding
    // the best "C" for a histogram can be done in O(N^2) time. As we have to
    // recover the answer, it's convenient to return the relevant intervals for
    // the 3 portions, as well as the heights and the overall area. Some
    // additional parts to be careful about are heights of 0 as this won't be a
    // valid rectangle.

    Result best;
    for(int col = 0; col < m; col++) {
        Result res = solve_histogram(col);
        if(res.area > best.area) {
            best = res;
        }
    }

    if(best.area == -1) {
        cout << -1 << '\n';
        return;
    }

    cout << best.area << '\n';

    int col = best.col;
    for(int row = best.r1_start; row <= best.r1_end; row++) {
        for(int c = col; c < col + best.w1; c++) {
            grid[row][c] = 8;
        }
    }
    for(int row = best.r2_start; row <= best.r2_end; row++) {
        for(int c = col; c < col + best.w2; c++) {
            grid[row][c] = 8;
        }
    }
    for(int row = best.r3_start; row <= best.r3_end; row++) {
        for(int c = col; c < col + best.w3; c++) {
            grid[row][c] = 8;
        }
    }

    for(int row = 0; row < n; row++) {
        for(int c = 0; c < m; c++) {
            if(c > 0) {
                cout << ' ';
            }
            cout << grid[row][c];
        }
        cout << '\n';
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
