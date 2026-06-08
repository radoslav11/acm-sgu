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

int p_debt, o_debt, s_debt;
int n;
vector<int> crystal_type;

void read() {
    cin >> p_debt >> o_debt >> s_debt;
    cin >> n;
    crystal_type.assign(n, 0);
    for(int i = 0; i < n; i++) {
        string s;
        cin >> s;
        int t = 0;
        for(int j = 0; j < 3; j++) {
            if(s[j] == 'B') {
                t |= (1 << j);
            }
        }
        crystal_type[i] = t;
    }
}

void solve() {
    // It's a bit hard to get a direct heuristic, but we can notice that the
    // problem is defined by P, O, S, and the counts of the 2^3=8 different
    // possible responses for a crystal. We can try to get a concrete
    // construction, like first using all BSS, SBS and SSB crystals, and then
    // doing a case analysis, but a simpler approach is to instead consider all
    // possible 8! orders of response priority, and all 3! orders of priority in
    // terms of the people. Then we will run the greedy that always tries to
    // match by priority.

    array<int, 8> counts{};
    for(int t: crystal_type) {
        counts[t]++;
    }
    array<int, 3> debts = {p_debt, o_debt, s_debt};

    array<int, 8> type_perm;
    iota(type_perm.begin(), type_perm.end(), 0);
    array<int, 3> people_perm = {0, 1, 2};
    const char names[] = {'P', 'O', 'S'};

    do {
        do {
            auto rem = counts;
            array<array<int, 8>, 3> assign{};
            bool ok = true;
            for(int pi = 0; pi < 3; pi++) {
                int person = people_perm[pi];
                int debt = debts[person];
                for(int ti = 0; ti < 8; ti++) {
                    int t = type_perm[ti];
                    if(rem[t] == 0 || debt <= 0) {
                        continue;
                    }
                    int value = ((t >> person) & 1) ? 2 : 1;
                    int take = min(rem[t], debt / value);
                    assign[person][t] += take;
                    rem[t] -= take;
                    debt -= take * value;
                }
                for(int ti = 0; ti < 8 && debt > 0; ti++) {
                    int t = type_perm[ti];
                    if(rem[t] == 0) {
                        continue;
                    }
                    int value = ((t >> person) & 1) ? 2 : 1;
                    assign[person][t]++;
                    rem[t]--;
                    debt -= value;
                }
                if(debt > 0) {
                    ok = false;
                    break;
                }
            }
            if(ok) {
                string result(n, 'P');
                for(int i = 0; i < n; i++) {
                    int t = crystal_type[i];
                    for(int p = 0; p < 3; p++) {
                        if(assign[p][t] > 0) {
                            assign[p][t]--;
                            result[i] = names[p];
                            break;
                        }
                    }
                }
                cout << result << '\n';
                return;
            }
        } while(next_permutation(type_perm.begin(), type_perm.end()));
    } while(next_permutation(people_perm.begin(), people_perm.end()));

    cout << "no solution\n";
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
