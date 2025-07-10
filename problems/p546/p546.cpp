#include <bits/stdc++.h>
#define endl '\n'

// #pragma GCC optimize ("O3")
// #pragma GCC target ("sse4")

#define SZ(x) ((int)x.size())
#define ALL(V) V.begin(), V.end()
#define L_B lower_bound
#define U_B upper_bound
#define pb push_back

using namespace std;
template<class T, class T2>
inline int chkmax(T& x, const T2& y) {
    return x < y ? x = y, 1 : 0;
}
template<class T, class T2>
inline int chkmin(T& x, const T2& y) {
    return x > y ? x = y, 1 : 0;
}
const int MAXN = (1 << 20);

int n, a, b;
string s;

void read() {
    cin >> n >> a >> b;

    if(a + b > n) {
        cout << -1 << endl;
        exit(0);
    }

    cin >> s;
    for(char c: s) {
        if(c == '0') {
            a--;
        } else if(c == '1') {
            b--;
        }
    }
}

void solve() {
    int answer = 0;
    if(a > 0 && b < 0) {
        int q = min(a, -b);
        answer += q;
        b += q;
        a -= q;

        for(char& c: s) {
            if(c == '1' && q) {
                c = '0', q--;
            }
        }
    }

    if(b > 0 && a < 0) {
        int q = min(-a, b);
        answer += q;
        b -= q;
        a += q;

        for(char& c: s) {
            if(c == '0' && q) {
                c = '1', q--;
            }
        }
    }

    answer += abs(a);
    answer += abs(b);

    for(char& c: s) {
        if(a < 0 && c == '0') {
            a++;
            c = '2';
            continue;
        }
        if(a > 0 && c == '2') {
            a--;
            c = '0';
            continue;
        }
        if(b < 0 && c == '1') {
            b++;
            c = '2';
            continue;
        }
        if(b > 0 && c == '2') {
            b--;
            c = '1';
            continue;
        }
    }

    cout << answer << endl;
    cout << s << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    read();
    solve();
    return 0;
}
