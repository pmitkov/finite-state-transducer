#ifndef STRING_UTIL_H
#define STRING_UTIL_H

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
            
#define X first
#define Y second

using namespace std;

vector<pair<char,char>> split(const string& a, const string& b) {
    vector<pair<char,char>> res;
    for (int i = 0; i < max(a.length(), b.length()); i++) {
        res.push_back({'$', '$'});
        if (i < a.length()) {
            res.back().X = a[i];
        }
        if (i < b.length()) {
            res.back().Y = b[i];
        }
    }
    return res;
}

pair<string,string> advanceFunc(const pair<string,string>& a, const pair<string,string>& b) {
    string l = a.X + b.X;
    string r = a.Y + b.Y;
    int len = 0;
    while (len < min(l.length(), r.length()) && l[len] == r[len]) {
        len++;
    }
    return {l.substr(len), r.substr(len)};
}

bool isBalancible(const pair<string,string>& p) {
    return p.X == "" || p.Y == "";
}

bool isEmptyAdvance(const pair<string,string>& p) {
    return p.X == "" && p.Y == "";
}

#endif