#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

struct Edge {
    int u, v, weight;
};

bool compare(const Edge &a, const Edge &b){
    return a.weight < b.weight;
}

class DisjointSet {
public:
    vector<int> parent;
    vector<int> rank;

    DisjointSet(int n) {
        parent.resize(n);
        rank.resize(n,0);
        for(int i = 0; i < n; i++){
            parent[i] = i;
        }
    }
    int find(int x){
        if(parent[x]!=x)
            parent[x]=find(parent[x]);

        return parent[x];
    }
    bool unionSets(int x, int y){
        int rootx = find(x);
        int rooty = find(y);

        if(rootx==rooty)
            return false;

        if(rank[rootx] < rank[rooty])
            parent[rootx] = rooty;

        else if(rank[rootx] > rank[rooty])
            parent[rooty] = rootx;

        else {
            parent[rooty] = rootx;
            rank[rootx]++; }

        return true;
    }
};

vector<Edge> kruskal(int n, vector<Edge> edges){
    vector<Edge> mst;

    sort(edges.begin(), edges.end(), compare);
    DisjointSet ds(n);

    for(auto e : edges){
        if(ds.unionSets(e.u,e.v)){
            mst.push_back(e);
            if(mst.size()==n-1) break;
        }
    }

    return mst;
}

vector<vector<pair<int,int>>> buildGraph(int n, vector<Edge> edges){
    vector<vector<pair<int,int>>> graph(n);

    for(auto e : edges){
        graph[e.u].push_back({e.v,e.weight});
        graph[e.v].push_back({e.u,e.weight});
    }

    return graph;
}

vector<Edge> prim(int n, vector<Edge> edges){
    vector<Edge> mst;
    vector<vector<pair<int,int>>> graph = buildGraph(n, edges);
    vector<bool> inMST(n, false);

    typedef pair<int, pair<int,int>> Node;

    priority_queue<Node, vector<Node>, greater<Node>> pq;

    inMST[0] = true;

    for(auto neighbor : graph[0]){
        pq.push({neighbor.second, {0, neighbor.first}});
    }
    while(!pq.empty() && mst.size() < n-1){
        auto top = pq.top(); pq.pop();

        int w = top.first, u = top.second.first, v = top.second.second;

        if(inMST[v])
            continue;

        inMST[v] = true;
        mst.push_back({u, v, w});

        for(auto neighbor : graph[v]){
            if(!inMST[neighbor.first])
                pq.push({neighbor.second, {v, neighbor.first}});
        }
    }
    return mst;
}

int main(){
    int n = 10;
    vector<Edge> edges = {
            {0,1,4},
            {0,7,8},
            {1,2,8},
            {1,7,11},
            {2,3,7},
            {2,5,4},
            {2,8,2},
            {3,4,9},
            {3,5,14},
            {4,5,10},
            {5,6,2},
            {6,7,1},
            {6,8,6},
            {6,9,7},
            {7,8,7},
            {7,9,9},
            {8,9,3}
    };
    vector<Edge> mstK = kruskal(n, edges);
    int totalK = 0;
    cout << "Kruskal's:" << endl;

    for(auto e : mstK){
        cout << "Edge " << e.u << " - " << e.v << " weight " << e.weight << endl;
        totalK += e.weight;
    }
    cout << "Total weight: " << totalK << endl;
    cout << endl;
    vector<Edge> mstP = prim(n, edges);
    int totalP = 0;

    cout << "Prim's:" << endl;
    for(auto e : mstP){
        cout << "Edge " << e.u << " - " << e.v << " weight " << e.weight << endl;
        totalP += e.weight;
    }

    cout << "Total weight: " << totalP << endl;
    return 0;
}
