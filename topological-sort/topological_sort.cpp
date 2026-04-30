#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <string>

using namespace std;

vector<string> topologicalSort(map<string, vector<string>> graph) {
    map<string, int> indegree; queue<string> q;
    vector<string> topo_order;
    
    for (map<string, vector<string>>::iterator it = graph.begin(); it != graph.end(); it++) {
        string course = it->first;
        
        if (indegree.find(course) == indegree.end()) 
            indegree[course] = 0;
        
        for (int i = 0; i < it->second.size(); i++) {
            string dependent = it->second[i];
            if (indegree.find(dependent) == indegree.end()) 
                indegree[dependent] = 0;
            
            indegree[dependent]++;
        }
    }
    
    for (map<string, int>::iterator it = indegree.begin(); it != indegree.end(); it++) {
        if (it->second == 0) 
            q.push(it->first);
    }
    
    while (!q.empty()) {
        string course = q.front();
        q.pop();
        topo_order.push_back(course);
        
        for (int i = 0; i < graph[course].size(); i++) {
            string dependent = graph[course][i];
            indegree[dependent]--;
            if (indegree[dependent] == 0) 
                q.push(dependent);
        }
    }
    return topo_order;
}

int main() {
    map<string, vector<string>> graph;
    
    graph["M221"] = {"M222", "M265", "Phys217"}; 
    graph["Engr104"] = {"CE122"};
    graph["M222"] = {"M223"};
    graph["CE122"] = {"CE222", "CE241"};
    graph["M265"] = {"CE223"};
    graph["Phys217"] = {"Engr201"};
    graph["CE222"] = {"CE223"}; 
    graph["CE223"] = {"CE224"};
    graph["M223"] = {"M327", "M311"}; 
    graph["M327"] = {"CE332", "Engr336"};
    graph["M311"] = {};
    graph["CE332"] = {}; 
    graph["Engr201"] = {"Engr271", "Engr337", "Engr336"};
    graph["Engr271"] = {}; 
    graph["Engr337"] = {"Engr430", "CE496"}; 
    graph["Engr430"] = {};
    graph["CE241"] = {"CE315", "CE401", "CE351"};
    graph["CE315"] = {"CE433", "CE338", "CE496"};
    graph["CE401"] = {};
    graph["CE351"] = {"CE496"}; 
    graph["CE433"] = {"CE496"}; 
    graph["CE338"] = {"CE496"};
    graph["Engr336"] = {"Engr431"};
    graph["Engr431"] = {"CE432", "CE496"}; 
    graph["CE432"] = {};
    graph["CE496"] = {"CE497"}; 
    graph["CE497"] = {};
    
    vector<string> order = topologicalSort(graph);

    cout << "Topological Ordering:" << endl;
    for (int i = 0; i < order.size(); i++) { cout << order[i] << endl; }

    return 0;
}
