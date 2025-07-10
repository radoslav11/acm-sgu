#include <bits/stdc++.h>
using namespace std;

using Point = complex<long double>;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, a, b;
    cin >> n >> a >> b;

    vector<Point> points(n + 1);
    long double x, y;
    cin >> x >> y;
    points[a] = Point(x, y);
    cin >> x >> y;
    points[b] = Point(x, y);

    if(a > b) {
        swap(a, b);
    }

    const long double PI = 3.14159265358979323846L;
    long double dist = abs(points[b] - points[a]);
    long double radius = dist / sin(PI * (b - a) / n) / 2;

    Point mid = (points[a] + points[b]) / 2.0L;
    Point center =
        mid +
        Point(
            (points[b].imag() - points[a].imag()) / tan(PI * (b - a) / n) / 2,
            -(points[b].real() - points[a].real()) / tan(PI * (b - a) / n) / 2
        );

    long double phi = asin((points[a].imag() - center.imag()) / radius);
    if(acos((points[a].real() - center.real()) / radius) > PI / 2) {
        phi = (phi >= 0 ? PI - phi : -PI - phi);
    }

    for(int i = 1; i <= n; ++i) {
        if(i != a && i != b) {
            long double delta = phi + 2 * PI * (a - i) / n;
            points[i] =
                center + Point(radius * cos(delta), radius * sin(delta));
        }
    }

    cout << fixed << setprecision(6);
    for(int i = 1; i <= n; ++i) {
        cout << points[i].real() << " " << points[i].imag() << "\n";
    }

    return 0;
}
