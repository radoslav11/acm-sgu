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
string s;

void read() { cin >> n >> s; }

int sg_match(int len) { return len >= 1 ? 1 : 0; }
int sg_diff(int len) { return 0; }
int sg_boundary(int len) { return len; }
int sg_free(int len) { return len % 2; }

void solve() {
    // This is a combinatorial game theory problem. We can use Sprague-Grundy
    // theorem: the game splits into independent segments of uncolored vertices,
    // and the overall SG value is the XOR of individual segment SG values.
    //
    // Each segment has boundary conditions based on neighboring colored
    // vertices:
    // - "match": both neighbors have the same color (e.g., 1...1 or 2...2).
    // - "diff": neighbors have different colors (e.g., 1...2 or 2...1).
    // - "boundary": one neighbor colored, other side is chain end (free)
    // - "free": both sides are chain ends (no colored neighbors)
    //
    // Computing SG values naively via mex is O(N^2), but we can spot the
    // pattern, as it's fairly similar:
    //
    // len | match | diff | boundary | free
    // ----|-------|------|----------|-----
    //   0 |     0 |    0 |        0 |    0
    //   1 |     1 |    0 |        1 |    1
    //   2 |     1 |    0 |        2 |    0
    //   3 |     1 |    0 |        3 |    1
    //   4 |     1 |    0 |        4 |    0
    //   5 |     1 |    0 |        5 |    1
    //   6 |     1 |    0 |        6 |    0
    //   7 |     1 |    0 |        7 |    1
    //   8 |     1 |    0 |        8 |    0
    //   9 |     1 |    0 |        9 |    1
    //  10 |     1 |    0 |       10 |    0
    //
    // So: sg_match(len)    = (len >= 1) ? 1 : 0
    //     sg_diff(len)     = 0
    //     sg_boundary(len) = len
    //     sg_free(len)     = len % 2

    int xor_sum = 0;
    int i = 0;

    while(i < n) {
        if(s[i] == '0') {
            int start = i;
            while(i < n && s[i] == '0') {
                i++;
            }
            int len = i - start;

            char left = (start > 0) ? s[start - 1] : 0;
            char right = (i < n) ? s[i] : 0;

            int sg;
            if(left == 0 && right == 0) {
                sg = sg_free(len);
            } else if(left == 0 || right == 0) {
                sg = sg_boundary(len);
            } else if(left == right) {
                sg = sg_match(len);
            } else {
                sg = sg_diff(len);
            }
            xor_sum ^= sg;
        } else {
            i++;
        }
    }

    cout << (xor_sum != 0 ? "FIRST" : "SECOND") << endl;
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
