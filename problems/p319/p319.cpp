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

int n, w, h;
vector<array<int, 4>> rects;

void read() {
    cin >> n >> w >> h;
    rects.resize(n + 1);
    rects[0] = {0, 0, w, h};
    for(int i = 1; i <= n; i++) {
        auto& r = rects[i];
        cin >> r[0] >> r[1] >> r[2] >> r[3];
        if(r[0] > r[2]) {
            swap(r[0], r[2]);
        }
        if(r[1] > r[3]) {
            swap(r[1], r[3]);
        }
    }
}

void solve() {
    // The problem has a slightly convoluted problem statement, but essentially
    // says that we have a large canvas with n rectangles that don't intersect
    // but can cover each other, and asks us to find the "empty" areas. The
    // structure of the rectangles is essentially a tree, so if we treat the
    // canvas as a separate rectangle with coordinates (0,0) and (W,H), we can
    // simply extract area(u) - SUM_{c child of u} area(c). The only question
    // left is how to build this tree. There is a fairly straight forward way
    // with a sweep-line approach:
    //
    // We sweep a vertical line left to right. For each rectangle we create 3
    // events sorted by x: two "open" events at x1 (for y1 and y3) and one
    // "close" event at x2. We maintain a map from y-coordinate to rectangle-id
    // representing which rectangle "owns" each y-interval at the current sweep
    // position. The canvas boundaries y=0 and y=H map to rectangle 0.
    //
    // When we open rectangle v (hit x1), we first query the map for the entry
    // just below y1 — that rectangle is v's parent. Then we insert y1->v and
    // y3->v into the map. When we close rectangle v (hit x2), we remove both
    // entries. Since rectangles don't touch or overlap, this correctly finds
    // the immediate enclosing rectangle for each frame.

    struct event {
        int x, y, id;
        bool operator<(const event& o) const {
            return x < o.x || (x == o.x && y < o.y);
        }
    };

    vector<event> events;
    for(int i = 1; i <= n; i++) {
        events.push_back({rects[i][0], rects[i][1], i});
        events.push_back({rects[i][0], rects[i][3], i});
        events.push_back({rects[i][2], 0, i});
    }
    sort(events.begin(), events.end());

    map<int, int> active;
    active[0] = 0;
    active[h] = 0;

    vector<int> parent(n + 1, 0);

    for(int i = 0; i < (int)events.size(); i++) {
        int x = events[i].x, y = events[i].y, v = events[i].id;
        if(x == rects[v][0]) {
            if(y == rects[v][1]) {
                auto it = active.lower_bound(y);
                --it;
                int below = it->second;
                if(rects[below][3] == it->first) {
                    parent[v] = parent[below];
                } else {
                    parent[v] = below;
                }
            }
            active[y] = v;
        } else {
            active.erase(rects[v][1]);
            active.erase(rects[v][3]);
        }
    }

    vector<int64_t> area(n + 1, 0);
    area[0] = (int64_t)w * h;
    for(int i = 1; i <= n; i++) {
        int64_t a =
            (int64_t)(rects[i][2] - rects[i][0]) * (rects[i][3] - rects[i][1]);
        area[i] += a;
        area[parent[i]] -= a;
    }

    sort(area.begin(), area.end());
    for(int i = 0; i <= n; i++) {
        cout << area[i];
        if(i < n) {
            cout << ' ';
        }
    }
    cout << '\n';
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
