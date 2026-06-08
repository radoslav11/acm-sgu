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

string regex_str;

void read() { cin >> regex_str; }

void solve() {
    // First, '!' is just shorthand for "???", so we expand it and only have to
    // reason about letters, '?' and '*'. The core DP is dp[l][r], the minimum
    // length of a palindrome that the regex segment s[l..r] can produce. Since
    // the produced string is a palindrome, its first and last characters must
    // agree, and they come from s[l] and s[r] respectively (or, if either is a
    // '*', from that '*' absorbing one extra character). That gives a small
    // case split on (s[l], s[r]): when both are non-'*' we peel both ends with
    // a compatible outer char (letters force it, '?' is free); when one is
    // '*' and the other isn't, the '*' may match empty (drop and recurse) or
    // absorb the matching char on its side while staying in place; when both
    // are '*' either may match empty.
    //
    // Length-optimal moves can produce different palindromes, so for the
    // lex-smallest answer we have to actually compare the candidates rather
    // than rely on a heuristic ('prefer drop' / 'prefer peel' both have
    // counterexamples, e.g. "*bab*ba" needs drop but "*!ba*!" needs peel).
    // We therefore build the actual pal[l][r] string for every cell, in
    // increasing-length order. Each cell of length L depends only on cells
    // of length L-1 and L-2, so we keep just three rolling layers of strings
    // alive at any time, which caps memory at O(n^2) chars and stays well
    // under the limit even at n=765 after '!' expansion.

    string s;
    for(char c: regex_str) {
        if(c == '!') {
            s += "???";
        } else {
            s += c;
        }
    }

    int n = s.size();
    if(n == 0) {
        cout << "YES\n\n";
        return;
    }

    const int INF = INT_MAX / 2;
    vector<vector<int>> dp(n, vector<int>(n, INF));

    auto is_star = [&](int p) { return s[p] == '*'; };
    auto is_q = [&](int p) { return s[p] == '?'; };

    auto get_dp = [&](int a, int b) -> int {
        if(a > b) {
            return 0;
        }
        return dp[a][b];
    };

    for(int i = 0; i < n; i++) {
        dp[i][i] = is_star(i) ? 0 : 1;
    }

    for(int len = 2; len <= n; len++) {
        for(int l = 0; l + len - 1 < n; l++) {
            int r = l + len - 1;
            int best = INF;

            if(is_star(l) && get_dp(l + 1, r) < INF) {
                best = min(best, get_dp(l + 1, r));
            }

            if(is_star(r) && get_dp(l, r - 1) < INF) {
                best = min(best, get_dp(l, r - 1));
            }

            if(!is_star(l) && !is_star(r)) {
                int inner = get_dp(l + 1, r - 1);
                bool ok = true;
                if(!is_q(l) && !is_q(r) && s[l] != s[r]) {
                    ok = false;
                }
                if(ok && inner < INF) {
                    best = min(best, inner + 2);
                }
            } else if(!is_star(l) && is_star(r)) {
                int inner = get_dp(l + 1, r);
                if(inner < INF) {
                    best = min(best, inner + 2);
                }
            } else if(is_star(l) && !is_star(r)) {
                int inner = get_dp(l, r - 1);
                if(inner < INF) {
                    best = min(best, inner + 2);
                }
            }

            dp[l][r] = best;
        }
    }

    if(dp[0][n - 1] >= INF) {
        cout << "NO\n";
        return;
    }

    vector<string> layer_a(n), layer_b(n), layer_c(n);
    vector<string>* lm2 = &layer_a;
    vector<string>* lm1 = &layer_b;
    vector<string>* cur = &layer_c;

    string answer;
    for(int len = 1; len <= n; len++) {
        for(int l = 0; l + len - 1 < n; l++) {
            int r = l + len - 1;
            (*cur)[l].clear();
            if(dp[l][r] >= INF) {
                continue;
            }

            if(len == 1) {
                if(is_star(l)) {
                    (*cur)[l] = "";
                } else if(is_q(l)) {
                    (*cur)[l] = "a";
                } else {
                    (*cur)[l] = string(1, s[l]);
                }
                continue;
            }

            int target = dp[l][r];
            bool have = false;
            string best;

            auto try_cand = [&](string cand) {
                if(!have || cand < best) {
                    best = std::move(cand);
                    have = true;
                }
            };

            if(!is_star(l) && !is_star(r)) {
                int inner = (len == 2) ? 0 : dp[l + 1][r - 1];
                if(inner != INF && inner + 2 == target) {
                    bool ok = true;
                    char c = 0;
                    if(!is_q(l) && !is_q(r)) {
                        if(s[l] == s[r]) {
                            c = s[l];
                        } else {
                            ok = false;
                        }
                    } else if(!is_q(l)) {
                        c = s[l];
                    } else if(!is_q(r)) {
                        c = s[r];
                    } else {
                        c = 'a';
                    }
                    if(ok) {
                        const string& ip =
                            (len == 2) ? string() : (*lm2)[l + 1];
                        string cand;
                        cand.reserve(ip.size() + 2);
                        cand += c;
                        cand += ip;
                        cand += c;
                        try_cand(std::move(cand));
                    }
                }
            }

            if(!is_star(l) && is_star(r)) {
                int inner = dp[l + 1][r];
                if(inner != INF && inner + 2 == target) {
                    char c = is_q(l) ? 'a' : s[l];
                    const string& ip = (*lm1)[l + 1];
                    string cand;
                    cand.reserve(ip.size() + 2);
                    cand += c;
                    cand += ip;
                    cand += c;
                    try_cand(std::move(cand));
                }
            }

            if(is_star(l) && !is_star(r)) {
                int inner = dp[l][r - 1];
                if(inner != INF && inner + 2 == target) {
                    char c = is_q(r) ? 'a' : s[r];
                    const string& ip = (*lm1)[l];
                    string cand;
                    cand.reserve(ip.size() + 2);
                    cand += c;
                    cand += ip;
                    cand += c;
                    try_cand(std::move(cand));
                }
            }

            if(is_star(l) && dp[l + 1][r] == target) {
                try_cand((*lm1)[l + 1]);
            }

            if(is_star(r) && dp[l][r - 1] == target) {
                try_cand((*lm1)[l]);
            }

            (*cur)[l] = std::move(best);
        }

        if(len == n) {
            answer = std::move((*cur)[0]);
            break;
        }

        vector<string>* tmp = lm2;
        lm2 = lm1;
        lm1 = cur;
        cur = tmp;
        for(auto& x: *cur) {
            x.clear();
            x.shrink_to_fit();
        }
    }

    cout << "YES\n" << answer << "\n";
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
