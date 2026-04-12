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

int n, m;
vector<string> init_lines, prog_lines;
map<string, string> vars;

void read() {
    cin >> n >> m;
    cin.ignore();

    vector<string> all_lines;
    string line;
    while(getline(cin, line)) {
        bool blank = true;
        for(char c: line) {
            if(c != ' ' && c != '\t' && c != '\r') {
                blank = false;
                break;
            }
        }
        if(!blank) {
            all_lines.push_back(line);
        }
    }

    init_lines.assign(all_lines.begin(), all_lines.begin() + n);
    prog_lines.assign(all_lines.begin() + n, all_lines.begin() + n + m);
}

struct substr_args {
    string name;
    int offset;
    int count;
    bool has_count;
};

substr_args parse_substr_args(const string& s) {
    int comma1 = s.find(',');
    string name = s.substr(0, comma1);
    string rest = s.substr(comma1 + 1);
    int comma2 = rest.find(',');
    if(comma2 == (int)string::npos) {
        return {name, stoi(rest), 0, false};
    }
    return {
        name, stoi(rest.substr(0, comma2)), stoi(rest.substr(comma2 + 1)), true
    };
}

string get_substr(const string& str, int offset, int count, bool has_count) {
    int len = str.size();
    int start = offset >= 0 ? offset : len + offset;
    int end;
    if(!has_count) {
        end = len;
    } else if(count >= 0) {
        end = start + count;
    } else {
        end = len + count;
    }
    return str.substr(start, end - start);
}

string strip_spaces(const string& s) {
    string result;
    for(char c: s) {
        if(c != ' ') {
            result += c;
        }
    }
    return result;
}

void solve() {
    // This is mostly an implementation problem, which does require a fair bit
    // of parsing.

    for(auto& line: init_lines) {
        int dollar = line.find('$');
        int i = dollar;
        string name;
        while(i < (int)line.size() &&
              (isalnum(line[i]) || line[i] == '$' || line[i] == '_')) {
            name += line[i++];
        }
        int q1 = line.find('"');
        int q2 = line.rfind('"');
        vars[name] = line.substr(q1 + 1, q2 - q1 - 1);
    }

    for(auto& raw_line: prog_lines) {
        string line = strip_spaces(raw_line);
        if(line.empty() || line == ";") {
            continue;
        }

        if(line.substr(0, 6) == "print(") {
            string content = line.substr(6, line.size() - 8);
            if(content.substr(0, 7) == "substr(") {
                string inner = content.substr(7, content.size() - 8);
                auto [name, offset, count, has_count] =
                    parse_substr_args(inner);
                cout << get_substr(vars[name], offset, count, has_count)
                     << "\n";
            } else {
                cout << vars[content] << "\n";
            }
        } else if(line.substr(0, 7) == "substr(") {
            int close_paren = line.find(')');
            string lhs_inner = line.substr(7, close_paren - 7);
            string rhs =
                line.substr(close_paren + 2, line.size() - close_paren - 3);

            auto [name1, offset1, count1, has_count1] =
                parse_substr_args(lhs_inner);

            string new_value;
            if(rhs.substr(0, 7) == "substr(") {
                string rhs_inner = rhs.substr(7, rhs.size() - 8);
                auto [name2, offset2, count2, has_count2] =
                    parse_substr_args(rhs_inner);
                new_value =
                    get_substr(vars[name2], offset2, count2, has_count2);
            } else {
                new_value = vars[rhs];
            }

            string& str = vars[name1];
            int len = str.size();
            int start = offset1 >= 0 ? offset1 : len + offset1;
            int end;
            if(!has_count1) {
                end = len;
            } else if(count1 >= 0) {
                end = start + count1;
            } else {
                end = len + count1;
            }

            str = str.substr(0, start) + new_value + str.substr(end);
        } else {
            int eq = line.find('=');
            string lhs = line.substr(0, eq);
            string rhs = line.substr(eq + 1, line.size() - eq - 2);

            if(rhs.substr(0, 7) == "substr(") {
                string rhs_inner = rhs.substr(7, rhs.size() - 8);
                auto [name2, offset2, count2, has_count2] =
                    parse_substr_args(rhs_inner);
                vars[lhs] =
                    get_substr(vars[name2], offset2, count2, has_count2);
            } else {
                vars[lhs] = vars[rhs];
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
