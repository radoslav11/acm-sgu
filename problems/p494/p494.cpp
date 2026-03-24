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

int n, w, r, c;
vector<int> lens;

void read() {
    cin >> n >> w >> r >> c;
    lens.resize(n + 1);
    for(int i = 0; i < n; i++) {
        cin >> lens[i];
    }
    lens[n] = 0;
}

void solve() {
    // Given the constraints, we could afford a solution that is O(N*(R+W+C)),
    // or something of this sort. Naively, we could have tried all possible
    // locations for the upper left corner of the image in O((N + R) * C), and
    // then in O(N) checked how many rows this would actually take. To speed
    // this up, we can actually do something very similar - in stead of choosing
    // the full location, let's just choose the column in O(C). Now we can try
    // all possible options for the image starting below the row that ends with
    // word number i. More precisely, we can compute something like next[pos],
    // which corresponds to having our current row ending on word pos equaling
    // the number of the last word after one more row, but with an image here
    // (at the already fixed column). This next[.] can be computed easily in
    // O(log N) with binary search or O(1) with sliding window. Afterwards it's
    // enough to just try every possible position, and then apply
    // next[next[...]] from each position R times, to see where we would end up
    // after R rows. The candidate for the answer is then pref_rows[pos] + R +
    // suff_rows[next^R[pos]+1]. The slowest part here is computing next^R[pos],
    // as we can't afford a O(R) loop. However, we can do binary lifting and do
    // this in O(log R) instead. Overall, this solution would potentially be
    // still slow as it's in O(W * N * (log N + log R)).
    //
    // To speed this up further, we make two key improvements. First, we
    // precompute a table len_next[v][length] for every word v and every
    // available length, storing how many words fit starting from v with that
    // much space, using a sliding window in O(N * W) total, giving O(1) per
    // query instead of O(log N). Second, we replace binary lifting for
    // computing next^R[pos] with path compression similar to union-find. For
    // a fixed column, we need next^R[v] for each starting row's first word
    // v. We process rows j = 0, 1, ..., and for each, follow the chain
    // storing at each visited node both the final result and the number of
    // steps it compressed. When a later query passes through an already-
    // visited node v, it jumps directly to v's stored result and only extends
    // the chain by the remaining steps. Crucially, later rows always reach
    // shared nodes with more remaining steps (since the starting word is
    // closer to v, fewer steps were consumed reaching it), so the chain only
    // ever extends forward. This in practice is very quick and could maybe
    // be shown to be O(N) but I didn't get a clean argument after thinking for
    // a bit.
    //
    // Another option is to think of the tree where the parent of some position
    // is next[pos]. We want to find the K-th parent of each position, which 
    // can actually be done with a stack and a DFS: whenever we enter a node, 
    // we add it to the stack, while when we exit we pop it. It's always enough 
    // to just get the element at position depth(pos)-K, which we can do with 
    // a DFS in O(N).

    vector<vector<int>> len_next(n + 1, vector<int>(w + 1, n));
    for(int v = 0; v <= n; v++) {
        int nxt = min(n, v + 1);
        len_next[v][lens[v]] = nxt;
        for(int k = lens[v] + 1, gap = 0; k <= w - c; k++) {
            gap++;
            if(nxt < n && gap >= 1 + lens[nxt]) {
                nxt++;
                gap = 0;
            }
            len_next[v][k] = nxt;
        }
    }

    auto fit = [&](int v, int space) -> int {
        if(v == n || space < lens[v]) {
            return v;
        }
        return len_next[v][space];
    };

    auto advance = [&](int v, pair<int, int> prev) -> pair<int, int> {
        int col = prev.second + lens[v - 1] + (v < n ? 1 : 0);
        int row = prev.first;
        if(col + lens[v] > w) {
            col = 0;
            row++;
        }
        return {row, col};
    };

    vector<int> row_start(n + 1, 0);
    pair<int, int> cur = {0, 0};
    int num_lines = 0;
    for(int j = 1; j <= n; j++) {
        cur = advance(j, cur);
        if(cur.second == 0) {
            row_start[cur.first] = j;
        }
        if(j == n - 1) {
            num_lines = cur.first;
        }
    }

    struct Comp {
        int result = 0, steps = 0;
    };
    vector<Comp> comp(n + 1);

    auto make_compress = [&](int gap_left, int gap_right) {
        fill(comp.begin(), comp.end(), Comp{});
        auto impl = [&, gap_left,
                     gap_right](auto&& self, int v, int steps) -> int {
            if(v == n || steps == 0) {
                return v;
            }
            if(comp[v].steps) {
                if(comp[v].result != v) {
                    comp[v].result =
                        self(self, comp[v].result, steps - comp[v].steps);
                }
            } else {
                int nxt = fit(fit(v, gap_left), gap_right);
                comp[v].result = (nxt != v) ? self(self, nxt, steps - 1) : nxt;
            }
            comp[v].steps = steps;
            return comp[v].result;
        };
        return [impl](int v, int steps) { return impl(impl, v, steps); };
    };

    vector<pair<int, int>> best(n + 1, {n + r + 1, 0});

    int ans = num_lines + r + 1;
    for(int col = 0; col + c <= w; col++) {
        auto path_compress = make_compress(col, w - col - c);
        for(int j = 0; j <= num_lines; j++) {
            int end = path_compress(row_start[j], r);
            best[end] = min(best[end], {j + r - (end == n), 0});
        }
    }

    for(int j = 0; j < n; j++) {
        best[j + 1] = min(best[j + 1], advance(j + 1, best[j]));
    }

    ans = min(ans, best[n].first + 1);
    cout << ans << '\n';
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T = 1;
    cin >> T;
    for(int test = 1; test <= T; test++) {
        read();
        solve();
    }

    return 0;
}
