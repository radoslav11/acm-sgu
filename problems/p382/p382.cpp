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

// base and base_digits must be consistent
const int base = 1000000000;
const int base_digits = 9;

struct bigint {
    vector<int> z;
    int sign;

    bigint() : sign(1) {}

    bigint(long long v) { *this = v; }

    bigint(const string& s) { read(s); }

    void operator=(const bigint& v) {
        sign = v.sign;
        z = v.z;
    }

    void operator=(long long v) {
        sign = 1;
        if(v < 0) {
            sign = -1, v = -v;
        }
        z.clear();
        for(; v > 0; v = v / base) {
            z.push_back(v % base);
        }
    }

    bigint operator+(const bigint& v) const {
        if(sign == v.sign) {
            bigint res = v;

            for(int i = 0, carry = 0;
                i < (int)max(z.size(), v.z.size()) || carry; ++i) {
                if(i == (int)res.z.size()) {
                    res.z.push_back(0);
                }
                res.z[i] += carry + (i < (int)z.size() ? z[i] : 0);
                carry = res.z[i] >= base;
                if(carry) {
                    res.z[i] -= base;
                }
            }
            return res;
        }
        return *this - (-v);
    }

    bigint operator-(const bigint& v) const {
        if(sign == v.sign) {
            if(abs() >= v.abs()) {
                bigint res = *this;
                for(int i = 0, carry = 0; i < (int)v.z.size() || carry; ++i) {
                    res.z[i] -= carry + (i < (int)v.z.size() ? v.z[i] : 0);
                    carry = res.z[i] < 0;
                    if(carry) {
                        res.z[i] += base;
                    }
                }
                res.trim();
                return res;
            }
            return -(v - *this);
        }
        return *this + (-v);
    }

    void operator*=(int v) {
        if(v < 0) {
            sign = -sign, v = -v;
        }
        for(int i = 0, carry = 0; i < (int)z.size() || carry; ++i) {
            if(i == (int)z.size()) {
                z.push_back(0);
            }
            long long cur = z[i] * (long long)v + carry;
            carry = (int)(cur / base);
            z[i] = (int)(cur % base);
            // asm("divl %%ecx" : "=a"(carry), "=d"(a[i]) : "A"(cur),
            // "c"(base));
        }
        trim();
    }

    bigint operator*(int v) const {
        bigint res = *this;
        res *= v;
        return res;
    }

    friend pair<bigint, bigint> divmod(const bigint& a1, const bigint& b1) {
        int norm = base / (b1.z.back() + 1);
        bigint a = a1.abs() * norm;
        bigint b = b1.abs() * norm;
        bigint q, r;
        q.z.resize(a.z.size());

        for(int i = a.z.size() - 1; i >= 0; i--) {
            r *= base;
            r += a.z[i];
            int s1 = b.z.size() < r.z.size() ? r.z[b.z.size()] : 0;
            int s2 = b.z.size() - 1 < r.z.size() ? r.z[b.z.size() - 1] : 0;
            int d = ((long long)s1 * base + s2) / b.z.back();
            r -= b * d;
            while(r < 0) {
                r += b, --d;
            }
            q.z[i] = d;
        }

        q.sign = a1.sign * b1.sign;
        r.sign = a1.sign;
        q.trim();
        r.trim();
        return make_pair(q, r / norm);
    }

