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

template<size_t N>
class GaussBitset {
  public:
    int n_var;
    vector<bitset<N + 1>> rows;
    vector<int> where;
    bool solvable = false;

    GaussBitset(int n_var_) : n_var(n_var_) {}

    void add_equation(const vector<int>& coeffs, int rhs) {
        bitset<N + 1> row;
        for(int i = 0; i < n_var; i++) {
            if(coeffs[i] % 2) {
                row[i] = 1;
            }
        }
        row[n_var] = rhs % 2;
        rows.push_back(row);
    }

    void eliminate() {
        int n_eq = (int)rows.size();
        where.assign(n_var, -1);
        int row = 0;
        for(int col = 0; col < n_var && row < n_eq; col++) {
            int sel = -1;
            for(int i = row; i < n_eq; i++) {
                if(rows[i][col]) {
                    sel = i;
                    break;
                }
            }
            if(sel == -1) {
                continue;
            }
            swap(rows[sel], rows[row]);
            where[col] = row;
            for(int i = 0; i < n_eq; i++) {
                if(i != row && rows[i][col]) {
                    rows[i] ^= rows[row];
                }
            }
            row++;
        }
        solvable = true;
        for(int i = row; i < n_eq; i++) {
            if(rows[i][n_var]) {
                solvable = false;
                break;
            }
        }
    }

    bool has_solution() const { return solvable; }

    vector<int> free_variables() const {
        vector<int> fv;
        for(int v = 0; v < n_var; v++) {
            if(where[v] == -1) {
                fv.push_back(v);
            }
        }
        return fv;
    }

    vector<int> any_solution() const {
        vector<int> x(n_var, 0);
        for(int v = 0; v < n_var; v++) {
            if(where[v] != -1) {
                x[v] = rows[where[v]][n_var];
            }
        }
        return x;
    }
};

int n;
vector<vector<int>> groups;
vector<int> initial;

void read() {
    cin >> n;
    groups.assign(n, {});
    for(int i = 0; i < n; i++) {
        int k;
        cin >> k;
        groups[i].resize(k);
        for(auto& x: groups[i]) {
            cin >> x;
            x--;
        }
    }
    initial.assign(n, 0);
    for(auto& x: initial) {
        cin >> x;
    }
}

void solve() {
    // To solve this, we can solve the problem twice - once to try and activate
    // everything to be white, and once to try to have everything black. WLOG,
    // let's try to make everything black, and have black = 1, and white = 0.
    // Let's also have n variables x1, ..., xn, where xi = 1 if we activated a
    // cell. This means that we have N linear equations defined as x[i] +
    // sum(x[neighbours(i)]) = 1 mod 2. This can be solved with Gauss in O(N^3),
    // and optimized with bitsets further to O(N^3/w). We implement the
    // optimized solution here.
    //
    // Something interesting, but less relevant for the problem: if the problem
    // was over a R x C grid, we can actually only create min(R, C) variables
    // rather than the full R*C, because the first row/column in a grid implies
    // all others.
    
    for(int target: {1, 0}) {
        vector<vector<int>> mat(n, vector<int>(n, 0));
        for(int i = 0; i < n; i++) {
            for(int c: groups[i]) {
                mat[c][i] = 1;
            }
        }

        GaussBitset<200> g(n);
        for(int c = 0; c < n; c++) {
            g.add_equation(mat[c], initial[c] ^ target);
        }

        g.eliminate();
        if(!g.has_solution()) {
            continue;
        }
        auto sol = g.any_solution();
        vector<int> clicks;
        for(int i = 0; i < n; i++) {
            if(sol[i]) {
                clicks.push_back(i + 1);
            }
        }
        cout << clicks.size() << '\n';
        for(size_t i = 0; i < clicks.size(); i++) {
            cout << clicks[i] << " \n"[i + 1 == clicks.size()];
        }
        if(clicks.empty()) {
            cout << '\n';
        }
        return;
    }
    cout << -1 << '\n';
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
