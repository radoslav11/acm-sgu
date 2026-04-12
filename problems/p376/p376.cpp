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

struct participant {
    int id;
    int64_t swim;
    int64_t ride;
    int64_t strength;
    int64_t total_time = 0;
};

int n, k;
vector<participant> parts;
vector<int> a, b;

void read() {
    cin >> n >> k;

    parts.resize(n);
    for(int i = 0; i < n; i++) {
        int w, r, p;
        cin >> w >> r >> p;
        parts[i].id = i + 1;
        parts[i].swim = 20000000 - w;
        parts[i].ride = 20000000 - r;
        parts[i].strength = p;
    }

    a.resize(k);
    cin >> a;
    b.resize(n);
    cin >> b;
}

void solve() {
    // This is solution is fairly straight forward: we can do a direct
    // simulation of the process with priority queue, or by iterating through
    // the stages and sorting the people at each stage (each person will be at
    // each stage exactly once which defines the mapping to the horses too).

    vector<vector<int64_t>> stage_horses(k, vector<int64_t>(n));
    for(int i = 0; i < k; i++) {
        for(int j = 0; j < n; j++) {
            int64_t ai = a[i];
            int64_t bj = b[j];
            stage_horses[i][j] = 3 * ai * ai + 5 * ai * bj + 2 * bj * bj;
        }
        sort(stage_horses[i].rbegin(), stage_horses[i].rend());
    }

    vector<int64_t> arrival(n);
    for(int i = 0; i < n; i++) {
        arrival[i] = parts[i].swim;
        parts[i].total_time = parts[i].swim;
    }

    for(int stage = 0; stage < k; stage++) {
        vector<int> order(n);
        iota(order.begin(), order.end(), 0);

        sort(order.begin(), order.end(), [&](int x, int y) {
            if(arrival[x] != arrival[y]) {
                return arrival[x] < arrival[y];
            }
            return parts[x].strength > parts[y].strength;
        });

        vector<int64_t> new_arrival(n);
        for(int pos = 0; pos < n; pos++) {
            int p = order[pos];
            int64_t horse = stage_horses[stage][pos];
            int64_t ride_time = parts[p].ride - horse;
            int64_t finish_this_stage = arrival[p] + ride_time;
            new_arrival[p] = finish_this_stage;
            parts[p].total_time = finish_this_stage;
        }

        arrival = new_arrival;
    }

    vector<int> final_order(n);
    iota(final_order.begin(), final_order.end(), 0);

    sort(final_order.begin(), final_order.end(), [&](int x, int y) {
        if(parts[x].total_time != parts[y].total_time) {
            return parts[x].total_time < parts[y].total_time;
        }
        return parts[x].strength > parts[y].strength;
    });

    for(int i = 0; i < n; i++) {
        if(i > 0) {
            cout << " ";
        }
        cout << parts[final_order[i]].id;
    }
    cout << endl;
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
