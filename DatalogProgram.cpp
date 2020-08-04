//
// Created by camer on 7/2/2020.
//

#include "DatalogProgram.h"

vector<Predicate> DatalogProgram::getSchemes(){return schemes;}
vector<Predicate> DatalogProgram::getFacts(){return facts;}
vector<Rule> DatalogProgram::getRules(){return rules;}
vector<Predicate> DatalogProgram::getQueries(){return queries;}

string DatalogProgram::toString() {
    gatherDomain(); //Gets all the strings in the Facts
    stringstream output;
    //Schemes
    output << "Schemes(" << schemes.size() << "):\n";
    for (unsigned int i = 0; i < schemes.size(); i++){
        output << "  " << schemes[i].toString() << '\n';
    }
    //Facts
    output << "Facts(" << facts.size() << "):\n";
    for (unsigned int i = 0; i < facts.size(); i++){
        output << "  " << facts[i].toString() << "." << '\n';
    }
    //Rules
    output << "Rules(" << rules.size() << "):\n";
    for (unsigned int i = 0; i < rules.size(); i++){
        output << "  " << rules[i].toString() << '\n';
    }
    //Queries
    output << "Queries(" << queries.size() << "):\n";
    for (unsigned int i = 0; i < queries.size(); i++){
        output << "  " << queries[i].toString() << "?" << '\n';
    }

    //Domain
    output << "Domain(" << domain.size() << "):\n";
    for (string const& stringDomain: domain){ //This is a 'foreach' loop. Kinda cool
        output << "  " << stringDomain << '\n';
    }
    return output.str();
}

void DatalogProgram::setSchemes(vector<Predicate> setter){
    for (unsigned int i = 0; i < setter.size(); i++){
        this->schemes.push_back(setter[i]);
    }
}
void DatalogProgram::setFacts(vector<Predicate> setter){
    for (unsigned int i = 0; i < setter.size(); i++){
        this->facts.push_back(setter[i]);
    }
}
void DatalogProgram::setQueries(vector<Predicate> setter){
    for (unsigned int i = 0; i < setter.size(); i++){
        this->queries.push_back(setter[i]);
    }
}
void DatalogProgram::setRules(vector<Rule> setter){
    for (unsigned int i = 0; i < setter.size(); i++){
        this->rules.push_back(setter[i]);
    }
}
void DatalogProgram::gatherDomain(){
    vector<Parameter> tempParameters;
    string tempString;
    for (unsigned int i = 0; i < facts.size(); i++){
        tempParameters = facts[i].getParameterList();
        for (unsigned int i = 0; i < tempParameters.size(); i++){
            if ( tempParameters[i].toString().at(0) == '\''){ //The parameter in the fact predicate is a string. Add to program domain.
                domain.insert(tempParameters[i].toString());
            }
        }
    }
}