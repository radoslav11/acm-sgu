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
int start_x, start_y;
vector<pair<int, int>> wells;

void read() {
    cin >> n >> start_x >> start_y;
    wells.resize(n);
    for(auto& w: wells) {
        cin >> w;
    }
}

void solve() {
    // Pure implementation. The driver picks two perpendicular directions for
    // phase 1 and the other two for phase 2, so the closed path is bounded by
    // two monotone staircases between the start (treated as origin) and the
    // opposite corner (a, b). The polygon therefore lives in one of the four
    // quadrants relative to the start. For each of the four orientations, we
    // mirror the wells into the first quadrant and look for the smallest
    // staircase polygon containing them.
    //
    // With wells in the first quadrant, set a = max(max_x, 1), b = max(max_y,
    // 1). Describe each staircase by the y-coordinate of its E-step in each
    // column: f_upper(x) for the upper boundary, f_lower(x) for the lower one,
    // both non-decreasing. Wells (xi, yi) require f_upper(xi) >= yi for xi < a
    // (well lies below/on upper path) and f_lower(xi - 1) <= yi for xi > 0
    // (well lies above/on lower path). For the polyline to be simple, the
    // upper path takes its first step N and its last step E (so f_upper(0) >=
    // 1 and f_upper(a - 1) = b), the lower path takes its first step E and
    // its last step N (so f_lower(0) = 0 and f_lower(a - 1) <= b - 1), and
    // for every interior column x in {1, ..., a - 1} the vertical segments
    // do not touch, i.e. f_lower(x) <= f_upper(x - 1) - 1.
    //
    // Greedy is optimal: pick f_upper as the smallest non-decreasing staircase
    // satisfying its lower bounds, then pick f_lower as the largest
    // non-decreasing staircase satisfying its upper bounds (which now include
    // the f_upper-dependent simple-polygon constraint). Increasing f_upper(x -
    // 1) only loosens the constraint on f_lower(x) by the same amount, so it
    // can never decrease the area. Try all four orientations and keep the
    // best; if none works, output -1.

    vector<pair<int, int>> shifted(n);
    for(int i = 0; i < n; i++) {
        shifted[i] = {wells[i].first - start_x, wells[i].second - start_y};
    }

    auto try_orientation = [&](int sx, int sy) -> pair<int64_t, string> {
        vector<pair<int, int>> w(n);
        for(int i = 0; i < n; i++) {
            w[i] = {sx * shifted[i].first, sy * shifted[i].second};
            if(w[i].first < 0 || w[i].second < 0) {
                return {LLONG_MAX, ""};
            }
        }

        int max_x = 0, max_y = 0;
        for(auto p: w) {
            max_x = max(max_x, p.first);
            max_y = max(max_y, p.second);
        }
        int a = max(max_x, 1);
        int b = max(max_y, 1);

        vector<int> max_y_at_col(a, -1);
        for(auto p: w) {
            if(p.first < a) {
                max_y_at_col[p.first] = max(max_y_at_col[p.first], p.second);
            }
        }

        vector<int> min_y_strictly_right(a, INT_MAX);
        for(auto p: w) {
            for(int xp = 0; xp < p.first && xp < a; xp++) {
                min_y_strictly_right[xp] =
                    min(min_y_strictly_right[xp], p.second);
            }
        }

        vector<int> f_upper(a, 0);
        f_upper[0] = max(1, max_y_at_col[0]);
        for(int x = 1; x < a; x++) {
            int wells_here = max(0, max_y_at_col[x]);
            f_upper[x] = max(f_upper[x - 1], wells_here);
        }
        f_upper[a - 1] = max(f_upper[a - 1], b);
        if(f_upper[a - 1] != b) {
            return {LLONG_MAX, ""};
        }

        vector<int> f_lower(a, 0);
        f_lower[0] = 0;
        for(int x = 1; x < a; x++) {
            int ub = f_upper[x - 1] - 1;
            if(min_y_strictly_right[x] != INT_MAX) {
                ub = min(ub, min_y_strictly_right[x]);
            }
            if(x == a - 1) {
                ub = min(ub, b - 1);
            }
            if(ub < f_lower[x - 1]) {
                return {LLONG_MAX, ""};
            }
            f_lower[x] = ub;
        }

        int64_t area = 0;
        for(int x = 0; x < a; x++) {
            area += f_upper[x] - f_lower[x];
        }
        if(area <= 0) {
            return {LLONG_MAX, ""};
        }

        string path;
        int prev = 0;
        for(int x = 0; x < a; x++) {
            for(int i = 0; i < f_upper[x] - prev; i++) {
                path += 'N';
            }
            path += 'E';
            prev = f_upper[x];
        }

        string lower_path;
        prev = 0;
        for(int x = 0; x < a; x++) {
            for(int i = 0; i < f_lower[x] - prev; i++) {
                lower_path += 'N';
            }
            lower_path += 'E';
            prev = f_lower[x];
        }
        for(int i = 0; i < b - prev; i++) {
            lower_path += 'N';
        }
        for(int i = (int)lower_path.size() - 1; i >= 0; i--) {
            path += (lower_path[i] == 'N' ? 'S' : 'W');
        }

        string result;
        for(char c: path) {
            char d = c;
            if(c == 'N') {
                d = (sy == 1 ? 'N' : 'S');
            } else if(c == 'S') {
                d = (sy == 1 ? 'S' : 'N');
            } else if(c == 'E') {
                d = (sx == 1 ? 'E' : 'W');
            } else if(c == 'W') {
                d = (sx == 1 ? 'W' : 'E');
            }
            result += d;
        }

        return {area, result};
    };

    pair<int64_t, string> best = {LLONG_MAX, ""};
    for(int sx: {1, -1}) {
        for(int sy: {1, -1}) {
            auto r = try_orientation(sx, sy);
            if(r.first < best.first) {
                best = r;
            }
        }
    }

    if(best.first == LLONG_MAX) {
        cout << -1 << '\n';
    } else {
        cout << best.first << '\n' << best.second << '\n';
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T = 1;
    for(int test = 1; test <= T; test++) {
        read();
        solve();
    }

    return 0;
}