    friend bigint sqrt(const bigint& a1) {
        bigint a = a1;
        while(a.z.empty() || a.z.size() % 2 == 1) {
            a.z.push_back(0);
        }

        int n = a.z.size();

        int firstDigit = (int)sqrt((double)a.z[n - 1] * base + a.z[n - 2]);
        int norm = base / (firstDigit + 1);
        a *= norm;
        a *= norm;
        while(a.z.empty() || a.z.size() % 2 == 1) {
            a.z.push_back(0);
        }

        bigint r = (long long)a.z[n - 1] * base + a.z[n - 2];
        firstDigit = (int)sqrt((double)a.z[n - 1] * base + a.z[n - 2]);
        int q = firstDigit;
        bigint res;

        for(int j = n / 2 - 1; j >= 0; j--) {
            for(;; --q) {
                bigint r1 =
                    (r - (res * 2 * base + q) * q) * base * base +
                    (j > 0 ? (long long)a.z[2 * j - 1] * base + a.z[2 * j - 2]
                           : 0);
                if(r1 >= 0) {
                    r = r1;
                    break;
                }
            }
            res *= base;
            res += q;

            if(j > 0) {
                int d1 =
                    res.z.size() + 2 < r.z.size() ? r.z[res.z.size() + 2] : 0;
                int d2 =
                    res.z.size() + 1 < r.z.size() ? r.z[res.z.size() + 1] : 0;
                int d3 = res.z.size() < r.z.size() ? r.z[res.z.size()] : 0;
                q = ((long long)d1 * base * base + (long long)d2 * base + d3) /
                    (firstDigit * 2);
            }
        }

        res.trim();
        return res / norm;
    }

    bigint operator/(const bigint& v) const { return divmod(*this, v).first; }

    bigint operator%(const bigint& v) const { return divmod(*this, v).second; }

    void operator/=(int v) {
        if(v < 0) {
            sign = -sign, v = -v;
        }
        for(int i = (int)z.size() - 1, rem = 0; i >= 0; --i) {
            long long cur = z[i] + rem * (long long)base;
            z[i] = (int)(cur / v);
            rem = (int)(cur % v);
        }
        trim();
    }

    bigint operator/(int v) const {
        bigint res = *this;
        res /= v;
        return res;
    }

    int operator%(int v) const {
        if(v < 0) {
            v = -v;
        }
        int m = 0;
        for(int i = z.size() - 1; i >= 0; --i) {
            m = (z[i] + m * (long long)base) % v;
        }
        return m * sign;
    }

    void operator+=(const bigint& v) { *this = *this + v; }
    void operator-=(const bigint& v) { *this = *this - v; }
    void operator*=(const bigint& v) { *this = *this * v; }
    void operator/=(const bigint& v) { *this = *this / v; }

    bool operator<(const bigint& v) const {
        if(sign != v.sign) {
            return sign < v.sign;
        }
        if(z.size() != v.z.size()) {
            return z.size() * sign < v.z.size() * v.sign;
        }
        for(int i = z.size() - 1; i >= 0; i--) {
            if(z[i] != v.z[i]) {
                return z[i] * sign < v.z[i] * sign;
            }
        }
        return false;
    }

    bool operator>(const bigint& v) const { return v < *this; }
    bool operator<=(const bigint& v) const { return !(v < *this); }
    bool operator>=(const bigint& v) const { return !(*this < v); }
    bool operator==(const bigint& v) const {
        return !(*this < v) && !(v < *this);
    }
    bool operator!=(const bigint& v) const { return *this < v || v < *this; }

    void trim() {
        while(!z.empty() && z.back() == 0) {
            z.pop_back();
        }
        if(z.empty()) {
            sign = 1;
        }
    }

    bool isZero() const { return z.empty() || (z.size() == 1 && !z[0]); }

    bigint operator-() const {
        bigint res = *this;
        res.sign = -sign;
        return res;
    }

    bigint abs() const {
        bigint res = *this;
        res.sign *= res.sign;
        return res;
    }

    long long longValue() const {
        long long res = 0;
        for(int i = z.size() - 1; i >= 0; i--) {
            res = res * base + z[i];
        }
        return res * sign;
    }

    friend bigint gcd(const bigint& a, const bigint& b) {
        return b.isZero() ? a : gcd(b, a % b);
    }
    friend bigint lcm(const bigint& a, const bigint& b) {
        return a / gcd(a, b) * b;
    }

