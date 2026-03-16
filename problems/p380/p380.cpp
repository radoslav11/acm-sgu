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
vector<int> h;

void read() {
    cin >> n;
    h.resize(n);
    cin >> h;
}

void solve() {
    // We could try to think of constructions, or quadratic DP solutions, but
    // actually it's always possible! This is known as the mountain climbing
    // problem, and it's true for any continuous function with a finite number
    // of "peaks". The wiki page below has a good proof, which roughly goes as
    // follows: Consider the graph with nodes represented by the two coordinates
    // of the climbers (x1, x2). We start from (1, n), and want to get to some
    // (x, x). We will show something stronger - (n, 1) is reachable from (1,
    // n), meaning that the climbers will switch their place and this implies
    // there is some (x, x) where they meet. Also note that we can keep
    // everything discretely, as it's enough to only keep a finite number of
    // vertices where either x1 or x2 is a peak (local minimum/maximum). Now
    // let's look at the degree of every vertex. Trivially, (1, n) and (n, 1)
    // have degree one as we can't exit the mountain. The core observation is
    // that all other vertices will have an even degree (we can go through the
    // cases), meaning that the only two vertices with odd degree are (1, n) and
    // (n, 1). However, a connected component has to have an even number of odd
    // degree vertices, meaning that (1, n) and (n, 1) are in the same connected
    // component. Therefore, we have showed that for any landscape the answer is
    // YES. The alternative approaches that simulate the climbing can actually
    // lead to quadratic complexity while finding the construction, so here we
    // will simply print YES.
    //
    // Unfortunately, after trying this you get WA2. This is because heights
    // can be negative, which means the mountain can dip below the starting
    // level. In the classical mountain climbing problem all heights are
    // non-negative, so (1, n) and (n, 1) are the only degree-1 vertices. With
    // negative heights, additional degree-1 vertices can appear, breaking the
    // parity argument. The correct approach is to actually try moving the two
    // climbers. Let's keep l and r, denoting the rightmost position of the lest
    // one, and the leftmost position of the right one. We will also maintain
    // the set of heights they can occupy denoted by low and high. Trivially, we
    // start from (l=1, r=n, low=0, high=0). The key observation is that when we
    // backtrack, we can reach i < l, or j > r, at any of the heights between
    // low and high. This means we only have the following cases:
    //
    //     1) Safe advance (no range change needed). If the next left vertex
    //       h[l+1] is inside [low, high], the left alpinist can move forward
    //       (right can pause/adjust to match height). There is an equivalent
    //       case for right alpinist moving.
    //
    //     2) Both must climb higher h[l+1] > high and h[r-1] > high. Then we
    //        can just get them closer together, to the lower of the two heights
    //        and update high (we move only one of the alpinist).
    //
    //     3) Both must climb lower h[l+1] < low and h[r-1] < low. Then we
    //        can just get them closer together, to the higher of the two
    //        heights and update low (we move only one of the alpinist).
    //
    //     4) In any other case, the answer is NO.
    //
    // If we eventually get to l >= r, we can terminate.

    int l = 0, r = n - 1;
    int low = 0, high = 0;
    while(l < r) {
        if(h[l + 1] >= low && h[l + 1] <= high) {
            l++;
        } else if(h[r - 1] >= low && h[r - 1] <= high) {
            r--;
        } else if(h[l + 1] > high && h[r - 1] > high) {
            if(h[l + 1] < h[r - 1]) {
                high = h[l + 1];
                l++;
            } else {
                high = h[r - 1];
                r--;
            }
        } else if(h[l + 1] < low && h[r - 1] < low) {
            if(h[l + 1] > h[r - 1]) {
                low = h[l + 1];
                l++;
            } else {
                low = h[r - 1];
                r--;
            }
        } else {
            cout << "NO" << endl;
            return;
        }
    }

    cout << "YES" << endl;
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
