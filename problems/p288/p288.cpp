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

int n;

void read() { cin >> n; }

void solve() {
    // We want to partition the edges of a complete graph into the least number
    // of matchings. This is actually called the 1-factorization of a graph, but
    // there is also a simple approach to think about this.
    //
    // Let's consider the case of n being odd. In this case at each round we
    // will always leave one vertex out so it's not hard to observe that we will
    // need at least n rounds. From now we will call the vertices people, and
    // the edges at one round handshakes. We will place the n people around a
    // rectangular table with one head chair and (n-1)/2 chairs on each side.
    // For a given arrangement of people, let's have a handshake between people
    // staying opposite of each other, where the person staying at the head
    // table will be left out. Now let's rotate everyone clockwise and repeat
    // the same process N times. We claim that each pair of people will make a
    // handshake exactly once. One way to convince ourselves of that is to
    // choose an arbitrary person and reflect the offset from him for his
    // handshakes. Consider it's the person staying "last", on the "top" part of
    // the table. On the first round he will shake hands with the person with
    // offset 1 from him. On the second round they rotate so he shakes hands
    // with person before him that has offset n-1 from him, afterwards he moves
    // but everyone else moves so it's n-3, and if we continue we will see that
    // the offset always changes by 2. This is also true for any other person,
    // it's just that the initial offset could be different. We know that n is
    // odd and the change is also 2, so gcd(n, 2) = 1 and each of the offsets
    // will be visited exactly once in n steps.
    //
    // For even n, the case is almost the same - we will use the same table so
    // that gcd(size_table, 2) = 1, but one person will be left standing. They
    // will always do a handshake with the person sitting on the head space and
    // it's clear this means that they will shake hands with everyone too. In
    // this case we only need n-1 rounds.

    if (n == 1) {
        cout << "0\n0\n";
        return;
    }

    int num_rounds = (n % 2 == 0) ? n - 1 : n;
    cout << num_rounds << '\n';

    int table_size = (n % 2 == 0) ? n - 1 : n;
    vector<vector<int>> schedule(n, vector<int>(n, 0));

    for(int r = 0; r < num_rounds; r++) {
        if(n % 2 == 0) {
            int head_person = (table_size - r) % table_size;
            schedule[head_person][n - 1] = r + 1;
            schedule[n - 1][head_person] = r + 1;
        }

        for(int i = 1; i <= (table_size - 1) / 2; i++) {
            int p1 = ((i - r) % table_size + table_size) % table_size;
            int p2 =
                ((table_size - i - r) % table_size + table_size) % table_size;
            schedule[p1][p2] = r + 1;
            schedule[p2][p1] = r + 1;
        }
    }

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if(j > 0) {
                cout << ' ';
            }
            cout << schedule[i][j];
        }
        cout << '\n';
    }
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
