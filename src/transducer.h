#ifndef TRANSDUCER_H
#define TRANSDUCER_H

#include <vector>
#include <complex>
#include <unordered_set>
#include <assert.h>
#include <string>
            
#define X first
#define Y second

using namespace std;

struct Transducer {
    vector<vector<pair<pair<char,char>,int>>> D;
    unordered_set<int> F;
    unordered_set<int> I;
};

struct WTransducer {
    vector<vector<pair<pair<string,string>,int>>> D;
    unordered_set<int> F;
    unordered_set<int> I;
};

inline unordered_set<string> accepting(const Transducer& T, const string& word, const vector<bool>& infNodes) {
    vector<tuple<int, int, string>> S;
    unordered_set<string> res;
    for (auto& i : T.I) {
        if (!infNodes[i]) {
            S.emplace_back(i, 0, "");
        }
    }
    while (!S.empty()) {
        vector<tuple<int,int,string>> SC;
        for (auto& t : S) {
            int state = get<0>(t);
            int sIdx = get<1>(t);
            if (sIdx == word.length()) {
                if (T.F.count(state)) {
                    res.insert(get<2>(t));
                }
            }
            for (auto& delta : T.D[state]) {
                if (infNodes[delta.Y]) {
                    continue;
                }
                if (delta.X.X == '$') {
                    assert(delta.X.Y != '$');
                    SC.emplace_back(delta.Y, sIdx, get<2>(t) + delta.X.Y);
                } else if (sIdx < word.length() && delta.X.X == word[sIdx]) {
                    if (delta.X.Y == '$') {
                        SC.emplace_back(delta.Y, sIdx + 1, get<2>(t));
                    } else {
                        SC.emplace_back(delta.Y, sIdx + 1, get<2>(t) + delta.X.Y);
                    }
                }
            }
        }
        S = SC;
    }
    return res;
}

#endif