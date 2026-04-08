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

struct Stone {
    int x1, x2, t;
};

int v, g, n;
vector<Stone> stones;

void read() {
    cin >> v >> g >> n;
    stones.resize(n);
    for(auto& [x1, x2, t]: stones) {
        cin >> x1 >> x2 >> t;
    }
}

void solve() {
    // The hardest part in this problem is all the details. Apart from that,
    // the key observation is that at each stone landing time, the set of
    // reachable positions forms a union of O(N) closed intervals, and that we
    // only care about O(N) important times (stone landings). Also, Aladdin will
    // move with either 0 (stay) or v speed at any moment. Any route can be
    // represented by only these two states. To transition from t[i] to t[i+1],
    // we expand each interval by v*dt in both directions (Aladdin can move at
    // up to speed v), merge overlapping intervals, then subtract the open
    // interiors (x1, x2) of stones landing at t[i+1]: endpoints are safe. At
    // each layer we check whether G can be reached before the next stone: from
    // interval [l, r] the candidate is t[i] + dist(G, [l,r]) / v, valid when
    // dist(G, [l,r]) <= v * (t[i+1] - t[i]). For reconstruction we store all
    // layers and trace back, finding a valid position within v*dt of the next
    // waypoint. Overall O(N^2) time and space.

    using Intervals = vector<pair<long long, long long>>;

    vector<int> times = {0};
    for(auto& [x1, x2, t]: stones) {
        times.push_back(t);
    }
    sort(times.begin(), times.end());
    times.erase(unique(times.begin(), times.end()), times.end());
    int p = (int)times.size();

    map<int, int> time_idx;
    for(int i = 0; i < p; i++) {
        time_idx[times[i]] = i;
    }

    vector<vector<pair<long long, long long>>> blocked(p);
    for(auto& [x1, x2, t]: stones) {
        if(x1 < x2) {
            blocked[time_idx[t]].emplace_back(x1, x2);
        }
    }
    for(auto& bl: blocked) {
        sort(bl.begin(), bl.end());
        Intervals merged;
        for(auto& [l, r]: bl) {
            if(!merged.empty() && l < merged.back().second) {
                merged.back().second = max(merged.back().second, r);
            } else {
                merged.emplace_back(l, r);
            }
        }
        bl = merged;
    }

    auto subtract = [](const Intervals& reach,
                       const Intervals& bl) -> Intervals {
        Intervals result;
        int bi = 0, nb = (int)bl.size();
        for(auto [l, r]: reach) {
            long long cl = l;
            while(bi < nb && bl[bi].second <= cl) {
                bi++;
            }
            while(bi < nb && bl[bi].first < r) {
                if(bl[bi].second <= cl) {
                    bi++;
                    continue;
                }
                if(bl[bi].first >= cl) {
                    result.emplace_back(cl, bl[bi].first);
                }
                cl = max(cl, bl[bi].second);
                if(bl[bi].second <= r) {
                    bi++;
                } else {
                    break;
                }
                if(cl >= r) {
                    break;
                }
            }
            if(cl <= r) {
                result.emplace_back(cl, r);
            }
        }
        return result;
    };

    vector<Intervals> layers(p);
    Intervals cur = {{0, 0}};
    layers[0] = cur;

    double best_time = 1e18;
    int best_layer = -1;
    double best_x = 0;

    auto check = [&](int layer, const Intervals& intervals) {
        for(auto& [l, r]: intervals) {
            long long dist;
            double x;
            if(g >= l && g <= r) {
                dist = 0;
                x = g;
            } else if(g < l) {
                dist = l - g;
                x = l;
            } else {
                dist = g - r;
                x = r;
            }
            double arrival = times[layer] + (double)dist / v;
            bool valid =
                (layer + 1 >= p) ||
                (dist <= (long long)v * (times[layer + 1] - times[layer]));
            if(valid && arrival < best_time - 1e-12) {
                best_time = arrival;
                best_layer = layer;
                best_x = x;
            }
        }
    };

    check(0, cur);

    for(int i = 0; i + 1 < p; i++) {
        long long D = (long long)v * (times[i + 1] - times[i]);

        Intervals expanded;
        for(auto& [l, r]: cur) {
            expanded.emplace_back(l - D, r + D);
        }

        Intervals merged;
        for(auto& [l, r]: expanded) {
            if(!merged.empty() && l <= merged.back().second) {
                merged.back().second = max(merged.back().second, r);
            } else {
                merged.emplace_back(l, r);
            }
        }

        cur = subtract(merged, blocked[i + 1]);
        layers[i + 1] = cur;

        check(i + 1, cur);
    }

    if(best_layer == -1) {
        cout << -1 << '\n';
        return;
    }

    vector<double> path(best_layer + 1);
    path[best_layer] = best_x;
    for(int i = best_layer - 1; i >= 0; i--) {
        long long D = (long long)v * (times[i + 1] - times[i]);
        double target = path[i + 1];
        for(auto& [l, r]: layers[i]) {
            double cl = max((double)l, target - D);
            double cr = min((double)r, target + D);
            if(cl <= cr + 1e-9) {
                path[i] = clamp(target, cl, cr);
                break;
            }
        }
    }

    vector<pair<double, double>> instructions;
    for(int i = 0; i < best_layer; i++) {
        double dt = times[i + 1] - times[i];
        double dx = path[i + 1] - path[i];
        instructions.emplace_back(dx / dt, dt);
    }

    double final_dx = g - path[best_layer];
    if(abs(final_dx) > 1e-12) {
        double speed = final_dx > 0 ? (double)v : (double)-v;
        instructions.emplace_back(speed, abs(final_dx) / v);
    }

    cout << (int)instructions.size() << '\n';
    cout << fixed << setprecision(15);
    for(auto& [w, t]: instructions) {
        cout << w << ' ' << t << '\n';
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
