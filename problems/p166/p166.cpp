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

string commands;

void read() { getline(cin, commands); }

void solve() {
    // The hard part about this question is to understand what exactly the input
    // and output format is, otherwise it's just implementation.

    vector<string> lines = {""};
    int row = 0, col = 0;

    auto pad_line = [&](int r, int c) {
        if(c > (int)lines[r].size()) {
            lines[r].resize(c, ' ');
        }
    };

    for(int i = 0; i < (int)commands.size(); i++) {
        char c = commands[i];
        if(c == 'Q') {
            break;
        } else if(c >= 'a' && c <= 'z' || c == ' ') {
            pad_line(row, col);
            lines[row].insert(lines[row].begin() + col, c);
            col++;
        } else if(c == 'L') {
            if(col > 0) {
                col--;
            }
        } else if(c == 'R') {
            col++;
        } else if(c == 'U') {
            if(row > 0) {
                row--;
            }
        } else if(c == 'D') {
            if(row + 1 < (int)lines.size()) {
                row++;
            }
        } else if(c == 'N') {
            if(col > (int)lines[row].size()) {
                col = lines[row].size();
            }
            string tail = lines[row].substr(col);
            lines[row] = lines[row].substr(0, col);
            lines.insert(lines.begin() + row + 1, tail);
            row++;
            col = 0;
        } else if(c == 'E') {
            col = lines[row].size();
        } else if(c == 'H') {
            col = 0;
        } else if(c == 'X') {
            if(col < (int)lines[row].size()) {
                lines[row].erase(lines[row].begin() + col);
            } else if(row + 1 < (int)lines.size()) {
                pad_line(row, col);
                lines[row] += lines[row + 1];
                lines.erase(lines.begin() + row + 1);
            }
        } else if(c == 'B') {
            if(col > 0) {
                if(col <= (int)lines[row].size()) {
                    lines[row].erase(lines[row].begin() + col - 1);
                }
                col--;
            } else if(row > 0) {
                int prev_len = lines[row - 1].size();
                lines[row - 1] += lines[row];
                lines.erase(lines.begin() + row);
                row--;
                col = prev_len;
            }
        }
    }

    for(int i = 0; i < (int)lines.size(); i++) {
        if(i > 0) {
            cout << "\n";
        }
        cout << lines[i];
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
