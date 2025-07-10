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

int m, n;

void read() { cin >> n >> m; }

vector<int> months = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

void solve() {
    if(m <= 0 || n <= 0 || m > 12 || n > months[m - 1]) {
        cout << "Impossible" << endl;
        return;
    }

    n--;
    for(int i = 1; i < m; i++) {
        n += months[i - 1];
    }
    cout << (n % 7) + 1 << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    read();
    solve();
    return 0;
}