    void read(const string& s) {
        sign = 1;
        z.clear();
        int pos = 0;
        while(pos < (int)s.size() && (s[pos] == '-' || s[pos] == '+')) {
            if(s[pos] == '-') {
                sign = -sign;
            }
            ++pos;
        }
        for(int i = s.size() - 1; i >= pos; i -= base_digits) {
            int x = 0;
            for(int j = max(pos, i - base_digits + 1); j <= i; j++) {
                x = x * 10 + s[j] - '0';
            }
            z.push_back(x);
        }
        trim();
    }

    friend istream& operator>>(istream& stream, bigint& v) {
        string s;
        stream >> s;
        v.read(s);
        return stream;
    }

    friend ostream& operator<<(ostream& stream, const bigint& v) {
        if(v.sign == -1) {
            stream << '-';
        }
        stream << (v.z.empty() ? 0 : v.z.back());
        for(int i = (int)v.z.size() - 2; i >= 0; --i) {
            stream << setw(base_digits) << setfill('0') << v.z[i];
        }
        return stream;
    }

    static vector<int> convert_base(
        const vector<int>& a, int old_digits, int new_digits
    ) {
        vector<long long> p(max(old_digits, new_digits) + 1);
        p[0] = 1;
        for(int i = 1; i < (int)p.size(); i++) {
            p[i] = p[i - 1] * 10;
        }
        vector<int> res;
        long long cur = 0;
        int cur_digits = 0;
        for(int i = 0; i < (int)a.size(); i++) {
            cur += a[i] * p[cur_digits];
            cur_digits += old_digits;
            while(cur_digits >= new_digits) {
                res.push_back(int(cur % p[new_digits]));
                cur /= p[new_digits];
                cur_digits -= new_digits;
            }
        }
        res.push_back((int)cur);
        while(!res.empty() && res.back() == 0) {
            res.pop_back();
        }
        return res;
    }

    typedef vector<long long> vll;

    static vll karatsubaMultiply(const vll& a, const vll& b) {
        int n = a.size();
        vll res(n + n);
        if(n <= 32) {
            for(int i = 0; i < n; i++) {
                for(int j = 0; j < n; j++) {
                    res[i + j] += a[i] * b[j];
                }
            }
            return res;
        }

        int k = n >> 1;
        vll a1(a.begin(), a.begin() + k);
        vll a2(a.begin() + k, a.end());
        vll b1(b.begin(), b.begin() + k);
        vll b2(b.begin() + k, b.end());

        vll a1b1 = karatsubaMultiply(a1, b1);
        vll a2b2 = karatsubaMultiply(a2, b2);

        for(int i = 0; i < k; i++) {
            a2[i] += a1[i];
        }
        for(int i = 0; i < k; i++) {
            b2[i] += b1[i];
        }

        vll r = karatsubaMultiply(a2, b2);
        for(int i = 0; i < (int)a1b1.size(); i++) {
            r[i] -= a1b1[i];
        }
        for(int i = 0; i < (int)a2b2.size(); i++) {
            r[i] -= a2b2[i];
        }

        for(int i = 0; i < (int)r.size(); i++) {
            res[i + k] += r[i];
        }
        for(int i = 0; i < (int)a1b1.size(); i++) {
            res[i] += a1b1[i];
        }
        for(int i = 0; i < (int)a2b2.size(); i++) {
            res[i + n] += a2b2[i];
        }
        return res;
    }

    bigint operator*(const bigint& v) const {
        vector<int> a6 = convert_base(this->z, base_digits, 6);
        vector<int> b6 = convert_base(v.z, base_digits, 6);
        vll a(a6.begin(), a6.end());
        vll b(b6.begin(), b6.end());
        while(a.size() < b.size()) {
            a.push_back(0);
        }
        while(b.size() < a.size()) {
            b.push_back(0);
        }
        while(a.size() & (a.size() - 1)) {
            a.push_back(0), b.push_back(0);
        }
        vll c = karatsubaMultiply(a, b);
        bigint res;
        res.sign = sign * v.sign;
        for(int i = 0, carry = 0; i < (int)c.size(); i++) {
            long long cur = c[i] + carry;
            res.z.push_back((int)(cur % 1000000));
            carry = (int)(cur / 1000000);
        }
        res.z = convert_base(res.z, 6, base_digits);
        res.trim();
        return res;
    }
};

