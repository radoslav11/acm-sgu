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

int n, s, v_min, v_max;
vector<int> xi, ri, gi, di;

void read() {
    cin >> n >> s >> v_min >> v_max;
    xi.resize(n);
    ri.resize(n);
    gi.resize(n);
    di.resize(n);
    for(int i = 0; i < n; i++) {
        cin >> xi[i] >> ri[i] >> gi[i] >> di[i];
    }
}

void solve() {
    // To have a green wave, for each traffic light we want the time we pass
    // ti = xi / v0, to be withing the green periods, or:
    //
    //        ti   \in [di - gi + k*(gi+ri)  ; di + k*(gi+ri)            ].
    //
    //     xi / v0 \in [di - gi + k*(gi+ri)  ; di + k*(gi+ri)            ].
    //
    //        v0   \in [xi / (di + k*(gi+ri)); xi / (di - gi + k*(gi+ri))].
    //
    // This means that the range of v0 will be the union or some ranges. If we
    // could compactly encode the set of v0, we could do a DP[traffic
    // light][set of v0 that are available]. The issue is that the set above
    // depends on k, and it's a floating point number. However, we can notice
    // that it's enough to notice that it's always possible for the v0 to lie on
    // a border of one of these ranges. Then it's clear which traffic lights we
    // violated and which we don't. In terms of candidates, we know that the
    // candidates have 10 <= v0 <= 50, so for any i, there are less than ~50
    // possible values for k, or overall that's around a million. This directly
    // gives us a O(#candidates * n) solution, but instead we can notice that
    // for each traffic light i, the ranges are non-overlapping. This means we
    // can instead think of this problem as having some points P, and ranges
    // L,R, and figuring out the point that is covered by the largest number of
    // ranges. This can be done by maintaining an array offline with +1 on L and
    // -1 on (R+1). We should compress the coordinates too.

    vector<pair<double, int>> events;

    for(int i = 0; i < n; i++) {
        int c = ri[i] + gi[i];
        for(int k = 0;; k++) {
            int gs = di[i] - gi[i] + k * c;
            int ge = di[i] + k * c;

            if((double)gs > (double)xi[i] / v_min) {
                break;
            }
            if(ge <= 0) {
                continue;
            }
            if((double)xi[i] / ge > (double)v_max) {
                continue;
            }

            double t_lo = max((double)gs, xi[i] / (double)v_max);
            double t_hi = min((double)ge, xi[i] / (double)v_min);
            if(t_lo < 1e-15) {
                t_lo = 1e-15;
            }
            if(t_lo > t_hi + 1e-12) {
                continue;
            }

            double vl = xi[i] / t_hi;
            double vh = xi[i] / t_lo;
            vl = max(vl, (double)v_min);
            vh = min(vh, (double)v_max);
            if(vl > vh + 1e-12) {
                continue;
            }

            events.push_back({vl, 1});
            events.push_back({vh, -1});
        }
    }

    sort(
        events.begin(), events.end(),
        [](const pair<double, int>& a, const pair<double, int>& b) {
            if(a.first != b.first) {
                return a.first < b.first;
            }
            return a.second > b.second;
        }
    );

    int cnt = 0, best_cnt = 0;
    double best_v = v_max;

    for(int i = 0; i < (int)events.size();) {
        double pos = events[i].first;
        while(i < (int)events.size() && events[i].first == pos &&
              events[i].second > 0) {
            cnt++;
            i++;
        }
        if(cnt > best_cnt || (cnt == best_cnt && pos > best_v)) {
            best_cnt = cnt;
            best_v = pos;
        }
        while(i < (int)events.size() && events[i].first == pos &&
              events[i].second < 0) {
            cnt--;
            i++;
        }
    }

    const double eps = 1e-9;
    vector<int> switched;
    for(int i = 0; i < n; i++) {
        double t = xi[i] / best_v;
        int c = ri[i] + gi[i];
        double rem = fmod(t - di[i] + gi[i], (double)c);
        if(rem < -eps) {
            rem += c;
        }
        if(rem > gi[i] + eps && rem < c - eps) {
            switched.push_back(i + 1);
        }
    }

    cout << fixed << setprecision(10) << best_v << "\n";
    cout << switched.size() << "\n";
    for(int i = 0; i < (int)switched.size(); i++) {
        if(i) {
            cout << " ";
        }
        cout << switched[i];
    }
    if(!switched.empty()) {
        cout << "\n";
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
