#ifndef E_ELIMINATION_H
#define E_ELIMINATION_H

#include <stdio.h>
#include <vector>
#include <stack>
#include <functional>
#include <numeric>
#include <complex>
#include <queue>
#include <map>
#include <unordered_map>
#include <list>
#include <set>
#include <unordered_set>
#include <assert.h>
#include <cstring>
#include <sstream>
#include <string>
#include <algorithm>
#include <ctime>

#include "transducer.h"

#define X first
#define Y second

using namespace std;

vector<unordered_set<int>> epsilon_closure(const Transducer& t) {
    vector<unordered_set<int>> res(t.D.size());
    for (int i = 0; i < t.D.size(); i++) {
        res[i].insert(i);
        queue<int> Q;
        Q.push(i);
        while (!Q.empty()) {
            int v = Q.front();
            Q.pop();
            for (auto& d : t.D[v]) {
                if (d.X == make_pair('$', '$') && !res[i].count(d.Y)) {
                    res[i].insert(d.Y);
                    Q.push(d.Y);
                }
            }
        }
    }
    return res;
}

Transducer epsilon_elimination(const Transducer& t) {
    vector<unordered_set<int>> C = epsilon_closure(t);
    Transducer t1;
    t1.D.resize(t.D.size());
    for (int i = 0; i < t.D.size(); i++) {
        for (auto& d : t.D[i]) {
            if (d.X != make_pair('$', '$')) {
                for (auto& j : C[d.Y]) {
                    t1.D[i].push_back({d.X, j});
                }
            }
        }
    }
    for (auto& f : t.F) {
        t1.F.insert(f);
    }
    for (auto& i : t.I) {
        for (auto& ni : C[i]) {
            t1.I.insert(ni);
        }
    }
    return t1;
}

#endif