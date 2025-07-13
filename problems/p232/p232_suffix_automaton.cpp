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
        map<G, int> to;
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
};

int n, k;
vector<int> d;

void read() {
    cin >> n >> k;
    string s;
    cin >> s;

    d.resize(n);
    for(int i = 0; i < n; i++) {
        d[i] = s[i] - '0';
    }
}

template<typename T>
vector<T> max_cyclic_shift(const vector<T>& v) {
    static SuffixAutomaton<vector<int>> sa = SuffixAutomaton<vector<int>>();
    sa.clear();
    for(int i = 0; i < (int)v.size(); i++) {
        sa.add_letter(v[i]);
    }
    for(int i = 0; i < (int)v.size(); i++) {
        sa.add_letter(v[i]);
    }

    vector<int> dp(sa.states.size(), -1);
    function<void(int)> dfs = [&](int v) {
        if(dp[v] != -1) {
            return;
        }

        dp[v] = 0;
        for(auto [_, u]: sa.states[v].to) {
            dfs(u);
            dp[v] = max(dp[v], 1 + dp[u]);
        }
    };

    for(int i = 0; i < (int)sa.states.size(); i++) {
        dfs(i);
    }

    int u = 0, need = n;
    vector<T> ans;
    while(need > 0) {
        int best_transition = -1;
        for(auto [c, v]: sa.states[u].to) {
            if(dp[v] + 1 >= need) {
                best_transition = max(best_transition, c);
            }
        }
        assert(best_transition != -1);
        ans.push_back(best_transition);
        u = sa.states[u].to[best_transition];
        need--;
    }

    return ans;
}

void solve() {
    vector<bool> used(n, false);
    vector<int8_t> ans;
    for(int i = 0; i < n; i++) {
        if(used[i]) {
            continue;
        }

        int pos = 0;
        vector<int8_t> cycle;
        for(int j = i; !used[j]; j = (j + k) % n) {
            used[j] = true;
            cycle.push_back(d[j]);
            pos++;
        }

        cycle = max_cyclic_shift(cycle);
        if(ans.empty() || cycle > ans) {
            ans = cycle;
        }
    }

    for(int i = 0; i < n; i++) {
        cout << (int)ans[i % ans.size()];
    }

    cout << '\n';
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
