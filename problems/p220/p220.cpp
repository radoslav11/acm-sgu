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
}

template<typename T>
ostream& operator<<(ostream& out, const vector<T>& a) {
    for(auto& x: a) {
        out << x << ' ';
    }
    return out;
}

int n, k;
vector<pair<int, int>> blackCells, whiteCells;
vector<bool> usedDiag1Black, usedDiag2Black;
vector<bool> usedDiag1White, usedDiag2White;
vector<long long> waysBlack, waysWhite;

void read() { cin >> n >> k; }

void backtrackBlack(int idx, int placed) {
    if(idx == (int)blackCells.size()) {
        if(placed <= k) {
            waysBlack[placed]++;
        }
        return;
    }
    backtrackBlack(idx + 1, placed);

    auto [r, c] = blackCells[idx];
    int d1 = r + c;
    int d2 = r - c + (n - 1);
    if(!usedDiag1Black[d1] && !usedDiag2Black[d2]) {
        usedDiag1Black[d1] = usedDiag2Black[d2] = true;
        backtrackBlack(idx + 1, placed + 1);
        usedDiag1Black[d1] = usedDiag2Black[d2] = false;
    }
}

void backtrackWhite(int idx, int placed) {
    if(idx == (int)whiteCells.size()) {
        if(placed <= k) {
            waysWhite[placed]++;
        }
        return;
    }
    backtrackWhite(idx + 1, placed);

    auto [r, c] = whiteCells[idx];
    int d1 = r + c;
    int d2 = r - c + (n - 1);
    if(!usedDiag1White[d1] && !usedDiag2White[d2]) {
        usedDiag1White[d1] = usedDiag2White[d2] = true;
        backtrackWhite(idx + 1, placed + 1);
        usedDiag1White[d1] = usedDiag2White[d2] = false;
    }
}

void solve() {
    blackCells.clear();
    whiteCells.clear();
    for(int r = 0; r < n; r++) {
        for(int c = 0; c < n; c++) {
            if(((r + c) & 1) == 0) {
                blackCells.emplace_back(r, c);
            } else {
                whiteCells.emplace_back(r, c);
            }
        }
    }

    usedDiag1Black.assign(2 * n, false);
    usedDiag2Black.assign(2 * n, false);
    usedDiag1White.assign(2 * n, false);
    usedDiag2White.assign(2 * n, false);

    waysBlack.assign(k + 1, 0LL);
    waysWhite.assign(k + 1, 0LL);

    backtrackBlack(0, 0);
    backtrackWhite(0, 0);

    long long ans = 0;
    for(int i = 0; i <= k; i++) {
        ans += waysBlack[i] * waysWhite[k - i];
    }

    cout << ans << "\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T = 1;
    while(T--) {
        read();
        solve();
    }

    return 0;
}
