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
vector<pair<int, int>> p;
vector<int64_t> prefix_len;

int64_t seg_len(int i) {
    return abs(p[i + 1].first - p[i].first) +
           abs(p[i + 1].second - p[i].second);
}

optional<pair<int, int>> get_intersection(int i, int j) {
    bool h1 = (p[i].second == p[i + 1].second);
    bool h2 = (p[j].second == p[j + 1].second);

    if(h1 == h2) {
        return nullopt;
    }

    if(!h1) {
        swap(i, j);
    }

    int hx1 = min(p[i].first, p[i + 1].first);
    int hx2 = max(p[i].first, p[i + 1].first);
    int hy = p[i].second;

    int vx = p[j].first;
    int vy1 = min(p[j].second, p[j + 1].second);
    int vy2 = max(p[j].second, p[j + 1].second);

    if(vx >= hx1 && vx <= hx2 && hy >= vy1 && hy <= vy2) {
        return make_pair(vx, hy);
    }

    return nullopt;
}

int64_t pos_on_track(int i, int px, int py) {
    return prefix_len[i] + abs(px - p[i].first) + abs(py - p[i].second);
}

void read() {
    cin >> n;
    p.resize(n);
    for(int i = 0; i < n; i++) {
        cin >> p[i].first >> p[i].second;
    }
}

void solve() {
    // The train occupies a contiguous portion of the track. As it moves,
    // different parts of the track are covered, meaning that the train
    // self-intersects when two different positions along the track map to the
    // same point P (or there is a cycle). We will anyways pass through every
    // cycle, meaning that we are bounded by the "shortest one". This means we
    // can simply try all possible pairs of segments, and if they intersect we
    // can directly figure out the length of the cycle: it's the length(start,
    // B) - length(start, A). Note that this might not be a simple cycle, but if
    // that's the case there is certainly a smaller one which we would consider
    // in this algorithm too. The complexity of this algorithm is O(N^2) which
    // is sufficient for the problem here.

    if(n <= 1) {
        cout << 0 << endl;
        return;
    }

    int m = n - 1;
    prefix_len.resize(m + 1);
    prefix_len[0] = 0;
    for(int i = 0; i < m; i++) {
        prefix_len[i + 1] = prefix_len[i] + seg_len(i);
    }

    int64_t ans = prefix_len[m];

    for(int i = 0; i < m; i++) {
        for(int j = i + 2; j < m; j++) {
            auto intersection = get_intersection(i, j);
            if(intersection) {
                auto [px, py] = *intersection;
                int64_t pos_i = pos_on_track(i, px, py);
                int64_t pos_j = pos_on_track(j, px, py);
                ans = min(ans, pos_j - pos_i);
            }
        }
    }

    cout << ans << endl;
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
