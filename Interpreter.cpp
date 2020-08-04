//
// Created by camer on 7/16/2020.
//

#include "Interpreter.h"

Interpreter::Interpreter(DatalogProgram setter){
    program = setter;
    programSchemes = program.getSchemes();
    programFacts = program.getFacts();
    programRules = program.getRules();
    programQueries = program.getQueries();

    //Make a Relation for each scheme Predicate, and put that relation in the Database data member
    for (unsigned int i = 0; i < programSchemes.size(); i++){
        vector<Parameter> schemeParameters = programSchemes[i].getParameterList();
        //Get all the parameters from the scheme and add them to the attribute vector
        for (unsigned int i = 0; i < schemeParameters.size(); i++){
            attributes.push_back(schemeParameters[i].toString());
        }
        Relation newRelation( programSchemes[i].getID(), Scheme(attributes) );
        database[newRelation.getName()] = newRelation; //Adds relation to the database
        attributes.resize(0);
    }

    //Make a Tuple for each fact Predicate, and put that Tuple in the appropriate Relation in the Database
    for (unsigned int i = 0; i < programFacts.size(); i++){
        Tuple tuple;
        string name = programFacts[i].getID();
        vector<Parameter> factValues = programFacts[i].getParameterList();
        for (unsigned int i = 0; i < factValues.size(); i++){
            tuple.push_back(factValues[i].toString());
        }
        //Finds the matching relation and adds the tuple
        database.find(name)->second.addTuple(tuple);
    }
    /**
    cout << "Rule Evaluation" << '\n';
    fixedPointAlgorithm();
    cout << '\n';
    cout << "Schemes populated after " << to_string(passes) << " passes through the Rules.\n" << '\n';
     */
    moreEfficientRuleEvaluation();
    cout << "Query Evaluation\n";
    evaluateAllQueries(programQueries);
}

Relation Interpreter::evaluatePredicate(Predicate query){
    Relation relation = database.find(query.getID())->second;
    string value;
    map<string, int> deja_vu;
    vector<string> paramNames;
    vector<int> indexes;

    vector<Parameter> params = query.getParameterList();
    for (unsigned int i = 0; i < params.size(); i++){
        paramNames.push_back(params[i].toString());
    }

    for (unsigned int i = 0; i < params.size(); i++){
        value = params[i].toString();
        if (params[i].constant()){ //The current parameter is a constant (string)
            relation = relation.select(i, value, query.getID(), Scheme(paramNames));
        }
        else{
            if (deja_vu.find(params[i].toString()) == deja_vu.end()) { //This is the first time we've seen the variable
                deja_vu[value] = i; //Saves in the map to compare to future vars of same name
                indexes.push_back(i);
            }
            else{
                relation = relation.select(deja_vu.find(value)->second, i, query.getID(), Scheme(paramNames) );
                deja_vu[value] = i;
            }
        }
    }
    relation = relation.project(indexes, query.getID(), paramNames);
    relation.rename(paramNames, indexes);
    return relation;
}

void Interpreter::evaluateAllQueries(vector<Predicate> queries){
    for (unsigned int i = 0; i < queries.size(); i++){
        evaluatedRelations.push_back(evaluatePredicate(queries[i]) );
    }
}

string Interpreter::printQueries(){
    vector<int> variableIndexes;
    vector<Parameter> origQueryScheme;
    string tempString;
    vector<string> tempVarString;
    for (unsigned int i = 0; i < evaluatedRelations.size(); i++){ //Go through each relation
        vector<string> names;
        variableIndexes = evaluatedRelations[i].getVariableIndexes();
        output << evaluatedRelations[i].getName() << '(';
        origQueryScheme = programQueries[i].getParameterList(); //This gets the original parameters from the query predicate. They were lost in the rename process.
        for (unsigned int j = 0; j < origQueryScheme.size(); j++){
            names.push_back(origQueryScheme[j].toString());
        }

        for (unsigned int k = 0; k < names.size(); k++){
            output << names[k];
            if (k < names.size() - 1){
                output << ',';
            }
        }

        output << ")? ";
        if (evaluatedRelations[i].getCollectionSize() > 0){
            output << "Yes(" << evaluatedRelations[i].getCollectionSize() << ")\n";
        }
        else {
            output << "No\n";
        }
        for (Tuple ob : evaluatedRelations[i].getCollection()){
            if (variableIndexes.size() > 0){ //Checks to see if vars were in the original query. If yes, then print lines below
                output << "  ";
                for (unsigned int i = 0; i < ob.size(); i++){
                    output << names[variableIndexes[i]] << '=' << ob[i];
                    if (i < ob.size() - 1){
                        output << ", ";
                    }
                }
                output << '\n';
            }
        }
    }
    return output.str();
}

void Interpreter::evaluateRule(Rule rule){
    //Output the rule to the terminal
    cout << rule.toString() << '\n';

    /*Evaluate the predicates on the right-hand side of the rule
    Join the relations that result
    Project the columns that appear in the head predicate
    Rename the relation to make it union-compatible
    Union with the relation in the database*/

    vector<Predicate> rightHand = rule.getPredicateList();
    vector<Relation> evaluatedRightHand;
    Relation joins;
    for (unsigned int i = 0; i < rightHand.size(); i++){
        evaluatedRightHand.push_back(evaluatePredicate(rightHand[i]));
    }
    for (unsigned int i = 0; i < evaluatedRightHand.size(); i++){
        if (i == 0) { joins = evaluatedRightHand[0]; } //There was only one predicate evaluated in Rule
        if ( (i+1) <= evaluatedRightHand.size() - 1){
            joins = joins.join(joins, evaluatedRightHand[i+1]);
        }
    }

    vector<string> headAttributes;
    //This gets the variables from the head predicate for projection. All params will be ID's (variables)
    for (unsigned int i = 0; i < rule.getHead().getParameterList().size(); i++){
        headAttributes.push_back( rule.getHead().getParameterList().at(i).toString() );
    }

    Relation ruleRelation = joins.projectRule(rule.getHead().getID(), headAttributes);

    //Rename to make union compatible with the relation that matches head of the rule.
    //1. Find the scheme relation in the database that matches the name of ruleRelation
    //2. rename ruleRelation scheme to the scheme of the database relation
    ruleRelation.rename( database[ruleRelation.getName()].getScheme() );
    database[ruleRelation.getName()].unionize(ruleRelation);
}

