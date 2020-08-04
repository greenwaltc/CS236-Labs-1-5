//
// Created by camer on 7/16/2020.
//

#ifndef PROJECT1_INTERPRETER_H
#define PROJECT1_INTERPRETER_H

#include <sstream>
#include <algorithm>
#include <vector>
#include <iostream>
#include <ctime>
#include "DatalogProgram.h"
#include "Database.h"
#include "Relation.h"
#include "Graph.h"
#include "Node.h"

using namespace std;

class Interpreter{
private:
    DatalogProgram program;
    Database database;
    vector<Relation> evaluatedRelations;
    unsigned int passes;

    vector<string> attributes;
    vector<Predicate> programSchemes;
    vector<Predicate> programFacts;
    vector<Rule> programRules;
    vector<Predicate> programQueries;
    stringstream output;

    Graph dependency_graph;
    Graph reverse_dependency_graph;

public:
    Interpreter(DatalogProgram setter);
    Relation evaluatePredicate(Predicate query);
    void evaluateAllQueries(vector<Predicate> queries);
    string printQueries();

    void evaluateRule(Rule rule);
    void evaluateAllRules(vector<Rule> rules);
    int count();
    void fixedPointAlgorithm();
    void fixedPointAlgorithm(vector<Rule> &to_evaluate);

    void moreEfficientRuleEvaluation();
    void create_graphs();
    void print_graph(Graph &graph);
    void scc_evaluation(const vector< set<Node> > &SCC);
    bool check_self_dependency(const set<Node>& node);
};

#endif //PROJECT1_INTERPRETER_H
