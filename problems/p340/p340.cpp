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

string line;

bool readline(istream& in, string& s) { return (bool)getline(in, s); }

void read() {
    if(!readline(cin, line)) {
        exit(0);
    }
}

void parse(const string& s, string& out) {
    vector<pair<const char*, bool>> close_stack;
    int n = s.size();
    for(int i = 0; i < n; i++) {
        char c = s[i];
        if(c == '{') {
            if(!close_stack.empty()) {
                close_stack.back().second = false;
            }
            continue;
        }
        if(c == '}') {
            if(!close_stack.empty()) {
                out += close_stack.back().first;
                close_stack.pop_back();
            }
            continue;
        }

        bool is_sup_sub = (c == '^' || c == '_');
        if(is_sup_sub) {
            out += (c == '^') ? "<sup>" : "<sub>";
            close_stack.push_back({(c == '^') ? "</sup>" : "</sub>", true});
            continue;
        }

        if(isalpha(c)) {
            out += "<i>";
            while(i < n && isalpha(s[i])) {
                out += s[i++];
            }
            out += "</i>";
            i--;
        } else if(c == '+' || c == '-' || c == '*' || c == '/') {
            out += "&nbsp;";
            out += c;
            out += "&nbsp;";
        } else {
            out += c;
        }

        while(!close_stack.empty() && close_stack.back().second) {
            out += close_stack.back().first;
            close_stack.pop_back();
        }
    }
}

void solve() {
    // This is mostly a parsing exercise.

    string s;
    for(char c: line) {
        if(isalnum(c) || c == '+' || c == '-' || c == '*' || c == '/' ||
           c == '^' || c == '_' || c == '{' || c == '}' || c == '(' ||
           c == ')') {
            s += c;
        }
    }
    string out;
    out.reserve(s.size() * 8);
    parse(s, out);
    cout << out << "\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    while(true) {
        read();
        solve();
    }

    return 0;
}
