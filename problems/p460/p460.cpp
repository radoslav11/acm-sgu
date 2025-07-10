#include <bits/stdc++.h>
#define endl '\n'

// #pragma GCC optimize ("O3")
// #pragma GCC target ("sse4")

using namespace std;
template<class T, class T2>
inline void chkmax(T& x, const T2& y) {
    if(x < y) {
        x = y;
    }
}
template<class T, class T2>
inline void chkmin(T& x, const T2& y) {
    if(x > y) {
        x = y;
    }
}
const int MAXN = (1 << 20);

string s;

void read() { cin >> s; }

void solve() {
    if(s.back() == 'y') {
        s.erase(prev(s.end()));
        cout << s << "ies" << endl;
    } else if((s.back() == 'h' && s[s.size() - 2] == 'c') || s.back() == 'o' || s.back() == 'x' || s.back() == 's') {
        cout << s << "es" << endl;
    } else if(s.back() == 'f' || (s[s.size() - 2] == 'f' && s.back() == 'e')) {
        while(s.back() != 'f') {
            s.pop_back();
        }
        s[s.size() - 1] = 'v';
        s += "es";
        cout << s << endl;
    } else {
        cout << s << "s" << endl;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int T;
    cin >> T;
    while(T--) {
        read();
        solve();
    }

    return 0;
}
