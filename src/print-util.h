#ifndef PRINT_UTIL_H
#define PRINT_UTIL_H

#include <stdio.h>
#include <vector>
#include <unordered_set>
#include <string>

#include "transducer.h"

#define X first
#define Y second

using namespace std;

inline void printTransducer(const Transducer& t, const vector<bool>& infNodes, const string& file) {
    FILE* fp = fopen(file.c_str(), "w");
    fprintf(fp, "digraph finite_state_machine {\n");
    fprintf(fp, "fontname=\"Helvetica,Arial,sans-serif\"\n");
    fprintf(fp, "node [fontname=\"Helvetica,Arial,sans-serif\"]\n");
    fprintf(fp, "edge [fontname=\"Helvetica,Arial,sans-serif\"]\n");
    fprintf(fp, "rankdir=LR;\n");
    fprintf(fp, "node [shape = doublecircle]; ");
    for (auto& f : t.F) {
        fprintf(fp, "%d ", f);
    }
    fprintf(fp, ";\n");
    fprintf(fp, "node [shape = circle];\n");
    for (auto& i : t.I) {
        if (!infNodes[i]) {
            fprintf(fp, "%d [color=\"green\"];\n", i);
        } else {
            fprintf(fp, "%d [style=wedged color=\"green:red\"];\n", i);
        }
        
    }
    for (int i = 0; i < t.D.size(); i++) {
        if (infNodes[i] && !t.I.count(i)) {
            fprintf(fp, "%d [color=\"red\"]\n", i);
        }
    }
    for (int i = 0; i < t.D.size(); i++) {
        for (auto& d : t.D[i]) {
            string label;
            label += d.X.X;
            label += ':';
            label += d.X.Y;
            fprintf(fp, "%d -> %d [label = \"%s\"];\n", i, d.Y, label.c_str());
        }
    }
    fprintf(fp, "}\n");
    fclose(fp);
}

inline void printTransducer(const Transducer& t, const string& file) {
    FILE* fp = fopen(file.c_str(), "w");
    fprintf(fp, "digraph finite_state_machine {\n");
    fprintf(fp, "fontname=\"Helvetica,Arial,sans-serif\"\n");
    fprintf(fp, "node [fontname=\"Helvetica,Arial,sans-serif\"]\n");
    fprintf(fp, "edge [fontname=\"Helvetica,Arial,sans-serif\"]\n");
    fprintf(fp, "rankdir=LR;\n");
    fprintf(fp, "node [shape = doublecircle]; ");
    for (auto& f : t.F) {
        fprintf(fp, "%d ", f);
    }
    fprintf(fp, ";\n");
    fprintf(fp, "node [shape = circle];\n");
    for (auto& i : t.I) {
        fprintf(fp, "%d [color=\"green\"];\n", i);  
    }
    for (int i = 0; i < t.D.size(); i++) {
        for (auto& d : t.D[i]) {
            string label;
            label += d.X.X;
            label += ':';
            label += d.X.Y;
            fprintf(fp, "%d -> %d [label = \"%s\"];\n", i, d.Y, label.c_str());
        }
    }
    fprintf(fp, "}\n");
    fclose(fp);
}

inline void printTransducer(const WTransducer& t, const string& file) {
    FILE* fp = fopen(file.c_str(), "w");
    fprintf(fp, "digraph finite_state_machine {\n");
    fprintf(fp, "fontname=\"Helvetica,Arial,sans-serif\"\n");
    fprintf(fp, "node [fontname=\"Helvetica,Arial,sans-serif\"]\n");
    fprintf(fp, "edge [fontname=\"Helvetica,Arial,sans-serif\"]\n");
    fprintf(fp, "rankdir=LR;\n");
    fprintf(fp, "node [shape = doublecircle]; ");
    for (auto& f : t.F) {
        fprintf(fp, "%d ", f);
    }
    fprintf(fp, ";\n");
    fprintf(fp, "node [shape = circle];\n");
    for (auto& i : t.I) {
        fprintf(fp, "%d [color=\"green\"];\n", i);  
    }
    for (int i = 0; i < t.D.size(); i++) {
        for (auto& d : t.D[i]) {
            string label = d.X.X + ":" + d.X.Y;
            fprintf(fp, "%d -> %d [label = \"%s\"];\n", i, d.Y, label.c_str());
        }
    }
    fprintf(fp, "}\n");
    fclose(fp);
}

#endif