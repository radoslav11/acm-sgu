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
map<string, string> parent;

string normalize(const string& s) {
    if(s.empty() || isdigit(s[0])) {
        return s;
    }
    string r = s;
    for(char& c: r) {
        c = tolower(c);
    }
    return r;
}

string find(const string& s) {
    auto it = parent.find(s);
    if(it == parent.end()) {
        return s;
    }
    return it->second = find(it->second);
}

int64_t resolve_token(const string& token) {
    string r = find(normalize(token));
    if(!r.empty() && isdigit(r[0])) {
        return stoll(r);
    }
    return 0;
}

int64_t power(int64_t base, int64_t exp) {
    bool neg = base < 0 && exp % 2 == 1;
    __int128 b = abs(base), result = 1;
    while(exp > 0) {
        if(exp & 1) {
            result *= b;
        }
        b *= b;
        exp >>= 1;
    }
    return neg ? -(int64_t)result : (int64_t)result;
}

int64_t custom_div(int64_t a, int64_t b) {
    int64_t sign = ((a >= 0) == (b >= 0)) ? 1 : -1;
    return sign * (abs(a) / abs(b));
}

int64_t custom_mod(int64_t a, int64_t b) {
    int64_t sign = ((a >= 0) == (b >= 0)) ? 1 : -1;
    int64_t rem = abs(a) % abs(b);
    return rem == 0 ? 0 : sign * rem;
}

struct Parser {
    const string& s;
    int pos;

    Parser(const string& str) : s(str), pos(0) {}

    void skip() {
        while(pos < (int)s.size() && s[pos] == ' ') {
            pos++;
        }
    }

    string read_token() {
        skip();
        if(pos >= (int)s.size()) {
            return "";
        }
        int start = pos;
        if(isdigit(s[pos])) {
            while(pos < (int)s.size() && isdigit(s[pos])) {
                pos++;
            }
        } else if(isalpha(s[pos])) {
            while(pos < (int)s.size() && isalnum(s[pos])) {
                pos++;
            }
        }
        return s.substr(start, pos - start);
    }

    int64_t parse_expr() {
        int64_t val = parse_term();
        while(true) {
            skip();
            if(pos < (int)s.size() && (s[pos] == '+' || s[pos] == '-')) {
                char op = s[pos++];
                int64_t r = parse_term();
                val = op == '+' ? val + r : val - r;
            } else {
                break;
            }
        }
        return val;
    }

    int64_t parse_term() {
        int64_t val = parse_power();
        while(true) {
            skip();
            if(pos < (int)s.size() &&
               (s[pos] == '*' || s[pos] == '/' || s[pos] == '%')) {
                char op = s[pos++];
                int64_t r = parse_power();
                if(op == '*') {
                    val *= r;
                } else if(op == '/') {
                    val = custom_div(val, r);
                } else {
                    val = custom_mod(val, r);
                }
            } else {
                break;
            }
        }
        return val;
    }

    int64_t parse_power() {
        int64_t val = parse_unary();
        skip();
        if(pos < (int)s.size() && s[pos] == '^') {
            pos++;
            return power(val, parse_power());
        }
        return val;
    }

    int64_t parse_unary() {
        skip();
        if(pos < (int)s.size() && s[pos] == '-') {
            pos++;
            return -parse_unary();
        }
        if(pos < (int)s.size() && s[pos] == '+') {
            pos++;
            return parse_unary();
        }
        return parse_atom();
    }

    int64_t parse_atom() {
        skip();
        if(pos < (int)s.size() && s[pos] == '(') {
            pos++;
            int64_t val = parse_expr();
            skip();
            if(pos < (int)s.size() && s[pos] == ')') {
                pos++;
            }
            return val;
        }
        return resolve_token(read_token());
    }
};

void read() {
    string line;
    while(getline(cin, line)) {
        lines.push_back(line);
    }
}

void solve() {
    // This is a fairly simple problem conceptually, but requires some parsing.
    // For the links of "define", we can use path compression.

    for(auto& line: lines) {
        int i = 0;
        while(i < (int)line.size() && line[i] == ' ') {
            i++;
        }
        if(line.compare(i, 5, "print") == 0) {
            string expr = line.substr(i + 5);
            Parser p(expr);
            cout << p.parse_expr() << "\n";
        } else if(line.compare(i, 6, "define") == 0) {
            string rest = line.substr(i + 6);
            istringstream iss(rest);
            string op1, op2;
            iss >> op1 >> op2;
            string a = normalize(op1), b = normalize(op2);
            if(!parent.count(a)) {
                string rb = find(b);
                if(rb != a) {
                    parent[a] = b;
                }
            }
        }
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
