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

struct specialty {
    string name;
    int required;
};

struct faculty {
    string name;
    int k;
    vector<int> subjects;
    vector<tuple<int, int, int>> thresholds;
    vector<specialty> specs;
};

int n, m;
vector<int> scores;
vector<faculty> faculties;

void read() {
    cin >> n >> m;
    scores.assign(m, 0);
    cin >> scores;

    faculties.assign(n, {});
    for(auto& f: faculties) {
        cin >> ws;
        getline(cin, f.name);
        cin >> f.k;
        f.subjects.assign(f.k, 0);
        cin >> f.subjects;
        f.thresholds.assign(f.k, {});
        for(auto& [x, y, z]: f.thresholds) {
            cin >> x >> y >> z;
        }

        int s;
        cin >> s;
        f.specs.assign(s, {});
        for(auto& sp: f.specs) {
            cin >> ws;
            getline(cin, sp.name);
            cin >> sp.required;
        }
    }
}

void solve() {
    // Pure implementation. For each faculty we look up Peter's score on
    // every required subject, convert it to a 2/3/4/5 grade via the
    // X<Y<Z thresholds, sum the grades, and accept every specialty whose
    // required sum is at most that total. The only mildly fiddly part is
    // the parsing, since faculty and specialty names can contain spaces,
    // so we read the input line by line.

    vector<string> accepted;
    for(const auto& f: faculties) {
        int total = 0;
        for(int j = 0; j < f.k; j++) {
            int t = scores[f.subjects[j] - 1];
            auto [x, y, z] = f.thresholds[j];
            int grade = t < x ? 2 : t < y ? 3 : t < z ? 4 : 5;
            total += grade;
        }
        for(const auto& sp: f.specs) {
            if(total >= sp.required) {
                accepted.push_back(f.name + ' ' + sp.name);
            }
        }
    }

    if(accepted.empty()) {
        cout << "Army\n";
    } else {
        for(const auto& line: accepted) {
            cout << line << '\n';
        }
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
