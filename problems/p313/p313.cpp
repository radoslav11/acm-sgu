#include <bits/stdc++.h>
#include <limits>

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

int n, L;
vector<int> a, b;

void read() {
    cin >> n >> L;
    a.resize(n);
    b.resize(n);
    cin >> a >> b;
}

struct Point {
    int x, i, type;
    Point(int x, int i, int type) : x(x), i(i), type(type) {}

    bool operator<(const Point& other) const {
        if(x != other.x) {
            return x < other.x;
        }
        if(type != other.type) {
            return type < other.type;
        }

        return i < other.i;
    }
};

void solve() {
    vector<Point> points;
    for(int i = 0; i < n; i++) {
        points.emplace_back(a[i], i, 1);
        points.emplace_back(b[i], i, -1);
    }

    sort(points.begin(), points.end());
    points.emplace_back(points[0].x + L, points[0].i, points[0].type);

    int64_t candidate = 0;
    int bal = 0;

    vector<int64_t> gaps;
    vector<int> balances;
    for(int i = 0; i < 2 * n; i++) {
        bal += points[i].type;
        balances.push_back(bal);

        int64_t gap = points[i + 1].x - points[i].x;
        gaps.push_back(gap);

        candidate += gap * bal;
    }

    vector<int> match(2 * n);
    iota(match.begin(), match.end(), 0);
    sort(match.begin(), match.end(), [&](int i, int j) {
        return balances[i] < balances[j];
    });

    int64_t best = numeric_limits<int64_t>::max(), len = 0, last = 0;
    int ans_i = 0;
    for(int i = 0; i < 2 * n; i++) {
        int j = match[i];
        candidate += len * (balances[j] - last);
        candidate -= (L - len) * (balances[j] - last);
        if(candidate < best) {
            best = candidate;
            ans_i = j;
        }
        last = balances[j];
        len += gaps[j];
    }

    cout << best << '\n';

    vector<int> ans(n);
    stack<Point> st;
    for(int pos = (ans_i + 1) % (2 * n), i = 0; i < 2 * n;
        i++, pos = (pos + 1) % (2 * n)) {
        if(st.empty() || st.top().type == points[pos].type) {
            st.push(points[pos]);
        } else {
            if(points[pos].type == 1) {
                ans[points[pos].i] = st.top().i + 1;
            } else {
                ans[st.top().i] = points[pos].i + 1;
            }
            st.pop();
        }
    }
    cout << ans << '\n';
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
