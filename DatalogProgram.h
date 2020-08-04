//
// Created by camer on 7/2/2020.
//

#ifndef DATALOGPROGRAM_H
#define DATALOGPROGRAM_H

#include <vector>
#include <string>
#include <sstream>
#include <set>
#include <iostream>
#include <assert.h>
#include "Predicate.h"
#include "Rule.h"
using namespace std;

class DatalogProgram {
private:
    vector<Predicate> schemes;
    vector<Predicate> facts;
    vector<Rule> rules;
    vector<Predicate> queries;
    set<string> domain;

public:
    void gatherDomain();
    void setSchemes(vector<Predicate> setter);
    void setFacts(vector<Predicate> setter);
    void setRules(vector<Rule> setter);
    void setQueries(vector<Predicate> setter);

    vector<Predicate> getSchemes();
    vector<Predicate> getFacts();
    vector<Rule> getRules();
    vector<Predicate> getQueries();

    string toString();
};

#endif