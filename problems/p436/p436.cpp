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

string input_text;

void read() {
    stringstream buffer;
    buffer << cin.rdbuf();
    input_text = buffer.str();
}

void solve() {
    // Pure implementation. Tokenize the input into decimal numbers, Diputs
    // numbers and "other" characters. A Diputs digit is one of `_.,-~='^"`
    // (least to most significant); a valid Diputs representation lists digits
    // in non-increasing significance and respects per-digit max quantities, so
    // we parse greedily under both constraints. Decimal numbers are parsed
    // greedily up to the Diputs maximum value, with a leading '0' forming the
    // single number 0. The encoding is a mixed-radix system, so conversion in
    // either direction is a small loop instead of enumerating ~836M values.
    // After tokenizing we flip each token's notation, optionally sort the
    // values when the total count is odd, and emit with original separators.

    const string diputs_chars = "_.,-~='^\"";
    const int max_counts[9] = {2, 3, 5, 7, 11, 13, 17, 19, 23};

    int64_t products[10];
    products[0] = 1;
    for(int i = 0; i < 9; i++) {
        products[i + 1] = products[i] * (max_counts[i] + 1);
    }

    int64_t max_num = products[9] - 1;

    array<int, 256> sig_of;
    sig_of.fill(-1);
    for(int i = 0; i < 9; i++) {
        sig_of[(unsigned char)diputs_chars[i]] = i;
    }

    auto diputs_to_decimal = [&](size_t start, size_t end) -> int64_t {
        int64_t value = 0;
        for(size_t k = start; k < end; k++) {
            value += products[sig_of[(unsigned char)input_text[k]]];
        }
        return value;
    };

    auto decimal_to_diputs = [&](int64_t n) -> string {
        if(n == 0) {
            return "O";
        }
        int counts[9];
        for(int i = 0; i < 9; i++) {
            counts[i] = (n / products[i]) % (max_counts[i] + 1);
        }
        string result;
        for(int i = 8; i >= 0; i--) {
            result.append(counts[i], diputs_chars[i]);
        }
        return result;
    };

    struct token {
        size_t start;
        size_t length;
        int64_t value;
        bool was_decimal;
    };

    vector<token> tokens;

    size_t n = input_text.size();
    for(size_t i = 0; i < n;) {
        unsigned char c = input_text[i];
        if(isdigit(c)) {
            size_t j = i;
            int64_t value = 0;
            if(c == '0') {
                j = i + 1;
            } else {
                while(j < n && isdigit((unsigned char)input_text[j])) {
                    int64_t next_val = value * 10 + (input_text[j] - '0');
                    if(next_val > max_num) {
                        break;
                    }
                    value = next_val;
                    j++;
                }
            }
            tokens.push_back({i, j - i, value, true});
            i = j;
        } else if(c == 'O') {
            tokens.push_back({i, 1, 0, false});
            i++;
        } else if(sig_of[c] >= 0) {
            size_t j = i;
            int last_sig = 8;
            int counts[9] = {};
            while(j < n) {
                int s = sig_of[(unsigned char)input_text[j]];
                if(s < 0 || s > last_sig || counts[s] >= max_counts[s]) {
                    break;
                }
                counts[s]++;
                last_sig = s;
                j++;
            }
            tokens.push_back({i, j - i, diputs_to_decimal(i, j), false});
            i = j;
        } else {
            i++;
        }
    }

    if(tokens.size() % 2 == 1) {
        vector<int64_t> values;
        values.reserve(tokens.size());
        for(auto& t: tokens) {
            values.push_back(t.value);
        }
        sort(values.begin(), values.end());
        for(size_t k = 0; k < tokens.size(); k++) {
            tokens[k].value = values[k];
        }
    }

    string result;
    result.reserve(input_text.size() * 2);
    size_t cursor = 0;
    for(auto& t: tokens) {
        while(cursor < t.start) {
            result += input_text[cursor++];
        }
        if(t.was_decimal) {
            result += decimal_to_diputs(t.value);
        } else {
            result += to_string(t.value);
        }
        cursor = t.start + t.length;
    }

    while(cursor < input_text.size()) {
        result += input_text[cursor++];
    }

    cout << result;
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
