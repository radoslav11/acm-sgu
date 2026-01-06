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
int64_t L, T;
vector<int64_t> p, c, d;

void read() {
    cin >> n >> L;
    p.resize(n);
    c.resize(n);
    d.resize(n);
    for(int i = 0; i < n; i++) {
        cin >> p[i] >> c[i] >> d[i];
    }
    cin >> T;
}

void solve() {
    // The solution uses a semi-popular trick about problems where there are
    // moving people / objects and they bump each other which results in them
    // changing directions. Conceptually the trick is simple - instead of them
    // changing directions, we can actually assume they simply pass each other
    // and instead possibly swap some attributes. This way, we can think of the
    // process as a set of N linear functions with slopes 1 or -1 (cw and ccw).
    // The colors are also simple to deduce at each time T as none of the d=1
    // lines ever change their color, while the d=-1 lines' colors are always
    // based on the last d=1 they intersect, meaning we can binary search to
    // find this.
    //
    // Something we haven't mentioned yet is that we actually have a circle and
    // not an infinite line in terms of the intersections. To solve this, we can
    // model each chameleon as an infinite number of lines. Say the initial
    // location was y. We then will have copies of the line at y+c*L for any c
    // \in Z. This way at any T, we can only look at the lines that fall into y
    // \in [0, L) - we can prove easily that there are exactly N of them.
    //
    // We should be a bit careful about the order of the chameleons - when we go
    // to the concept of lines, we assume that they cross each other, while in
    // reality they turn around. We need this as we ultimately want to
    // reconstruct the answer. The core observation is that when the chameleons
    // bump, they never change their relative order. However, we still need to
    // find where at least one of these chameleons is. Let's look at the y=0
    // line, and the d=1 and d=-1 intersections with it. If the first
    // intersection is with d=-1, then the N-th chameleon crossed and became the
    // first. Then if the next intersection was with d=1, it means that that
    // same chameleon (note relative order is always the same so we know it's
    // that particular chameleon) crossed back and became the N-th again. This
    // argument can be further generalized, and we can see that each d=1
    // corresponds to right cyclic shift, and each d=-1 corresponds to a left
    // one. Therefore, for every line with binary search or simple math we can
    // figure out the number of c values (clones) that will intersect with y=0
    // until time T. Then we can sum the contributions based on d=1 and d=-1 and
    // get the final "cyclic" shift of the ordering.
    //
    // The above is enough to craft some solution running in O(N log N), but we
    // will now describe how to implement it cleanly.  For simplicity, let's
    // first sort all chameleons by their initial positions and we will remap
    // when we output. We have the invariant that every chameleon appears at
    // y+c*L is kept for any time T, so we know that we can simply get the set
    // of final positions as (p+T*d) mod L. The relative order is kept, so we
    // only want to find which of them is the first. Here comes the second
    // observation about intersections with y=0, and cyclic shifts. We should
    // be a bit careful about points with p=0 when we start.

    auto mod = [](int64_t x, int64_t m) { return ((x % m) + m) % m; };

    vector<int> order(n);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [](int i, int j) { return p[i] < p[j]; });

    vector<int64_t> sorted_p(n), sorted_c(n), sorted_d(n);
    for(int i = 0; i < n; i++) {
        sorted_p[i] = p[order[i]];
        sorted_c[i] = c[order[i]];
        sorted_d[i] = d[order[i]];
    }

    vector<pair<int64_t, int>> cw_info;
    for(int i = 0; i < n; i++) {
        if(sorted_d[i] == 1) {
            int64_t final_pos = mod(sorted_p[i] + T, L);
            cw_info.push_back({final_pos, i});
        }
    }
    sort(cw_info.begin(), cw_info.end());

    vector<int64_t> cw_final;
    for(int i = 0; i < (int)cw_info.size(); i++) {
        cw_final.push_back(cw_info[i].first);
    }

    int64_t shift = 0;
    for(int i = 0; i < n; i++) {
        if(sorted_d[i] == 1) {
            shift += (sorted_p[i] + T) / L;
        } else {
            if(sorted_p[i] == 0) {
                shift -= (L + T - 1) / L;
            } else {
                shift -= (L - sorted_p[i] + T - 1) / L;
            }
        }
    }
    shift = mod(shift, n);

    vector<double> slot_pos(n);
    vector<int64_t> slot_dir(n);
    vector<int64_t> slot_color(n);

    for(int i = 0; i < n; i++) {
        slot_pos[i] = mod(sorted_p[i] + T * sorted_d[i], L);
        slot_dir[i] = sorted_d[i];

        if(sorted_d[i] == 1) {
            slot_color[i] = sorted_c[i];
        } else {
            if(cw_info.empty()) {
                slot_color[i] = sorted_c[i];
            } else {
                int64_t q = sorted_p[i];
                int64_t ccw_final_pos = slot_pos[i];
                auto it = lower_bound(
                    cw_info.begin(), cw_info.end(),
                    make_pair(ccw_final_pos + 1, -1)
                );
                if(it == cw_info.end()) {
                    it = cw_info.begin();
                }
                int64_t p = sorted_p[it->second];
                int64_t t0 = (q >= p) ? (q - p) / 2 : (q + L - p) / 2;
                if(t0 > T) {
                    slot_color[i] = sorted_c[i];
                } else {
                    slot_color[i] = sorted_c[it->second];
                }
            }
        }
    }

    vector<int> slot_order(n);
    iota(slot_order.begin(), slot_order.end(), 0);
    sort(slot_order.begin(), slot_order.end(), [&](int i, int j) {
        return slot_pos[i] < slot_pos[j];
    });

    vector<double> final_pos(n);
    vector<int64_t> final_color(n), final_dir(n);

    for(int i = 0; i < n; i++) {
        int slot = slot_order[mod(i + shift, n)];
        final_pos[i] = slot_pos[slot];
        final_dir[i] = slot_dir[slot];
        final_color[i] = slot_color[slot];
    }

    vector<int> inv_order(n);
    for(int i = 0; i < n; i++) {
        inv_order[order[i]] = i;
    }

    cout << fixed << setprecision(3);
    for(int i = 0; i < n; i++) {
        int idx = inv_order[i];
        cout << final_pos[idx] << " " << final_color[idx] << " "
             << final_dir[idx] << "\n";
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
