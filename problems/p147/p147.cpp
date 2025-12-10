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
pair<int, int> a, b, c;

void read() {
    cin >> n;
    cin >> a >> b >> c;
}

void solve() {
    // The solution to this problem is case work. Probably the hardest part is
    // to actually parse what it asks for. Essentially, we want to check if the
    // "black-white" king can intersect the path of the two other kings. The two
    // other kings will always take max(|x1-x2|, |y1-y2|) steps to meet (time is
    // divided by 2 because they make two steps for a single step of the
    // "black-white"). As N isn't large, we can iterate through the steps until
    // we meet. At step T, we can figure out the range of y coordinates that are
    // reachable by the two kings, and see if the "black-white" can be on any of
    // these cells. This solution is in O(N).

    if(a == c || b == c) {
        cout << "YES\n0\n";
        return;
    }

    int dx = abs(a.first - b.first);
    int dy = abs(a.second - b.second);
    if(dx < dy) {
        swap(a.first, a.second);
        swap(b.first, b.second);
        swap(c.first, c.second);
    }

    int D = max(dx, dy);
    int max_steps = D / 2 - 1;
    int x_dir = (b.first > a.first) ? 1 : -1;

    if(max_steps <= 0) {
        cout << "NO\n" << D - 1 << "\n";
        return;
    }

    auto clamp = [](int v, int lo, int hi) { return max(lo, min(hi, v)); };

    for(int t = 1; t <= max_steps; t++) {
        int x3 = c.first - t, x4 = c.first + t;
        int y3 = c.second - t, y4 = c.second + t;

        auto check = [&](pair<int, int> king, pair<int, int> other, int dir) {
            int x = king.first + dir * t;
            int y_min =
                max(clamp(king.second - t, 1, n),
                    clamp(other.second - D + t, 1, n));
            int y_max =
                min(clamp(king.second + t, 1, n),
                    clamp(other.second + D - t, 1, n));

            if(x < x3 || x > x4) {
                return false;
            }
            if(x == x3 || x == x4) {
                return max(y_min, y3) <= min(y_max, y4);
            }
            return (y_min <= y3 && y3 <= y_max) || (y_min <= y4 && y4 <= y_max);
        };

        if(check(a, b, x_dir) || check(b, a, -x_dir)) {
            cout << "YES\n" << t << "\n";
            return;
        }
    }

    cout << "NO\n" << D - 1 << "\n";
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
