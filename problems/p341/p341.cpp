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

enum GateType { G_INPUT, G_NOT, G_DFF, G_AND, G_NAND, G_OR, G_NOR };

struct Junction {
    int type = -1;
    vector<int> inputs;
};

vector<Junction> junctions;
unordered_map<string, int> name_id;
vector<int> input_order;
vector<int> output_order;
vector<int> dff_ids;
vector<string> tick_inputs;

static string trim(const string& s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if(a == string::npos) {
        return "";
    }
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

static int get_id(const string& name) {
    auto it = name_id.find(name);
    if(it != name_id.end()) {
        return it->second;
    }
    int id = (int)junctions.size();
    name_id[name] = id;
    junctions.emplace_back();
    return id;
}

static int op_type(const string& op) {
    if(op == "NOT") {
        return G_NOT;
    }
    if(op == "DFF") {
        return G_DFF;
    }
    if(op == "AND") {
        return G_AND;
    }
    if(op == "NAND") {
        return G_NAND;
    }
    if(op == "OR") {
        return G_OR;
    }
    if(op == "NOR") {
        return G_NOR;
    }
    return -1;
}

void read() {
    string line;
    bool reading_values = false;
    while(getline(cin, line)) {
        if(!line.empty() && line[0] == '#') {
            continue;
        }
        string t = trim(line);
        if(t.empty()) {
            continue;
        }
        if(!reading_values) {
            if(t == "INPUT VALUES") {
                reading_values = true;
                continue;
            }
            if(t.rfind("INPUT(", 0) == 0) {
                size_t l = t.find('('), r = t.find(')');
                string name = trim(t.substr(l + 1, r - l - 1));
                int id = get_id(name);
                junctions[id].type = G_INPUT;
                input_order.push_back(id);
            } else if(t.rfind("OUTPUT(", 0) == 0) {
                size_t l = t.find('('), r = t.find(')');
                string name = trim(t.substr(l + 1, r - l - 1));
                int id = get_id(name);
                output_order.push_back(id);
            } else {
                size_t eq = t.find('=');
                string lhs = trim(t.substr(0, eq));
                string rhs = trim(t.substr(eq + 1));
                size_t l = rhs.find('('), r = rhs.find(')');
                string op = trim(rhs.substr(0, l));
                string args = rhs.substr(l + 1, r - l - 1);
                int id = get_id(lhs);
                junctions[id].type = op_type(op);
                if(junctions[id].type == G_DFF) {
                    dff_ids.push_back(id);
                }
                stringstream ss(args);
                string tok;
                while(getline(ss, tok, ',')) {
                    tok = trim(tok);
                    if(!tok.empty()) {
                        int arg_id = get_id(tok);
                        junctions[id].inputs.push_back(arg_id);
                    }
                }
            }
        } else {
            tick_inputs.push_back(t);
        }
    }
}

void solve() {
    // This is again a problem where arguably parsing is the hardest part. We
    // can an AST for the expression and then process through it at each query:
    // there are of the order of 5000 nodes, and of the order of 500 queries, so
    // a naive implementation should work.

    int N = (int)junctions.size();
    int D = (int)dff_ids.size();
    vector<int> dff_pos(N, -1);
    for(int i = 0; i < D; i++) {
        dff_pos[dff_ids[i]] = i;
    }
    vector<int> dff_out(D, 0);
    vector<int> val(N, 0), seen(N, 0);
    int stamp = 0;

    function<int(int)> eval = [&](int u) -> int {
        if(seen[u] == stamp) {
            return val[u];
        }
        seen[u] = stamp;
        const Junction& j = junctions[u];
        switch(j.type) {
            case G_INPUT:
                break;
            case G_DFF:
                val[u] = dff_out[dff_pos[u]];
                break;
            case G_NOT:
                val[u] = 1 - eval(j.inputs[0]);
                break;
            case G_AND: {
                int r = 1;
                for(int v: j.inputs) {
                    r &= eval(v);
                }
                val[u] = r;
                break;
            }
            case G_NAND: {
                int r = 1;
                for(int v: j.inputs) {
                    r &= eval(v);
                }
                val[u] = 1 - r;
                break;
            }
            case G_OR: {
                int r = 0;
                for(int v: j.inputs) {
                    r |= eval(v);
                }
                val[u] = r;
                break;
            }
            case G_NOR: {
                int r = 0;
                for(int v: j.inputs) {
                    r |= eval(v);
                }
                val[u] = 1 - r;
                break;
            }
        }
        return val[u];
    };

    string out;
    for(const string& tick: tick_inputs) {
        stamp++;
        for(size_t i = 0; i < input_order.size(); i++) {
            int u = input_order[i];
            val[u] = tick[i] - '0';
            seen[u] = stamp;
        }
        out.assign(output_order.size(), '0');
        for(size_t i = 0; i < output_order.size(); i++) {
            out[i] = char('0' + eval(output_order[i]));
        }
        cout << out << '\n';
        vector<int> next_dff(D);
        for(int d = 0; d < D; d++) {
            next_dff[d] = eval(junctions[dff_ids[d]].inputs[0]);
        }
        dff_out.swap(next_dff);
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
