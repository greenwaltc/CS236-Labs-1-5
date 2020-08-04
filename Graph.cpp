//
// Created by camer on 7/31/2020.
//

#include "Graph.h"

void Graph::dfs_forest(){
    postorder_num = 1;
    for (map<int, Node>::iterator it = nodes.begin(); it != nodes.end(); it++){
        it->second.visited = false;
    }

    for (map<int, Node>::iterator it = nodes.begin(); it != nodes.end(); it++){
        if (it->second.visited == false){
            dfs(it->second);
        }
    }
}

void Graph::dfs(Node &x) {
    x.visited = true;
    for (int i : x.adjacency_list){
        if (nodes[i].visited == false){
            dfs(nodes[i]);
        }
    }
    x.postorder_number = postorder_num++;
}

bool cmp(const pair<int, Node>& a, const pair<int, Node>& b){
    return (a.second.postorder_number > b.second.postorder_number);
}

void Graph::order_by_postnum(){
    for (pair<int, Node> ob : nodes){
        sorted.push_back(ob);
    }
    sort(sorted.begin(), sorted.end(), cmp);
}

void Graph::copy_postorder_nums(const Graph &to_copy) {
    for (pair<int, Node> copy : to_copy.nodes){
        for (unsigned int i = 0; i < nodes.size(); i++){
            if (copy.first == (int)i){
                nodes[i].postorder_number = copy.second.postorder_number;
            }
        }
    }
}

void Graph::scc_search() {
    for (auto &node : sorted){
        node.second.visited = false;
    }
    for (unsigned int i = 0; i < sorted.size(); i++){
        if (sorted[i].second.visited == false) {
            scc_dfs(sorted[i].second);
            SCC.push_back(temp);
            temp.clear();
        }
    }
}

void Graph::scc_dfs(Node& x){
    int pair = 0;
    temp.insert(x);
    x.visited = true;
    for (int i : x.adjacency_list){
        for (unsigned int j = 0; j < sorted.size(); j++){
            if (sorted[j].first == i){
                pair = j;
            }
        }
        if (sorted[pair].second.visited == false){
            scc_dfs(sorted[pair].second);
        }
    }
}