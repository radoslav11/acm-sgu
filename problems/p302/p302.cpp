#include <bits/stdc++.h>

using namespace std;

template<typename T1, typename T2>
ostream &operator<<(ostream &out, const pair<T1, T2> &x) {
    return out << x.first << ' ' << x.second;
}

template<typename T1, typename T2>
istream &operator>>(istream &in, pair<T1, T2> &x) {
    return in >> x.first >> x.second;
}

template<typename T>
istream &operator>>(istream &in, vector<T> &a) {
    for(auto &x: a) {
        in >> x;
    }
    return in;
};

template<typename T>
ostream &operator<<(ostream &out, const vector<T> &a) {
    for(auto x: a) {
        out << x << ' ';
    }
    return out;
};

string s;

void read() { cin >> s; }

template<typename T>
void pop_back(vector<T> &st, int len) {
    for(int i = 0; i < len; i++) {
        st.pop_back();
    }
}

void pop_back(string &s, int len) {
    for(int i = 0; i < len; i++) {
        s.pop_back();
    }
}

string string_from_last(const vector<pair<char, int>> &st, int len) {
    if(st.size() < len) {
        return "";
    }

    string res;
    for(int i = 0; i < len; i++) {
        res.push_back(st[st.size() - len + i].first);
    }
    return res;
}

string string_from_last(const string &s, int len) {
    if(s.size() < len) {
        return "";
    }

    string res;
    for(int i = 0; i < len; i++) {
        res.push_back(s[s.size() - len + i]);
    }
    return res;
}

char make_upper(char c) {
    if('a' <= c && c <= 'z') {
        return c - 'a' + 'A';
    }
    return c;
}

char make_lower(char c) {
    if('A' <= c && c <= 'Z') {
        return c - 'A' + 'a';
    }
    return c;
}

void solve() {
    // Use '1' char as opening <UP> 
    // Use '2' char as opening <DOWN> 

    vector<pair<char, int>> st;
    for(int i = 0; i < (int)s.size(); i++) {
        char c = s[i];
        st.push_back({c, i});
        if(string_from_last(st, 4) == "<UP>") {
            pop_back(st, 4);
            st.push_back({'1', -1});
        } else if(string_from_last(st, 5) == "</UP>") {
            pop_back(st, 5);
            while(st.back().first != '1') {
                s[st.back().second] = make_upper(s[st.back().second]);
                st.pop_back();
            }
            st.pop_back();
        } else if(string_from_last(st, 6) == "<DOWN>") {
            pop_back(st, 6);
            st.push_back({'2', -1});
        } else if(string_from_last(st, 7) == "</DOWN>") {
            pop_back(st, 7);
            while(st.back().first != '2') {
                s[st.back().second] = make_lower(s[st.back().second]);
                st.pop_back();
            }
            st.pop_back();
        }
    }

    string ans;
    for(auto c: s) {
        ans.push_back(c);
        if(string_from_last(ans, 4) == "<UP>") {
            pop_back(ans, 4);
        } else if(string_from_last(ans, 5) == "</UP>") {
            pop_back(ans, 5);
        } else if(string_from_last(ans, 6) == "<DOWN>") {
            pop_back(ans, 6);
        } else if(string_from_last(ans, 7) == "</DOWN>") {
            pop_back(ans, 7);
        }
    }

    cout << ans << '\n';
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T = 1;
    // cin >> T;
    for(int test = 1; test <= T; test++) {
        read();
        // cout << "Case #" << test << ": ";
        solve();
    }

    return 0;
}
