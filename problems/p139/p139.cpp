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

vector<int> a;

void read() {
    a.resize(16);
    cin >> a;
}

int inversions(vector<int>& arr) {
    int count = 0;
    for(int i = 0; i < arr.size(); i++) {
        for(int j = i + 1; j < arr.size(); j++) {
            if(arr[i] > arr[j]) {
                count++;
            }
        }
    }
    return count;
}

void solve() {
    // The standard solution would be to do A* or some other search algorithm,
    // which is sufficiently fast for 4x4, but interestingly Johnson & Story in
    // 1879 proved that there are only two equivalence classes in the 15 puzzle
    // - essentially based on the parity of the permutation achieved by the rows
    // of the table. This makes the problem easier to solve, as we are only
    // interested in YES/NO rather than least number of steps.
    // More can be found in these two sources:
    // - https://www.jstor.org/stable/2369492?origin=crossref&seq=1
    // -
    // https://www.cs.cmu.edu/afs/cs/academic/class/15859-f01/www/notes/15-puzzle.pdf
    //
    // The sources describe one way of showing the fact there are two
    // equivalence classes, but there is also arguably a simpler explanation.
    // Let's consider the permutation given by replacing 0 with 16, and going
    // row by row, each row starting from the left. This means that the original
    // grid's permutation is the identity and so even. Let's say we made some
    // operations to the grid and now 16 is on position i. The operations we can
    // perform are swap i with i-1,i+1, i+n, and i-n. Let's consider how the
    // number of inversions changes - there are effectively 2 x 2 symmetric
    // cases:
    //
    //    1) We swap i with i-1 or i+1. Trivially i is the largest element, so
    //       the number of inversions will change with either +1 or -1. In both
    //       cases with 1 mod 2.
    //
    //    2) We swap i with i-n or i+n. This case is slightly more
    //       complicated as we have n-1 elements between the two we are swapping and 
    //       j =def= i-n or i+n is not the "largest element" to make the number
    //       of inversions predictable. WLOG, we will assume that j = i-n, and that
    //       in p[j+1:i] there are exactly k elements that are less than p[j]. This 
    //       means there are n-1-k elements greater than p[j]. The inversions with p[j]
    //       as part of them will change with exactly n-2k-1. However, p[i] = 16 will also
    //       now contribute to more inversions - n to be precise. This means that overall
    //       the inversions change by 2n-2k-1, which mod 2 actually also ends up being 1.
    //
    // Therefore, we showed that the parity of the inversions (permutation) changes every
    // time we move the 16. This gives us the invariant that the parity of number of moves 
    // of 16 is always the same as the parity of the permutation as this is the case in the 
    // initial permutation, or it's enough to check that:
    //     
    //    manhattan_distance((n-1,n-1), (i / 4, i % 4)) = parity(p) mod 2

    int sum = 0;
    for(int i = 0; i < 16; i++) {
        if(a[i] == 0) {
            sum = (3 - i / 4) + (3 - i % 4);
            a[i] = 16;
            break;
        }
    }

    sum += inversions(a);

    cout << (sum % 2 == 0 ? "YES" : "NO") << '\n';
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
