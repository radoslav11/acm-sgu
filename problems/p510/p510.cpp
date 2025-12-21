#include <bits/stdc++.h>
// #include <coding_library/strings/suffix_automaton.hpp>

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

template<class T = string>
class SuffixAutomaton {
  private:
    using G = conditional_t<
        is_same_v<T, const char*> || is_same_v<T, char*> || is_array_v<T>, char,
        typename T::value_type>;

    struct State {
        unordered_map<G, int> to;
        int len;
        int link;
        State(int l = 0, int lnk = -1) : len(l), link(lnk) {}
    };

    int check_replace_with_clone(int p, G c) {
        int q = states[p].to[c];
        if(states[p].len + 1 == states[q].len) {
            return q;
        }

        int clone = states.size();
        states.emplace_back(states[p].len + 1, states[q].link);
        states[clone].to = states[q].to;
        while(p >= 0 && states[p].to[c] == q) {
            states[p].to[c] = clone;
            p = states[p].link;
        }
        states[q].link = clone;
        return clone;
    }

  public:
    int last;
    vector<State> states;

    SuffixAutomaton() : last(0) { clear(); }
    SuffixAutomaton(const T& s) { init(s); }

    void add_letter(G c) {
        if(states[last].to.count(c)) {
            int clone = check_replace_with_clone(last, c);
            last = clone;
            return;
        }

        int p = last;
        last = states.size();
        states.emplace_back(states[p].len + 1);

        while(p >= 0 && !states[p].to.count(c)) {
            states[p].to[c] = last;
            p = states[p].link;
        }

        if(p == -1) {
            states[last].link = 0;
            return;
        }

        int q_or_clone = check_replace_with_clone(p, c);
        states[last].link = q_or_clone;
    }

    void add_string(const T& s) {
        last = 0;
        for(char c: s) {
            add_letter(c);
        }
    }

    void clear() {
        states.clear();
        states.emplace_back();
        last = 0;
    }

    void init(const T& s) {
        clear();
        add_string(s);
    }

    vector<vector<int>> build_suffix_link_tree() {
        vector<vector<int>> adj(states.size());
        for(int i = 1; i < (int)states.size(); i++) {
            adj[states[i].link].push_back(i);
        }
        return adj;
    }

    vector<vector<int>> states_by_length() {
        vector<vector<int>> ret(states.size());
        for(int i = 0; i < (int)states.size(); i++) {
            ret[states[i].len].push_back(i);
        }
        return ret;
    }
};

int cnt_distinct(string s) {
    auto sa = SuffixAutomaton<string>(s);
    int ans = 0;
    for(int i = 1; i < (int)sa.states.size(); i++) {
        ans += sa.states[i].len - sa.states[sa.states[i].link].len;
    }

    return ans;
}

int n;

void read() { cin >> n; }

