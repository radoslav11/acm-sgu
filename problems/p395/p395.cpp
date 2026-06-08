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
vector<int> rec_type;
vector<int> rec_arg;
vector<string> names;

void read() {
    cin >> n;
    rec_type.assign(n, 0);
    rec_arg.assign(n, 0);
    map<string, int> id;
    for(int i = 0; i < n; i++) {
        char c;
        cin >> c;
        if(c == '=') {
            rec_type[i] = 2;
            cin >> rec_arg[i];
        } else {
            rec_type[i] = (c == '+') ? 0 : 1;
            string nm;
            cin >> nm;
            auto it = id.find(nm);
            if(it == id.end()) {
                int k = names.size();
                id[nm] = k;
                names.push_back(nm);
                rec_arg[i] = k;
            } else {
                rec_arg[i] = it->second;
            }
        }
    }
}

void solve() {
    // One left-to-right pass over the records, emitting each original
    // record and only inserting an extra one when the next record cannot
    // be consumed as-is. present[x] = 1 for the named guests inside whose
    // name still appears later in the input (others fold into the
    // anonymous bag), anon is a stack of the names currently filling the
    // anonymous slots, and cnt = (# present named) + anon.size(). Leaving
    // is a set operation, so person intervals can cross and a nested-
    // bracket model would be wrong; the genuinely hard cells are '= k'
    // because they pin the exact headcount.
    //
    // Name collapse: when we place a '+ x' and last_at[x] == i, x's name
    // never appears again so that guest can be treated as anonymous and
    // pushed onto anon instead of into present. This is the trick that
    // keeps the state size linear - a brute force that kept the present
    // set explicit would generate every subset while closing m guests at
    // a single '= 0' cell, blowing up to 2^m states.
    //
    // The forced cases first. '+ x' with x already inside is a '++' that
    // needs a separator: emit '- x' first, then the '+ x'. '- x' with x
    // absent needs a feed: emit '+ x' first, then the '- x' (the pair
    // leaves cnt unchanged). Everything else just consumes the record.
    //
    // The interesting cells are '= k'. Each insert there can do "double
    // duty" - serve the cell AND a future record - if we pick the right
    // person. The two priorities below are exactly the ones that fuse the
    // double uses; each is a local exchange argument over the next
    // occurrence of each name.
    //
    // '= k' below cnt evicts in this order. Guests whose next record is
    // '+ x' go first, in ascending position of that '+ x': we would have
    // had to evict them before that record anyway, so the insert here IS
    // the separator we would have paid later (one insert serves two
    // needs). Then anon, popped LIFO: anonymous guests have no future
    // obligation, so closing one costs exactly one insert and nothing
    // else. Then the remaining named guests, whose next record is '- x',
    // by descending position of that close: evicting one of these breaks
    // a natural pair (the future '- x' will need a feed), so each is
    // worth one extra insert later; among them the latest close is the
    // most likely to be broken anyway by some later '= k', so it is the
    // least valuable to keep.
    //
    // '= k' above cnt admits feeds before fresh guests. A name x is a
    // feed candidate iff x is absent and its next record is '- x';
    // admitting x now means the inserted '+ x' is reused by that '- x'
    // for free (one insert serves two needs). Earliest-close candidates
    // first because their entries get redeemed soonest. Anything left
    // over draws a name from next_fresh(), an enumerator of unused
    // lowercase strings.

    int d = names.size();
    vector<vector<int>> occ(d);
    for(int i = 0; i < n; i++) {
        if(rec_type[i] != 2) {
            occ[rec_arg[i]].push_back(i);
        }
    }
    vector<int> last_at(d, -1);
    for(int x = 0; x < d; x++) {
        if(!occ[x].empty()) {
            last_at[x] = occ[x].back();
        }
    }
    auto next_occ = [&](int x, int from) {
        auto& v = occ[x];
        auto it = lower_bound(v.begin(), v.end(), from);
        return it == v.end() ? INT_MAX : *it;
    };

    set<string> used_names(names.begin(), names.end());
    vector<int> digits(1, 0);
    auto next_fresh = [&]() {
        while(true) {
            string s;
            for(int v: digits) {
                s.push_back((char)('a' + v));
            }
            int p = (int)digits.size() - 1;
            while(p >= 0 && digits[p] == 25) {
                digits[p] = 0;
                p--;
            }
            if(p < 0) {
                digits.assign(digits.size() + 1, 0);
            } else {
                digits[p]++;
            }
            if(!used_names.count(s)) {
                return s;
            }
        }
    };

    vector<char> present(d, 0);
    vector<string> anon;
    int cnt = 0;
    vector<string> out;

    auto open_named = [&](int x) {
        out.push_back("+ " + names[x]);
        present[x] = 1;
        cnt++;
    };
    auto close_named = [&](int x) {
        out.push_back("- " + names[x]);
        present[x] = 0;
        cnt--;
    };
    auto open_anon = [&](const string& nm) {
        out.push_back("+ " + nm);
        anon.push_back(nm);
        cnt++;
    };
    auto close_anon = [&]() {
        out.push_back("- " + anon.back());
        anon.pop_back();
        cnt--;
    };

    for(int i = 0; i < n; i++) {
        int t = rec_type[i], a = rec_arg[i];
        if(t == 0) {
            if(present[a]) {
                close_named(a);
            }
            if(last_at[a] == i) {
                open_anon(names[a]);
            } else {
                open_named(a);
            }
        } else if(t == 1) {
            if(present[a]) {
                close_named(a);
            } else {
                out.push_back("+ " + names[a]);
                out.push_back("- " + names[a]);
            }
        } else {
            int k = a;
            if(cnt < k) {
                int need = k - cnt;
                vector<pair<int, int>> cand;
                for(int x = 0; x < d; x++) {
                    if(!present[x]) {
                        int nx = next_occ(x, i);
                        if(nx != INT_MAX && rec_type[nx] == 1) {
                            cand.push_back({nx, x});
                        }
                    }
                }
                sort(cand.begin(), cand.end());
                int done = 0;
                for(auto& pr: cand) {
                    if(done >= need) {
                        break;
                    }
                    open_named(pr.second);
                    done++;
                }
                while(done < need) {
                    open_anon(next_fresh());
                    done++;
                }
            } else if(cnt > k) {
                int need = cnt - k;
                vector<pair<int, int>> liab, useful;
                for(int x = 0; x < d; x++) {
                    if(present[x]) {
                        int nx = next_occ(x, i);
                        if(nx != INT_MAX && rec_type[nx] == 0) {
                            liab.push_back({nx, x});
                        } else {
                            useful.push_back({nx, x});
                        }
                    }
                }
                sort(liab.begin(), liab.end());
                sort(useful.begin(), useful.end(), greater<>());
                int rem = need;
                for(auto& pr: liab) {
                    if(rem == 0) {
                        break;
                    }
                    close_named(pr.second);
                    rem--;
                }
                while(rem > 0 && !anon.empty()) {
                    close_anon();
                    rem--;
                }
                for(auto& pr: useful) {
                    if(rem == 0) {
                        break;
                    }
                    close_named(pr.second);
                    rem--;
                }
            }
            out.push_back("= " + to_string(k));
        }
    }

    cout << out.size() << '\n';
    for(auto& line: out) {
        cout << line << '\n';
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
