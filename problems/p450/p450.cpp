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

const int inf = (int)2e9 + 42;

int n, m, t;
vector<int> cap, start, group_size, deadline, eat_time;

void read() {
    cin >> n >> m >> t;
    cap.resize(n);
    for(int i = 0; i < n; i++) {
        cin >> cap[i];
    }

    start.resize(m);
    group_size.resize(m);
    deadline.resize(m);
    eat_time.resize(m);
    for(int i = 0; i < m; i++) {
        cin >> start[i] >> group_size[i] >> deadline[i] >> eat_time[i];
    }
}

vector<vector<int>> cc;
vector<double> satisfaction;
vector<bool> seated;

struct Option {
    int min_dist;
    int max_dist;
    int idx;
    int pos;
};

vector<Option> opts;

priority_queue<
    pair<int64_t, int>, vector<pair<int64_t, int>>, greater<pair<int64_t, int>>>
    ev;
queue<int> waiting;

bool is_better(const Option& a, const Option& b) {
    if(a.min_dist != b.min_dist) {
        return a.min_dist > b.min_dist;
    }
    if(a.max_dist != b.max_dist) {
        return a.max_dist > b.max_dist;
    }
    if(a.idx != b.idx) {
        return a.idx < b.idx;
    }
    return a.pos < b.pos;
}

bool can_seat(int g) {
    bool found = false;
    Option best_opt;

    for(int c = 0; c < n; c++) {
        int sum = 0;
        int last_left, last_right;

        for(int i = 0; i + group_size[g] <= cap[c]; i++) {
            if(i == 0) {
                for(int j = 0; j < group_size[g]; j++) {
                    sum += cc[c][j];
                }
                last_left = -1;
                last_right = group_size[g];
            } else {
                sum += cc[c][i + group_size[g] - 1] - cc[c][i - 1];
                if(cc[c][i - 1]) {
                    last_left = i - 1;
                }
                last_right = max(last_right, i + (int)group_size[g]);
            }
            while(last_right < cap[c] && !cc[c][last_right]) {
                last_right++;
            }

            if(!sum) {
                int left_span = (last_left == -1) ? inf : (i - 1 - last_left);
                int right_span = (last_right == cap[c])
                                     ? inf
                                     : (last_right - i - group_size[g]);
                Option cur_opt = {
                    min(left_span, right_span), max(left_span, right_span), c, i
                };

                if(!found || is_better(cur_opt, best_opt)) {
                    found = true;
                    best_opt = cur_opt;
                }
            }
        }
    }

    if(found) {
        opts[g] = best_opt;
        seated[g] = true;
        for(int i = 0; i < group_size[g]; i++) {
            cc[best_opt.idx][best_opt.pos + i] = 1;
        }
    }

    return found;
}

void remove_group(int g) {
    Option& opt = opts[g];
    for(int i = 0; i < group_size[g]; i++) {
        cc[opt.idx][opt.pos + i] = 0;
    }
}

void solve() {
    // The simulator is deterministic, so this is essentially an implementation
    // problem. We can notice that, constraints for N and C are tiny, so we
    // don't even need to implement operations with BSTs. In theory we
    // should be able to use a BST over empty ranges and one over
    // counters, and always find the largest empty interval which will be split
    // in half. However, for given constraints we don't really need this. We
    // can simply keep an event priority queue, and implement each leaving or
    // entering operation naively.

    cc.resize(n);
    for(int i = 0; i < n; i++) {
        cc[i].assign(cap[i], 0);
    }

    satisfaction.resize(m);
    opts.resize(m);
    seated.assign(m, false);

    for(int i = 0; i < m; i++) {
        satisfaction[i] = -1;
        ev.emplace(start[i], i);
        ev.emplace(start[i] + deadline[i], m);
    }

    while(!ev.empty()) {
        int64_t cur_time = ev.top().first;
        if(cur_time >= t) {
            break;
        }

        while(!ev.empty() && ev.top().first == cur_time) {
            int id = ev.top().second;
            ev.pop();

            if(id < 0) {
                remove_group(-id - 1);
            }
            if(id >= 0 && id < m) {
                waiting.push(id);
            }
        }

        while(!waiting.empty()) {
            int g = waiting.front();
            if(cur_time <= start[g] + deadline[g]) {
                if(can_seat(g)) {
                    ev.emplace(cur_time + eat_time[g], -(g + 1));
                    satisfaction[g] =
                        (double)(start[g] + deadline[g] - cur_time) /
                        deadline[g];
                    waiting.pop();
                    continue;
                }
            }
            if(cur_time >= start[g] + deadline[g]) {
                waiting.pop();
                continue;
            }
            break;
        }
    }

    int64_t tot_sz = 0;
    double tot = 0.0;
    for(int i = 0; i < m; i++) {
        tot += satisfaction[i] * group_size[i];
        tot_sz += group_size[i];
    }

    cout << fixed << setprecision(16) << tot / tot_sz << "\n";
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