vector<string> prec = {
    "",
    "a",
    "aa",
    "ab",
    "aaaa",
    "aab",
    "abc",
    "aaab",
    "aaba",
    "aabc",
    "abcd",
    "aaaba",
    "aaabc",
    "aabac",
    "aabcd",
    "abcde",
    "aaabba",
    "aaabac",
    "aaabcd",
    "aabacd",
    "aabcde",
    "abcdef",
    "aaaabcd",
    "aaabaca",
    "aaabacd",
    "aaabcde",
    "aabacde",
    "aabcdef",
    "abcdefg",
    "aaaabacd",
    "aaaabcde",
    "aaabacad",
    "aaabacde",
    "aaabcdef",
    "aabacdef",
    "aabcdefg",
    "abcdefgh",
    "aaaabacad",
    "aaaabacde",
    "aaaabcdef",
    "aaabacade",
    "aaabacdef",
    "aaabcdefg",
    "aabacdefg",
    "aabcdefgh",
    "abcdefghi",
    "aaaabaacde",
    "aaaabacade",
    "aaaabacdef",
    "aaaabcdefg",
    "aaabacadef",
    "aaabacdefg",
    "aaabcdefgh",
    "aabacdefgh",
    "aabcdefghi",
    "abcdefghij",
    "aaaaabcdefg",
    "aaaabaacdef",
    "aaaabacadef",
    "aaaabacdefg",
    "aaaabcdefgh",
    "aaabacadefg",
    "aaabacdefgh",
    "aaabcdefghi",
    "aabacdefghi",
    "aabcdefghij",
    "abcdefghijk",
    "aaaaabacdefg",
    "aaaaabcdefgh",
    "aaaabaacdefg",
    "aaaabacadefg",
    "aaaabacdefgh",
    "aaaabcdefghi",
    "aaabacadefgh",
    "aaabacdefghi",
    "aaabcdefghij",
    "aabacdefghij",
    "aabcdefghijk",
    "abcdefghijkl",
    "aaaaabacadefg",
    "aaaaabacdefgh",
    "aaaaabcdefghi",
    "aaaabaacdefgh",
    "aaaabacadefgh",
    "aaaabacdefghi",
    "aaaabcdefghij",
    "aaabacadefghi",
    "aaabacdefghij",
    "aaabcdefghijk",
    "aabacdefghijk",
    "aabcdefghijkl",
    "abcdefghijklm",
    "aaaaabaacdefgh",
    "aaaaabacadefgh",
    "aaaaabacdefghi",
    "aaaaabcdefghij",
    "aaaabaacdefghi",
    "aaaabacadefghi",
    "aaaabacdefghij",
    "aaaabcdefghijk",
    "aaabacadefghij",
    "aaabacdefghijk",
    "aaabcdefghijkl",
    "aabacdefghijkl",
    "aabcdefghijklm",
    "abcdefghijklmn",
    "aaaaabaacadefgh",
    "aaaaabaacdefghi",
    "aaaaabacadefghi",
    "aaaaabacdefghij",
    "aaaaabcdefghijk",
    "aaaabaacdefghij",
    "aaaabacadefghij",
    "aaaabacdefghijk",
    "aaaabcdefghijkl",
    "aaabacadefghijk",
    "aaabacdefghijkl",
    "aaabcdefghijklm",
    "aabacdefghijklm",
    "aabcdefghijklmn",
    "abcdefghijklmno",
    "aaaaaabcdefghijk",
    "aaaaabaacadefghi",
    "aaaaabaacdefghij",
    "aaaaabacadefghij",
    "aaaaabacdefghijk",
    "aaaaabcdefghijkl",
    "aaaabaacdefghijk",
    "aaaabacadefghijk",
    "aaaabacdefghijkl",
    "aaaabcdefghijklm",
    "aaabacadefghijkl",
    "aaabacdefghijklm",
    "aaabcdefghijklmn",
    "aabacdefghijklmn",
    "aabcdefghijklmno",
    "abcdefghijklmnop",
    "aaaaaabacdefghijk",
    "aaaaaabcdefghijkl",
    "aaaaabaacadefghij",
    "aaaaabaacdefghijk",
    "aaaaabacadefghijk",
    "aaaaabacdefghijkl",
    "aaaaabcdefghijklm",
    "aaaabaacdefghijkl",
    "aaaabacadefghijkl",
    "aaaabacdefghijklm",
    "aaaabcdefghijklmn",
    "aaabacadefghijklm",
    "aaabacdefghijklmn",
    "aaabcdefghijklmno",
    "aabacdefghijklmno",
    "aabcdefghijklmnop",
    "abcdefghijklmnopq",
    "aaaaaabacadefghijk",
    "aaaaaabacdefghijkl",
    "aaaaaabcdefghijklm",
    "aaaaabaacadefghijk",
    "aaaaabaacdefghijkl",
    "aaaaabacadefghijkl",
    "aaaaabacdefghijklm",
    "aaaaabcdefghijklmn",
    "aaaabaacdefghijklm",
    "aaaabacadefghijklm",
    "aaaabacdefghijklmn",
    "aaaabcdefghijklmno",
    "aaabacadefghijklmn",
    "aaabacdefghijklmno",
    "aaabcdefghijklmnop",
    "aabacdefghijklmnop",
    "aabcdefghijklmnopq",
    "abcdefghijklmnopqr",
    "aaaaaabaacdefghijkl",
    "aaaaaabacadefghijkl",
    "aaaaaabacdefghijklm",
    "aaaaaabcdefghijklmn",
    "aaaaabaacadefghijkl",
    "aaaaabaacdefghijklm",
    "aaaaabacadefghijklm",
    "aaaaabacdefghijklmn",
    "aaaaabcdefghijklmno",
    "aaaabaacdefghijklmn",
    "aaaabacadefghijklmn",
    "aaaabacdefghijklmno",
    "aaaabcdefghijklmnop",
    "aaabacadefghijklmno",
    "aaabacdefghijklmnop",
    "aaabcdefghijklmnopq",
    "aabacdefghijklmnopq",
    "aabcdefghijklmnopqr",
    "abcdefghijklmnopqrs",
    "aaaaaabaacadefghijkl",
    "aaaaaabaacdefghijklm",
    "aaaaaabacadefghijklm",
    "aaaaaabacdefghijklmn",
    "aaaaaabcdefghijklmno",
    "aaaaabaacadefghijklm",
    "aaaaabaacdefghijklmn",
    "aaaaabacadefghijklmn",
    "aaaaabacdefghijklmno",
    "aaaaabcdefghijklmnop",
    "aaaabaacdefghijklmno",
    "aaaabacadefghijklmno",
    "aaaabacdefghijklmnop",
    "aaaabcdefghijklmnopq",
    "aaabacadefghijklmnop",
    "aaabacdefghijklmnopq",
    "aaabcdefghijklmnopqr",
    "aabacdefghijklmnopqr",
    "aabcdefghijklmnopqrs",
    "abcdefghijklmnopqrst",
    "aaaaaabaacadaefghijkl",
    "aaaaaabaacadefghijklm",
    "aaaaaabaacdefghijklmn",
    "aaaaaabacadefghijklmn",
    "aaaaaabacdefghijklmno",
    "aaaaaabcdefghijklmnop",
    "aaaaabaacadefghijklmn",
    "aaaaabaacdefghijklmno",
    "aaaaabacadefghijklmno",
    "aaaaabacdefghijklmnop",
    "aaaaabcdefghijklmnopq",
    "aaaabaacdefghijklmnop",
    "aaaabacadefghijklmnop",
    "aaaabacdefghijklmnopq",
    "aaaabcdefghijklmnopqr",
    "aaabacadefghijklmnopq",
    "aaabacdefghijklmnopqr",
    "aaabcdefghijklmnopqrs",
    "aabacdefghijklmnopqrs",
    "aabcdefghijklmnopqrst",
    "abcdefghijklmnopqrstu",
    "aaaaaaabcdefghijklmnop",
    "aaaaaabaacadaefghijklm",
    "aaaaaabaacadefghijklmn",
    "aaaaaabaacdefghijklmno",
    "aaaaaabacadefghijklmno",
    "aaaaaabacdefghijklmnop",
    "aaaaaabcdefghijklmnopq",
    "aaaaabaacadefghijklmno",
    "aaaaabaacdefghijklmnop",
    "aaaaabacadefghijklmnop",
    "aaaaabacdefghijklmnopq",
    "aaaaabcdefghijklmnopqr",
    "aaaabaacdefghijklmnopq",
    "aaaabacadefghijklmnopq",
    "aaaabacdefghijklmnopqr",
    "aaaabcdefghijklmnopqrs",
    "aaabacadefghijklmnopqr",
    "aaabacdefghijklmnopqrs",
    "aaabcdefghijklmnopqrst",
    "aabacdefghijklmnopqrst",
    "aabcdefghijklmnopqrstu",
    "abcdefghijklmnopqrstuv",
    "aaaaaaabacdefghijklmnop",
    "aaaaaaabcdefghijklmnopq",
    "aaaaaabaacadaefghijklmn",
    "aaaaaabaacadefghijklmno",
    "aaaaaabaacdefghijklmnop",
    "aaaaaabacadefghijklmnop",
    "aaaaaabacdefghijklmnopq",
    "aaaaaabcdefghijklmnopqr",
    "aaaaabaacadefghijklmnop",
    "aaaaabaacdefghijklmnopq",
    "aaaaabacadefghijklmnopq",
    "aaaaabacdefghijklmnopqr",
    "aaaaabcdefghijklmnopqrs",
    "aaaabaacdefghijklmnopqr",
    "aaaabacadefghijklmnopqr",
    "aaaabacdefghijklmnopqrs",
    "aaaabcdefghijklmnopqrst",
    "aaabacadefghijklmnopqrs",
    "aaabacdefghijklmnopqrst",
    "aaabcdefghijklmnopqrstu",
    "aabacdefghijklmnopqrstu",
    "aabcdefghijklmnopqrstuv",
    "abcdefghijklmnopqrstuvw",
    "aaaaaaabacadefghijklmnop",
    "aaaaaaabacdefghijklmnopq",
    "aaaaaaabcdefghijklmnopqr",
    "aaaaaabaacadaefghijklmno",
    "aaaaaabaacadefghijklmnop",
    "aaaaaabaacdefghijklmnopq",
    "aaaaaabacadefghijklmnopq",
    "aaaaaabacdefghijklmnopqr",
    "aaaaaabcdefghijklmnopqrs",
    "aaaaabaacadefghijklmnopq",
    "aaaaabaacdefghijklmnopqr",
    "aaaaabacadefghijklmnopqr",
    "aaaaabacdefghijklmnopqrs",
    "aaaaabcdefghijklmnopqrst",
    "aaaabaacdefghijklmnopqrs",
    "aaaabacadefghijklmnopqrs",
    "aaaabacdefghijklmnopqrst",
    "aaaabcdefghijklmnopqrstu",
    "aaabacadefghijklmnopqrst",
    "aaabacdefghijklmnopqrstu",
    "aaabcdefghijklmnopqrstuv",
    "aabacdefghijklmnopqrstuv",
    "aabcdefghijklmnopqrstuvw",
    "abcdefghijklmnopqrstuvwx"
};

