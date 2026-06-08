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

void read() { cin >> n >> m; }

void solve() {
    // The first observation is that given n and m, we already know the number
    // of circle lines. To formalize this, we can notice that the graph is
    // planar and connected. Using Euler's theorem we know that |V| - |E| + |F|
    // = 2, or n - m + k = 1, where k is the number of cycles. Now the problem
    // is slightly different as we can start with having c cycles with 3
    // vertices each, and we need to figure out if we could allocate the rest of
    // the vertices. The main constraint is that each of the cycles can be at
    // most 10 vertices, but we can use the heuristic where we first try
    // expanding the 3 vertex cycles, and then try to append the rest of the
    // vertices as radial lines. If we either don't have enough vertices to
    // build the initial 3-sized cycles or we are left with some vertices after
    // expanding them and building the radial lines it's impossible. If we try
    // to make a more direct way of checking if there is a solution, we can
    // derive it as follows. By Euler's formula the number of circles must be
    // k = m - n + 1. Let s denote the total sum of circle sizes. Since
    // consecutive circles share one station, the circles together occupy
    // s - (k - 1) distinct stations. Exactly k - 1 of those are transfer
    // stations, so s - (k - 1) - (k - 1) = s - 2(k - 1) non-transfer circle
    // stations remain available to host radial terminals. Each radial
    // contributes one edge and one terminal station, so the number of radials
    // is fixed at n - (s - (k - 1)) = n + k - 1 - s, which must be non-negative
    // and at most s - 2(k - 1). The first constraint gives the upper bound s <=
    // n + k - 1, and the second rearranges to s >= ceil((n + 3k - 3) / 2). On
    // top of that, every circle has between 3 and 10 stations, which forces 3k
    // <= s <= 10k. Putting everything together, a valid s exists if and only if
    // it lies in [max(3k, ceil((n + 3k - 3) / 2)), min(10k, n + k - 1)].

    int k = m - n + 1;
    if(k < 1) {
        cout << "No solution\n";
        return;
    }

    int s_min = max(3 * k, (n + 3 * k - 2) / 2);
    int s_max = min(10 * k, n + k - 1);
    if(s_min > s_max) {
        cout << "No solution\n";
        return;
    }

    int s = s_min;
    vector<int> sizes(k, 3);
    int extra = s - 3 * k;
    for(int i = 0; i < k && extra > 0; i++) {
        int add = min(extra, 7);
        sizes[i] += add;
        extra -= add;
    }

    vector<vector<int>> circles(k);
    int next_id = 1;
    int prev_shared = -1;
    for(int i = 0; i < k; i++) {
        auto& c = circles[i];
        if(prev_shared != -1) {
            c.push_back(prev_shared);
        }
        while((int)c.size() < sizes[i]) {
            c.push_back(next_id++);
        }
        if(i + 1 < k) {
            prev_shared = c.back();
        }
    }

    vector<int> attach;
    for(int i = 0; i < k; i++) {
        int sz = sizes[i];
        for(int j = 0; j < sz; j++) {
            bool is_transfer = (i > 0 && j == 0) || (i + 1 < k && j == sz - 1);
            if(!is_transfer) {
                attach.push_back(circles[i][j]);
            }
        }
    }

    vector<pair<int, int>> radials;
    for(int t = next_id, idx = 0; t <= n; t++, idx++) {
        radials.emplace_back(attach[idx], t);
    }

    cout << k << '\n';
    for(int i = 0; i < k; i++) {
        cout << sizes[i];
        for(int x: circles[i]) {
            cout << ' ' << x;
        }
        cout << '\n';
    }
    cout << radials.size() << '\n';
    for(auto [a, b]: radials) {
        cout << a << ' ' << b << '\n';
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
