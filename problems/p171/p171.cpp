#include <bits/stdc++.h>
using namespace std;

struct Zone {
    int q, cap, id;
};

struct Student {
    int p, w, id;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int K;
    cin >> K;

    vector<Zone> zones(K);
    long long totalN = 0;
    for(int i = 0; i < K; i++) {
        cin >> zones[i].cap;
        totalN += zones[i].cap;
    }

    for(int i = 0; i < K; i++) {
        cin >> zones[i].q;
        zones[i].id = i;
    }

    vector<Student> students(totalN);
    for(int i = 0; i < totalN; i++) {
        cin >> students[i].p;
    }

    for(int i = 0; i < totalN; i++) {
        cin >> students[i].w;
        students[i].id = i;
    }

    sort(students.begin(), students.end(), [&](auto& a, auto& b) {
        return a.w > b.w;
    });

    sort(zones.begin(), zones.end(), [&](auto& a, auto& b) {
        return a.q < b.q;
    });

    vector<int> answer(totalN, -1);

    for(auto& st: students) {
        int bestZoneIndex = -1;
        for(int j = K - 1; j >= 0; j--) {
            if(zones[j].q < st.p && zones[j].cap > 0) {
                bestZoneIndex = j;
                break;
            }
        }
        if(bestZoneIndex != -1) {
            answer[st.id] = zones[bestZoneIndex].id + 1;
            zones[bestZoneIndex].cap--;
        }
    }

    int zIndex = 0;
    for(int i = 0; i < totalN; i++) {
        if(answer[i] == -1) {
            while(zIndex < K && zones[zIndex].cap == 0) {
                zIndex++;
            }
            answer[i] = zones[zIndex].id + 1;
            zones[zIndex].cap--;
        }
    }

    for(int i = 0; i < totalN; i++) {
        cout << answer[i] << (i + 1 < totalN ? ' ' : '\n');
    }

    return 0;
}
