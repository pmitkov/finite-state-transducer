#include <stdio.h>
#include <stdlib.h>
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
#include <iostream>

#include "print-util.h"
#include "transducer.h"
#include "constructions.h"
#include "trim.h"
#include "e-elimination.h"
#include "functional-test.h"
#include "parser.h"
            
#define X first
#define Y second

using namespace std;
bool ___debug = false;

const int BUFF_SIZE = (1 << 24);

int main() {
    string s = "($:a)*";
    //getline(cin, s);
    Transducer t = parseRegex(s);
    printTransducer(t, "graphics/initial.dot");
    t = epsilon_elimination(t);
    t = trim(t);
    vector<bool> V = markInfiniteNodes(t);
    printTransducer(t, V, "graphics/after_e_trim.dot");
    if (checkInfinitelyAmbiguous(t)) {
        printf("Transducer is infinitely ambiguous\n");
    } else {
        if (checkFunctionality(t, V)) {
        printf("Transducer is functional\n");
        } else {
            printf("Transducer is not functional\n");
        }
    }
    string inp = "MCMXXLIMCMXIMMDXCMMCMMD";
    unordered_set<string> v = accepting(t, inp, V);
    for (auto& s : v) {
        printf("%s\n", s.c_str());
    }
    return 0;
}