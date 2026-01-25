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

int n, h;

void read() { cin >> n >> h; }

void solve() {
    // The solution is fairly simple - we can notice that the structure we build
    // increases with "triangles". The first one is of size 1, then 3, then 5
    // and so on. This means that there are at most O(sqrt(n)) pumpkins that
    // will be killed, and we can directly simulate the solution. To be more
    // precise, we can find the first pumpkin that will be killed in O(h) =
    // O(sqrt(n)) by looping through the "triangles". Note that h = O(sqrt(n))
    // because it is guaranteed there is at least one number in the output.

    // 1 + 3 + ... + 2h+1 = (h+1)^2
    int pos = max((h + 1) * (h + 1), 1), l = h + 1;
    vector<int> front, back;

    back.push_back(pos);
    while(pos + l <= n) {
        pos = pos + l;
        if(pos <= n) {
            front.push_back(pos);
        }
        pos = pos + l;
        if(pos <= n) {
            back.push_back(pos);
        }
        l++;
    }

    reverse(front.begin(), front.end());
    cout << front << back << '\n';
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
