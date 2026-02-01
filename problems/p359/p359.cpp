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
vector<string> lines;

void read() {
    cin >> n;
    lines.resize(n);
    cin >> lines;
}

void solve() {
    // This turns out to just be an implementation problem, and particularly in
    // C++ we can just use pointers and directly implement this.
    int* pa = new int(0);
    int* pb = new int(0);
    int* pc = new int(0);
    int* pd = new int(0);

    for(int i = 0; i < n; i++) {
        string line = lines[i];

        if(!line.empty() && line.back() == ';') {
            line.pop_back();
        }

        if(line.substr(0, 8) == "writeln(") {
            string inside = line.substr(8, line.size() - 8);
            if(!inside.empty() && inside.back() == ')') {
                inside.pop_back();
            }
            if(inside.size() != 2 || inside[1] != '^') {
                continue;
            }
            char p = inside[0];
            int val;
            if(p == 'a') {
                val = *pa;
            } else if(p == 'b') {
                val = *pb;
            } else if(p == 'c') {
                val = *pc;
            } else if(p == 'd') {
                val = *pd;
            } else {
                continue;
            }

            cout << val << endl;
        } else {
            size_t pos = line.find(":=");
            if(pos == string::npos) {
                continue;
            }

            string left = line.substr(0, pos);
            string right = line.substr(pos + 2);

            if(left.back() == '^') {
                if(left.size() != 2 || left[1] != '^') {
                    continue;
                }
                char p1 = left[0];
                int* ptr1;
                if(p1 == 'a') {
                    ptr1 = pa;
                } else if(p1 == 'b') {
                    ptr1 = pb;
                } else if(p1 == 'c') {
                    ptr1 = pc;
                } else if(p1 == 'd') {
                    ptr1 = pd;
                } else {
                    continue;
                }

                if(isdigit(right[0]) && right.size() == 1) {
                    int val = right[0] - '0';
                    *ptr1 = val;
                } else {
                    if(right.size() != 2 || right[1] != '^') {
                        continue;
                    }
                    char p2 = right[0];
                    int val;
                    if(p2 == 'a') {
                        val = *pa;
                    } else if(p2 == 'b') {
                        val = *pb;
                    } else if(p2 == 'c') {
                        val = *pc;
                    } else if(p2 == 'd') {
                        val = *pd;
                    } else {
                        continue;
                    }

                    *ptr1 = val;
                }
            } else {
                if(left.size() != 1) {
                    continue;
                }
                char p1 = left[0];
                if(right.size() != 1) {
                    continue;
                }
                char p2 = right[0];

                int* newptr;
                if(p2 == 'a') {
                    newptr = pa;
                } else if(p2 == 'b') {
                    newptr = pb;
                } else if(p2 == 'c') {
                    newptr = pc;
                } else if(p2 == 'd') {
                    newptr = pd;
                } else {
                    continue;
                }

                if(p1 == 'a') {
                    pa = newptr;
                } else if(p1 == 'b') {
                    pb = newptr;
                } else if(p1 == 'c') {
                    pc = newptr;
                } else if(p1 == 'd') {
                    pd = newptr;
                }
            }
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
        // cout << "Case #" << test << ": ";
        solve();
    }

    return 0;
}
