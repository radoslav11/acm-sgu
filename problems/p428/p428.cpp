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

void solve() {
    // The hard part in the problem is the uniqueness of the rebus solution.
    // However, this can be done by forcing one of the two numbers to be unique.
    // Let's take AA..AA, and assume that A will be mapped to 1. Then to
    // guarantee uniqueness, we also need to make sure the digits to letters are
    // not permutable. This can be done by making an ordering. In particular, we
    // want to use at least 9-1=8 different letters and add a condition of the
    // type A < B. This can be forced by having some digits where l[i] = A, r[i]
    // = letter, and result[i] = next(letter). We have to be careful about
    // buffers, so to guarantee this we will make sure l[i-1] = r[i-1] = A, and
    // result[i-1] = B. To make this concrete, let's take A-I. A corresponds to
    // 1, B to 2, C to 3 and so on until I to 9. We now need to place the 8
    // constraints. We can essentially create a string of length 8*2=16, where
    // odd positions are enforcing the no carry requirement, while even will
    // enforce the unique order of the letters. Then it's enough to just permute
    // the order of constraints.

    string perm = "ABCDEFGH";

    for(int steps = 0; steps < 1000; steps++) {
        next_permutation(perm.begin(), perm.end());

        string l, r, result;
        for(int i = 0; i < 8; i++) {
            l.push_back('A');
            l.push_back('A');
            r.push_back('A');
            r.push_back(perm[i]);
            result.push_back('B');
            result.push_back(perm[i] + 1);
        }
        cout << l << "+" << r << "=" << result << endl;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T = 1;
    // cin >> T;
    for(int test = 1; test <= T; test++) {
        // cout << "Case #" << test << ": ";
        solve();
    }

    return 0;
}
