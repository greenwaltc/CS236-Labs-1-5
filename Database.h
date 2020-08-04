//
// Created by camer on 7/16/2020.
//

#ifndef PROJECT1_DATABASE_H
#define PROJECT1_DATABASE_H

#include <map>
#include "Relation.h"
using namespace std;

class Database : public map<string, Relation>{
private:

public:
    void addRelation(Relation setter);
    void addTuple(string name, Tuple addTuple);
};

#endif //PROJECT1_DATABASE_H
