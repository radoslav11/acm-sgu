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

string text;

string lower(const string& s) {
    string res;
    for(char c: s) {
        res += tolower(c);
    }
    return res;
}

vector<string> split_words(const string& s) {
    istringstream is(s);
    vector<string> res;
    string w;
    while(is >> w) {
        res.push_back(w);
    }
    return res;
}

void read() {
    string line;
    while(getline(cin, line)) {
        if(!text.empty() && text.back() != ' ') {
            text += ' ';
        }
        text += line;
    }
}

void solve() {
    // This is a fairly simple problem, with a somewhat convoluted problem
    // statement. We simply split the text into sentences by '.', for each
    // sentence find words matching "can/may/must/should" (insert "not") or
    // "is/are" (insert "no" and remove following "a/the" if that's the case).
    // Then we can just output all candidates.

    auto is_not_word = [](const string& s) {
        string lo = lower(s);
        return lo == "can" || lo == "may" || lo == "must" || lo == "should";
    };
    auto is_no_word = [](const string& s) {
        string lo = lower(s);
        return lo == "is" || lo == "are";
    };
    auto is_a_or_the = [](const string& s) {
        string lo = lower(s);
        return lo == "a" || lo == "the";
    };

    vector<string> sentences;
    string cur;
    for(char c: text) {
        if(c == '.') {
            sentences.push_back(cur);
            cur.clear();
        } else {
            cur += c;
        }
    }

    vector<vector<string>> results;
    for(auto& sent: sentences) {
        auto words = split_words(sent);
        int n = words.size();
        for(int i = 0; i < n; i++) {
            if(!is_not_word(words[i]) && !is_no_word(words[i])) {
                continue;
            }

            vector<string> out(words.begin(), words.begin() + i + 1);
            bool no = is_no_word(words[i]);
            out.push_back(no ? "no" : "not");
            int start = i + 1;
            start += (no && start < n && is_a_or_the(words[start]));
            out.insert(out.end(), words.begin() + start, words.end());
            results.push_back(out);
        }
    }

    cout << results.size() << '\n';
    for(auto& r: results) {
        for(int i = 0; i < (int)r.size(); i++) {
            cout << r[i];
            if(i + 1 == (int)r.size()) {
                cout << '.';
            } else {
                cout << ' ';
            }
        }
        cout << '\n';
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
