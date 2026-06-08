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
string s;

void read() { cin >> n >> s; }

void solve() {
    // After every odd turn 2i+1, the fence must be a palindrome of length
    // 2i+1 whose multiset equals the input prefix s[0..2i]. The Q-count and
    // the parity of the center letter are forced by this multiset, but the
    // arrangement is not. We will commit to one specific palindrome at
    // every odd turn, which we call the target:
    //
    //   target = QC QC ... [filler] [center] [filler] ... CQ CQ
    //
    // Concretely, we pair Q's with C's from both ends inward for as long as
    // both letters are still available, fill the remaining middle slots on
    // each side with the majority letter, and place the parity-determined
    // letter at the center.
    //
    // We will rely on two observations:
    //
    //     1) The canonical shape only shifts by O(1) when (q_count, c_count)
    //        change by +2 between two consecutive odd turns. The
    //        alternating prefix may extend by one pair, the filler section
    //        may shift by one slot, and the center may flip parity.
    //        Therefore, after the fence becomes (previous target) plus the
    //        two new letters He just played, the new target differs from
    //        the fence in at most four positions.
    //
    //     2) In a binary alphabet, two strings with the same multiset and
    //        Hamming distance at most 4 always factor their differing
    //        indices a < b < c < d into two complementary swap pairs. So
    //        swap(a, b) on the even turn followed by swap(c, d) on the
    //        odd turn rewrites the fence into the target, which is
    //        exactly the budget of two swaps available between
    //        consecutive odd turns. With two mismatches we only need the
    //        odd-turn swap, and with zero mismatches we pass on both
    //        turns.
    //
    // Together these two observations give an O(n^2) construction with no
    // search, scoring, or backtracking.

    int m = 2 * n - 1;
    string fence(m, ' '), target(m, ' ');
    int q_count = 0, c_count = 0;
    vector<pair<int, int>> actions;

    auto add_letter = [&](char c) { (c == 'Q' ? q_count : c_count)++; };

    fence[0] = s[0];
    add_letter(s[0]);
    actions.emplace_back(0, 0);

    for(int i = 1; i < n; i++) {
        char c_even = s[2 * i - 1], c_odd = s[2 * i];
        fence[2 * i - 1] = c_even;
        fence[2 * i] = c_odd;
        add_letter(c_even);
        add_letter(c_odd);

        target[i] = (q_count & 1) ? 'Q' : 'C';
        int rem_q = q_count, rem_c = c_count;
        (rem_q & 1) ? --rem_q : --rem_c;

        int alt_per_side = min(rem_q, rem_c);
        for(int j = 0; j < alt_per_side; j += 2) {
            target[j] = target[2 * i - j] = 'Q';
            target[j + 1] = target[2 * i - j - 1] = 'C';
        }
        char filler = (rem_q > rem_c) ? 'Q' : 'C';
        for(int j = alt_per_side, k = abs(rem_q - rem_c); k > 0; ++j, k -= 2) {
            target[j] = target[2 * i - j] = filler;
        }

        vector<int> mismatches;
        for(int j = 0; j <= 2 * i; j++) {
            if(fence[j] != target[j]) {
                mismatches.push_back(j + 1);
            }
        }

        if((int)mismatches.size() > 4) {
            cout << "He\n";
            return;
        }

        if(mismatches.empty()) {
            actions.emplace_back(0, 0);
            actions.emplace_back(0, 0);
        } else if(mismatches.size() == 2) {
            actions.emplace_back(0, 0);
            actions.emplace_back(mismatches[0], mismatches[1]);
        } else {
            actions.emplace_back(mismatches[0], mismatches[1]);
            actions.emplace_back(mismatches[2], mismatches[3]);
        }

        copy_n(target.begin(), 2 * i + 1, fence.begin());
    }

    cout << "Qc\n";
    for(auto& [a, b]: actions) {
        cout << a << ' ' << b << '\n';
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
