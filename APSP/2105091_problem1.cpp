#include <iostream>
#include <vector>
#include<bits/stdc++.h>


using namespace std;

const int INF = 1e9;

class Graph
{
    int numCities;
    vector<vector<pair<int, int>>> adjList;

public:
    Graph(int n) : numCities(n), adjList(n + 1) {}

    void addEdge(int u, int v, int w)
    {
        adjList[u].emplace_back(v, w);
        adjList[v].emplace_back(u, w);
    }

    const vector<vector<pair<int, int>>>& getAdjList() const
    {
        return adjList;
    }
};

void smallestCities(const vector<vector<int>>&distanceMatrix,int n, int m,int thresholdCost)
{
    int minCities=INF;
    vector<int>resultCities;
    for(int i=1;i<=n;i++)
    {
        int reachableCities=0;
        for(int dist:distanceMatrix[i])
        {
            if(dist<=thresholdCost)
            {
                ++reachableCities;
            }
        }
        if(reachableCities<minCities)
        {
            minCities=reachableCities;
            resultCities={i};
        }else if(reachableCities==minCities)
        {
            resultCities.push_back(i);
        }
    }

    for(int city:resultCities)
    {
        cout<<city<<" ";
    }
    cout<<endl;
}

void buildDistanceMatrix(const Graph& graph, int n, int m,int thresholdCost)
{
    vector<vector<int>> distanceMatrix(n + 1, vector<int>(n + 1, INF));

    for (int i = 1; i <= n; i++)
    {
        distanceMatrix[i][i] = 0;
    }

    const vector<vector<pair<int, int>>>& roads = graph.getAdjList();


    for (int u = 1; u <= n; u++)
    {
        for (const auto& edge : roads[u])
        {
            int v = edge.first;
            int w = edge.second;
            distanceMatrix[u][v] = w;
            distanceMatrix[v][u] = w;

        }
    }

    for(int k=1; k<=n; k++)
    {
        for(int i=1; i<=n; i++)
        {
            for(int j=1; j<=n; j++)
            {
                distanceMatrix[i][j]=min(distanceMatrix[i][j],distanceMatrix[i][k]+distanceMatrix[k][j]);

            }
        }
    }

    smallestCities(distanceMatrix,n,m,thresholdCost);

}



int main()
{
    int n, m;
    cin >> n >> m;
    Graph graph(n);

    for (int i = 0; i < m; i++)
    {
        int u, v, w;
        cin >> u >> v >> w;
        graph.addEdge(u, v, w);
    }

    int thresholdCost;
    cin >> thresholdCost;

    buildDistanceMatrix(graph,n,m,thresholdCost);


    return 0;
}
