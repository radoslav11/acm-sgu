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

struct team_info {
    string name;
    int solved;
    long long penalty;
    int rank;
    vector<int> rejects_unsolved;
};

int n_problems;
vector<team_info> teams;
int balloons;
string my_team;

bool read_nonempty_line(string& line) {
    while(getline(cin, line)) {
        for(char c: line) {
            if(!isspace((unsigned char)c)) {
                return true;
            }
        }
    }
    return false;
}

bool is_team_row(const string& line) {
    for(char c: line) {
        if(c == '.' || c == '+' || c == '-') {
            return true;
        }
    }
    return false;
}

team_info parse_team_row(const string& line) {
    team_info t;
    int last = (int)line.size() - 1;
    while(last >= 0 && isspace((unsigned char)line[last])) {
        last--;
    }
    auto skip_spaces = [&]() {
        while(last >= 0 && line[last] == ' ') {
            last--;
        }
    };
    auto read_digits_back = [&]() {
        long long val = 0, base = 1;
        while(last >= 0 && isdigit((unsigned char)line[last])) {
            val += base * (line[last] - '0');
            base *= 10;
            last--;
        }
        return val;
    };
    for(int p = 0; p < n_problems; p++) {
        skip_spaces();
        long long num = read_digits_back();
        skip_spaces();
        char sign = line[last--];
        if(sign != '+') {
            t.rejects_unsolved.push_back((int)num);
        }
    }
    skip_spaces();
    t.penalty = read_digits_back();
    skip_spaces();
    t.solved = (int)read_digits_back();

    int s = 0;
    while(s < (int)line.size() && line[s] == ' ') {
        s++;
    }
    int rank = 0;
    while(s < (int)line.size() && isdigit((unsigned char)line[s])) {
        rank = rank * 10 + (line[s] - '0');
        s++;
    }
    while(s < (int)line.size() && line[s] == ' ') {
        s++;
    }
    while(last >= s && line[last] == ' ') {
        last--;
    }
    t.rank = rank;
    t.name = line.substr(s, last - s + 1);
    return t;
}

bool read() {
    string line;
    if(!read_nonempty_line(line)) {
        return false;
    }
    {
        istringstream iss(line);
        string tok;
        iss >> tok >> tok >> tok >> tok;
        n_problems = 0;
        while(iss >> tok) {
            n_problems++;
        }
    }
    teams.clear();
    while(true) {
        if(!read_nonempty_line(line)) {
            return false;
        }
        if(!is_team_row(line)) {
            balloons = stoi(line);
            break;
        }
        teams.push_back(parse_team_row(line));
    }
    if(!read_nonempty_line(line)) {
        return false;
    }
    int s = 0;
    while(s < (int)line.size() && isspace((unsigned char)line[s])) {
        s++;
    }
    int e = (int)line.size();
    while(e > s && isspace((unsigned char)line[e - 1])) {
        e--;
    }
    my_team = line.substr(s, e - s);
    return true;
}

bool better_than(const team_info& a, const team_info& b) {
    if(a.solved != b.solved) {
        return a.solved > b.solved;
    }
    return a.penalty < b.penalty;
}

void accept_one(team_info& t) {
    auto it = min_element(t.rejects_unsolved.begin(), t.rejects_unsolved.end());
    t.solved++;
    t.penalty += 241 + 20LL * (*it);
    t.rejects_unsolved.erase(it);
}

int high_pos(vector<team_info> ts, int my_idx, int balloons_left) {
    team_info& me = ts[my_idx];
    while(balloons_left > 0 && !me.rejects_unsolved.empty()) {
        accept_one(me);
        balloons_left--;
    }
    int n = (int)ts.size();
    bool absorbed_elsewhere;
    if(my_idx != n - 1) {
        absorbed_elsewhere = true;
    } else if(my_idx == 0) {
        absorbed_elsewhere = true;
    } else {
        absorbed_elsewhere = ts[my_idx - 1].solved < n_problems;
    }
    if(absorbed_elsewhere) {
        balloons_left = 0;
    }
    if(balloons_left > 0) {
        me.penalty += 20LL * balloons_left;
    }
    int rank = my_idx + 1;
    for(int i = my_idx - 1; i >= 0 && !better_than(ts[i], me); i--) {
        rank--;
    }
    return rank;
}

int low_pos(vector<team_info> ts, int my_idx, int balloons_left) {
    int n = (int)ts.size();
    for(int i = my_idx - 1; i >= 0 && ts[i].rank == ts[my_idx].rank; i--) {
        if(i == 0 || ts[i - 1].rank != ts[my_idx].rank) {
            swap(ts[i], ts[my_idx]);
            my_idx = i;
            break;
        }
    }
    team_info& me = ts[my_idx];
    int rank = me.rank;
    int i = my_idx + 1;
    while(balloons_left > 0 && i < n) {
        int j = i;
        while(i < n && ts[i].solved == ts[j].solved) {
            i++;
        }
        int det = me.solved - ts[j].solved;
        if(balloons_left < det) {
            break;
        }
        for(int l = j; l < i; l++) {
            for(int k = 0; k < det; k++) {
                accept_one(ts[l]);
            }
        }
        for(int l = j; balloons_left >= det && l < i; l++) {
            if(better_than(ts[l], me)) {
                balloons_left -= det;
                rank++;
            }
        }
        for(int l = j; balloons_left >= det + 1 && l < i; l++) {
            if(!better_than(ts[l], me) && !ts[l].rejects_unsolved.empty()) {
                balloons_left -= (det + 1);
                accept_one(ts[l]);
                rank++;
            }
        }
    }
    return rank;
}

void solve() {
    // This is purely an implementation problem: a careful parser plus a
    // greedy simulation, no clever algorithmic trick.
    //
    // Best (smallest) place: our team takes as many of its unsolved problems
    // as possible at minute 241, choosing the unsolved problem with the
    // fewest existing rejects first to minimize the added penalty. Any
    // leftover balloons are assumed absorbed by other teams without changing
    // our rank. The only case where they cannot be absorbed elsewhere is
    // when we are last AND every team above us has already solved every
    // problem: then each leftover balloon is forced onto our team as an
    // extra reject (+20 penalty). Once our state is fixed, the rank is
    // 1 + (# teams originally above us that still beat the new state). The
    // standings are sorted, so we scan upward from our position and stop at
    // the first beater.
    //
    // Worst (largest) place: keep our team's state untouched and try to push
    // as many of the teams currently below us past us. We first move our
    // team to the head of its rank-tie block so that tied teams below can
    // be made into overtakes through the +1-accept path. Then walk the
    // table in groups of equal solved count. A team in a group with
    // det = my_solved - their_solved fewer accepts overtakes us with either
    // det accepts (tied solved, win on penalty) or det + 1 accepts (strictly
    // out-solving us). Within each group we greedily count teams that
    // overtake at cost det, then teams that overtake at cost det + 1, until
    // balloons run out or no more groups are reachable. Each overtake adds
    // one to the rank.

    int my_idx = -1;
    for(int i = 0; i < (int)teams.size(); i++) {
        if(teams[i].name == my_team) {
            my_idx = i;
            break;
        }
    }
    int hp = high_pos(teams, my_idx, balloons);
    int lp = low_pos(teams, my_idx, balloons);
    cout << hp << ' ' << lp << '\n';
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    while(read()) {
        solve();
    }

    return 0;
}
