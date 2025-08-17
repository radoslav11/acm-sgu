// #include <bits/stdc++.h>

// #define FI(i,a,b) for(int i=(a);i<=(b);i++)
// #define FD(i,a,b) for(int i=(a);i>=(b);i--)

// #define LL long long
// #define Ldouble long double
// #define PI 3.14159265358979323846264338327950288419

// #define PII pair<int,int>
// #define PLL pair<LL,LL>
// #define mp make_pair
// #define fi first
// #define se second

// using namespace std;

// #define eps 1E-6

// int n;
// double x[305], y[305], rad[305];
// double cx, cy, cr;
// double dist[305][305];

// double sq(double x){
// 	return x * x;
// }

// int main(){
// 	scanf("%d", &n);
// 	FI(i, 1, n) cin >> x[i] >> y[i] >> rad[i];
// 	cin >> cx >> cy >> cr;
// 	FI(i, 1, n){
// 		x[i] -= cx;
// 		y[i] -= cy;
// 		rad[i] += cr;
// 	}
// 	FI(i, 1, n) FI(j, 1, n) if(i != j){
// 		if(sq(x[i] - x[j]) + sq(y[i] - y[j]) + eps > sq(rad[i] + rad[j]))
// dist[i][j] = 1E18; 		else if(i > j) dist[i][j] = -dist[j][i]; else{
// double ang1 = acos(x[i] / sqrt(sq(x[i]) + sq(y[i]))); 			if(y[i] +
// eps < 0) ang1 = 2 * PI - ang1; 			double ang2 = acos(x[j] /
// sqrt(sq(x[j]) + sq(y[j]))); 			if(y[j] + eps < 0) ang2 = 2 * PI - ang2;

// 			double d;
// 			if(ang1 < ang2) d = ang2 - ang1;
// 			else d = ang2 + 2 * PI - ang1;
// 			if(d > PI) dist[i][j] = d - 2 * PI;
// 			else dist[i][j] = d;
// 		}
// 	}

//     for(int i = 1; i <= n; i++) {
//         for(int j = 1; j <= n; j++) {
//             cout << setw(5) << dist[i][j] << " ";
//         }
//         cout << endl;
//     }

// 	FI(k, 1, n) FI(i, 1, n) FI(j, 1, n)
// 		if(dist[i][k] + dist[k][j] + eps < dist[i][j]) dist[i][j] = dist[i][k] +
// dist[k][j]; 	FI(i, 1, n) if(dist[i][i] < -1){ 		printf("NO\n");
// return 0;
// 	}
// 	printf("YES\n");
// 	return 0;
// }

/*

Compiling...

Running Test Case 1 with debugging flags

    0 1.5708 -1.5708 1e+18 1e+18 1e+18 1e+18
-1.5708     0 1e+18 1.5708 1e+18 1e+18 1e+18
1.5708 1e+18     0 1e+18 -0.404892 1e+18 1e+18
1e+18 -1.5708 1e+18     0 1e+18 1e+18 1e+18
1e+18 1e+18 0.404892 1e+18     0 -0.380506 1e+18
1e+18 1e+18 1e+18 1e+18 0.380506     0 -0.321751
1e+18 1e+18 1e+18 1e+18 1e+18 0.321751     0
YES

*/

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

const double PI = acos(-1);
const double eps = 1e-6;

int n;
vector<double> x, y, rad;
double cx, cy, cr;

void read() {
    cin >> n;
    x.resize(n);
    y.resize(n);
    rad.resize(n);
    for(int i = 0; i < n; i++) {
        cin >> x[i] >> y[i] >> rad[i];
    }
    cin >> cx >> cy >> cr;
}

void solve() {
    // The idea is to check if the captain is enclosed by a cycle of overlapping
    // islands. We first transform the problem by centering at the captain's
    // position and expanding island radii by the captain's radius. This reduces
    // the captain to a point at origin.
    //
    // We then build a graph where islands are nodes. If two islands overlap (or
    // touch), we create directed edges between them with weights equal to the
    // signed angle between their centers as viewed from the origin.
    //
    // If there exists a cycle in this graph with non-zero total angle
    // (specifically, a negative cycle), it means the islands form a loop around
    // the origin, trapping the captain. We use Floyd-Warshall to find shortest
    // paths and check if any node has a negative cycle to itself.

    for(int i = 0; i < n; i++) {
        x[i] -= cx;
        y[i] -= cy;
        rad[i] += cr;
    }

    vector<vector<double>> dist(n, vector<double>(n, 0));
    for(int i = 0; i < n; i++) {
        for(int j = i + 1; j < n; j++) {
            double center_dist_sq =
                (x[i] - x[j]) * (x[i] - x[j]) + (y[i] - y[j]) * (y[i] - y[j]);
            double sum_rad_sq = (rad[i] + rad[j]) * (rad[i] + rad[j]);

            if(center_dist_sq + eps > sum_rad_sq) {
                dist[i][j] = 1e18;
                dist[j][i] = 1e18;
            } else {
                double alpha = acos(x[i] / sqrt(x[i] * x[i] + y[i] * y[i]));
                double beta = acos(x[j] / sqrt(x[j] * x[j] + y[j] * y[j]));
                if(y[i] < -eps) {
                    alpha = 2 * PI - alpha;
                }
                if(y[j] < -eps) {
                    beta = 2 * PI - beta;
                }

                double angle_diff = beta - alpha;
                if(angle_diff < 0) {
                    angle_diff += 2 * PI;
                }

                if(angle_diff > PI) {
                    angle_diff -= 2 * PI;
                }
                dist[i][j] = angle_diff;
                dist[j][i] = -angle_diff;
            }
        }
    }

    for(int k = 0; k < n; k++) {
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < n; j++) {
                dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j] + eps);
            }
        }
    }

    for(int i = 0; i < n; i++) {
        if(dist[i][i] < -PI) {
            cout << "NO" << endl;
            return;
        }
    }

    cout << "YES" << endl;
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
