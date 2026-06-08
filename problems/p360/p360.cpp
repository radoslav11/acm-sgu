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

struct ast_node {
    char kind;
    char val;
    int func_id;
    vector<char> args;
};

int n, m;
vector<string> grid;
int start_r, start_c;
string source;

map<char, int> func_id_of;
vector<vector<ast_node>> func_body;
vector<vector<char>> func_params;

int get_or_create_func(char name) {
    auto it = func_id_of.find(name);
    if(it != func_id_of.end()) {
        return it->second;
    }
    int id = (int)func_id_of.size();
    func_id_of[name] = id;
    func_body.emplace_back();
    func_params.emplace_back();
    return id;
}

int parse_pos = 0;

vector<char> parse_arg_letters() {
    parse_pos++;
    vector<char> args;
    while(source[parse_pos] != ')') {
        if(source[parse_pos] != ',') {
            args.push_back(source[parse_pos]);
        }
        parse_pos++;
    }
    parse_pos++;
    return args;
}

vector<ast_node> parse_block() {
    vector<ast_node> ops;
    parse_pos++;
    while(source[parse_pos] != '}') {
        char c = source[parse_pos];
        if(c == 'L' || c == 'R' || c == 'C') {
            ops.push_back({'C', c, -1, {}});
            parse_pos++;
        } else if(islower((unsigned char)c)) {
            parse_pos++;
            int fid = get_or_create_func(c);
            vector<char> args = parse_arg_letters();
            ops.push_back({'F', 0, fid, args});
        } else {
            ops.push_back({'A', c, -1, {}});
            parse_pos++;
        }
    }
    parse_pos++;
    return ops;
}

void parse_function_def() {
    char name = source[parse_pos];
    parse_pos++;
    int fid = get_or_create_func(name);
    func_params[fid] = parse_arg_letters();
    func_body[fid] = parse_block();
}

void read() {
    cin >> n >> m;
    grid.resize(n);
    for(int i = 0; i < n; i++) {
        cin >> grid[i];
    }
    char ch;
    while(cin.get(ch)) {
        if(!isspace((unsigned char)ch)) {
            source += ch;
        }
    }
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            if(grid[i][j] == 'R') {
                start_r = i;
                start_c = j;
            }
        }
    }
}

int dr[4] = {-1, 0, 1, 0};
int dc[4] = {0, 1, 0, -1};

int total_commands;
vector<pair<int, int>> visit_order;
set<pair<int, int>> visited_set;

struct call_result {
    int end_r, end_c, end_d;
    int cmd_count;
    vector<pair<int, int>> stars;
};

map<tuple<int, int, int, int, int>, call_result> cache;

int pack_args(const vector<char>& args) {
    int p = 0;
    for(char c: args) {
        int v = (c == 'L' ? 0 : (c == 'R' ? 1 : 2));
        p = p * 3 + v;
    }
    return p;
}

void register_star(int r, int c) {
    if(grid[r][c] == '*' && !visited_set.count({r, c})) {
        visited_set.insert({r, c});
        visit_order.push_back({r, c});
    }
}

void apply_command(
    char cmd, int& r, int& c, int& d, vector<pair<int, int>>& star_log
) {
    if(cmd == 'L') {
        d = (d + 3) % 4;
    } else if(cmd == 'R') {
        d = (d + 1) % 4;
    } else if(cmd == 'C') {
        int nr = r + dr[d];
        int nc = c + dc[d];
        if(nr >= 0 && nr < n && nc >= 0 && nc < m) {
            r = nr;
            c = nc;
            if(grid[r][c] == '*') {
                star_log.push_back({r, c});
                register_star(r, c);
            }
        }
    }
    total_commands++;
}

call_result execute_function(
    int fid, const vector<char>& bindings, int r, int c, int d
);

call_result execute_block(
    const vector<ast_node>& ops, const vector<char>& params,
    const vector<char>& bindings, int r, int c, int d
) {
    call_result res;
    res.cmd_count = 0;
    for(const auto& op: ops) {
        if(op.kind == 'C') {
            int prev = total_commands;
            apply_command(op.val, r, c, d, res.stars);
            res.cmd_count += total_commands - prev;
        } else if(op.kind == 'A') {
            char cmd = 0;
            for(int i = 0; i < (int)params.size(); i++) {
                if(params[i] == op.val) {
                    cmd = bindings[i];
                    break;
                }
            }
            int prev = total_commands;
            apply_command(cmd, r, c, d, res.stars);
            res.cmd_count += total_commands - prev;
        } else {
            vector<char> sub;
            sub.reserve(op.args.size());
            for(char a: op.args) {
                if(a == 'L' || a == 'R' || a == 'C') {
                    sub.push_back(a);
                } else {
                    for(int i = 0; i < (int)params.size(); i++) {
                        if(params[i] == a) {
                            sub.push_back(bindings[i]);
                            break;
                        }
                    }
                }
            }
            call_result child = execute_function(op.func_id, sub, r, c, d);
            r = child.end_r;
            c = child.end_c;
            d = child.end_d;
            for(const auto& s: child.stars) {
                res.stars.push_back(s);
            }
            res.cmd_count += child.cmd_count;
        }
    }
    res.end_r = r;
    res.end_c = c;
    res.end_d = d;
    return res;
}

call_result execute_function(
    int fid, const vector<char>& bindings, int r, int c, int d
) {
    auto key = make_tuple(fid, pack_args(bindings), r, c, d);
    auto it = cache.find(key);
    if(it != cache.end()) {
        const auto& cached = it->second;
        for(const auto& [sr, sc]: cached.stars) {
            register_star(sr, sc);
        }
        total_commands += cached.cmd_count;
        return cached;
    }
    call_result res =
        execute_block(func_body[fid], func_params[fid], bindings, r, c, d);
    cache[key] = res;
    return res;
}

void solve() {
    // Pure implementation simulation. The robot executes at most 1000 commands,
    // so we just run the B++ program directly: parse each function definition
    // into a list of operators (direct command L/R/C, argument reference, or
    // function call) and execute m() from the robot's start cell. Function
    // arguments are substituted at every call site, since each argument is
    // itself L, R, C or an argument of the caller.
    //
    // To keep the work bounded even when the call tree branches heavily, we
    // memoize dp[(func_id, bound_args, start_state)] -> (end_state, ordered
    // stars stepped on during the call, command count). On a cache hit we
    // replay the cached star list (registering any not-yet-visited cell
    // globally) and jump to the cached end state without re-walking the
    // body, so each unique key is executed at most once.

    parse_pos = 0;
    while(parse_pos < (int)source.size()) {
        parse_function_def();
    }

    int main_id = get_or_create_func('m');
    total_commands = 0;
    execute_function(main_id, {}, start_r, start_c, 0);

    cout << visit_order.size() << '\n';
    for(auto [r, c]: visit_order) {
        cout << (r + 1) << ' ' << (c + 1) << '\n';
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
