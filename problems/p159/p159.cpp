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

int b, n;

void read() { cin >> b >> n; }

void pop_carry(int pos, vector<int>& carry) {
    if(pos >= n || carry[pos] < b) {
        return;
    }

    int full = carry[pos] / b;
    carry[pos] %= b;
    if(pos + 1 < n) {
        carry[pos + 1] += full;
        pop_carry(pos + 1, carry);
    }
}

void rec(int pos, vector<int>& carry, vector<int>& x, vector<string>& ans) {
    if(pos == n) {
        string candidate = "";
        for(int i = n - 1; i >= 0; i--) {
            if(x[i] >= 10) {
                candidate.push_back(x[i] - 10 + 'A');
            } else {
                candidate.push_back(x[i] + '0');
            }
        }
        ans.push_back(candidate);
        return;
    }

    vector<int> carry_save = carry;
    for(int d = (pos == n - 1) && (n != 1); d < b; d++) {
        x.push_back(d);
        if((carry[pos] + (1 + (pos != 0)) * x[0] * x[pos]) % b != x[pos]) {
            x.pop_back();
            continue;
        }

        for(int j = 0; j <= pos; j++) {
            int val = (1 + (pos != j)) * x[pos] * x[j];
            if(pos + j < n) {
                carry[j + pos] += val;
            }
            pop_carry(pos + j, carry);
        }

        if(carry[pos] == x[pos]) {
            rec(pos + 1, carry, x, ans);
        }

        carry = carry_save;
        x.pop_back();
    }
}

void solve() {
    // The first thing we should notice is that the solution wants us to
    // output all self-replicating numbers. This should immediately suggest
    // looking at brute force solutions. In particular, the number of such
    // numbers is likely not very high. Let us start with considering how
    // a self-replicating number would. Let X = SUM x[i] * b^i. Then we have:
    //
    //     X^2 = X   (mod b^n)
    //     (SUM x[i] * b^i) * (SUM x[i] * b^i) = X   (mod b^n)
    //     SUM x[i] * x[j] * b^(i + j) = X   (mod b^n)
    //
    // In particular, we can notice that all terms with i+j >= n get ignored
    // because of mod b^n. Furthermore, if we start building X from i = 0, we
    // can notice that we can incrementally add the contribution with the
    // previous digits. Lets do a brute force starting from i = 0 and going,
    // where we will keep carry[k] for each b^k. We will try every possible
    // value for x[i], but we know that for i > 0 it should satisfy that:
    //
    //     (carry[i] + 2 * x[i] * x[0]) = x[i]   (mod b)
    //     carry[i] = (1 - 2 * x[0]) * x[i]   (mod b)
    //
    // Most importantly, this means that there are either 0 or 1 solutions after
    // i = 0. The constraint is b <= 36, so we don't have to be careful about
    // quickly finding x[i], but this makes the brute force viable - after
    // choosing x[0] the number is determined (if it's possible).

    vector<int> carry(n, 0), x;
    vector<string> ans;
    rec(0, carry, x, ans);

    cout << ans.size() << endl;
    for(int i = 0; i < (int)ans.size(); i++) {
        cout << ans[i] << endl;
    }
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
