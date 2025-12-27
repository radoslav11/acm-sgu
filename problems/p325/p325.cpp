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

string s;

void read() { cin >> s; }

int64_t count_inversions(vector<int>& a) {
    int64_t inv = 0;
    int n = a.size();
    vector<int> temp(n);

    function<void(int, int)> merge_sort = [&](int l, int r) {
        if(l >= r) {
            return;
        }
        int mid = l + (r - l) / 2;
        merge_sort(l, mid);
        merge_sort(mid + 1, r);

        int i = l, j = mid + 1, k = l;
        while(i <= mid && j <= r) {
            if(a[i] <= a[j]) {
                temp[k++] = a[i++];
            } else {
                temp[k++] = a[j++];
                inv += mid - i + 1;
            }
        }
        while(i <= mid) {
            temp[k++] = a[i++];
        }
        while(j <= r) {
            temp[k++] = a[j++];
        }
        for(int i = l; i <= r; i++) {
            a[i] = temp[i];
        }
    };

    merge_sort(0, n - 1);
    return inv;
}

void solve() {
    // There is a fairly straight forward greedy which we can optimize with
    // interval trees or inversions. Let's start with fixing the middle
    // character when |s| is odd - it's going to be the unique odd frequency
    // element. Afterwards, the greedy will go from the back, say currently it's
    // s[i], it will try to find the first occurrences s[f] == s[i], and move
    // s[f] to the front. It might not be immediately clear why this is correct,
    // but we can think of the contributions and how the matches will look.
    //
    // First off, for some character C, let the positions where it occurs in s
    // be i[1], ..., i[k]. It's not hard to see that we will match (i[1], i[k]),
    // (i[2], i[k - 1]) and so on, as otherwise we will make irrelevant swaps
    // between some of these pairs. Also, if k is odd, then i[(k + 1) / 2] will
    // be in the middle.
    //
    // We already handled the case of |s| being odd as we know what goes to the
    // middle. We now have split all positions into |s| / 2 pairs (l[i], r[i]).
    // We can think of the minimum number of swaps as sum of contributions
    // between each pair of pairs. In particular, we will never swap two
    // elements from the same pair (as they are equal and it doesn't make
    // sense), and for each pair of pairs we will make either 0 or 1 swaps.
    // Let's consider a few cases:
    //
    //     1) (l[i], r[i]) fully overlaps or is fully covered by (l[j], r[j]).
    //        In this case, we will never make a swap between these two pairs.
    //        It's clear that in the final solution it just makes sense for the
    //        outer pair to cover the inner one, so we can coordinate the swaps
    //        to have this covering invariant always held.
    //
    //     2) (l[i], r[i]) and (l[j], r[j]) intersect. Then we certainly need to
    //        break this intersection at some point so that one of them covers
    //        the other. After this is broken, we are essentially left with (1)
    //        which we can always maintain. Pairs like this contribute 1 to
    //        the answer.
    //
    // Both of the above contributions will be held no matter what palindrome we
    // go for (noting that we have the assumption of an optimal matching of the
    // equal characters). It's not hard to observe that the greedy does exactly
    // that. If we only keep (1) and (2), unfortunately we will be left with a
    // "tree" structure in terms of the covering of the intervals. However, to
    // have a palindrome, we actually want a chain - in particular, we want
    // there to be nested intervals (1, |s|), (2, |s|-1) and so on. The issues
    // comes from pairs that have both (l[i], r[i]) in one of the halves. Let's
    // say we have A such pairs with both endpoints in the left halve. Then we
    // also have A endpoints with both halves on the right side. The only
    // contribution we haven't accounted for is coming from these. Let's think
    // what the greedy would do - when faced with such an interval, it will
    // either be the leftmost or rightmost (based on L/R), and place one of the
    // endpoints on the opposite end. We can convince ourselves that this is
    // optimal by considering the relative order of pairs in each of these
    // halves. It never makes sense to swap the order of two pairs in the same
    // side, as we just gain 2 additional swaps for nothing. This means the
    // leftmost or rightmost such pair will become the outermost one, and the
    // greedy does precisely that.
    //
    // All in all, the above should convince ourselves that the greedy is
    // correct, but now we want to implement it O(n log n) as that's what's
    // required by this problem. The easiest way is to create a target
    // permutation we want to achieve rather than actually doing the swaps. This
    // is fairly easy if we run the greedy. We then will simply find the number
    // of inversions in the permutation.

    map<char, deque<int>> pos;
    for(int i = 0; i < (int)s.size(); i++) {
        pos[s[i]].push_back(i);
    }

    int cnt_odd = 0;
    for(auto [c, p]: pos) {
        cnt_odd += p.size() % 2;
    }

    if(cnt_odd != s.size() % 2) {
        cout << -1 << endl;
        return;
    }

    int l_p = 0, r_p = s.size() - 1;
    vector<int> p(s.size());
    vector<bool> used(s.size());

    for(int r = (int)s.size() - 1; r >= 0; r--) {
        if(used[r]) {
            continue;
        }

        used[r] = true;
        if(pos[s[r]].size() == 1) {
            p[s.size() / 2] = r;
            continue;
        }

        int l = pos[s[r]].front();
        pos[s[l]].pop_front();
        pos[s[r]].pop_back();

        used[l] = true;
        p[l_p++] = l;
        p[r_p--] = r;
    }

    cout << count_inversions(p) << endl;
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
