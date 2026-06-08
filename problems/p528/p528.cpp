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

int m;
string c;

void read() { cin >> m >> c; }

void solve() {
    // Single linear scan with a state machine. The state has two parts:
    //   frames   - stack of open containers, each LIST, DICT_K (boundary inside
    //              a dict, expecting next key item or 'e') or DICT_V (just saw
    //              a key, must read a value item next; 'e' not allowed)
    //   progress - what we're in the middle of parsing: NONE (at a clean
    //              boundary), INT (digits then 'e'), STR_LEN (digits then ':',
    //              accumulated length in str_val) or STR_BODY (str_val chars of
    //              raw body left to consume)
    //
    // To answer "can the prefix be completed within m chars?" we maintain
    // g_sum, the sum of per-frame g_cost values for all open frames. g_cost is
    // the cost to close a frame assuming whatever sits inside it will complete
    // on its own (just needs the trailing chars): 1 for LIST ('e'), 3 for
    // DICT_K ("0:" filler value + 'e'), 1 for DICT_V ('e' after the pending
    // value finishes). f_cost is the cost to close from a clean boundary
    // *inside* that frame, with no item lined up: 1 for LIST/DICT_K, 3 for
    // DICT_V (must add "0:" then 'e'). Then min_remaining is finish_cost +
    // g_sum when an item is in progress, or f_cost(top) + (g_sum -
    // g_cost(top)) at a clean boundary. Special case: empty stack and no item
    // started yet needs 2 chars for the smallest object.
    //
    // A prefix length j is feasible iff parsing c[0..j-1] never violated a rule
    // and j + min_remaining <= m; we track the largest such j, and print "ok"
    // only when the whole string parsed and the root object closed.

    enum Frame { LIST, DICT_K, DICT_V };
    enum Progress { NONE, INT, STR_LEN, STR_BODY };

    vector<Frame> frames;
    Progress progress = NONE;
    int int_digits = 0;
    bool int_zero = false;
    long long str_val = 0;
    int str_len_digits = 0;
    bool str_len_zero = false;
    bool root_started = false;
    bool root_completed = false;
    long long g_sum = 0;

    auto g_cost = [](Frame f) -> int { return (f == DICT_K) ? 3 : 1; };

    auto f_cost = [](Frame f) -> int { return (f == DICT_V) ? 3 : 1; };

    auto min_remaining = [&]() -> long long {
        if(root_completed) {
            return 0;
        }

        if(progress == NONE) {
            if(!root_started) {
                return 2;
            }

            if(frames.empty()) {
                return 0;
            }

            Frame top = frames.back();
            return f_cost(top) + (g_sum - g_cost(top));
        }

        long long finish_cost = 0;
        if(progress == INT) {
            finish_cost = (int_digits == 0 ? 1 : 0) + 1;
        } else if(progress == STR_LEN) {
            finish_cost = 1 + str_val;
        } else {
            finish_cost = str_val;
        }

        return finish_cost + g_sum;
    };

    auto item_completed = [&]() {
        if(frames.empty()) {
            root_completed = true;
        } else {
            Frame& top = frames.back();
            if(top == DICT_K) {
                g_sum -= g_cost(DICT_K);
                top = DICT_V;
                g_sum += g_cost(DICT_V);
            } else if(top == DICT_V) {
                g_sum -= g_cost(DICT_V);
                top = DICT_K;
                g_sum += g_cost(DICT_K);
            }
        }
    };

    auto start_item = [&](char ch) -> bool {
        if(ch == 'i') {
            progress = INT;
            int_digits = 0;
            int_zero = false;
            return true;
        }

        if(ch == 'l') {
            frames.push_back(LIST);
            g_sum += g_cost(LIST);
            return true;
        }

        if(ch == 'd') {
            frames.push_back(DICT_K);
            g_sum += g_cost(DICT_K);
            return true;
        }

        if(ch >= '0' && ch <= '9') {
            progress = STR_LEN;
            str_val = ch - '0';
            str_len_digits = 1;
            str_len_zero = (ch == '0');
            return true;
        }

        return false;
    };

    auto process = [&](char ch) -> bool {
        if(root_completed) {
            return false;
        }

        if(progress == INT) {
            if(ch >= '0' && ch <= '9') {
                if(int_zero && int_digits >= 1) {
                    return false;
                }

                if(ch == '0' && int_digits == 0) {
                    int_zero = true;
                }

                int_digits++;
                return true;
            }

            if(ch == 'e') {
                if(int_digits == 0) {
                    return false;
                }

                progress = NONE;
                item_completed();
                return true;
            }

            return false;
        }

        if(progress == STR_LEN) {
            if(ch >= '0' && ch <= '9') {
                if(str_len_zero && str_len_digits >= 1) {
                    return false;
                }

                str_val = str_val * 10 + (ch - '0');
                str_len_digits++;
                return true;
            }

            if(ch == ':') {
                progress = STR_BODY;
                if(str_val == 0) {
                    progress = NONE;
                    item_completed();
                }

                return true;
            }

            return false;
        }

        if(progress == STR_BODY) {
            str_val--;
            if(str_val == 0) {
                progress = NONE;
                item_completed();
            }

            return true;
        }

        if(frames.empty()) {
            if(root_started) {
                return false;
            }

            root_started = true;
            return start_item(ch);
        }

        Frame top = frames.back();
        if(top == LIST || top == DICT_K) {
            if(ch == 'e') {
                g_sum -= g_cost(top);
                frames.pop_back();
                item_completed();
                return true;
            }

            return start_item(ch);
        }

        if(ch == 'e') {
            return false;
        }

        return start_item(ch);
    };

    int n = (int)c.size();
    int j_max = 0;

    for(int i = 0; i < n; i++) {
        if(!process(c[i])) {
            break;
        }

        long long new_j = i + 1;
        if(new_j + min_remaining() > (long long)m) {
            break;
        }

        j_max = (int)new_j;
    }

    if(j_max == n && root_completed) {
        cout << "ok\n";
    } else {
        cout << "Error at position " << j_max << "!\n";
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
