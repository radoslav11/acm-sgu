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

vector<string> lines;

void read() {
    string line;
    while(getline(cin, line)) {
        lines.push_back(line);
    }
}

void process(const vector<string>& items, bool wrap_in_li) {
    int n = (int)items.size();
    int i = 0;
    while(i < n) {
        char c = items[i][0];
        if(c == '#' || c == '*') {
            int j = i;
            while(j < n && !items[j].empty() && items[j][0] == c) {
                j++;
            }
            if(j - i >= 2) {
                if(wrap_in_li) {
                    cout << "<li>\n";
                }
                cout << (c == '#' ? "<ol>" : "<ul>") << "\n";
                vector<string> stripped;
                for(int k = i; k < j; k++) {
                    stripped.push_back(items[k].substr(1));
                }
                process(stripped, true);
                cout << (c == '#' ? "</ol>" : "</ul>") << "\n";
                if(wrap_in_li) {
                    cout << "</li>\n";
                }
                i = j;
                continue;
            }
        }
        if(wrap_in_li) {
            cout << "<li>\n";
        }
        cout << items[i] << "\n";
        if(wrap_in_li) {
            cout << "</li>\n";
        }
        i++;
    }
}

void solve() {
    // The algorithm here isn't complicated. It's again a parsing problem.
    process(lines, false);
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
