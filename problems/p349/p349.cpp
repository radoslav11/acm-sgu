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

const double eps = 1e-9;

int n, m;
vector<pair<double, double>> wolves, sheep;

void read() {
    cin >> n >> m;
    wolves.resize(n);
    sheep.resize(m);
    for(auto& [alpha, beta]: wolves) {
        int x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        alpha = atan2((double)y1, (double)x1);
        beta = atan2((double)y2, (double)x2);
        if(alpha > beta) {
            swap(alpha, beta);
        }
    }
    for(auto& [alpha, beta]: sheep) {
        int x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        alpha = atan2((double)y1, (double)x1);
        beta = atan2((double)y2, (double)x2);
        if(alpha > beta) {
            swap(alpha, beta);
        }
    }
}

void solve() {
    // Convert 2D problem to 1D angle ranges and use greedy approach:
    // Union overlapping sheep ranges, sort wolves by right endpoint,
    // greedily place each wolf as far right as possible without hitting sheep.
    //
    // Note that in this problem we have Y >= 1, meaning that all angles will be
    // positive. It also means that we don't actually need to look at all
    // rotations to run the greedy for, and we can simply sort by atan2 angle
    // starting from 0.0.

    if(!sheep.empty()) {
        sort(sheep.begin(), sheep.end());
        vector<pair<double, double>> merged;
        auto current = sheep[0];

        for(int i = 1; i < (int)sheep.size(); i++) {
            if(sheep[i].first <= current.second + eps) {
                current.second = max(current.second, sheep[i].second);
            } else {
                merged.push_back(current);
                current = sheep[i];
            }
        }
        merged.push_back(current);
        sheep = merged;
    }

    sort(wolves.begin(), wolves.end(), [](const auto& a, const auto& b) {
        return a.second < b.second;
    });

    int pos_sheep = 0;
    double last = -1e9;
    int count = 0;

    for(auto& wolf: wolves) {
        if(wolf.first < last + eps) {
            continue;
        }

        while(pos_sheep < (int)sheep.size() &&
              wolf.second > sheep[pos_sheep].second) {
            pos_sheep++;
        }

        double rightmost = wolf.second;
        if(pos_sheep < (int)sheep.size()) {
            rightmost = min(rightmost, sheep[pos_sheep].first - eps);
        }

        if(rightmost < wolf.first - eps) {
            cout << "No solution\n";
            return;
        }

        last = rightmost;
        count++;
    }

    cout << count << "\n";
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
