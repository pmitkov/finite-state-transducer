#ifndef CONSTRUCTIONS
#define CONSTRUCTIONS

#include <vector>
#include <string>

#include "transducer.h"
#include "string-util.h"

#define X first
#define Y second

using namespace std;

Transducer get(const pair<string,string>& p) {
    vector<pair<char,char>> C = split(p.X, p.Y);
    Transducer t;
    t.D.resize(C.size() + 1);
    t.F.insert(C.size());
    t.I.insert(0);
    for (int i = 0; i < C.size(); i++) {
        t.D[i].push_back({{C[i].X, C[i].Y}, i + 1});
    }
    return t;
}

Transducer tUnion(const Transducer& t1, const Transducer& t2) {
    Transducer t;
    t.D.resize(t1.D.size() + t2.D.size() + 1);
    t.D[0].push_back({{'$', '$'}, 1});
    t.D[0].push_back({{'$', '$'}, t1.D.size() + 1});
    for (int i = 0; i < t1.D.size(); i++) {
        for (auto& d : t1.D[i]) {
            t.D[i + 1].push_back({d.X, d.Y + 1});
        }
    }
    for (int i = 0; i < t2.D.size(); i++) {
        for (auto& d : t2.D[i]) {
            t.D[i + t1.D.size() + 1].push_back({d.X, d.Y + t1.D.size() + 1});
        }
    }
    for (auto& f : t1.F) {
        t.F.insert(f + 1);
    }
    for (auto& f : t2.F) {
        t.F.insert(f + t1.D.size() + 1);
    }
    t.I.insert(0);
    return t;
}

Transducer kleeneStar(const Transducer& t1) {
    Transducer t;
    t.D.resize(t1.D.size() + 1);
    t.D[0].push_back({{'$', '$'}, 1});
    for (auto& f : t1.F) {
        t.D[f + 1].push_back({{'$', '$'}, 0});
    }
    for (int i = 0; i < t1.D.size(); i++) {
        for (auto& d : t1.D[i]) {
            t.D[i + 1].push_back({d.X, d.Y + 1});
        }
    }
    t.F.insert(0);
    for (auto& f : t1.F) {
        t.F.insert(f + 1);
    }
    t.I.insert(0);
    return t;
}

Transducer monoidalProduct(const Transducer& t1, const Transducer& t2) {
    Transducer t;
    t.D.resize(t1.D.size() + t2.D.size());
    for (int i = 0; i < t1.D.size(); i++) {
        t.D[i] = t1.D[i];
    }
    for (int i = 0; i < t2.D.size(); i++) {
        for (auto& d : t2.D[i]) {
            t.D[i + t1.D.size()].push_back({d.X, d.Y + t1.D.size()});
        }
    }
    for (auto& f : t1.F) {
        t.D[f].push_back({{'$', '$'}, t1.D.size()});
    }
    for(auto& f : t2.F) {
        t.F.insert(f + t1.D.size());
    }
    t.I.insert(0);
    return t;
}

#endif