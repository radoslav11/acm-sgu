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
void read() { cin >> n; }

void do_print_weigh(const vector<int>& l, const vector<int>& r, int lev) {
    cout << string(lev * 2, ' ') << "weigh ";
    for(size_t i = 0; i < l.size(); i++) {
        if(i > 0) {
            cout << "+";
        }
        cout << l[i];
    }
    cout << " vs ";
    for(size_t i = 0; i < r.size(); i++) {
        if(i > 0) {
            cout << "+";
        }
        cout << r[i];
    }
    cout << "\n";
}

class Solution {
  public:
    vector<int> lhs;
    vector<int> rhs;
    Solution* equal_branch;
    Solution* less_branch;
    Solution* greater_branch;
    int fake_if_leaf;

    Solution(vector<int> _lhs, vector<int> _rhs) {
        lhs = _lhs;
        rhs = _rhs;
        equal_branch = nullptr;
        less_branch = nullptr;
        greater_branch = nullptr;
        fake_if_leaf = 0;
    }

    Solution(int f) {
        fake_if_leaf = f;
        equal_branch = nullptr;
        less_branch = nullptr;
        greater_branch = nullptr;
    }

    static Solution* build_unknown(
        const vector<int>& suspects, const vector<int>& known_good
    );

    static Solution* build_signed(
        const vector<int>& light, const vector<int>& heavy,
        const vector<int>& known_good
    );

    void print(int lev) const {
        if(fake_if_leaf > 0) {
            cout << string(lev * 2, ' ') << "fake " << fake_if_leaf << "\n";
            return;
        }
        do_print_weigh(lhs, rhs, lev);
        if(less_branch) {
            cout << string(lev * 2, ' ') << "case <:\n";
            less_branch->print(lev + 1);
        }
        if(equal_branch) {
            cout << string(lev * 2, ' ') << "case =:\n";
            equal_branch->print(lev + 1);
        }
        if(greater_branch) {
            cout << string(lev * 2, ' ') << "case >:\n";
            greater_branch->print(lev + 1);
        }
        cout << string(lev * 2, ' ') << "end\n";
    }
};

int pow3(int k) {
    int p = 1;
    while(k--) {
        p *= 3;
    }

    return p;
}

Solution* Solution::build_unknown(
    const vector<int>& suspects, const vector<int>& known_good
) {
    int sz = suspects.size();
    if(sz == 0) {
        return nullptr;
    }
    if(sz == 1) {
        return new Solution(suspects[0]);
    }

    if(!known_good.empty()) {
        int k = 0;
        while((pow3(k) + 1) / 2 < sz) {
            k++;
        }
        int b = min(pow3(k - 1), sz);

        vector<int> weighed(suspects.begin(), suspects.begin() + b);
        vector<int> remaining(suspects.begin() + b, suspects.end());
        vector<int> good_on_scale(known_good.begin(), known_good.begin() + b);

        Solution* node = new Solution(weighed, good_on_scale);
        node->less_branch = build_signed(weighed, {}, known_good);
        node->greater_branch = build_signed({}, weighed, known_good);
        if(!remaining.empty()) {
            vector<int> new_good = known_good;
            new_good.insert(new_good.end(), weighed.begin(), weighed.end());
            node->equal_branch = build_unknown(remaining, new_good);
        }
        return node;
    }

    int k = 0;
    while((pow3(k) - 1) / 2 < sz) {
        k++;
    }
    int a = (pow3(k - 1) - 1) / 2;
    a = max(a, 1);
    a = min(a, (sz - 1) / 2);

    vector<int> left(suspects.begin(), suspects.begin() + a);
    vector<int> right(suspects.begin() + a, suspects.begin() + 2 * a);
    vector<int> off(suspects.begin() + 2 * a, suspects.end());

    Solution* node = new Solution(left, right);
    vector<int> new_known;
    new_known.insert(new_known.end(), left.begin(), left.end());
    new_known.insert(new_known.end(), right.begin(), right.end());

    node->less_branch = build_signed(left, right, off);
    node->greater_branch = build_signed(right, left, off);
    if(!off.empty()) {
        node->equal_branch = build_unknown(off, new_known);
    }
    return node;
}

