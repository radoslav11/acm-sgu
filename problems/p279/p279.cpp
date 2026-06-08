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

int n;
vector<int> a;

void read() {
    cin >> n;
    a.resize(n);
    cin >> a;
}

void solve() {
    // The two conditions force a lot of structure: the unprimed objects appear
    // in some order pi_1, pi_2, ..., pi_n, the primed objects appear in that
    // exact same order, and every i sits before its own i'. So a bipermutation
    // is nothing more than a merge of the sequence pi with a copy of itself
    // (the primes), where the r-th prime lands somewhere after the r-th
    // unprimed.
    //
    // Against that picture b_ij just selects, for an ordered pair (i, j), which
    // copy of j to compare i against - the unprimed j when j is the larger
    // value, the primed j' when j is the smaller one. So a(i) counts the j
    // whose selected copy ends up after i, which reading off the merge is
    //
    //     a(i) = #{j > i : i before j in pi} + #{j < i : i before j' in pi}.
    //
    // The hint (i <. b_ij and j <. b_jk imply i <. b_ik) says this "i before
    // b_ij" relation is transitive, and that consistency is what lets us
    // rebuild an ordering from the a-values instead of searching blindly.
    //
    // We grow pi from the back, fixing one value at a time: step k pins down
    // pi_{n-k}, so we start by choosing the last unprimed value pi_n and end
    // with pi_1. Once v = pi_r is set, every value chosen so far sits to the
    // right of v in pi, so the first count in a(v) is just how many of them
    // exceed v, exactly larger_picked[v], while the second is how many of the
    // smaller ones have their prime land to the right of v too. The first part
    // settles as soon as we pick v; the second is still ours to set, between 0
    // and the number of smaller values placed. So v can take step k exactly
    // when larger_picked[v] <= a(v) <= k; step 0, in particular, admits only
    // values with a(v) = 0. But this is only a legality test - several values
    // may pass it at once, and which one we actually take is settled by the
    // prime placement below.
    //
    // It remains to decide where the primes sit. Scanning pi from the right,
    // let g count the placed values whose prime lies to the right of v; since
    // the primes keep pi's order, those are always the g most recently placed
    // ones - the prefix picked[0..g-1]. Placing v then needs exactly
    // need = a(v) - larger_picked[v] of the smaller placed values to land in
    // that prefix. smaller_pos[v] is the vector of picked indices that hold a
    // value smaller than v; we want exactly need of them to fall inside the
    // prefix, so g has to sit past entry need-1 and at or before entry need:
    //
    //     g_lo = smaller_pos[v][need - 1] + 1,  g_hi = smaller_pos[v][need],
    //
    // with g_lo = 0 when need = 0 (nothing to include) and g_hi = k when
    // need = total_smaller (nothing left to exclude). The g only grows as we
    // move left, so it must also be above g_prev. Among the legal values we
    // keep the one whose forced g = max(g_lo, g_prev) is smallest, since a low
    // g leaves the most slack for the rest; ties go to the smallest value,
    // because picking v bumps larger_picked for every value below it, dragging
    // those toward the dead state larger_picked > a(.), so the smallest legal
    // value is the one that endangers the fewest others. A step with no legal
    // value means NO; otherwise f_r = n - g turns each cut back into a prime
    // count and the interleaving is emitted directly.

    for(int i = 0; i < n; i++) {
        if(a[i] < 0 || a[i] > n - 1) {
            cout << "NO\n";
            return;
        }
    }

    vector<int> picked;
    vector<bool> used(n + 1, false);
    vector<int> larger_picked(n + 1, 0);
    vector<vector<int>> smaller_pos(n + 1);
    vector<int> g(n, 0);
    int g_prev = 0;

    for(int k = 0; k < n; k++) {
        int best = -1, best_g = INT_MAX;
        for(int v = 1; v <= n; v++) {
            if(used[v]) {
                continue;
            }

            int need = a[v - 1] - larger_picked[v];
            int total_smaller = (int)smaller_pos[v].size();
            if(need < 0 || need > total_smaller) {
                continue;
            }

            int g_lo = (need == 0) ? 0 : smaller_pos[v][need - 1] + 1;
            int g_hi = (need == total_smaller) ? k : smaller_pos[v][need];

            int cur_g = max(g_lo, g_prev);
            if(cur_g > g_hi) {
                continue;
            }

            if(cur_g < best_g) {
                best = v;
                best_g = cur_g;
            }
        }

        if(best == -1) {
            cout << "NO\n";
            return;
        }

        picked.push_back(best);
        used[best] = true;
        g[k] = best_g;
        g_prev = best_g;
        for(int v = 1; v <= n; v++) {
            if(v < best) {
                larger_picked[v]++;
            } else if(v > best) {
                smaller_pos[v].push_back(k);
            }
        }
    }

    vector<int> pi(n + 1), f(n + 1);
    for(int r = 1; r <= n; r++) {
        pi[r] = picked[n - r];
        f[r] = n - g[n - r];
    }

    vector<int> seq;
    seq.reserve(2 * n);
    int next_primed = 1;
    for(int r = 1; r <= n; r++) {
        while(next_primed <= f[r]) {
            seq.push_back(-pi[next_primed]);
            next_primed++;
        }

        seq.push_back(pi[r]);
    }

    cout << "YES\n";
    cout << seq << '\n';
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
