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
vector<int64_t> a;

void read() {
    cin >> n;
    a.resize(n);
    cin >> a;
}

void solve() {
    // The problem asks for the least number of operations of either multiplying
    // by 2, or subtracting 1 from everything, required to make everything zero.
    // First we can notice that we need at least M = max(a) steps of
    // subtracting 1. We can show this is enough. Particularly, say we look at
    // some M/2 < x < M. Then with a single double, we can reach 2x >= M, and
    // there is no point in doubling further. But if we first subtract and then
    // double, we get to 2(x-1), while at that M := M - 1. What's more relevant
    // is the difference between M and x. Subtracting doesn't change it, while
    // doubling will overshoot it into 2(x-s)-(M-s) = 2x-M-s. Ideally we
    // shouldn't overshoot M, as that would create a new maximum. Therefore, we
    // can try to solve for s, or s = 2x-M. One constraint we have is that x >=
    // s >= 0. In other words we might have to first double until we reach a
    // point where we can actually match with M. After that point, we will find
    // s, wait for some subtraction steps (=s), double, and now have the two
    // positions will be equal. The above construction already performs M
    // subtracting steps, but we now need to make sure that doubling is also
    // optimal. This is not hard to notice, as the above approach will do
    // ~log(M/x) doubling steps, and this is the least we need, because we will
    // perform M subtracting steps anyways. We shouldn't forget that the problem
    // also asks for the actual construction if the answer is small enough.

    int64_t M = *max_element(a.begin(), a.end());
    int64_t ans = M;

    vector<vector<int>> ops(min(M, 1024ll));
    vector<int> pre_ops;
    for(int i = 0; i < n; i++) {
        int64_t x = a[i];
        while(x < M) {
            int64_t s = 2 * x - M;
            if(s >= 0 && s <= x) {
                ans++;
                if(0 <= s && s <= 1000) {
                    ops[s].push_back(i + 1);
                }
                break;
            }

            pre_ops.push_back(i + 1);
            ans++;
            x *= 2;
        }
    }

    cout << ans << endl;
    if(ans <= 1000) {
        for(int pos: pre_ops) {
            cout << "science mission to the planet " << pos << endl;
        }
        for(int i = 0; i < M; ++i) {
            for(int pos: ops[i]) {
                cout << "science mission to the planet " << pos << endl;
            }
            cout << "flying mission" << endl;
        }
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
