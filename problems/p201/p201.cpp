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

class BigInt {
  private:
    static const int BASE = (int)1e9;
    static const int BASE_LEN = 9;
    vector<int> digits;

  public:
    BigInt() : digits(1, 0) {}

    BigInt(int x) {
        if(x == 0) {
            digits = {0};
        } else {
            while(x > 0) {
                digits.push_back(x % BASE);
                x /= BASE;
            }
        }
    }

    BigInt& operator+=(const BigInt& other) {
        long long carry = 0;
        int max_size = max(digits.size(), other.digits.size());

        digits.resize(max_size);

        for(int i = 0; i < max_size || carry; i++) {
            if(i == digits.size()) {
                digits.push_back(0);
            }

            long long sum = digits[i] + carry;
            if(i < other.digits.size()) {
                sum += other.digits[i];
            }

            digits[i] = sum % BASE;
            carry = sum / BASE;
        }

        return *this;
    }

    BigInt operator+(const BigInt& other) const {
        BigInt result = *this;
        result += other;
        return result;
    }

    friend ostream& operator<<(ostream& out, const BigInt& x) {
        string result;
        for(int i = 0; i < (int)x.digits.size(); i++) {
            int val = x.digits[i];
            for(int j = 0; j < BASE_LEN; j++) {
                result.push_back('0' + val % 10);
                val /= 10;
            }
        }

        while(!result.empty() && result.back() == '0') {
            result.pop_back();
        }
        reverse(result.begin(), result.end());

        if(result.empty()) {
            result = "0";
        }
        out << result;
        return out;
    }
};

string alphabet;
int alphabet_size;
int k;
int initial_state;
vector<int> terminal_states;
vector<vector<int>> phi;
vector<vector<int>> chi;
int n;

void read() {
    cin >> alphabet;
    alphabet_size = alphabet.size();

    cin >> k;
    cin >> initial_state;
    initial_state--;

    int num_terminal;
    cin >> num_terminal;

    terminal_states.resize(num_terminal);
    for(int i = 0; i < num_terminal; i++) {
        cin >> terminal_states[i];
        terminal_states[i]--;
    }

    phi.assign(k, vector<int>(alphabet_size));
    for(int i = 0; i < k; i++) {
        for(int j = 0; j < alphabet_size; j++) {
            cin >> phi[i][j];
            phi[i][j]--;
        }
    }

    chi.assign(k, vector<int>(alphabet_size));
    for(int i = 0; i < k; i++) {
        for(int j = 0; j < alphabet_size; j++) {
            cin >> chi[i][j];
        }
    }

    cin >> n;
}

void solve() {
    /*
     * SGU Problem 201: Non Absorbing DFA
     *
     * Problem: Count the number of strings of length N that are accepted by a
     * DFA with non-absorbing edges.
     *
     * A DFA with non-absorbing edges has:
     * - Σ: input alphabet
     * - U: set of states
     * - s: initial state
     * - T: set of terminal states
     * - φ: transition function U × Σ → U
     * - χ: absorption function U × Σ → {0, 1}
     *
     * When χ(u, c) = 1, the character is not consumed (non-absorbing edge).
     * When χ(u, c) = 0, the character is consumed normally.
     *
     * Solution approach:
     * 1. Precompute effective transitions: For each (state, char), find the
     * final state after following all non-absorbing transitions.
     * 2. Use DP: dp[i][state] = number of ways to reach 'state' after consuming
     * exactly i characters.
     *
     * We use BigInt because the answer can be very large (up to
     * alphabet_size^N).
     */

    // Precompute effective transitions by resolving non-absorbing edges
    // trans[state][char] = final state after following all non-absorbing
    // transitions.
    vector<vector<int>> trans(k, vector<int>(alphabet_size, -2));

    function<void(int, int)> dfs = [&](int state, int ch) {
        if(chi[state][ch] == 0) {
            trans[state][ch] = state;
            return;
        }

        if(trans[state][ch] != -2) {
            return;
        }

        // Mark as being computed to detect cycles
        trans[state][ch] = -1;

        int next_state = phi[state][ch];
        dfs(next_state, ch);
        trans[state][ch] = trans[next_state][ch];
    };

    for(int state = 0; state < k; state++) {
        for(int ch = 0; ch < alphabet_size; ch++) {
            if(trans[state][ch] == -2) {
                dfs(state, ch);
            }
        }
    }

    // Dynamic Programming
    // dp[i][state] = number of ways to be in 'state' after consuming exactly i
    // characters.
    vector<vector<BigInt>> dp(n + 1, vector<BigInt>(k, BigInt(0)));
    dp[0][initial_state] = BigInt(1);

    for(int length = 0; length < n; length++) {
        for(int current_state = 0; current_state < k; current_state++) {
            for(int ch = 0; ch < alphabet_size; ch++) {
                if(trans[current_state][ch] == -1) {
                    // Infinite loop of non-absorbing edges - skip
                    continue;
                }

                int absorbing_state = trans[current_state][ch];
                int next_state = phi[absorbing_state][ch];
                dp[length + 1][next_state] += dp[length][current_state];
            }
        }
    }

    BigInt result(0);
    for(int terminal_state: terminal_states) {
        result += dp[n][terminal_state];
    }

    cout << result << endl;
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
