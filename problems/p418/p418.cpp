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

string src;

enum tok_kind { word_tok, str_tok, punct_tok };

struct token {
    tok_kind kind;
    string val;
};

struct symbol {
    bool terminal;
    char ch;
    string name;
};

struct segment {
    bool conditional;
    string call_name;
    vector<pair<char, vector<segment>>> branches;
    bool else_error;
};

vector<token> toks;
size_t pos;

void tokenize() {
    size_t i = 0, n = src.size();
    while(i < n) {
        char c = src[i];
        if(isspace((unsigned char)c)) {
            i++;
            continue;
        }

        if(isalpha((unsigned char)c)) {
            string w;
            while(i < n && isalpha((unsigned char)src[i])) {
                w += (char)tolower((unsigned char)src[i]);
                i++;
            }
            toks.push_back({word_tok, w});
        } else if(c == '\'') {
            i++;
            string content;
            while(i < n) {
                if(src[i] == '\'') {
                    if(i + 1 < n && src[i + 1] == '\'') {
                        content += '\'';
                        i += 2;
                    } else {
                        i++;
                        break;
                    }
                } else {
                    content += src[i];
                    i++;
                }
            }
            toks.push_back({str_tok, content});
        } else {
            toks.push_back({punct_tok, string(1, c)});
            i++;
        }
    }
}

bool at_word(const string& w) {
    return toks[pos].kind == word_tok && toks[pos].val == w;
}

vector<segment> parse_segments();

segment parse_conditional() {
    segment s;
    s.conditional = true;
    s.else_error = false;
    pos++;  // if
    while(true) {
        pos++;  // peek
        pos++;  // =
        char c = toks[pos].val[0];
        pos++;  // character literal
        pos++;  // then
        pos++;  // begin
        pos++;  // skip
        pos++;  // ;
        vector<segment> inner = parse_segments();
        pos++;  // end
        s.branches.push_back({c, std::move(inner)});

        if(at_word("else")) {
            pos++;  // else
            if(at_word("if")) {
                pos++;  // if
                continue;
            }
            pos++;  // error
            pos++;  // ;
            s.else_error = true;
            break;
        }

        pos++;  // ;
        break;
    }

    return s;
}

vector<segment> parse_segments() {
    vector<segment> segs;
    while(!at_word("end")) {
        if(at_word("if")) {
            segs.push_back(parse_conditional());
        } else {
            segment s;
            s.conditional = false;
            s.call_name = toks[pos].val;
            pos++;  // name
            pos++;  // ;
            segs.push_back(std::move(s));
        }
    }

    return segs;
}

vector<vector<symbol>> expand_segments(const vector<segment>& segs);

vector<vector<symbol>> expand_segment(const segment& s) {
    if(!s.conditional) {
        symbol sym{false, 0, s.call_name};
        return {{sym}};
    }

    vector<vector<symbol>> out;
    for(const auto& br: s.branches) {
        symbol t{true, br.first, ""};
        for(auto& inner: expand_segments(br.second)) {
            vector<symbol> prod{t};
            prod.insert(prod.end(), inner.begin(), inner.end());
            out.push_back(std::move(prod));
        }
    }
    if(!s.else_error) {
        out.push_back({});
    }

    return out;
}

vector<vector<symbol>> expand_segments(const vector<segment>& segs) {
    vector<vector<symbol>> result = {{}};
    for(const auto& s: segs) {
        vector<vector<symbol>> seg_prods = expand_segment(s);
        vector<vector<symbol>> next;
        for(auto& a: result) {
            for(auto& b: seg_prods) {
                vector<symbol> c = a;
                c.insert(c.end(), b.begin(), b.end());
                next.push_back(std::move(c));
            }
        }
        result = std::move(next);
    }

    return result;
}

string render(const vector<symbol>& prod) {
    string out;
    size_t i = 0;
    while(i < prod.size()) {
        if(prod[i].terminal) {
            out += '\'';
            while(i < prod.size() && prod[i].terminal) {
                out += prod[i].ch;
                i++;
            }
            out += '\'';
        } else {
            out += '<';
            out += prod[i].name;
            out += '>';
            i++;
        }
    }

    return out;
}

void read() {
    stringstream ss;
    ss << cin.rdbuf();
    src = ss.str();
}

void solve() {
    // This is a pure parsing/implementation problem: read the Pascal source,
    // recover each parsing routine's body, and emit the grammar it encodes.
    //
    // The standard way to handle a task like this is to split it into two
    // stages. First a tokenizer walks the raw text once and turns it into a
    // flat list of tokens, collapsing all the optional whitespace away: a
    // maximal run of letters becomes one case-folded word token, a run wrapped
    // in apostrophes becomes a string token, and every other byte is its own
    // punctuation token. After this stage the language is regular in the
    // tokens, so the messy "whitespace may appear anywhere between words" rule
    // never has to be thought about again.
    //
    // The second stage is a recursive-descent parser over that token list. A
    // single cursor (pos) scans forward and each grammar construct gets one
    // function that consumes exactly its tokens and advances the cursor:
    // parse_segments reads a run of segments until the closing End, and
    // parse_conditional reads an If / Else-If / Else chain, recursing back into
    // parse_segments for each branch body. Because the input is guaranteed
    // well-formed we advance the cursor positionally instead of validating.
    //
    // A parsing routine is any "Procedure <name>; Begin ..." with Skip and
    // Error excluded and Peek being a Function, so a linear scan over the
    // tokens finds every definition and parses its body.
    //
    // Finally each body is expanded into productions. Evaluating the If's of a
    // routine in every non-Error way is just a cartesian product: a segment
    // list expands to the concatenation of its segments' expansions, an
    // unconditional call contributes its nonterminal, and a conditional
    // contributes, per branch, the guessed character (a terminal, since Skip
    // consumes it) followed by that branch's body, plus the empty alternative
    // when there is no trailing Else Error. Rendering merges adjacent terminals
    // into one quoted literal, and both the alternatives on a line and the
    // lines themselves are sorted lexicographically.

    tokenize();

    vector<pair<string, vector<segment>>> routines;
    pos = 0;
    while(pos < toks.size()) {
        bool is_def =
            toks[pos].kind == word_tok && toks[pos].val == "procedure" &&
            pos + 3 < toks.size() && toks[pos + 1].kind == word_tok &&
            toks[pos + 3].kind == word_tok && toks[pos + 3].val == "begin" &&
            toks[pos + 1].val != "skip" && toks[pos + 1].val != "error";
        if(is_def) {
            string name = toks[pos + 1].val;
            pos += 4;  // procedure name ; begin
            vector<segment> body = parse_segments();
            pos++;  // end
            pos++;  // ;
            routines.push_back({name, std::move(body)});
        } else {
            pos++;
        }
    }

    vector<string> lines;
    for(auto& [name, body]: routines) {
        vector<string> rendered;
        for(auto& prod: expand_segments(body)) {
            rendered.push_back(render(prod));
        }
        sort(rendered.begin(), rendered.end());

        string line = "<" + name + ">::=";
        for(size_t i = 0; i < rendered.size(); i++) {
            if(i) {
                line += '|';
            }
            line += rendered[i];
        }
        lines.push_back(line);
    }

    sort(lines.begin(), lines.end());
    for(auto& line: lines) {
        cout << line << "\n";
    }
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
