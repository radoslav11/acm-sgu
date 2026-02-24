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

string expr;

void read() { cin >> expr; }

int pos;

bool parse_low(int mask);
bool parse_mid(int mask);
bool parse_high(int mask);
bool parse_atom(int mask);

bool parse_atom(int mask) {
    if(expr[pos] == '(') {
        pos++;
        bool result = parse_low(mask);
        pos++;
        return result;
    }
    char var = expr[pos++];
    return (mask >> (var - 'a')) & 1;
}

bool parse_high(int mask) {
    if(expr[pos] == '!') {
        pos++;
        return !parse_high(mask);
    }
    return parse_atom(mask);
}

bool parse_mid(int mask) {
    bool result = parse_high(mask);
    while(pos < (int)expr.size() && expr[pos] == '&') {
        pos++;
        result = result & parse_high(mask);
    }
    return result;
}

bool parse_low(int mask) {
    bool result = parse_mid(mask);
    while(pos < (int)expr.size()) {
        if(expr[pos] == '|' && expr[pos + 1] == '|') {
            pos += 2;
            result = result | parse_mid(mask);
        } else if(expr[pos] == '<' && expr[pos + 1] == '=' && expr[pos + 2] == '>') {
            pos += 3;
            result = result == parse_mid(mask);
        } else if(expr[pos] == '=' && expr[pos + 1] == '>') {
            pos += 2;
            result = !result | parse_mid(mask);
        } else if(expr[pos] == '#') {
            pos++;
            result = result != parse_mid(mask);
        } else {
            break;
        }
    }
    return result;
}

bool evaluate_expression(int mask) {
    pos = 0;
    return parse_low(mask);
}

string create_dnf() {
    vector<bool> used_vars(10, false);
    for(char c: expr) {
        if(c >= 'a' && c <= 'j') {
            used_vars[c - 'a'] = true;
        }
    }

    vector<int> used_indices;
    for(int i = 0; i < 10; i++) {
        if(used_vars[i]) {
            used_indices.push_back(i);
        }
    }

    int num_used = used_indices.size();
    set<int> true_assignments;

    for(int mask = 0; mask < (1 << 10); mask++) {
        if(evaluate_expression(mask)) {
            int assignment = 0;
            for(int j = 0; j < num_used; j++) {
                if((mask >> used_indices[j]) & 1) {
                    assignment |= (1 << j);
                }
            }
            true_assignments.insert(assignment);
        }
    }

    if(true_assignments.empty()) {
        return "a&!a";
    }

    string result;
    bool first = true;
    for(int assignment: true_assignments) {
        if(!first) {
            result += "||";
        }
        first = false;
        for(int j = 0; j < num_used; j++) {
            if(j > 0) {
                result += '&';
            }
            if(!((assignment >> j) & 1)) {
                result += '!';
            }
            result += char('a' + used_indices[j]);
        }
    }
    return result;
}

void solve() {
    // Notice that there are 10 variables, meaning there are only 2^10 possible
    // results. This means that if we evaluate all of them, we could in theory
    // look at which values the output is true, and enumerate all of them as a
    // DNF (disjunctive normal form), without brackets, because the conjunction
    // has higher priority than disjunction. Not very relevant to the problem,
    // but if we were to have the opposite priority, one approach could have
    // been to go for a CNF, which can be done by having a DNF of !f(a,...,j),
    // and then negating the resulting DNF with De Morgan's Law.
    //
    // In terms of complexity, we could do this by naively evaluating, but a
    // slightly better approach is to recursively do it and return the full
    // 2^vars table as the constant would be a bit lower and we don't have to
    // worry a lot about how quickly we are searching for the lowest priority
    // operator. However, that's not actually needed and we can go for other
    // standard evaluation approaches by either recursively simulating a stack,
    // or directly using inverse polish notation.
    //
    // The final expression will have less than 2^vars * (vars*3 + 1)
    // characters, which is withing the bound of ~32K for 10 variables. The *3
    // is because we could in theory have a negation, the variable, and the &
    // operator between each atom in each clause. The original expression has
    // length less than 2048 characters, so even the naive approach of
    // evaluating each combination would have been fast enough.

    string result = create_dnf();
    cout << result << '\n';
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