Solution* Solution::build_signed(
    const vector<int>& light, const vector<int>& heavy,
    const vector<int>& known_good
) {
    int L = light.size(), H = heavy.size();
    int tot = L + H;
    if(tot == 0) {
        return nullptr;
    }
    if(tot == 1) {
        return new Solution(light.empty() ? heavy[0] : light[0]);
    }
    if(tot == 2) {
        if(L == 0) {
            Solution* node = new Solution({heavy[0]}, {heavy[1]});
            node->less_branch = new Solution(heavy[1]);
            node->greater_branch = new Solution(heavy[0]);
            return node;
        } else if(H == 0) {
            Solution* node = new Solution({light[0]}, {light[1]});
            node->less_branch = new Solution(light[0]);
            node->greater_branch = new Solution(light[1]);
            return node;
        } else {
            Solution* node = new Solution({light[0]}, {known_good[0]});
            node->less_branch = new Solution(light[0]);
            node->equal_branch = new Solution(heavy[0]);
            return node;
        }
    }

    int s = (tot + 1) / 3;
    int h = min(s, H / 2);
    int l = s - h;
    if(2 * l > L) {
        l = L / 2;
        h = s - l;
    }

    vector<int> lp, rp;
    for(int i = 0; i < h; i++) {
        lp.push_back(heavy[i]);
    }
    for(int i = 0; i < l; i++) {
        lp.push_back(light[i]);
    }
    for(int i = h; i < 2 * h; i++) {
        rp.push_back(heavy[i]);
    }
    for(int i = l; i < 2 * l; i++) {
        rp.push_back(light[i]);
    }

    vector<int> gt_heavy(heavy.begin(), heavy.begin() + h);
    vector<int> gt_light(light.begin() + l, light.begin() + 2 * l);

    vector<int> eq_light(light.begin() + 2 * l, light.end());
    vector<int> eq_heavy(heavy.begin() + 2 * h, heavy.end());

    vector<int> lt_light(light.begin(), light.begin() + l);
    vector<int> lt_heavy(heavy.begin() + h, heavy.begin() + 2 * h);

    Solution* node = new Solution(lp, rp);
    node->less_branch = build_signed(lt_light, lt_heavy, known_good);
    node->equal_branch = build_signed(eq_light, eq_heavy, known_good);
    node->greater_branch = build_signed(gt_light, gt_heavy, known_good);
    return node;
}

void solve() {
    // Nowadays, this is a fairly popular setting, although there are different
    // variants. It's commonly known as the balance puzzle. Naive idea is that
    // you would need log2(n) comparisons, but if you compare two halves as a
    // start and know if the outlier is heavier / lighter, you can instead split
    // into 3 groups, compare two of them, and depending on whether they are <,
    // =, or >, go to the corresponding part. This gives us a log3(n), but we
    // need to first know if we have something heavier or lighter.
    //
    // The optimal strategy uses ceil(log3(2n+1)) weightings and maintains two
    // kinds of sub-problems: unknown (suspects whose direction is unknown,
    // plus known-good coins) and signed (light-suspects + heavy-suspects +
    // known-good coins).
    //
    // For the unknown state without good coins (the initial call), we weigh
    // a = (3^(k-1) - 1) / 2 suspects against a other suspects, leaving at
    // least one off the scale (to serve as a known-good reference later).
    // Imbalance yields a signed sub-problem of size 2a; balance yields an
    // unknown sub-problem of size n - 2a with 2a newly confirmed good coins.
    //
    // For the unknown state with good coins available, we weigh b = 3^(k-1)
    // suspects against b known-good coins. Imbalance tells us the direction
    // (all b suspects are light-only or heavy-only), giving a signed
    // sub-problem of size b. Balance eliminates those b suspects entirely,
    // leaving n - b unknowns. The capacity with good coins is (3^k + 1) / 2,
    // versus (3^k - 1) / 2 without.
    //
    // For the signed state with L light-suspects and H heavy-suspects
    // (total T), we put s = (T + 1) / 3 coins on each pan, mixing light and
    // heavy suspects so that each of the three outcomes (<, =, >) yields a
    // signed sub-problem of size roughly T/3. Specifically, we place h
    // heavy-suspects and l = s - h light-suspects on each side (with h chosen
    // so 2h <= H and 2l <= L). Left-heavier implicates heavy coins on the
    // left and light coins on the right; left-lighter implicates the
    // opposite; balance implicates the off-scale remainder.
    //
    // The base cases end up being:
    //     tot = 1 is the fake.
    //     tot = 2 same-direction weighs one against the other.
    //     tot = 2 mixed weighs a suspect against a known-good coin.
    //
    // An interesting note is that this is a fairly popular problem from quant
    // interviews and appearing in Heard on The Street book as question 1.18,
    // but the solutions given there also tries to identify if the coin is
    // heavier or lighter (and not only finds the outlier). This makes the
    // optimum log3(2n+3) instead, which might be different for say N=13. It's
    // still a good read and it has other nice puzzles:
    //
    // https://www.amazon.com/Heard-Street-Quantitative-Questions-Interviews/dp/0994103867

    vector<int> coins(n);
    iota(coins.begin(), coins.end(), 1);

    int w = 0;
    while((pow3(w) - 1) / 2 < n) {
        w++;
    }

    cout << "need " << w << " weighings\n";
    Solution* root = Solution::build_unknown(coins, {});
    root->print(0);
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
