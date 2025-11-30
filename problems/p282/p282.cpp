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

int64_t mod_pow(int64_t b, int64_t e, int64_t mod) {
    int64_t res = 1;
    b %= mod;
    while(e) {
        if(e & 1) {
            res = res * b % mod;
        }
        b = b * b % mod;
        e >>= 1;
    }
    return res;
}

int64_t mod_inverse(int64_t a, int64_t mod) { return mod_pow(a, mod - 2, mod); }

int n, m;
int64_t p;
vector<int64_t> fact;

int64_t gen(int last, int sum_left, vector<int>& cur) {
    if(sum_left == 0) {
        int64_t ex = 0;
        for(int c: cur) {
            ex += c / 2;
        }
        for(int i = 0; i < (int)cur.size(); i++) {
            for(int j = i + 1; j < (int)cur.size(); j++) {
                ex += gcd(cur[i], cur[j]);
            }
        }

        int64_t prod_ci = 1;
        for(int c: cur) {
            prod_ci = prod_ci * c % p;
        }
        map<int, int> freq;
        for(int c: cur) {
            freq[c]++;
        }

        int64_t prod_mfact = 1;
        for(auto pr: freq) {
            prod_mfact = prod_mfact * fact[pr.second] % p;
        }

        int64_t inv_denom = mod_inverse(prod_ci * prod_mfact % p, p);
        int64_t fix = mod_pow(m, ex, p);
        int64_t contrib = fix * inv_denom % p;
        return contrib;
    }
    int64_t total = 0;
    for(int x = min(last, sum_left); x >= 1; x--) {
        cur.push_back(x);
        total = (total + gen(x, sum_left - x, cur)) % p;
        cur.pop_back();
    }
    return total;
}

void read() { cin >> n >> m >> p; }

void solve() {
    // We use Burnside's lemma to count distinct colored complete graphs up to
    // isomorphism. The actions are permutations of the vertices, and by
    // Burnside's lemma we need to count fixed points for each permutation. A
    // coloring is fixed by permutation p if edge (i,j) has the same color as
    // edge (p[i], p[j]).
    //
    // Consider a cycle in the permutation: i[1], i[2], ..., i[k] where i[j+1] =
    // p[i[j]]. For the coloring to be fixed, edge (i[1], i[2]) must have the
    // same color as (p[i[1]], p[i[2]]) = (i[2], i[3]), and so on. This creates
    // orbits of edges that must share the same color.
    //
    // For edges between two different cycles of sizes c1 and c2: choosing any
    // x, y from the two cycles, we get (x,y) = (p[x], p[y]) = (p[p[x]],
    // p[p[y]]), etc. This visits every gcd(c1, c2) pair, yielding gcd(c1, c2)
    // orbits.
    //
    // For edges within the same cycle of size c: we have c*(c-1)/2 edges. For
    // any edge (x,y), we have (x,y) = (p[x], p[y]). Edges can be grouped by
    // their distance (y-x) mod c, and edges at distance k are in the same orbit
    // as edges at distance c-k (since we have a cycle). This gives c/2 orbits
    // per cycle.
    //
    // Computing this for every permutation is exponential in n. However, we
    // only care about the cycle structure, not the specific permutation. We
    // group by partitions of n (cycle sizes c1, c2, ..., ck). The number of
    // partitions of n is manageable - for n=53 it's only ~200k
    // (https://oeis.org/A000041).
    //
    // To summarize, we iterate through all partitions into cycles and count:
    //
    //     - Number of permutations with that cycle type: n! / (product of all
    //       ci * product of mult(i)!), where mult(i) is the frequency of cycle
    //       size i.
    //     - Number of edge orbits: sum of (ci / 2) over all cycles (intra-cycle
    //       orbits) + sum of gcd(ci, cj) over all pairs i < j (inter-cycle
    //       orbits).
    //     - Number of fixed colorings for that partition: m^(number of edge
    //       orbits).

    fact.assign(n + 1, 1);
    for(int i = 1; i <= n; i++) {
        fact[i] = fact[i - 1] * i % p;
    }

    vector<int> cur;
    int64_t ans = gen(n, n, cur);
    cout << ans << endl;
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
