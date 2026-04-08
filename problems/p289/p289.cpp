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

const int STATES = 19683;
const int lines[8][3] = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {0, 3, 6},
                         {1, 4, 7}, {2, 5, 8}, {0, 4, 8}, {2, 4, 6}};

int dp[STATES];
bool reachable[STATES];

int get_cell(int state, int i) {
    for(int j = 0; j < i; j++) {
        state /= 3;
    }
    return state % 3;
}

int set_cell(int state, int i, int v) {
    int pw = 1;
    for(int j = 0; j < i; j++) {
        pw *= 3;
    }
    return state + v * pw;
}

int check_winner(int state) {
    for(auto& line: lines) {
        int a = get_cell(state, line[0]);
        int b = get_cell(state, line[1]);
        int c = get_cell(state, line[2]);
        if(a && a == b && b == c) {
            return a;
        }
    }
    return 0;
}

int count_pieces(int state) {
    int cnt = 0;
    for(int i = 0; i < 9; i++) {
        if(get_cell(state, i)) {
            cnt++;
        }
    }
    return cnt;
}

void precompute() {
    memset(reachable, false, sizeof(reachable));
    reachable[0] = true;

    function<void(int)> dfs = [&](int state) {
        int w = check_winner(state);
        if(w) {
            dp[state] = (w == 1) ? 1 : -1;
            return;
        }
        int cnt = count_pieces(state);
        if(cnt == 9) {
            dp[state] = 0;
            return;
        }
        int player = (cnt % 2 == 0) ? 1 : 2;
        bool maximizing = (player == 1);
        dp[state] = maximizing ? -2 : 2;
        for(int i = 0; i < 9; i++) {
            if(get_cell(state, i) == 0) {
                int nxt = set_cell(state, i, player);
                if(!reachable[nxt]) {
                    reachable[nxt] = true;
                    dfs(nxt);
                }
                if(maximizing) {
                    dp[state] = max(dp[state], dp[nxt]);
                } else {
                    dp[state] = min(dp[state], dp[nxt]);
                }
            }
        }
    };

    dfs(0);
}

string board[3];

void solve() {
    // We can do a DP over all 3^9 possible board states by doing a DFS from
    // the empty board. Each state encodes the board in base 3 (0 = empty, 1 =
    // X, 2 = O). For each state we compute the minimax outcome with perfect
    // play: 1 = X wins, -1 = O wins, 0 = draw. Only states reachable through
    // valid play are marked, so illegal positions are detected by checking
    // reachability. Each query then just encodes the board and looks up the
    // precomputed result. In practice only ~5k states out of 19683 are
    // reachable through valid play.

    int state = 0;
    for(int i = 0; i < 9; i++) {
        char c = board[i / 3][i % 3];
        if(c == 'X') {
            state = set_cell(state, i, 1);
        } else if(c == '0') {
            state = set_cell(state, i, 2);
        }
    }

    if(!reachable[state]) {
        cout << "Illegal position.\n";
    } else if(dp[state] == 1) {
        cout << "X wins.\n";
    } else if(dp[state] == -1) {
        cout << "0 wins.\n";
    } else {
        cout << "Game is a draw.\n";
    }
}

bool read() {
    cin >> board[0];
    if(board[0] == "Qc") {
        return false;
    }
    cin >> board[1] >> board[2];
    return true;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    precompute();

    while(read()) {
        solve();
    }

    return 0;
}
