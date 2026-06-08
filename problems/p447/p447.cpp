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

string commands;

void read() { getline(cin, commands); }

struct Cmd {
    int val;
    int len;
    string str;
};

inline vector<Cmd> generate_commands() {
    vector<Cmd> res;
    int bases[] = {1, 2, 4, 8, 16, 32, 64};
    int unit_vals[] = {64, 32, 16, 8, 4, 2, 1};
    for(int i = 0; i < 7; i++) {
        string s = "R" + to_string(bases[i]);
        int half = unit_vals[i];
        int total = half;
        res.push_back({total, (int)s.size(), s});
        while(half > 1) {
            half /= 2;
            total += half;
            s += '.';
            res.push_back({total, (int)s.size(), s});
        }
    }
    return res;
}

inline int parse_total_value(const string& cmds) {
    int total = 0;
    int n = cmds.size();
    int i = 0;
    while(i < n) {
        i++;
        int base = 0;
        while(i < n && isdigit((unsigned char)cmds[i])) {
            base = base * 10 + (cmds[i] - '0');
            i++;
        }
        int unit = 64 / base;
        int sub = unit;
        while(i < n && cmds[i] == '.') {
            unit /= 2;
            sub += unit;
            i++;
        }
        total += sub;
    }
    return total;
}

inline bool is_strict_prefix(const string& a, const string& b) {
    return a.size() < b.size() && b.compare(0, a.size(), a) == 0;
}

inline vector<int> compute_dp(int V, const vector<Cmd>& cmds) {
    const int INF = INT_MAX;
    vector<int> dp(V + 1, INF);
    dp[0] = 0;
    for(int v = 1; v <= V; v++) {
        for(const auto& c: cmds) {
            if(c.val <= v && dp[v - c.val] != INF) {
                int cand = dp[v - c.val] + c.len;
                if(cand < dp[v]) {
                    dp[v] = cand;
                }
            }
        }
    }
    return dp;
}

inline int pick_best_command(
    int v, const vector<Cmd>& cmds, const vector<int>& dp
) {
    vector<int> feasible;
    for(int i = 0; i < (int)cmds.size(); i++) {
        const auto& c = cmds[i];
        if(c.val <= v && dp[v - c.val] != INT_MAX &&
           dp[v - c.val] + c.len == dp[v]) {
            feasible.push_back(i);
        }
    }

    vector<int> survivors;
    for(int i: feasible) {
        bool dominated = false;
        for(int j: feasible) {
            if(i != j && is_strict_prefix(cmds[i].str, cmds[j].str)) {
                dominated = true;
                break;
            }
        }
        if(!dominated) {
            survivors.push_back(i);
        }
    }

    int best = survivors[0];
    for(int i: survivors) {
        if(cmds[i].str < cmds[best].str) {
            best = i;
        }
    }
    return best;
}

void solve() {
    // Work in 1/64 units so every duration becomes a positive integer.
    // Each rest command is "R" + base + k dots, with base in
    // {1,2,4,8,16,32,64} and the dotted half-chain not falling below the
    // 1/64 unit; this gives 7+6+5+4+3+2+1 = 28 distinct commands. Let V be
    // the total value of the input. Then dp[v] = shortest expression for
    // value v is an unbounded-knapsack DP over the 28 commands.
    //
    // For lex-smallest recovery, observe that every character that may
    // appear inside a command past the leading 'R' is either a digit
    // ('0'..'9' = 48..57) or '.' (46), all strictly less than 'R' (82).
    // After a command ends, the next character is the start of the next
    // command, which is always 'R'. So if at value v two feasible
    // candidates a, b satisfy "a.str is a strict prefix of b.str", their
    // full answers agree on the first |a.str| chars; at index |a.str|,
    // candidate a's char comes from the next command (it's 'R') while
    // candidate b's char is a digit or '.', strictly smaller. Hence b
    // always beats a, and we can discard every feasible command dominated
    // by a longer feasible command sharing its prefix. The survivors then
    // have pairwise non-prefix-related strings, so their full answers
    // differ inside their own c.str and the lex-smallest c.str wins.
    // Recovery is therefore a plain string compare among at most 28
    // fixed strings at each step.

    auto cmds = generate_commands();
    int V = parse_total_value(commands);
    auto dp = compute_dp(V, cmds);

    string ans;
    int v = V;
    while(v > 0) {
        int idx = pick_best_command(v, cmds, dp);
        ans += cmds[idx].str;
        v -= cmds[idx].val;
    }
    cout << ans << "\n";
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
