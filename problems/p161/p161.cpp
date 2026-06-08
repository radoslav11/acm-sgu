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

const int MAXN = 100;
using BS = bitset<MAXN>;

int n, m, K;
vector<string> formulas;
vector<BS> up, down;

void read() {
    cin >> n >> m;
    up.assign(n, BS());
    for(int i = 0; i < n; i++) {
        up[i][i] = 1;
    }

    for(int i = 0; i < m; i++) {
        int s, t;
        cin >> s >> t;
        up[s - 1][t - 1] = 1;
    }

    for(int k = 0; k < n; k++) {
        for(int i = 0; i < n; i++) {
            if(up[i][k]) {
                up[i] |= up[k];
            }
        }
    }

    down.assign(n, BS());
    for(int x = 0; x < n; x++) {
        for(int z = 0; z < n; z++) {
            if(up[z][x]) {
                down[x][z] = 1;
            }
        }
    }

    cin >> K;
    string line;
    getline(cin, line);
    while((int)formulas.size() < K && getline(cin, line)) {
        if(line.find_first_not_of(" \t\r") != string::npos) {
            formulas.push_back(line);
        }
    }
}

vector<BS> H;
map<string, int> h_index;
vector<vector<int>> and_tbl, or_tbl, imp_tbl, eq_tbl;
vector<int> not_tbl;
int zero_idx, one_idx;

enum { T_ZERO, T_ONE, T_VAR, T_LPAR, T_RPAR, T_NOT, T_AND, T_OR, T_IMP, T_EQ };
enum { OP_CONST, OP_VAR, OP_NOT, OP_AND, OP_OR, OP_IMP, OP_EQ };

struct Token {
    int type, var;
};

struct Node {
    int op, a, b;
};

vector<Token> toks;
int pos;
vector<Node> nodes;
bool used[26];

int make_node(int op, int a, int b) {
    nodes.push_back({op, a, b});
    return (int)nodes.size() - 1;
}

int parse_equiv();

int parse_atom() {
    Token t = toks[pos];
    if(t.type == T_LPAR) {
        pos++;
        int e = parse_equiv();
        pos++;
        return e;
    }
    if(t.type == T_ZERO) {
        pos++;
        return make_node(OP_CONST, zero_idx, 0);
    }
    if(t.type == T_ONE) {
        pos++;
        return make_node(OP_CONST, one_idx, 0);
    }
    pos++;
    used[t.var] = true;
    return make_node(OP_VAR, t.var, 0);
}

int parse_neg() {
    if(toks[pos].type == T_NOT) {
        pos++;
        return make_node(OP_NOT, parse_neg(), 0);
    }
    return parse_atom();
}

int parse_conj() {
    int left = parse_neg();
    while(toks[pos].type == T_AND) {
        pos++;
        left = make_node(OP_AND, left, parse_neg());
    }
    return left;
}

int parse_disj() {
    int left = parse_conj();
    while(toks[pos].type == T_OR) {
        pos++;
        left = make_node(OP_OR, left, parse_conj());
    }
    return left;
}

int parse_impl() {
    int left = parse_disj();
    if(toks[pos].type == T_IMP) {
        pos++;
        return make_node(OP_IMP, left, parse_impl());
    }
    return left;
}

int parse_equiv() {
    vector<int> terms = {parse_impl()};
    while(toks[pos].type == T_EQ) {
        pos++;
        terms.push_back(parse_impl());
    }
    if(terms.size() == 1) {
        return terms[0];
    }

    int node = make_node(OP_EQ, terms[0], terms[1]);
    for(int i = 2; i < (int)terms.size(); i++) {
        node =
            make_node(OP_AND, node, make_node(OP_EQ, terms[i - 1], terms[i]));
    }
    return node;
}

int cur_assign[26];

int eval(int u) {
    Node nd = nodes[u];
    switch(nd.op) {
        case OP_CONST:
            return nd.a;
        case OP_VAR:
            return cur_assign[nd.a];
        case OP_NOT:
            return not_tbl[eval(nd.a)];
        case OP_AND:
            return and_tbl[eval(nd.a)][eval(nd.b)];
        case OP_OR:
            return or_tbl[eval(nd.a)][eval(nd.b)];
        case OP_IMP:
            return imp_tbl[eval(nd.a)][eval(nd.b)];
        default:
            return eq_tbl[eval(nd.a)][eval(nd.b)];
    }
}

