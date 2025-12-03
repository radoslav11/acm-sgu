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

int64_t A, B;

void read() { cin >> A >> B; }

vector<int64_t> solve_even(int64_t A, int64_t B) {
    assert(A % 2 == 0 && B % 2 == 0);

    int64_t pw2 = 2;
    vector<int64_t> steps;
    while(A < B) {
        while(A + 2 * pw2 <= B && 2 * pw2 < A && A % (2 * pw2) == 0) {
            pw2 *= 2;
        }

        while(A + pw2 > B) {
            pw2 /= 2;
        }

        steps.push_back(A);
        A += pw2;
    }

    steps.push_back(B);
    return steps;
}

int64_t min_divisor(int64_t x) {
    for(int64_t d = 2; d * d <= x; d++) {
        if(x % d == 0) {
            return d;
        }
    }

    return -1;
}

void solve() {
    // The first thing we notice is that we don't want the shortest solution,
    // but just any with <= 500 steps. The constraints for A and B are fairly
    // large at 10^12, so it's unlikely that we can get something directly based
    // on the divisors so that it fits within 500 steps. A common idea in
    // constructive problems like this is to explore powers of 2 and then
    // achieve something that is logarithmic. In particular, let's consider the
    // case where both A < B are even. As A and B are both even, there will be
    // some K >= 1, such that A mod 2^K = 0 and A + 2^K <= B (and 2^K < A due to
    // 1 < d < A). Let us choose the largest such K and make the operation A +=
    // 2^K. There are two cases:
    //
    //     1) The constraint was that K was the largest power of 2. If that is
    //        the case, we know that A mod 2^(K+1) = 2^K. Then after the
    //        operation, A mod 2^(K+1) = 0. This means on the next step we would
    //        be able to choose K + 1.
    //
    //     2) The second constraint was bounding or A+2^K <= B. If this is the
    //        case, it's not hard to observe that we will be done in < K steps
    //        using the same procedure. This is because the difference B-K is
    //        even and the operation represents removing the largest bit from
    //        the binary notation. Note that even after adding 2^K, the
    //        divisibility by all K' < K is still kept which allows us to
    //        perform this algorithm if we always choose the largest K.
    //
    // We can have < log(B) times that we hit case 1, followed by again less
    // than log(B) times of case 2. This is logarithmic and within the 500
    // bound. Hence, we now have a solution for the case of both A and B being
    // even.
    //
    // What about the case when A or B is odd? In this case, all divisors of
    // both A and B are odd. We can notice that we can always get from all even
    // numbers to all other even numbers (apart from corner case of A = 2), so
    // it might make sense to make both A and B even. To do this, let's denote
    // by min_divisor(x) the smallest number d such that x mod d = 0 and 1 < d <
    // x. If A is odd, we will have the first operation be A += min_divisor(A).
    // If B is odd, we will have the last operation be A += min_divisor(B).
    // Clearly, it would be impossible if adjusted_A > adjusted_B, where
    // adjusted_A = A + min_divisor(A), and adjusted_B = B - min_divisor(B).
    //
    // Computing min_divisor(x) can be trivially done in O(sqrt(x)), while
    // the rest of the algorithm has O(log(B)) steps, where each step of finding
    // K can be trivially implemented in O(log(B)) too, although if we reuse the
    // last steps value of K as a start and move it as a pointer, the actual
    // amortized complexity will be just O(log(B)). Either way, this is
    // dominated by the min_divisor(x) computation.

    int64_t d1 = min_divisor(A), dk = min_divisor(B);
    int64_t adjusted_A = A, adjusted_B = B;
    if(A % 2 == 1) {
        adjusted_A = A + d1;
    }
    if(B % 2 == 1) {
        adjusted_B = B - dk;
    }
    if(d1 == -1 || dk == -1 || adjusted_A > adjusted_B) {
        cout << "Impossible" << endl;
        return;
    }

    vector<int64_t> ans = solve_even(adjusted_A, adjusted_B);
    if(adjusted_A != A) {
        cout << A << endl;
    }
    for(int64_t x: ans) {
        cout << x << endl;
    }
    if(adjusted_B != B) {
        cout << B << endl;
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
