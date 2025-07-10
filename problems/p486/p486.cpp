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
const int MAXN = (1 << 10);

string s1, s2;

void read() { cin >> s1 >> s2; }

void solve() {
    map<char, int> c1;
    set<char> cows_and_bulls;
    int bulls = 0;

    for(int i = 0; i < s1.size(); i++) {
        if(s1[i] == s2[i]) {
            bulls++;
        }
        c1[s1[i]] = 1;
    }

    for(int i = 0; i < s1.size(); i++) {
        if(c1[s2[i]]) {
            cows_and_bulls.insert(s2[i]);
        }
    }

    cout << bulls << " " << cows_and_bulls.size() - bulls << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    read();
    solve();
    return 0;
}
