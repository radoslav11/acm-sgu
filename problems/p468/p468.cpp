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

int N;
vector<pair<int, int>> knight_moves = {{1, 2},  {1, -2},  {2, 1},  {2, -1},
                                       {-1, 2}, {-1, -2}, {-2, 1}, {-2, -1}};
mt19937 rng(42);

void read() { cin >> N; }

bool is_valid_move(int x, int y, const vector<vector<int>>& board) {
    return x >= 0 && x < N && y >= 0 && y < N && board[x][y] == 0;
}

int get_degree(int x, int y, const vector<vector<int>>& board) {
    int degree = 0;
    for(const auto& [dx, dy]: knight_moves) {
        int next_x = x + dx, next_y = y + dy;
        if(is_valid_move(next_x, next_y, board)) {
            degree++;
        }
    }
    return degree;
}

bool find_tour(vector<vector<int>>& board) {
    int current_x = uniform_int_distribution<int>(0, N - 1)(rng);
    int current_y = uniform_int_distribution<int>(0, N - 1)(rng);
    board[current_x][current_y] = 1;

    for(int position = 2; position <= N * N; position++) {
        vector<pair<int, int>> next_moves;
        for(int i = 0; i < knight_moves.size(); i++) {
            int next_x = current_x + knight_moves[i].first;
            int next_y = current_y + knight_moves[i].second;
            if(is_valid_move(next_x, next_y, board)) {
                int degree = get_degree(next_x, next_y, board);
                next_moves.emplace_back(degree, i);
            }
        }

        if(next_moves.empty()) {
            return false;
        }

        auto [_, move_index] =
            *min_element(next_moves.begin(), next_moves.end());

        current_x += knight_moves[move_index].first;
        current_y += knight_moves[move_index].second;
        board[current_x][current_y] = position;
    }
    return true;
}

void solve() {
    // Uses Warnsdorff's heuristic to greedily select the next move with the
    // fewest onward moves Randomly breaks ties among moves with the same
    // minimum degree. This is not guaranteed to be polynomial time but highly
    // effective for N ≤ 250.

    if(N == 2 || N == 3 || N == 4) {
        cout << "No solution." << endl;
        return;
    }

    vector<vector<int>> board(N, vector<int>(N, 0));
    while(true) {
        board.assign(N, vector<int>(N, 0));
        shuffle(knight_moves.begin(), knight_moves.end(), rng);
        if(find_tour(board)) {
            break;
        }
    }

    cout << "There is solution:" << endl;
    for(const auto& row: board) {
        for(int i = 0; i < N; i++) {
            cout << " " << row[i];
        }
        cout << endl;
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
