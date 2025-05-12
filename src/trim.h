#ifndef TRIM_H
#define TRIM_H

#include <stdio.h>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <assert.h>

#include "transducer.h"
            
#define X first
#define Y second

using namespace std;

Transducer trim(const Transducer& t) {
    unordered_set<int> F;
    unordered_set<int> B;
    queue<int> QF;
    for (int i : t.I) {
        QF.push(i);
        F.insert(i);
    }
    while (!QF.empty()) {
        int v = QF.front();
        QF.pop();
        for (auto& d : t.D[v]) {
            if (!F.count(d.Y)) {
                QF.push(d.Y);
                F.insert(d.Y);
            }
        }
    }
    vector<vector<int>> TG(t.D.size());
    for (int i = 0; i < t.D.size(); i++) {
        for (auto& d : t.D[i]) {
            TG[d.Y].push_back(i);
        }
    }
    queue<int> QB;
    for (auto& f : t.F) {
        QB.push(f);
        B.insert(f);
    }
    while (!QB.empty()) {
        int v = QB.front();
        QB.pop();
        for (auto& c : TG[v]) {
            if (!B.count(c)) {
                QB.push(c);
                B.insert(c);
            }
        }
    }
    // Transducer is already trimmed
    if (F.size() == t.D.size() && B.size() == t.D.size()) {
        return t;
    }
    int idx = 0;
    unordered_map<int,int> M;
    for (int i = 0; i < t.D.size(); i++) {
        if (F.count(i) && B.count(i)) {
            M[i] = idx;
            idx++;
        }
    }
    assert(idx < t.D.size());
    Transducer t1;
    t1.D.resize(idx);
    for (int i = 0; i < t.D.size(); i++) {
        if (M.count(i) && t.I.count(i)) {
            t1.I.insert(M[i]);
        }
        if (M.count(i) && t.F.count(i)) {
            t1.F.insert(M[i]);
        }
        if (!M.count(i)) {
            continue;
        }
        for (auto& d : t.D[i]) {
            if (M.count(d.Y)) {
                t1.D[M[i]].push_back({d.X, M[d.Y]});
            }
        }
    }
    return t1;
}

WTransducer trim(const WTransducer& t) {
    unordered_set<int> F;
    unordered_set<int> B;
    queue<int> QF;
    for (int i : t.I) {
        QF.push(i);
        F.insert(i);
    }
    while (!QF.empty()) {
        int v = QF.front();
        QF.pop();
        for (auto& d : t.D[v]) {
            if (!F.count(d.Y)) {
                QF.push(d.Y);
                F.insert(d.Y);
            }
        }
    }
    vector<vector<int>> TG(t.D.size());
    for (int i = 0; i < t.D.size(); i++) {
        for (auto& d : t.D[i]) {
            TG[d.Y].push_back(i);
        }
    }
    queue<int> QB;
    for (auto& f : t.F) {
        QB.push(f);
        B.insert(f);
    }
    while (!QB.empty()) {
        int v = QB.front();
        QB.pop();
        for (auto& c : TG[v]) {
            if (!B.count(c)) {
                QB.push(c);
                B.insert(c);
            }
        }
    }
    // Transducer is already trimmed
    if (F.size() == t.D.size() && B.size() == t.D.size()) {
        return t;
    }
    int idx = 0;
    unordered_map<int,int> M;
    for (int i = 0; i < t.D.size(); i++) {
        if (F.count(i) && B.count(i)) {
            M[i] = idx;
            idx++;
        }
    }
    assert(idx < t.D.size());
    WTransducer t1;
    t1.D.resize(idx);
    for (int i = 0; i < t.D.size(); i++) {
        if (M.count(i) && t.I.count(i)) {
            t1.I.insert(M[i]);
        }
        if (M.count(i) && t.F.count(i)) {
            t1.F.insert(M[i]);
        }
        if (!M.count(i)) {
            continue;
        }
        for (auto& d : t.D[i]) {
            if (M.count(d.Y)) {
                t1.D[M[i]].push_back({d.X, M[d.Y]});
            }
        }
    }
    return t1;
}

#endif