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

int64_t x, y, z;

bool read() { return bool(cin >> x >> y >> z); }

void solve() {
    // We must walk every passageway of the X*Y*Z lattice graph at least once
    // and may start and finish at any two cabins, which is the open route
    // inspection (Chinese postman) problem. The answer is E, the number of
    // passageways, plus the fewest edges we are forced to repeat.
    //
    // A connected graph can be covered by a single walk that uses every edge
    // exactly once precisely when it has zero or two odd-degree vertices, and
    // when it has two they must be the walk's start and finish. We are allowed
    // to repeat edges, so the plan is to repeat just enough of them to drive
    // the count of odd-degree cabins down to two. Repeating every edge along a
    // path from u to v changes the degree parity only at the two ends u and v,
    // every interior vertex of the path gains two and stays the same parity. So
    // we can cancel the odd cabins by pairing them up and, for each pair,
    // repeating a shortest path between the two. The box is solid, so a
    // shortest path is just the Manhattan distance. Two odd cabins may be left
    // unpaired to serve as the endpoints, so the number of repeated edges we
    // add is the minimum weight perfect matching of the odd cabins with two of
    // them dropped.
    //
    // Which cabins are odd? Along an axis of size > 1, a coordinate at an end
    // (0 or size-1) has one neighbour on that axis while an interior coordinate
    // has two. A cabin's degree is therefore odd exactly when an odd number of
    // its coordinates are at an end: either one of them, which is a cabin
    // strictly inside one of the six faces, or all three, which is one of the
    // eight corners. A cabin with two end-coordinates lies on an edge of the
    // box and has even degree.
    //
    // The cabins strictly inside a single face all share the same fixed
    // end-coordinate and range over the interior values of the other two axes,
    // so they form a (p-2) x (q-2) grid in which lattice neighbours are one
    // step apart. We tile that grid with dominoes and repeat each domino's
    // single edge. The two faces of each orientation contribute (p-2)(q-2)
    // repeats, so summing the three orientations (a <= b <= c) gives D =
    // (a-2)(b-2) + (b-2)(c-2) + (a-2)(c-2). This is the bulk of the answer and
    // the part that scales with the box.
    //
    // The cabins a domino tiling can never place are the eight corners. A
    // corner's three neighbours all sit on box edges (two end-coordinates) and
    // have even degree, so a corner has no adjacent odd cabin to pair with. An
    // open walk gets two free odd-degree vertices though, namely its start and
    // finish, so we let two of the eight corners stay odd and use them as the
    // endpoints. That leaves 8 - 2 = 6 corners that still need repairing, which
    // pair up into 3 detours.
    //
    // Repairing a corner interacts with the tiling, which is the subtle part.
    // Picture one face whose interior is a 4 x 4 block: 16 cells, 8 dominoes,
    // no strays, with the face's 4 corners each sitting one diagonal step
    // (distance 2) off the block. Pairing those corners with one another is
    // costly because they are far apart. The cheaper move is to send each
    // corner 2 steps into the nearest block cell. That steals the 4
    // block-corner cells, yet the punctured 12-cell block still tiles, just
    // with 2 fewer dominoes. The four repairs therefore cost 4 x 2 for the
    // detours, offset by the 2 dominoes we no longer lay, for a net of only 6
    // above the bulk tiling, well below the 10 that pairing the corners
    // directly would run. The a-cases below are this same trade played out at
    // the scale of the real box.
    //
    // The cost of the three detours is a small constant set by the thinnest
    // dimension a and a couple of parities. The regimes below were worked out
    // by hand and then checked against a brute-force matching:
    //
    // - a = 2: the box is two cabins thick, so two corners on the same
    //          thin-axis edge are one step apart. Pair the six corners into
    //          three such adjacent pairs, one repeated edge each, for a total
    //          of 3.
    //
    // - a = 3: the box is three thick, so two corners now sit two steps apart
    //          along the thin axis. With b and c both even the six corners pair
    //          off that way for a total of 6. An odd b or c makes some faces
    //          have odd interior area, each leaving one stray cell that has to
    //          be picked up too, which raises the total to 7 for one odd
    //          dimension and to 9 for two.
    //
    // - a >= 4: corners are now too far apart to pair cheaply with one another,
    //           so each repaired corner instead steps one cabin into the
    //           nearest face, a detour of length 2. Repairing the six corners
    //           this way, with the odd faces' stray cells absorbed into those
    //           same detours, always nets out to 9 no matter how large the box
    //           grows.
    //
    // - a = 1: a flat sheet has no corners, and the pairing reduces to the
    //          planar value b + c - 5, plus one when b and c are both odd. A 1
    //          x 1 x c stick is a single path and needs no repeats at all.

    array<int64_t, 3> d = {x, y, z};
    sort(d.begin(), d.end());
    auto [a, b, c] = d;

    int64_t edges = (x - 1) * y * z + x * (y - 1) * z + x * y * (z - 1);

    int64_t extra;
    if(a == 1 && b == 1) {
        extra = 0;
    } else if(a == 1) {
        extra = max<int64_t>(0, b + c - 5 + (b % 2 == 1 && c % 2 == 1));
    } else {
        int64_t base =
            (a - 2) * (b - 2) + (b - 2) * (c - 2) + (a - 2) * (c - 2);
        int64_t correction;
        if(a == 2) {
            correction = 3;
        } else if(a == 3) {
            correction = array<int64_t, 3>{6, 7, 9}[b % 2 + c % 2];
        } else {
            correction = 9;
        }

        extra = base + correction;
    }

    cout << edges + extra << '\n';
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    for(int test = 1; read(); test++) {
        cout << "Case #" << test << ": ";
        solve();
    }

    return 0;
}
