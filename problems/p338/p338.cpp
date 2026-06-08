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

double X;

void read() { cin >> X; }

void solve() {
    // Three sequential actors are resolved in reverse and composed. Sums live
    // in 1..27 with >21 meaning bust; EVs are in player-profit units (+1 win,
    // -1 loss). "Must roll at least once" is encoded by averaging over the
    // outcomes of one forced roll from 0. All ties break toward rolling.
    //
    // DP 1 - croupier vs Big Man alone, parametrized by Big Man's stop value
    // Sb in 1..21. Let f_b(Sc) be the player's EV from croupier state Sc; the
    // croupier picks the action that minimizes it. The boundary is
    // f_b(Sc)=+1 for Sc>21 (croupier busts). For Sc<=21 we compare
    // stop_v=-1 if Sc>=Sb else +1 (the croupier wins ties) against
    // roll_v=avg f_b(Sc+1..Sc+6), and set f_b(Sc)=min(stop_v, roll_v).
    // Big Man's EV if he stops at Sb is then U_b(Sb)=avg f_b(1..6), with
    // U_b(Sb)=-1 for Sb>21 (he already lost, no croupier rolls needed).
    //
    // DP 2 - Big Man's own stop/roll plan. Backward over his sum Sb:
    // V_bm(Sb)=max(U_b(Sb), avg V_bm(Sb+1..Sb+6)) for Sb<=21, V_bm=-1 above.
    // The argmax stored as action_bm[Sb]. A forward sweep seeded with
    // reach[1..6]=1/6 then pushes mass through roll-states and collects
    // dist_b on stop-states (1..21) and on bust states (22..27) separately.
    //
    // DP 3 - the real croupier vs both, parametrized by (Sb, Sa). The
    // croupier knows both scores and maximizes total casino EV =
    // X*(casino term from Big Man) + (casino term from Andrew). We carry
    // two scalars per croupier state: g(Sc) for casino EV and a(Sc) for
    // Andrew's own EV under whatever policy maximizes g. The key subtlety
    // is that a does not take a max/min: it just follows the action chosen
    // by g. Boundaries at Sc>21 use the bust terms directly. For Sc in
    // 21..1 we compute stop_casino, stop_andrew, roll_casino, roll_andrew,
    // pick by comparing roll_casino vs stop_casino, and assign both g and a
    // from the winning branch. Forcing one croupier roll gives Andrew's EV
    // if he stops at Sa: stop_a(Sa | Sb)=avg a(1..6).
    //
    // DP 4 - Andrew's own stop/roll plan, parametrized by Sb. Backward over
    // Sa: V_a(Sa)=max(stop_a(Sa | Sb), avg V_a(Sa+1..Sa+6)) for Sa<=21,
    // V_a=-1 above. Andrew's EV facing this Sb is andrew_ev(Sb)=avg V_a(1..6).
    //
    // Mixing those EVs over Big Man's distribution and mapping the EV in
    // [-1, 1] back to a probability gives the answer:
    //
    //   P(Andrew wins) = (sum_Sb dist_b[Sb] * andrew_ev(Sb) + 1) / 2.
    //
    // Complexity is O(21^2 * 27) - a few thousand floating-point ops.

    const int N = 28;

    auto roll_avg = [&](const vector<double>& v, int s) {
        double t = 0.0;
        for(int d = 1; d <= 6; d++) {
            t += v[s + d];
        }
        return t / 6.0;
    };

    vector<double> U_b(N, -1.0);
    for(int Sb = 1; Sb <= 21; Sb++) {
        vector<double> f(N, 0.0);
        for(int Sc = 22; Sc < N; Sc++) {
            f[Sc] = 1.0;
        }
        for(int Sc = 21; Sc >= 1; Sc--) {
            double stop_v = (Sc >= Sb) ? -1.0 : 1.0;
            double roll_v = roll_avg(f, Sc);
            f[Sc] = (roll_v <= stop_v) ? roll_v : stop_v;
        }
        U_b[Sb] = roll_avg(f, 0);
    }

    vector<double> V_bm(N, -1.0);
    vector<int> action_bm(N, 0);
    for(int Sb = N - 1; Sb >= 1; Sb--) {
        if(Sb > 21) {
            continue;
        }
        double stop_v = U_b[Sb];
        double roll_v = roll_avg(V_bm, Sb);
        if(roll_v >= stop_v) {
            V_bm[Sb] = roll_v;
            action_bm[Sb] = 1;
        } else {
            V_bm[Sb] = stop_v;
            action_bm[Sb] = 0;
        }
    }

    vector<double> reach(N, 0.0);
    for(int d = 1; d <= 6; d++) {
        reach[d] = 1.0 / 6.0;
    }
    vector<double> dist_b(N, 0.0);
    for(int Sb = 1; Sb <= 21; Sb++) {
        if(action_bm[Sb] == 1) {
            for(int d = 1; d <= 6; d++) {
                reach[Sb + d] += reach[Sb] / 6.0;
            }
        } else {
            dist_b[Sb] = reach[Sb];
        }
    }
    for(int Sb = 22; Sb < N; Sb++) {
        dist_b[Sb] = reach[Sb];
    }

    auto andrew_ev_given = [&](int Sb) {
        vector<double> stop_andrew(N, -1.0);
        for(int Sa = 1; Sa <= 21; Sa++) {
            vector<double> g(N, 0.0), a(N, 0.0);
            double big_bust = (Sb > 21) ? X : -X;
            double and_bust_casino = (Sa > 21) ? 1.0 : -1.0;
            double and_bust_andrew = (Sa > 21) ? -1.0 : 1.0;
            for(int Sc = 22; Sc < N; Sc++) {
                g[Sc] = big_bust + and_bust_casino;
                a[Sc] = and_bust_andrew;
            }
            for(int Sc = 21; Sc >= 1; Sc--) {
                double stop_big = (Sb > 21) ? X : ((Sc >= Sb) ? X : -X);
                double stop_and_c = (Sa > 21) ? 1.0 : ((Sc >= Sa) ? 1.0 : -1.0);
                double stop_and_a =
                    (Sa > 21) ? -1.0 : ((Sc >= Sa) ? -1.0 : 1.0);
                double stop_casino = stop_big + stop_and_c;
                double roll_casino = roll_avg(g, Sc);
                double roll_andrew = roll_avg(a, Sc);
                if(roll_casino >= stop_casino) {
                    g[Sc] = roll_casino;
                    a[Sc] = roll_andrew;
                } else {
                    g[Sc] = stop_casino;
                    a[Sc] = stop_and_a;
                }
            }
            stop_andrew[Sa] = roll_avg(a, 0);
        }

        vector<double> V_a(N, -1.0);
        for(int Sa = N - 1; Sa >= 1; Sa--) {
            if(Sa > 21) {
                continue;
            }
            double stop_v = stop_andrew[Sa];
            double roll_v = roll_avg(V_a, Sa);
            V_a[Sa] = (roll_v >= stop_v) ? roll_v : stop_v;
        }
        return roll_avg(V_a, 0);
    };

    double total_ev = 0.0;
    for(int Sb = 1; Sb < N; Sb++) {
        if(dist_b[Sb] == 0.0) {
            continue;
        }
        total_ev += dist_b[Sb] * andrew_ev_given(Sb);
    }

    double prob = (total_ev + 1.0) / 2.0;
    cout << fixed << setprecision(6) << prob << '\n';
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
