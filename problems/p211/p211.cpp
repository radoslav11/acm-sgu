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
vector<int> queries;

void read() {
    cin >> n >> m;
    queries.resize(m);
    cin >> queries;
}

void solve() {
    // The key is to maintain a register state where adding 2^p never requires
    // long carry chains. Since each register holds {0,1,2} (a redundant base-2
    // representation), the same number has many encodings and we can pick one
    // that keeps carries local. A good approach is to find some invariant that
    // will allow something like this and keep it throughout the updates.
    //
    // A good example for invariant is the following: between any two 2s there
    // is always at least one 0. This guarantees that carries can be absorbed in
    // O(1) changes per addition. The only part is that we should also make sure
    // that the invariant is kept after this addition.
    //
    // For each operation of adding 2^p, let next_two = nearest index > p with
    // reg = 2, and prev_zero = rightmost index < next_two with reg = 0. We
    // consider three cases:
    //
    //     1) p >= prev_zero (no zero between p and next_two):
    //
    //            Change: reg[p]++, reg[next_two] -= 2, reg[next_two+1]++.
    //            Net: +2^p - 2^{next_two+1} + 2^{next_two+1} = +2^p.
    //            3 changes.
    //
    //        The invariant ensures reg[p] < 2 (otherwise p and next_two would
    //        be two 2s with no 0 between them) and reg[next_two+1] < 2
    //        (adjacent to a 2), so both +1s produce <= 2. Invariant is
    //        preserved: if reg[p] becomes 2, reg[next_two] becoming 0 is the
    //        separator above. If reg[next_two+1] becomes 2 it was 1, meaning
    //        next_two was the first 2 above it, so a 0 already existed between
    //        next_two+1 and the next 2 further up. Nothing up there changed.
    //
    //   2) reg[p] != 0 (there is a zero between p and next_two):
    //
    //          Change: reg[p]--, reg[p+1]++.
    //          Net: -2^p + 2^{p+1} = +2^p.
    //          2 changes.
    //
    //      reg[p+1]++ can create a 2 at most, but prev_zero lies between p
    //      and next_two, so a 0 separator already exists nearby.
    //
    //   3) reg[p] == 0:
    //
    //          Change: reg[p]++.
    //          Net: +2^p.
    //          1 change.
    //
    //      No new 2 is created. The 0 at p can't be the sole separator between
    //      two 2s: if reg[p] == 0 then prev_zero >= p, but reaching case 3
    //      requires skipping case 1, which means either next_two > n (no 2
    //      above) or prev_zero > p (another 0 above p still separates).

    vector<int> reg(n + 2, 0);
    set<int> pos_for_val[3];
    for(int i = 0; i <= n; i++) {
        pos_for_val[0].insert(i);
    }
    pos_for_val[2].insert(n + 1);

    auto apply = [&](int pos, int delta) {
        pos_for_val[reg[pos]].erase(pos);
        reg[pos] += delta;
        pos_for_val[reg[pos]].insert(pos);
        cout << ' ' << pos << ' ' << reg[pos];
    };

    for(int p: queries) {
        int next_two = *pos_for_val[2].upper_bound(p);
        int prev_zero = *prev(pos_for_val[0].lower_bound(next_two));

        if(next_two <= n && p >= prev_zero) {
            cout << 3;
            apply(p, 1);
            apply(next_two, -2);
            apply(next_two + 1, 1);
        } else if(reg[p] != 0) {
            cout << 2;
            apply(p, -1);
            apply(p + 1, 1);
        } else {
            cout << 1;
            apply(p, 1);
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
        solve();
    }

    return 0;
}
