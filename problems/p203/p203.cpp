#include <iostream>
#include <queue>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    queue<long long> q1;
    queue<long long> q2;

    for(int i = 0; i < n; i++) {
        long long freq;
        cin >> freq;
        q1.push(freq);
    }

    long long totalBits = 0;

    while(q1.size() + q2.size() > 1) {
        long long first, second;

        for(int i = 0; i < 2; i++) {
            if(q2.empty() || (!q1.empty() && q1.front() < q2.front())) {
                if(i == 0) {
                    first = q1.front();
                } else {
                    second = q1.front();
                }
                q1.pop();
            } else {
                if(i == 0) {
                    first = q2.front();
                } else {
                    second = q2.front();
                }
                q2.pop();
            }
        }

        long long combined = first + second;
        totalBits += combined;
        q2.push(combined);
    }

    cout << totalBits << endl;

    return 0;
}
