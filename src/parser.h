#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <stack>
#include <assert.h>
#include <string>
#include <regex>

#include "transducer.h"
#include "constructions.h"

#define X first
#define Y second

using namespace std;

inline string sanitize(const string& expr) {
    string res = expr;
    res = regex_replace(res, regex("\\)\\("), ").(");
    res = regex_replace(res, regex("\\)\\*\\("), ")*.(");
    return res; 
}

inline int opPrecedence(char op) {
    if (op == '*') {
        return 3;
    } else if (op == '.') {
        return 2;
    } else if (op == '|') {
        return 1;
    } else {
        assert(false);
    }
}

inline bool isOperator(char c) {
    return c == '*' || c == '|' || c == '.';
}

inline vector<string> infixToPostfix(const string& expr) {
    vector<string> res;
    stack<char> ops;
    for (int i = 0; i < expr.length(); i++) {
        char c = expr[i];
        if (isOperator(c)) {
            while (!ops.empty() && ops.top() != '(' && opPrecedence(ops.top()) > opPrecedence(c)) {
                res.emplace_back(1, ops.top());
                ops.pop();
            }
            ops.push(c);
        } else if (c == ')') {
            while (ops.top() != '(') {
                res.emplace_back(1, ops.top());
                ops.pop();
            }
            ops.pop();
        } else if (c == '(') {
            ops.push('(');
        } else {
            string s(1, c);
            while (expr[i + 1] != ')') {
                s += expr[i + 1];
                i++;
            }
            res.push_back(s);
        }
    }
    while (!ops.empty()) {
        res.emplace_back(1, ops.top());
        ops.pop();
    }
    return res;
}

inline pair<string,string> splitExpr(const string& expr) {
    string l;
    string r;
    int idx = 0;
    while (expr[idx] != ':') {
        l += expr[idx++];
    }
    idx++;
    while (idx < expr.length()) {
        r += expr[idx++];
    }
    if (l == "$") {
        l = "";
    }
    if (r == "$") {
        r = "";
    }
    return {l, r};
}

inline Transducer parseRegex(const string& regex) {
    if (regex == "()") {
        return Transducer();
    }
    vector<string> postfixExpr = infixToPostfix(sanitize(regex));
    stack<Transducer> S;
    for (auto& expr : postfixExpr) {
        if (expr == "*") {
            Transducer t = S.top();
            S.pop();
            S.push(kleeneStar(t));
        } else if (expr == ".") {
            Transducer t2 = S.top();
            S.pop();
            Transducer t1 = S.top();
            S.pop();
            S.push(monoidalProduct(t1, t2));
        } else if (expr == "|") {
            Transducer t2 = S.top();
            S.pop();
            Transducer t1 = S.top();
            S.pop();
            S.push(tUnion(t1, t2));
        } else {
            const pair<string,string> p = splitExpr(expr);
            S.push(get(p));
        }
    }
    assert(S.size() == 1);
    return S.top();
}

#endif