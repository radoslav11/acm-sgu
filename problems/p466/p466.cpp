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

int N, Q;
string slots;

set<tuple<int, int, int>> clusters_by_size;
set<pair<int, int>> clusters_by_pos;
map<int, int> cluster_head_to_id;
map<int, pair<int, int>> cluster_info;
map<int, vector<pair<int, int>>> query_allocations;
int next_cluster_id = 0;

void add_cluster(int head, int size) {
    int id = next_cluster_id++;
    clusters_by_size.insert({size, head, size});
    clusters_by_pos.insert({head, id});
    cluster_head_to_id[head] = id;
    cluster_info[id] = {head, size};
}

void remove_cluster(int id) {
    auto [head, size] = cluster_info[id];
    clusters_by_size.erase({size, head, size});
    clusters_by_pos.erase({head, id});
    cluster_info.erase(id);
}

int normalize(int pos) {
    if(pos <= 0) {
        return pos + N;
    }
    if(pos > N) {
        return pos - N;
    }
    return pos;
}

vector<pair<int, int>> get_intervals(int head, int size) {
    vector<pair<int, int>> intervals;
    if(head + size - 1 <= N) {
        intervals.push_back({head, head + size - 1});
    } else {
        intervals.push_back({head, N});
        intervals.push_back({1, size - (N - head + 1)});
    }
    return intervals;
}

void print_intervals(vector<pair<int, int>>& intervals) {
    sort(intervals.begin(), intervals.end());

    bool first = true;
    for(auto [l, r]: intervals) {
        if(!first) {
            cout << ",";
        }
        first = false;
        if(l == r) {
            cout << l;
        } else {
            cout << l << "-" << r;
        }
    }
    cout << "\n";
}

void read() {
    cin >> N >> Q;
    cin >> slots;
}

void solve() {
    /*
       We can do everything with std::set / binary search trees. We maintain two
       ways of accessing the clusters - one based on their lengths, and another
       based on their number / left location. Then searching for the correct
       cluster can be done with lower bound, and with some casework we can
       update what happens to the adjacent clusters (on addition and removal).
       We have to be extra careful about the wrapping.
    */

    int blocked = -1;
    for(int i = 1; i <= N; i++) {
        if(slots[i - 1] == 'X') {
            blocked = i;
            break;
        }
    }

    if(blocked == -1) {
        add_cluster(1, N);
    } else {
        int i = blocked;
        do {
            if(slots[i - 1] == '.') {
                int head = i;
                int size = 0;
                while(slots[i - 1] == '.') {
                    size++;
                    i = normalize(i + 1);
                }
                add_cluster(head, size);
            } else {
                i = normalize(i + 1);
            }
        } while(i != blocked);
    }

    for(int q = 1; q <= Q; q++) {
        string type;
        cin >> type;

        if(type == "PARK") {
            int S;
            cin >> S;

            auto it = clusters_by_size.lower_bound({S, -1, -1});
            if(it == clusters_by_size.end()) {
                cout << "NO ROOM\n";
            } else {
                auto [size, head, _] = *it;
                int cluster_id = cluster_head_to_id[head];

                vector<pair<int, int>> allocated = get_intervals(head, S);
                query_allocations[q] = allocated;
                print_intervals(allocated);

                remove_cluster(cluster_id);

                if(size > S) {
                    int new_head = normalize(head + S);
                    add_cluster(new_head, size - S);
                }
            }
        } else {
            int qi;
            cin >> qi;

            auto _tmp_q_it = query_allocations.find(qi);
            assert(_tmp_q_it != query_allocations.end());

            vector<pair<int, int>> intervals = _tmp_q_it->second;
            query_allocations.erase(_tmp_q_it);

            int head = intervals[0].first;
            int size = 0;
            for(auto [l, r]: intervals) {
                size += (r - l + 1);
            }

            if(clusters_by_pos.empty()) {
                add_cluster(head, size);
                continue;
            }

            auto cw_it =
                clusters_by_pos.lower_bound({normalize(head + size), -1});

            auto ccw_it = prev(
                cw_it != clusters_by_pos.begin() ? cw_it : clusters_by_pos.end()
            );

            if(cw_it == clusters_by_pos.end()) {
                cw_it = clusters_by_pos.begin();
            }

            int l_id = ccw_it->second, r_id = cw_it->second;

            bool merge_left =
                normalize(
                    cluster_info[l_id].first + cluster_info[l_id].second
                ) == head;
            bool merge_right =
                normalize(head + size) == cluster_info[r_id].first;

            if(merge_left) {
                head = cluster_info[l_id].first;
                size += cluster_info[l_id].second;
                remove_cluster(l_id);
            }

            if(merge_right) {
                size += cluster_info[r_id].second;
                remove_cluster(r_id);
            }

            if(size == N) {
                head = 1;
            }
            add_cluster(head, size);
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int T = 1;
    for(int test = 1; test <= T; test++) {
        read();
        solve();
    }
    return 0;
}
