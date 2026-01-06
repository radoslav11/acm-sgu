#include <bits/stdc++.h>

using namespace std;

char buf[1 << 20];
int pos, len;

inline char gc() {
    if(pos == len) {
        len = fread(buf, 1, sizeof(buf), stdin);
        pos = 0;
    }
    return pos < len ? buf[pos++] : EOF;
}

inline int read_int() {
    int x = 0;
    char c;
    while((c = gc()) < '0')
        ;
    do {
        x = x * 10 + c - '0';
    } while((c = gc()) >= '0');
    return x;
}

inline double read_double() {
    int x = 0;
    char c;
    while((c = gc()) < '0')
        ;
    do {
        x = x * 10 + c - '0';
    } while((c = gc()) >= '0');
    if(c == '.') {
        int frac = 0, div = 1;
        while((c = gc()) >= '0') {
            frac = frac * 10 + c - '0';
            div *= 10;
        }
        return x + (double)frac / div;
    }
    return x;
}

int T, N;
vector<int> qty;
vector<vector<double>> price;

void read() {
    T = read_int();
    N = read_int();
    qty.resize(N);
    for(int i = 0; i < N; i++) {
        qty[i] = read_int();
    }
    price.resize(T + 1, vector<double>(N));
    for(int t = 0; t <= T; t++) {
        for(int i = 0; i < N; i++) {
            price[t][i] = read_double();
        }
    }
}

void solve() {
    // This is more of an implementation problem, as there is a full description
    // of what we want to do. Naively following, we get a O(N^2 * T)  solution
    // which might have to be optimized to pass.
    //
    // We need W'SW where S is the covariance matrix. Instead of
    // building S explicitly (O(N^2 * T)), we can notice that the W'SW risk of
    // the portfolio is just the variance of the portfolio return. The portfolio
    // return at time t is sum of w[i] * r[t][i], i.e. the weighted sum of asset
    // returns. So we compute this weighted return for each day, subtract its
    // mean, square, and average. This gives us the portfolio variance directly
    // in O(N * T).
    //
    // Formally: S[i][j] = (1/T) * sum_t (r[t][i] - m[i]) * (r[t][j] - m[j])
    //
    // W'SW = sum_{i,j} w[i] * S[i][j] * w[j]
    //      = sum_{i,j} w[i] * w[j] * (1/T) * sum_t (r[t][i]-m[i])*(r[t][j]-m[j]) 
    //      = (1/T) * sum_t sum_{i,j} w[i]*(r[t][i]-m[i]) * w[j]*(r[t][j]-m[j]) 
    //      = (1/T) * sum_t (sum_i w[i]*(r[t][i]-m[i]))^2 
    //      = (1/T) * sum_t dot[t]^2
    //
    // where dot[t] = sum_i w[i] * (r[t][i] - m[i]) is the demeaned portfolio
    // return.
    //
    // We might also need to implement fast reading to make this problem pass on
    // codeforces.

    double VP = 0;
    vector<double> w(N);
    for(int i = 0; i < N; i++) {
        w[i] = qty[i] * price[0][i];
        VP += w[i];
    }
    for(int i = 0; i < N; i++) {
        w[i] /= VP;
    }

    vector<double> sum(N);
    for(int t = 1; t <= T; t++) {
        for(int i = 0; i < N; i++) {
            sum[i] += (price[t - 1][i] - price[t][i]) / price[t][i];
        }
    }
    vector<double> m(N);
    double mP = 0;
    for(int i = 0; i < N; i++) {
        m[i] = sum[i] / T;
        mP += w[i] * m[i];
    }

    double var = 0;
    for(int t = 1; t <= T; t++) {
        double dot = 0;
        for(int i = 0; i < N; i++) {
            double r = (price[t - 1][i] - price[t][i]) / price[t][i] - m[i];
            dot += w[i] * r;
        }
        var += dot * dot;
    }
    var /= T;

    double sP = sqrt(var);
    double VaR = -VP * (mP - 1.644854 * sP);

    printf("%.2f\n", VaR);
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