int n;

void read() { cin >> n; }

void solve() {
    // Gorin and Kukushkin (2004) studied the moments of the Cantor function
    //
    //     I_n = integral from 0 to 1 of f(x)^n dx
    //
    // and proved they are rational. A reference is
    //
    //     https://www.ams.org/journals/spmj/2004-15-03/S1061-0022-04-00817-9/
    //              S1061-0022-04-00817-9.pdf
    //
    // The Cantor function satisfies the self-similarity relations
    //
    //     f(x/3)       = f(x) / 2,
    //     f(x)         = 1/2                for x in [1/3, 2/3],
    //     f(x/3 + 2/3) = 1/2 + f(x) / 2.
    //
    // Split the integral over the three pieces [0, 1/3], [1/3, 2/3], [2/3, 1]
    // and read off each contribution:
    //
    //     [0, 1/3]:    sub u = 3x, dx = du/3, so
    //                  integral of f(x)^n dx
    //                = (1/3) * integral of (f(u)/2)^n du
    //                = (1 / (3 * 2^n)) * I_n.
    //
    //     [1/3, 2/3]:  f is constant 1/2 here, so the integral is just the
    //                  length of the interval times (1/2)^n,
    //                = (1/3) * (1/2)^n
    //                = 1 / (3 * 2^n).
    //
    //     [2/3, 1]:    sub u = 3x - 2, then f(x) = (1 + f(u)) / 2, so
    //                  integral of f(x)^n dx
    //                = (1/3) * integral of ((1 + f(u)) / 2)^n du
    //                = (1 / (3 * 2^n)) * integral of (1 + f(u))^n du,
    //                  and binomial-expanding (1 + f(u))^n gives
    //                = (1 / (3 * 2^n)) * sum_{k=0..n} C(n, k) * I_k.
    //
    // Adding the three pieces back together,
    //
    //     I_n = (1 / (3 * 2^n)) * I_n
    //         + (1 / (3 * 2^n))
    //         + (1 / (3 * 2^n)) * sum_{k=0..n} C(n, k) * I_k.
    //
    // Pulling I_n to the left and simplifying yields the clean recurrence
    //
    //     (3 * 2^n - 2) * I_n = 1 + sum_{k=0..n-1} C(n, k) * I_k,
    //
    // with I_0 = 1. Sanity checks:
    //
    //     I_1 = 2 / 4 = 1/2,
    //     I_2 = 3 / 10.
    //
    // For n up to 50 the numerator and denominator blow up well past 64-bit
    // range, so we carry the recurrence with bigint arithmetic. To avoid an
    // O(n^2) chain of gcd-reductions, we keep a running lcm common[m] of the
    // denominators q_0, ..., q_{m-1} and build the numerator of I_m once over
    // that common denominator, doing exactly one gcd-reduction per m. The
    // binomial coefficients C(50, k) all fit in int64.

    vector<vector<int64_t>> binom(n + 1, vector<int64_t>(n + 1, 0));
    for(int i = 0; i <= n; i++) {
        binom[i][0] = 1;
        for(int j = 1; j <= i; j++) {
            binom[i][j] =
                binom[i - 1][j - 1] + (j < i ? binom[i - 1][j] : (int64_t)0);
        }
    }

    vector<bigint> p(n + 1), q(n + 1);
    p[0] = bigint(1);
    q[0] = bigint(1);

    bigint common(1);
    for(int m = 1; m <= n; m++) {
        bigint num = common;
        for(int k = 0; k < m; k++) {
            num = num + (common / q[k]) * p[k] * bigint(binom[m][k]);
        }

        bigint den = common * bigint((int64_t)3 * ((int64_t)1 << m) - 2);
        bigint g = gcd(num, den);
        p[m] = num / g;
        q[m] = den / g;

        bigint h = gcd(common, q[m]);
        common = (common / h) * q[m];
    }

    cout << p[n] << "/" << q[n] << "\n";
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
