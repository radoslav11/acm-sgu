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

vector<int> balls;
string state;

void read() {
    balls.resize(7);
    cin >> balls >> state;
}

void solve() {
    // An important note in the statement is "You may assume that if there is at
    // least one red ball, there are all colours.", which makes this fairly
    // simple. We can greedily alternate reds with black (the most expensive
    // colour, worth 7) to maximize score. Each red-black pair scores 1+7=8
    // points. After all reds are exhausted, all the colours are potted in
    // increasing order (2+3+4+5+6+7=27). If state is RED, we first pot the best
    // available colour (which goes back on the table since the last red was the
    // previous shot), gaining extra points. With no reds, simply sum the
    // remaining colours, plus the best colour again if state is RED.

    int reds = balls[0];
    int colour_sum = 0;
    for(int i = 1; i < 7; i++) {
        colour_sum += balls[i] * (i + 1);
    }

    int score = 0;
    if(reds > 0) {
        score = reds * 8 + colour_sum;
        if(state == "RED") {
            score += 7;
        }
    } else {
        score = colour_sum;
        if(state == "RED") {
            for(int i = 6; i >= 1; i--) {
                if(balls[i]) {
                    score += i + 1;
                    break;
                }
            }
        }
    }

    cout << score << "\n";
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
