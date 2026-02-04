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

int64_t l, v1, v2, t;

void read() { cin >> l >> v1 >> v2 >> t; }

void solve() {
    // One way to make this problem easier to argue about is to consider the
    // position of each person as moving along an "unfolded" line, where instead
    // of turning around at walls, we imagine the road being mirrored
    // infinitely. Each person's position can be described by a phase:
    //
    //     phase_c = v1*t for the cyclist,
    //     phase_p = v2*t for the pedestrian.
    //
    // The real position is recovered by "folding" the phase back into [0, l]:
    //
    //   - Cyclist starts at 0 moving right:
    //         real_pos = phase_c mod 2l if <= l,
    //     else:
    //         2l - (phase_c mod 2l).
    //
    //   - Pedestrian starts at l moving left:
    //         real_pos = l - (phase_p mod 2l) if phase_p mod 2l <= l
    //     else
    //         (phase_p mod 2l) - l.
    //
    // They meet when their real positions coincide. Analyzing all cases of
    // which "segment" each is in (moving left or right), we get two types
    // of meetings:
    //
    // 1. Approaching each other: (v1 + v2) * t = l (mod 2l)
    //    Meeting times: t = l*(2k+1)/(v1+v2) for k = 0, 1, 2, ...
    //
    // 2. One catching up to the other: |v1 - v2| * t = l (mod 2l)
    //    Meeting times: t = l*(2k+1)/|v1-v2| for k = 0, 1, 2, ...
    //    (Only exists if v1 != v2)
    //
    // Count of type 1 in (0, T]:
    //     number of k >= 0 with l*(2k+1) <= T*(v1+v2),
    // which equals (T*(v1+v2) + l) / (2*l) using integer division.
    // Similarly for type 2 with |v1-v2|.
    //
    // Overlap: a time t is counted in both if (2k+1)*(v1-v2) =
    // (2m+1)*(v1+v2) for some k, m >= 0. We can use observations from extended
    // Euclid. Let g = gcd(v1+v2, |v1-v2|), s' = (v1+v2)/g, d' = |v1-v2|/g.
    // Since gcd(s', d') = 1, we need s' | (2k+1) and d' | (2m+1). For these to
    // be odd (as required), both s' and d' must be odd. If so, overlap times
    // are t = l*(2k+1)*s'/(v1+v2) = l*(2k+1)/g for odd (2k+1) that are
    // multiples of s', i.e., t = l*n/g for odd n = 1, 3, 5, ... The count is
    // (T*g + l) / (2*l).
    //
    // Then the final answer is count1 + count2 - overlap.

    int64_t s = v1 + v2;
    int64_t d = abs(v1 - v2);

    int64_t count1 = (t * s + l) / (2 * l);
    int64_t count2 = (d > 0) ? (t * d + l) / (2 * l) : 0;

    int64_t overlap = 0;
    if(d > 0) {
        int64_t g = gcd(s, d);
        if((s / g) % 2 == 1 && (d / g) % 2 == 1) {
            overlap = (t * g + l) / (2 * l);
        }
    }

    cout << count1 + count2 - overlap << '\n';
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