void precompute() {
    vector<string> ans(n + 5, "-1");
    ans[1] = "a";
    ans[2] = "aa";
    ans[3] = "ab";
    ans[4] = "aaaa";

    for(int cnt = 1; cnt < n; cnt++) {
        char max_c = *max_element(ans[cnt].begin(), ans[cnt].end()) + 1;
        for(char c = 'a'; c <= max_c; c++) {
            for(int pos = 0; pos <= (int)ans[cnt].size(); pos++) {
                string cand = ans[cnt].substr(0, pos) + c;
                if(pos != ans[cnt].size()) {
                    cand += ans[cnt].substr(pos, (int)ans[cnt].size() - pos);
                }

                int new_cnt = cnt_distinct(cand);
                if(new_cnt > n) {
                    continue;
                }
                if(ans[new_cnt] == "-1" ||
                   (ans[new_cnt].size() > cand.size()) ||
                   (ans[new_cnt].size() == cand.size() && ans[new_cnt] > cand
                   )) {
                    ans[new_cnt] = cand;
                }
            }
        }
    }

    cout << "vector<string> prec = {\"\"";
    for(int i = 1; i <= n; i++) {
        cout << ", \"" << ans[i] << "\"";
        assert(ans[i] == prec[i]);
    }
    cout << "};\n";
}

