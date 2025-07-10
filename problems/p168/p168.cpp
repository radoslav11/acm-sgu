#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>
using namespace std;

vector<vector<short>> solve(int N, int M, const vector<vector<short>>& A) {
    vector<vector<short>> Q(
        M, vector<short>(N + M, numeric_limits<short>::max())
    );

    for(int x = 0; x < N; x++) {
        for(int y = 0; y < M; y++) {
            Q[y][x + y] = A[x][y];
        }
    }

    for(int i = M - 1; i >= 0; i--) {
        for(int j = N + M - 1; j >= 0; j--) {
            short curr = Q[i][j];
            short down =
                (i + 1 < M) ? Q[i + 1][j] : numeric_limits<short>::max();
            short right =
                (j + 1 < N + M) ? Q[i][j + 1] : numeric_limits<short>::max();
            Q[i][j] = min({curr, down, right});
        }
    }

    vector<vector<short>> B(N, vector<short>(M));
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < M; j++) {
            B[i][j] = Q[j][i + j];
        }
    }

    return B;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    vector<vector<short>> A(N, vector<short>(M));
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < M; j++) {
            cin >> A[i][j];
        }
    }

    vector<vector<short>> B = solve(N, M, A);

    for(int i = 0; i < N; i++) {
        for(int j = 0; j < M; j++) {
            cout << B[i][j] << (j == M - 1 ? '\n' : ' ');
        }
    }

    return 0;
}
