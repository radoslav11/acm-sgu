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
vector<pair<int, int>> pnts;

void read() {
    cin >> n;
    pnts.resize(n);
    cin >> pnts;
}

bool check_k1(const vector<int>& indices, int64_t d) {
    if(indices.empty()) {
        return true;
    }
    int64_t left = -2e9, right = 2e9;
    int64_t bottom = -2e9, top = 2e9;
    for(int idx: indices) {
        int x = pnts[idx].first, y = pnts[idx].second;
        left = max(left, 2ll * x - d);
        right = min(right, 2ll * x + d);
        bottom = max(bottom, 2ll * y - d);
        top = min(top, 2ll * y + d);
    }
    return right >= left && top >= bottom;
}

bool check_k2(const vector<int>& indices, int64_t d) {
    if(indices.empty()) {
        return true;
    }

    int64_t min_x = 2e9, max_x = -2e9;
    int64_t min_y = 2e9, max_y = -2e9;
    for(int idx: indices) {
        min_x = min(min_x, (int64_t)pnts[idx].first);
        max_x = max(max_x, (int64_t)pnts[idx].first);
        min_y = min(min_y, (int64_t)pnts[idx].second);
        max_y = max(max_y, (int64_t)pnts[idx].second);
    }

    bool diag1 = true;
    for(int idx: indices) {
        int64_t x = pnts[idx].first, y = pnts[idx].second;
        bool in_bl = (x <= min_x + d && y <= min_y + d);
        bool in_tr = (x >= max_x - d && y >= max_y - d);
        if(!in_bl && !in_tr) {
            diag1 = false;
            break;
        }
    }
    if(diag1) {
        return true;
    }

    bool diag2 = true;
    for(int idx: indices) {
        int64_t x = pnts[idx].first, y = pnts[idx].second;
        bool in_br = (x >= max_x - d && y <= min_y + d);
        bool in_tl = (x <= min_x + d && y >= max_y - d);
        if(!in_br && !in_tl) {
            diag2 = false;
            break;
        }
    }
    return diag2;
}

bool check_k3(const vector<int>& indices, int64_t d) {
    if(indices.empty()) {
        return true;
    }

    int64_t min_x = 2e9, max_x = -2e9;
    int64_t min_y = 2e9, max_y = -2e9;
    for(int idx: indices) {
        min_x = min(min_x, (int64_t)pnts[idx].first);
        max_x = max(max_x, (int64_t)pnts[idx].first);
        min_y = min(min_y, (int64_t)pnts[idx].second);
        max_y = max(max_y, (int64_t)pnts[idx].second);
    }

    auto try_corner = [&](int64_t cx, int64_t cy) {
        vector<int> remaining;
        for(int idx: indices) {
            int64_t x = pnts[idx].first, y = pnts[idx].second;
            if(x < cx || x > cx + d || y < cy || y > cy + d) {
                remaining.push_back(idx);
            }
        }
        return check_k2(remaining, d);
    };

    return try_corner(min_x, min_y) || try_corner(max_x - d, min_y) ||
           try_corner(min_x, max_y - d) || try_corner(max_x - d, max_y - d);
}

void solve() {
    // One way to start with this problem is with a binary search on the answer.
    // We now want to check if a set of points S can be covered by 3 squares.
    // Let's have a function check(S, K) that equals true if we can cover S
    // using K squares.
    //
    // When K = 1, we can instead check the valid ranges of X and Y where the
    // square center can be. In particular if we have a point (x, y), then the
    // center should certainly be in the square defined by (x - D/2, y - D/2)
    // and (x + D/2, y + D/2). Then if there is a non-empty region, we are done.
    //
    // For K = 2, we can notice that there is always an optimal solution where
    // two squares are positioned at opposite corners of the bounding box. We
    // check both diagonal configurations: bottom-left + top-right, and
    // bottom-right + top-left. For each point, we verify it falls within at
    // least one of the two corner squares.
    //
    // For K = 3, we try placing the first square at each of the 4 corners of
    // the bounding box. For each placement, we mark the covered points and
    // recursively check if the remaining points can be solved with K = 2.
    // Since we try all 4 corners and K = 2 tries both diagonals, this covers
    // all possible configurations.
    //
    // The complexity is O(N log D) where D is the coordinate range, as each
    // check is O(N) and we binary search on the answer.

    vector<int> all_indices(n);
    iota(all_indices.begin(), all_indices.end(), 0);

    int64_t low = 0, high = 2000000042, mid, ret = high;
    while(low <= high) {
        mid = (low + high) / 2;
        if(check_k3(all_indices, mid)) {
            ret = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    cout << ret << '\n';
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
