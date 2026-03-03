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
pair<int, int> p_cop, p_thief;
string first_mover;

void read() {
    cin >> n >> m;
    cin >> p_cop >> p_thief;
    cin >> first_mover;
}

void solve() {
    // This is a direct constructive problem with analyzing the cases. WLOG, we
    // will assume that the distance by x is greater or equal than the distance
    // by y. And similarly we can assume cop_x <= thief_x. If the cop moves
    // first, it would make sense to close down the larger distance (or the one
    // on x). Now we can assume that the thief moves first. There are a few
    // cases:
    //
    //     1) Corner case of m = 1 or n = 1. Then the thief will immediately go
    //        to the border and the cop will eventually catch him. Depending on
    //        the parity, we might have to make one extra move as they can't
    //        stop moving.
    //
    //     2) Let's make a chess board. If neither of n or m is 1, and they
    //        start at different colours, the thief can always choose a cell
    //        that isn't occupied by the cop and is the same colour as the cop's
    //        current cell. Therefore, the game can continue indefinitely.
    //
    //     3) In all other cases, the cop can eventually get to a state where
    //        he catches the thief. We can notice that the optimal strategy of
    //        the thief is to go to one of the corners.
    //
    // A naive way is to try all 4 possible corners, but it's also not very
    // complicated to get the explicit closed form of the answer.

    if(abs(p_cop.first - p_thief.first) < abs(p_cop.second - p_thief.second)) {
        swap(p_cop.first, p_cop.second);
        swap(p_thief.first, p_thief.second);
        swap(n, m);
    }

    if(p_cop.first > p_thief.first) {
        p_cop.first *= -1;
        p_thief.first *= -1;
        p_cop.first += n + 1;
        p_thief.first += n + 1;
    }

    int ans = 0;
    if(first_mover == "C") {
        p_cop.first++;
        ans++;
    }

    if(p_cop == p_thief) {
        cout << ans << endl;
        return;
    }

    if(m == 1) {
        ans += (n - p_cop.first - 1) * 2;
        ans += (p_cop.first ^ p_thief.first) & 1;
        cout << ans << endl;
        return;
    }

    if(((p_cop.first ^ p_cop.second) ^ (p_thief.first ^ p_thief.second)) & 1) {
        cout << 0 << endl;
        return;
    }

    if(abs(p_cop.first - p_thief.first) == abs(p_cop.second - p_thief.second)) {
        if(p_cop.second < p_thief.second) {
            p_cop.second *= -1;
            p_thief.second *= -1;
            p_cop.second += m + 1;
            p_thief.second += m + 1;
        }
        ans += (p_cop.second - p_cop.first + n - 2) * 2;
        cout << ans << endl;
        return;
    }

    ans += (n - p_cop.first - 1 + max(p_cop.second - 1, m - p_cop.second)) * 2;
    cout << ans << endl;
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
