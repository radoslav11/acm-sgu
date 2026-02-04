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

bool is_letter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

void read() {
    string line;
    getline(cin, line);
    while(!line.empty() && (line.back() == '\r' || line.back() == '\n')) {
        line.pop_back();
    }
    if(line.size() >= 2 && line.front() == '"' && line.back() == '"') {
        text = line.substr(1, line.size() - 2);
    } else {
        text = "\x01";
    }
}

string dash_string(int k) {
    if(k == 0) {
        return "";
    }
    if(k == 1) {
        return "-";
    }

    string result;
    if(k % 2 == 1) {
        result = " --- ";
        k -= 3;
    } else {
        result = " -- ";
        k -= 2;
    }
    while(k > 0) {
        result += "-- ";
        k -= 2;
    }
    return result;
}

void solve() {
    // This is more of an exercise in parsing. There isn't really a complicated
    // algorithms idea here. We extract words and count dashes between them,
    // then validate that boundary dashes can be expressed as short/long dashes
    // (not single hyphens which require words on both sides).

    vector<string> words;
    vector<int> dashes;

    int dash_count = 0;
    int i = 0;
    while(i < (int)text.size()) {
        char c = text[i];
        if(c == ' ') {
            i++;
        } else if(c == '-') {
            dash_count++;
            i++;
        } else if(is_letter(c)) {
            dashes.push_back(dash_count);
            dash_count = 0;
            string word;
            while(i < (int)text.size() && is_letter(text[i])) {
                word += text[i];
                i++;
            }
            words.push_back(word);
        } else {
            cout << "error\n";
            return;
        }
    }
    dashes.push_back(dash_count);

    if(words.empty()) {
        if(dash_count == 1) {
            cout << "error\n";
        } else if(dash_count > 1) {
            cout << "\"" << dash_string(dash_count) << "\"\n";
        } else {
            cout << "\"\"\n";
        }
        return;
    }

    int n = words.size();
    if(dashes[0] == 1 || dashes[n] == 1) {
        cout << "error\n";
        return;
    }

    ostringstream oss;
    if(dashes[0] > 0) {
        oss << dash_string(dashes[0]);
    }
    oss << words[0];

    for(int j = 1; j < n; j++) {
        int k = dashes[j];
        if(k == 0) {
            oss << " ";
        } else if(k == 1) {
            oss << "-";
        } else {
            oss << dash_string(k);
        }
        oss << words[j];
    }

    if(dashes[n] > 0) {
        oss << dash_string(dashes[n]);
    }

    cout << "\"" << oss.str() << "\"\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    for(int test = 1; test <= T; test++) {
        read();
        solve();
    }

    return 0;
}
