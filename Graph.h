//
// Created by camer on 7/31/2020.
//

#ifndef PROJECT1_GRAPH_H
#define PROJECT1_GRAPH_H


#include "Node.h"
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

class Graph{
private:
    int postorder_num;
public:
    map<int, Node> nodes;
    vector < pair<int, Node> > sorted;
    vector< set<Node> > SCC;
    set<Node> temp;

    void dfs_forest();
    void dfs(Node &x);
    void scc_search();
    void order_by_postnum();
    void copy_postorder_nums(const Graph &to_copy);
    void scc_dfs(Node& x);

    Graph(){}
};

#endif //PROJECT1_GRAPH_H
