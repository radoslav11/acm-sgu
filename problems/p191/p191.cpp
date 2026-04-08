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

char init;
string s;

void read() { cin >> init >> s; }

void solve() {
    // The problem statement is somewhat hard to parse, but it boils down to a
    // context-free grammar. Let S_A and S_B be nonterminals representing empty
    // stands signed by A and B, and let 'A', 'B' be terminals (filled stands).
    // The two operations give us the following productions:
    //     S_A -> 'B'
    //     S_A -> 'A' S_B S_A
    //     S_B -> 'A'
    //     S_B -> 'B' S_A S_B
    //
    // We need to check if the given string can be derived starting from S_A or
    // S_B (given in input). This grammar is LL(1) since the first terminal
    // uniquely determines which production to apply: if c == X we must expand,
    // if c is the opponent we must use the terminal production. So we can use
    // standard predictive parsing with a stack of pending nonterminals. For each
    // character c, pop the top S_X: if c is the opponent of X, it matches a
    // terminal production and we consume it. If c == X, we apply the expand
    // rule, pushing S_X and then S_{opponent} on top. The string is valid iff
    // the stack is empty at the end.

    auto opponent = [](char c) { return c == 'A' ? 'B' : 'A'; };

    vector<char> stack = {init};
    for(char c: s) {
        if(stack.empty()) {
            cout << "NO" << endl;
            return;
        }

        char top = stack.back();
        stack.pop_back();

        if(c == top) {
            stack.push_back(top);
            stack.push_back(opponent(top));
        }
    }

    cout << (stack.empty() ? "YES" : "NO") << endl;
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