void solve() {
    // The graph's reachability gives a partial order on the vertices, and the
    // model H is exactly the Heyting algebra of that poset. The bridge is a
    // bijection between antichains and down-closed sets (order ideals): an
    // antichain a corresponds to its downward closure, and conversely an ideal
    // corresponds to its set of maximal elements. We store every H element by
    // its ideal as a bitset, which makes the operations clean.
    //
    // Under this encoding the problem's "0" (false) is the full poset (the
    // largest ideal) and "1" (true) is the empty ideal. Because truth is
    // measured by reverse inclusion of ideals, the connectives come out dual to
    // naive set logic:
    //
    // - conjunction a/\b is the union of the two ideals;
    // - disjunction a\/b is their intersection;
    // - implication a=>b is the smallest ideal C with ideal(a) | C covering
    //   ideal(b), which equals the downward closure of the maximal elements of
    //   ideal(b) that do not already lie in ideal(a);
    // - negation is a=>0 (the pseudocomplement), and == is (a=>b)/\(b=>a).
    //
    // These are precisely the Heyting operations, so a formula is "valid" iff
    // it evaluates to the top element 1 for every assignment of ideals to its
    // variables; classical-only laws such as X\/~X or ~~X=>X fail whenever the
    // poset is non-trivial, while genuine intuitionistic theorems survive.
    //
    // Implementation: enumerate all ideals by growing from the empty set one
    // maximal element at a time (the guarantee |H|<=100 keeps this small),
    // precompute the full operation tables over H, parse each formula by
    // precedence (~ then & then | then => then ==, with => right-associative
    // and == expanding to a conjunction of consecutive pairwise equivalences)
    // into an expression tree, and brute force every variable assignment using
    // table lookups.
    //
    // The cost is dominated by those assignments: a formula with v distinct
    // variables ranges each variable independently over all of H, so it needs
    // D^v evaluations, and one evaluation is just a linear walk over the tree
    // with O(1) table lookups. There is a single graph, hence a single D, so
    // the statement's "Sum(Di)<=10^6" cannot mean Sum(D)=K*D (that is at most
    // 20*100 and would be a vacuous bound). It only bites if Di is the per
    // formula work D^v, and indeed 10^6 = 100^3 is exactly the worst single
    // formula (D=100 with three variables). So this reads as a promise that the
    // overall number of assignments Sum(D^v) is bounded, which is precisely
    // what makes plain full enumeration pass.

    H.clear();
    h_index.clear();

    BS empty;
    H.push_back(empty);
    h_index[empty.to_string()] = 0;

    for(size_t front = 0; front < H.size(); front++) {
        BS cur = H[front];
        for(int x = 0; x < n; x++) {
            if(cur[x]) {
                continue;
            }

            BS need = down[x] & ~cur;
            if(need.count() != 1) {
                continue;
            }

            BS nxt = cur;
            nxt[x] = 1;
            string key = nxt.to_string();
            if(h_index.find(key) == h_index.end()) {
                h_index[key] = (int)H.size();
                H.push_back(nxt);
            }
        }
    }

    int d = (int)H.size();
    BS full;
    for(int i = 0; i < n; i++) {
        full[i] = 1;
    }

    one_idx = h_index[empty.to_string()];
    zero_idx = h_index[full.to_string()];

    and_tbl.assign(d, vector<int>(d));
    or_tbl.assign(d, vector<int>(d));
    imp_tbl.assign(d, vector<int>(d));
    eq_tbl.assign(d, vector<int>(d));
    not_tbl.assign(d, 0);

    for(int i = 0; i < d; i++) {
        for(int j = 0; j < d; j++) {
            and_tbl[i][j] = h_index[(H[i] | H[j]).to_string()];
            or_tbl[i][j] = h_index[(H[i] & H[j]).to_string()];

            BS res;
            for(int x = 0; x < n; x++) {
                if(H[j][x] && !H[i][x] && (up[x] & H[j]).count() == 1) {
                    res |= down[x];
                }
            }
            imp_tbl[i][j] = h_index[res.to_string()];
        }
    }

    for(int i = 0; i < d; i++) {
        not_tbl[i] = imp_tbl[i][zero_idx];
    }

    for(int i = 0; i < d; i++) {
        for(int j = 0; j < d; j++) {
            eq_tbl[i][j] = and_tbl[imp_tbl[i][j]][imp_tbl[j][i]];
        }
    }

    for(const string& formula: formulas) {
        toks.clear();
        for(size_t i = 0; i < formula.size(); i++) {
            char c = formula[i];
            if(c == ' ' || c == '\t' || c == '\r') {
                continue;
            }

            if(c == '0') {
                toks.push_back({T_ZERO, 0});
            } else if(c == '1') {
                toks.push_back({T_ONE, 0});
            } else if(c >= 'A' && c <= 'Z') {
                toks.push_back({T_VAR, c - 'A'});
            } else if(c == '(') {
                toks.push_back({T_LPAR, 0});
            } else if(c == ')') {
                toks.push_back({T_RPAR, 0});
            } else if(c == '~') {
                toks.push_back({T_NOT, 0});
            } else if(c == '&') {
                toks.push_back({T_AND, 0});
            } else if(c == '|') {
                toks.push_back({T_OR, 0});
            } else if(c == '=') {
                if(i + 1 < formula.size() && formula[i + 1] == '>') {
                    toks.push_back({T_IMP, 0});
                    i++;
                } else {
                    toks.push_back({T_EQ, 0});
                }
            }
        }
        toks.push_back({-1, 0});

        nodes.clear();
        memset(used, 0, sizeof(used));
        pos = 0;
        int root = parse_equiv();

        vector<int> vars;
        for(int v = 0; v < 26; v++) {
            if(used[v]) {
                vars.push_back(v);
            }
        }

        memset(cur_assign, 0, sizeof(cur_assign));
        vector<int> digit(vars.size(), 0);
        bool valid = true;
        while(true) {
            for(size_t p = 0; p < vars.size(); p++) {
                cur_assign[vars[p]] = digit[p];
            }

            if(eval(root) != one_idx) {
                valid = false;
                break;
            }

            size_t i = 0;
            for(; i < vars.size(); i++) {
                if(++digit[i] < d) {
                    break;
                }
                digit[i] = 0;
            }
            if(i == vars.size()) {
                break;
            }
        }

        cout << (valid ? "valid" : "invalid") << '\n';
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
