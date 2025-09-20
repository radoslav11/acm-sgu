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
vector<double> a;

void read() {
    cin >> n;
    a.resize(n);
    cin >> a;
}

void solve() {
    // The key observation is that the initial sequence has average of 2.0, and
    // all elements are within [1.95; 2.05]. We will show that the answer is
    // always "yes". A helpful reformulation is that we want all subarrays to
    // have average within [1.90;2.10], or equivalently, if we subtract 2.0 from
    // each element, all subarrays must have sum within [-0.10;0.10]. The
    // initial average of 2.0 also means that the sum of all elements is 0.0. To
    // have each sum within [-0.10;0.10], it sufficient for all prefix sums to
    // be within [-0.05;0.05], which we can guarantee by constructing the
    // permutation greedily:
    //
    // - If the current prefix sum is positive, we know that there is certainly
    //   at least one negative element remaining (since the total sum is 0.0),
    //   so we add the smallest element remaining.
    //
    // - If the current prefix sum is non-positive, we add the largest element
    //   remaining. We are guaranteed that there is at least one positive
    //   element remaining, since otherwise the total sum would be negative.
    //
    // This way, we guarantee that the prefix sum always stays within
    // [-min_element;max_element], which is a subset of [-0.05;0.05].

    for(auto& x: a) {
        x -= 2.0;
    }

    vector<int> perm(n), ans;
    iota(perm.begin(), perm.end(), 0);
    sort(perm.begin(), perm.end(), [&](int i, int j) { return a[i] < a[j]; });

    double sum = 0.0;
    int l = 0, r = n - 1;
    for(int i = 0; i < n; i++) {
        if(sum > 0) {
            sum += a[perm[l++]];
            ans.push_back(perm[l - 1] + 1);
        } else {
            sum += a[perm[r--]];
            ans.push_back(perm[r + 1] + 1);
        }
    }

    cout << "yes\n";
    cout << ans << '\n';
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