void solve() {
    // Counting the #distinct substrings can be done efficiently with a suffix
    // data structure, and we will use this as a quick oracle. Generating test
    // cases is a bit more complex, particularly, because we are interested in
    // the shortest, and lexicographically smallest strings for each n. The n <=
    // 300 should suggest that the length of the shortest string will likely be
    // O(sqrt(n)), which is quite low and should give us intuition to try a
    // brute force approach. There are different ways to go about making a brute
    // force, either by incrementally looking at lengths, and then pruning (if
    // there is no way to complete the desired N), but the way I implemented
    // here was based on a BFS / beam search like idea. Let for every N,
    // maintain best[N], which is the set of K smallest strings that can be
    // produced from best[1...N-1] by adding a single character. We can do a
    // straight forward DP solution for that, and then ans[N] = min(best[N]),
    // where min is first by length and then lexicographically. I first run this
    // for K = 30 and K = 50, and discovered that there is no difference between
    // these two, which suggested that we might need a very small K. I tried, K
    // = 2, and that was also the same. The only difference between K = 1 and K
    // = 2 turned out to be because of ans[4], which should be 'aaaa'. This
    // suggested a stronger conjecture: ans(n) can be constructed using ans(1),
    // ... , ans(n-1) for any n > 4. However, proving this doesn't seem straight
    // forward.
    //
    // Depending on how efficiently we implement this idea, we can directly pass
    // the problem, as the oracle complexity is O(len * |sigma|), all strings
    // have O(sqrt(n)) length, and we have O(len * |sigma|) transitions at every
    // step. We also visit n different states. However, we can also just
    // precompute the answers.

    // precompute();
    cout << prec[n] << endl;
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
