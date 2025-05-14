#ifndef FUNCTIONAL_TEST_H
#define FUNCTIONAL_TEST_H

#include <vector>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <string>

#include "transducer.h"
#include "string-util.h"
#include "trim.h"

#define X first
#define Y second

using namespace std;

inline void tarjan(int v, const vector<vector<pair<pair<char,char>,int>>> &D, vector<int>& ID, stack<int>& S, vector<int>& LOW, vector<bool>& ON, int& cnt) {
    ID[v] = LOW[v] = cnt++;
    ON[v] = true;
    S.push(v);
    for (auto& d : D[v]) {
        if (d.X.X == '$' && !ID[d.Y]) {
            tarjan(d.Y, D, ID, S, LOW, ON, cnt);
        }
        if (ON[d.Y] && LOW[d.Y] < LOW[v]) {
            LOW[v] = LOW[d.Y];
        }
    }
    if (ID[v] == LOW[v]) {
        while (true) {
            int c = S.top();
            S.pop();
            LOW[c] = LOW[v];
            ON[c] = false;
            if (c == v) {
                break;
            }
        }
    }
}

inline vector<bool> markInfiniteComps(const vector<vector<pair<pair<char,char>,int>>>& D, vector<int>& LOW, int cCount) {
    vector<bool> iC(cCount);
    for (int i = 0; i < D.size(); i++) {
        for (auto& d : D[i]) {
            if (LOW[i] == LOW[d.Y] && d.X.X == '$' && d.X.Y != '$') {
                iC[LOW[i]] = true;
            }
        }
    }
    return iC;
}

inline vector<bool> markInfiniteNodes(const Transducer& t) {
    int n = t.D.size();
    vector<int> ID(n);
    vector<int> LOW(n);
    vector<bool> ON(n);
    stack<int> S;
    int cnt = 1;
    for (int i = 0; i < n; i++) {
        if (!ID[i]) {
            tarjan(i, t.D, ID, S, LOW, ON, cnt);
        }
    }
    vector<bool> infComps = markInfiniteComps(t.D, LOW, cnt - 1);
    vector<bool> infNodes(n);
    for (int i = 0; i < n; i++) {
        if (infComps[LOW[i]]) {
            infNodes[i] = true;
        }
    }
    return infNodes;
}

inline vector<pair<string,int>>& getEpsClosureForward(int v, const vector<vector<pair<pair<char,char>,int>>>& D, unordered_map<int,vector<pair<string,int>>>& DP) {
    if (DP.count(v)) {
        return DP[v];
    }
    vector<pair<string,int>> res;
    res.push_back({"", v});
    for (auto& d : D[v]) {
        if (d.X.X == '$') {
            auto& c = getEpsClosureForward(d.Y, D, DP);
            for(auto& p : c) {
                res.push_back({string(1, d.X.Y) + p.X, p.Y});
            }
        }
    }
    return DP[v] = res;
}

inline vector<pair<string,int>>& getEpsClosureBackward(int v, const vector<vector<pair<pair<char,char>,int>>>& DT, unordered_map<int,vector<pair<string,int>>>& DP) {
    if (DP.count(v)) {
        return DP[v];
    }
    vector<pair<string,int>> res;
    res.push_back({"", v});
    for (auto& d : DT[v]) {
        if (d.X.X == '$') {
            auto& c = getEpsClosureBackward(d.Y, DT, DP);
            for(auto& p : c) {
                res.push_back({p.X + string(1, d.X.Y), p.Y});
            }
        }
    }
    return DP[v] = res;
}

inline WTransducer buildWTransducer(const Transducer& t) {
    unordered_map<int,vector<pair<string,int>>> DPF;
    unordered_map<int,vector<pair<string,int>>> DPB;
    vector<vector<pair<pair<char,char>,int>>> DT(t.D.size());
    for (int i = 0; i < t.D.size(); i++) {
        for (auto& d : t.D[i]) {
            DT[d.Y].push_back({d.X, i});
        }
    }
    WTransducer t1;
    t1.D.resize(t.D.size());
    for (int i = 0; i < t.D.size(); i++) {
        for(auto& d : t.D[i]) {
            if (d.X.X != '$') {
                auto& lc = getEpsClosureBackward(i, DT, DPB);
                auto& rc = getEpsClosureForward(d.Y, t.D, DPF);
                for(auto& p1 : lc) {
                    for (auto& p2 : rc) {
                        t1.D[p1.Y].push_back({{string(1, d.X.X), p1.X + d.X.Y + p2.X}, p2.Y});
                    }
                }
            }
        }
    }
    t1.F = t.F;
    t1.I = t.I;
    for (auto& i : t.I) {
        auto& rc = getEpsClosureForward(i, t.D, DPF);
        for (auto& p : rc) {
            if (t.F.count(p.Y)) {
                t1.F.insert(i);
            }
        }
    }
    return t1;
}

inline WTransducer buildSquaredOutputTransducer(const WTransducer& t) {
    WTransducer t1;
    int n = t.D.size();
    t1.D.resize(n * n);
    for (int i = 0; i < n; i++) {
        for (auto& d1 : t.D[i]) {
            for (int j = 0; j < n; j++) {
                for (auto& d2 : t.D[j]) {
                    if (d1.X.X == d2.X.X) {
                        t1.D[i * n + j].push_back({{d1.X.Y, d2.X.Y}, d1.Y * n + d2.Y});
                    }
                }
            }
        }
    }
    for (auto& i1 : t.I) {
        for (auto& i2: t.I) {
            t1.I.insert(i1 * n + i2);
        }
    }
    for (auto& f1 : t.F) {
        for (auto& f2 : t.F) {
            t1.F.insert(f1 * n + f2);
        }
    }
    return t1;
}

inline bool checkInfinitelyAmbiguous(const Transducer& t) {
    vector<bool> V = markInfiniteNodes(t);
    for (auto b : V) {
        if (b) {
            return true;
        }
    }
    return false;
}

inline bool checkBalancible(const WTransducer& t) {
    unordered_map<int,pair<string,string>> M;
    for(auto& i : t.I) {
        M[i] = {"", ""};
    }
    bool hasChanged = true;
    while (hasChanged) {
        hasChanged = false;
        for (auto& p : M) {
            int s = p.X;
            for (auto& d : t.D[s]) {
                pair<string,string> aa = advanceFunc(p.Y, d.X);
                if (!isBalancible(aa)) {
                    return false;
                }
                if (M.count(d.Y)) {
                    if (M[d.Y] != aa) {
                        return false;
                    }
                } else {
                    if (t.F.count(d.Y) && !isEmptyAdvance(aa)) {
                        return false;
                    }
                    M[d.Y] = aa;
                    hasChanged = true;
                }
            }
        }
    }
    return true;
}

inline bool checkFunctionality(const Transducer& t, const vector<bool>& infNodes) {
    unordered_set<string> s = accepting(t, "", infNodes);
    // Check if empty word is functional
    if (s.size() > 1) {
        return false;
    }
    WTransducer sq = trim(buildSquaredOutputTransducer(trim(buildWTransducer(t))));
    return checkBalancible(sq);
}

#endif