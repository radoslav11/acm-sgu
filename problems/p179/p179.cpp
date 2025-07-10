#include <iostream>
#include <string>
using namespace std;

string nextSequence(string s) {
    int n = s.length();

    int i = n - 2;
    while(i >= 0 && s.substr(i, 2) == "()") {
        i -= 2;
    }

    if(i < 0) {
        return "No solution";
    }

    while(i >= 0 && s[i] == ')') {
        i--;
    }
    if(i < 0) {
        return "No solution";
    }

    s[i] = ')';

    int open = 0, close = 0;
    for(int j = 0; j <= i; j++) {
        if(s[j] == '(') {
            open++;
        } else {
            close++;
        }
    }

    int remainingOpen = n / 2 - open;
    int remainingClose = n / 2 - close;

    string result = s.substr(0, i + 1);
    for(int j = 0; j < remainingOpen; j++) {
        result += '(';
    }
    for(int j = 0; j < remainingClose; j++) {
        result += ')';
    }

    return result;
}

int main() {
    string s;
    cin >> s;
    cout << nextSequence(s) << endl;
    return 0;
}
