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

const int WEEK_MIN = 7 * 24 * 60;
const int WEEK_OFFSET = 3 * 24 * 60;
const int DOMAIN_END = 600000;
const int NOBODY = -2;

int n, m, t_hours;
int t1, t2;
int64_t thr;
vector<int> d_start, d_gid;
vector<int> up_a, up_b, up_e, up_gid;

unordered_map<string, int> name_id;

map<int, int> mp;
unordered_map<int64_t, int64_t> acc;
int bad_count = 0;

int get_gid(const string& name) {
    auto it = name_id.find(name);
    if(it != name_id.end()) {
        return it->second;
    }
    int id = (int)name_id.size();
    name_id[name] = id;
    return id;
}

int parse_dt(const string& date, const string& time) {
    static const int cum[12] = {0,   31,  59,  90,  120, 151,
                                181, 212, 243, 273, 304, 334};
    int mon = stoi(date.substr(5, 2));
    int day = stoi(date.substr(8, 2));
    int hh = stoi(time.substr(0, 2));
    int mm = stoi(time.substr(3, 2));
    int doy = cum[mon - 1] + (day - 1);
    return doy * 1440 + hh * 60 + mm;
}

int read_dt() {
    string date, time;
    cin >> date >> time;
    return parse_dt(date, time);
}

int64_t count_working_days(int64_t d) {
    int64_t full_weeks = d / 7, rem = d % 7;
    int64_t c = full_weeks * 5;
    for(int j = 0; j < rem; j++) {
        if((3 + j) % 7 < 5) {
            c++;
        }
    }
    return c;
}

int64_t working_prefix(int64_t t) {
    int64_t days = t / 1440, rem = t % 1440;
    int64_t c = count_working_days(days) * 540;
    if((days + 3) % 7 < 5) {
        c += max(int64_t(0), min(rem, int64_t(1080)) - 540);
    }
    return c;
}

int64_t nonworking(int l, int r) {
    return (int64_t)(r - l) - (working_prefix(r) - working_prefix(l));
}

void add_interval(int g, int l, int r, int delta) {
    if(g < 0) {
        return;
    }

    int cl = max(l, t1), cr = min(r, t2);
    if(cl >= cr) {
        return;
    }

    int w0 = (cl + WEEK_OFFSET) / WEEK_MIN;
    int w1 = (cr - 1 + WEEK_OFFSET) / WEEK_MIN;
    for(int w = w0; w <= w1; w++) {
        int64_t ws = (int64_t)w * WEEK_MIN - WEEK_OFFSET;
        int seg_l = max((int64_t)cl, ws);
        int seg_r = min((int64_t)cr, ws + WEEK_MIN);
        if(seg_l >= seg_r) {
            continue;
        }

        int64_t key = (int64_t)g * 64 + w;
        int64_t before = acc[key];
        int64_t after = before + (int64_t)delta * nonworking(seg_l, seg_r);
        bad_count += (after > thr) - (before > thr);
        acc[key] = after;
    }
}

map<int, int>::iterator split(int pos) {
    auto it = mp.lower_bound(pos);
    if(it != mp.end() && it->first == pos) {
        return it;
    }

    --it;
    mp[pos] = it->second;
    return mp.find(pos);
}

void assign(int b, int e, int g) {
    auto it_r = split(e);
    auto it_l = split(b);
    for(auto it = it_l; it != it_r; ++it) {
        add_interval(it->second, it->first, next(it)->first, -1);
    }

    mp.erase(it_l, it_r);
    mp[b] = g;
    add_interval(g, b, e, +1);
}

void read() {
    cin >> n >> m >> t_hours;
    thr = (int64_t)t_hours * 60;
    t1 = read_dt();
    t2 = read_dt();

    d_start.resize(n);
    d_gid.resize(n);
    for(int i = 0; i < n; i++) {
        d_start[i] = read_dt();
        string name;
        cin >> name;
        d_gid[i] = get_gid(name);
    }

    up_a.resize(m);
    up_b.resize(m);
    up_e.resize(m);
    up_gid.resize(m);
    for(int i = 0; i < m; i++) {
        up_a[i] = read_dt();
        up_b[i] = read_dt();
        up_e[i] = read_dt();
        string name;
        cin >> name;
        up_gid[i] = get_gid(name);
    }
}

void solve() {
    // Pure implementation. Dates are turned into minutes since 2009-01-01
    // 00:00. The schedule evolves over wall-clock time: it starts as the
    // initial assignment and each update at time A overwrites the slot [B, E)
    // with one guardian. We must integrate the wall-clock time inside [t1, t2]
    // during which the currently-active version of the schedule is valid, i.e.
    // no guardian works more than T non-working hours in any calendar week
    // (only duty inside [t1, t2] counts).
    //
    // The current schedule is kept as an interval-assignment structure (an ODT
    // on a std::map): mp[start] = guardian, covering [start, next). For every
    // interval we add its non-working minutes within [t1, t2], split per
    // calendar week, into acc[(guardian, week)], and keep bad_count = number of
    // those pairs exceeding the limit; the schedule is valid exactly when
    // bad_count == 0. An update splits at B and E, subtracts the contributions
    // of the erased pieces, and adds the new one, so bad_count stays current in
    // amortized near-constant work per update.
    //
    // Updates arrive in non-decreasing time order, so we sweep them: the
    // version active just before applying update i holds from the previous
    // event time until A_i, and we add that wall-clock span (clipped to [t1,
    // t2]) to the valid total whenever bad_count == 0. Ties at the same A_i
    // give zero-length spans and contribute nothing. The answer is the valid
    // span divided by (t2 - t1).

    mp.clear();
    acc.clear();
    bad_count = 0;

    mp[0] = NOBODY;
    for(int i = 0; i < n; i++) {
        mp[d_start[i]] = d_gid[i];
    }
    mp[DOMAIN_END] = -1;

    for(auto it = mp.begin(); next(it) != mp.end(); ++it) {
        add_interval(it->second, it->first, next(it)->first, +1);
    }

    int64_t valid = 0;
    int prev = t1;
    for(int i = 0; i < m; i++) {
        int seg_l = max(t1, prev), seg_r = min(t2, up_a[i]);
        if(seg_r > seg_l && bad_count == 0) {
            valid += seg_r - seg_l;
        }

        if(up_a[i] >= t2) {
            prev = t2;
            break;
        }

        assign(up_b[i], up_e[i], up_gid[i]);
        prev = up_a[i];
    }

    int seg_l = max(t1, prev), seg_r = t2;
    if(seg_r > seg_l && bad_count == 0) {
        valid += seg_r - seg_l;
    }

    cout << setprecision(17) << (double)valid / (double)(t2 - t1) << '\n';
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