void Interpreter::evaluateAllRules(vector<Rule> rules){
    for (unsigned int i = 0; i < rules.size(); i++){
        evaluateRule(rules[i]);
    }
}

int Interpreter::count(){
    int size = 0;
    //For each relation in the database
    //for each tuple in the relation
    //Add one to counter
    for (pair<string, Relation> ob : database){
        size += ob.second.getCollectionSize();
    }
    return size;
}

void Interpreter::fixedPointAlgorithm(){
    int before = 0;
    int after = 0;
    passes = 0;
    do {
        before = count();
        evaluateAllRules(programRules);
        passes++;
        after = count();
    } while (before < after);
}

void Interpreter::fixedPointAlgorithm(vector<Rule> &to_evaluate){
    int before = 0;
    int after = 0;
    passes = 0;
    do {
        before = count();
        evaluateAllRules(to_evaluate);
        passes++;
        after = count();
    } while (before < after);
}

/*Outlines the critical steps in performing a more
 * efficient rule evaluation based on dependency
 * graphs -- like a wrapper function
 */
void Interpreter::moreEfficientRuleEvaluation(){
    create_graphs();

    cout << "Dependency Graph\n";
    print_graph(dependency_graph);
    cout << '\n';

    reverse_dependency_graph.dfs_forest();
    reverse_dependency_graph.order_by_postnum();
    dependency_graph.copy_postorder_nums(reverse_dependency_graph);
    dependency_graph.order_by_postnum();
    dependency_graph.scc_search();
    scc_evaluation(dependency_graph.SCC);
}

/*This function iterates through the Datalog Program Rules,
 * builds nodes,
 * finds dependencies between rules, and
 * generates two dependency graphs (stored as private data vars)
 * --regular dependency and reverse dependency
 * Params: none
 * Return: none
 * Modifies already declared objects
 */
void Interpreter::create_graphs(){
    Predicate head_predicate;
    Predicate one_rightside_pred;

    for (unsigned int i = 0; i < programRules.size(); i++){
        dependency_graph.nodes[i] = Node(i);

        for (unsigned int j = 0; j < programRules[i].getPredicateList().size(); j++){
            one_rightside_pred = programRules[i].getPredicateList()[j];

            for (unsigned int k = 0; k < programRules.size(); k++){
                head_predicate = programRules[k].getHead();
                reverse_dependency_graph.nodes[k].ID = k;
                //reverse_dependency_graph.nodes[k] = Node(k);
                if (one_rightside_pred.getID() == head_predicate.getID()){ //This rule depends on another rule
                    //Create dependency relationship
                    dependency_graph.nodes[i].adjacency_list.insert(k);
                    //Create reverse dependency relationship
                    reverse_dependency_graph.nodes[k].adjacency_list.insert(i);
                }
            }
        }
    }
}

void Interpreter::print_graph(Graph &graph){
    for (pair<int, Node> ob : graph.nodes){
        cout << ('R' + to_string(ob.first) + ':');
        for (set<int>::iterator it = ob.second.adjacency_list.begin(); it != ob.second.adjacency_list.end(); it++){
            cout << ('R' + to_string(*it));
            it++;
            if (it != ob.second.adjacency_list.end()){
                cout << ",";
            }
            it--;
        }
        cout << '\n';
    }
}

void Interpreter::scc_evaluation(const vector< set<Node> > &SCC) {
    vector<Rule> to_evaluate;
    cout << "Rule Evaluation\n";
    for (unsigned int i = 0; i < SCC.size(); i++){
        cout << "SCC: ";

        for (set<Node>::iterator it = SCC[i].begin(); it != SCC[i].end(); it++){
            to_evaluate.push_back(programRules[it->ID]);
            cout << 'R' << to_string(it->ID);
            it++;
            if (it != SCC[i].end()) { cout << ','; }
            it--;
        }

        cout << '\n';

        if (check_self_dependency(SCC[i])){
            if (SCC[i].size() == 1) fixedPointAlgorithm(to_evaluate);
            else if (SCC[i].size() == 0){
                passes = 1;
                evaluateRule(to_evaluate[0]);
            }
            else fixedPointAlgorithm(to_evaluate);
        }
        else {
            if (SCC[i].size() <= 1){
                passes = 1;
                evaluateRule(to_evaluate[0]);
            }
            else fixedPointAlgorithm(to_evaluate);
        }

        to_evaluate.resize(0);

        cout << to_string(passes) << " passes: ";
        for (set<Node>::iterator it = SCC[i].begin(); it != SCC[i].end(); it++){
            cout << 'R' << to_string(it->ID);
            it++;
            if (it != SCC[i].end()) { cout << ','; }
            it--;
        }
        cout << '\n';
    }
    cout << '\n';
}

bool Interpreter::check_self_dependency(const set<Node>& node){
    int current_ID;
    for (Node ob : node){
        current_ID = ob.ID;
        for (int i : ob.adjacency_list){
            if (current_ID == i) return true;
        }
    }
    return false;
}