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

struct building {
    int64_t l, r, h;
};

int n;
vector<int64_t> L, R, H;

void read() {
    cin >> n;
    L.assign(n, 0);
    R.assign(n, 0);
    H.assign(n, 0);
    for(int i = 0; i < n; i++) {
        cin >> L[i] >> R[i] >> H[i];
    }
}

void solve() {
    // Buildings are disjoint, so sort them by L (which also sorts them by R).
    // Each one has a left side and a right side that must be fully lit. A side
    // is fully lit exactly when its bottom corner (at height 0) is visible to
    // some lamp, because the rest of the vertical side sits above the bottom
    // and is at least as easy to see over the flat roofs in between.
    //
    // A lamp at (px, py) sees the left side of a building to its right unless a
    // roof in between cuts off the bottom. Sweeping the buildings rightward and
    // keeping the steepest "shadow" ray cast so far (the line grazing a
    // previous top-right corner) lets us mark, in one pass, every left side
    // whose bottom clears that shadow; a building at least as tall as the lamp
    // blocks everything further out and ends the sweep. Right sides are handled
    // by the mirror sweep going leftward, so a single lamp marks every side it
    // covers via two cross-product sweeps.
    //
    // The placement can be done greedily, left to right. For the left side of a
    // building the only useful vantage is the gap to its immediate left: if
    // that neighbour is shorter we light the side from the building's own
    // top-left corner, otherwise from the neighbour's top-right corner, which
    // looks straight across the empty gap.
    //
    // For an uncovered right side at (R[i], 0) the lamp must come from the
    // right, and we want it as high as possible relative to that bottom corner,
    // i.e. the future building k whose top-left corner (L[k], H[k]) makes the
    // steepest ray up from (R[i], 0). The steepest-corner choice is optimal by
    // an exchange argument. A lamp covers R[i] only if the straight segment
    // from (R[i], 0) to it grazes over every roof in between, so the segment's
    // slope must be at least the slope to each intervening top-left corner; the
    // binding constraint is exactly the steepest such corner, k. Any lamp that
    // covers R[i] therefore sits on or above the ray through (R[i], 0) and
    // (L[k], H[k]), which means k itself is the lowest, left-most, hence
    // highest angle that still sees R[i]. Among all valid lamps it has
    // the smallest slope to every later right side as well, so the set of right
    // sides any other valid lamp can reach is a subset of what k reaches:
    // committing to k never loses future coverage, and since we only ever place
    // a lamp when a side is genuinely uncovered (at most one per adjacent pair,
    // left side first), each placement is forced and maximally far-reaching,
    // which gives the minimum. Two zero-height sentinels, one just left of all
    // buildings and one just right of them, let the gap logic treat the two
    // ends uniformly: after sorting they are the first and last entries, so
    // every real side faces a defined neighbour and the rightmost right side
    // falls back to the left sentinel.

    vector<building> a(n);
    for(int i = 0; i < n; i++) {
        a[i] = {L[i], R[i], H[i]};
    }

    sort(a.begin(), a.end(), [](const building& x, const building& y) {
        return x.l < y.l;
    });

    int64_t left_x = a.front().l, right_x = a.back().r;
    a.insert(a.begin(), {left_x - 2, left_x - 1, 0});
    a.push_back({right_x + 1, right_x + 2, 0});

    int last = (int)a.size() - 1;

    vector<char> lit_left(last + 1, 0), lit_right(last + 1, 0);

    // p1 / q1 < p2 / q2, where each slope is a signed distance over a height.
    auto lt_slope = [](int64_t p1, int64_t q1, int64_t p2, int64_t q2) {
        return p1 * q2 < p2 * q1;
    };

    auto cover_left = [&](int64_t px, int64_t py) {
        int64_t bn = 0, bd = 1;
        for(int j = 0; j < last; j++) {
            if(a[j].r <= px) {
                continue;
            }
            if(!lt_slope(a[j].l - px, py, bn, bd)) {
                lit_left[j] = 1;
            }
            if(a[j].h >= py) {
                break;
            }
            if(lt_slope(bn, bd, a[j].r - px, py - a[j].h)) {
                bn = a[j].r - px;
                bd = py - a[j].h;
            }
        }
    };

    auto cover_right = [&](int64_t px, int64_t py) {
        int64_t bn = 0, bd = 1;
        for(int j = last - 1; j >= 1; j--) {
            if(a[j].l >= px) {
                continue;
            }
            if(!lt_slope(px - a[j].r, py, bn, bd)) {
                lit_right[j] = 1;
            }
            if(a[j].h >= py) {
                break;
            }
            if(lt_slope(bn, bd, px - a[j].l, py - a[j].h)) {
                bn = px - a[j].l;
                bd = py - a[j].h;
            }
        }
    };

    int ans = 0;
    auto place = [&](int64_t px, int64_t py) {
        ans++;
        cover_left(px, py);
        cover_right(px, py);
    };

    for(int i = 0; i < last; i++) {
        if(!lit_left[i + 1]) {
            if(a[i].h < a[i + 1].h) {
                place(a[i + 1].l, a[i + 1].h);
            } else {
                place(a[i].r, a[i].h);
            }

            continue;
        }

        if(!lit_right[i]) {
            int best = 0;
            int64_t bn = 1, bd = 0;
            for(int j = i + 1; j < last; j++) {
                if(!lt_slope(bn, bd, a[j].l - a[i].r, a[j].h)) {
                    best = j;
                    bn = a[j].l - a[i].r;
                    bd = a[j].h;
                }
            }

            place(a[best].l, a[best].h);
        }
    }

    cout << ans << '\n';
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T = 1;
    cin >> T;
    for(int test = 1; test <= T; test++) {
        read();
        solve();
    }

    return 0;
}
