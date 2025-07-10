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

int n;
vector<string> emails;

bool is_symbol(char ch) {
    return isalpha(ch) || isdigit(ch) || ch == '_' || ch == '-';
}

bool is_valid_prefix(const string& prefix) {
    if(prefix.empty() || prefix.front() == '.' || prefix.back() == '.') {
        return false;
    }
    bool last_char_was_dot = false;
    for(char ch: prefix) {
        if(ch == '.') {
            if(last_char_was_dot) {
                return false;
            }
            last_char_was_dot = true;
        } else if(!is_symbol(ch)) {
            return false;
        } else {
            last_char_was_dot = false;
        }
    }
    return true;
}

bool is_valid_domain(const string& domain) {
    if(domain.size() != 2 && domain.size() != 3) {
        return false;
    }
    for(char ch: domain) {
        if(!isalpha(ch)) {
            return false;
        }
    }
    return true;
}

bool is_valid_suffix(const string& suffix) {
    size_t pos = suffix.rfind('.');
    if(pos == string::npos) {
        return false;
    }
    string prefix = suffix.substr(0, pos);
    string domain = suffix.substr(pos + 1);
    return is_valid_prefix(prefix) && is_valid_domain(domain);
}

bool is_valid_email(const string& email) {
    size_t pos = email.find('@');
    if(pos == string::npos) {
        return false;
    }
    string prefix = email.substr(0, pos);
    string suffix = email.substr(pos + 1);
    return is_valid_prefix(prefix) && is_valid_suffix(suffix);
}

void read() {
    cin >> n;
    cin.ignore();
    emails.resize(n);
    for(auto& email: emails) {
        getline(cin, email);
    }
}

void solve() {
    vector<string> results;
    for(const auto& email: emails) {
        results.push_back(is_valid_email(email) ? "YES" : "NO");
    }
    for(const auto& result: results) {
        cout << result << '\n';
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T = 1;
    for(int test = 1; test <= T; test++) {
        read();
        solve();
    }

    return 0;
}
