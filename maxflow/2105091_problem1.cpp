#include <iostream>
#include <bits/stdc++.h>

using namespace std;


  int N,M;
  int n_max,f_max;

bool bfs(vector<vector<int>> rGraph,int s,int t,int parent[]){
    bool visited[N+1];
    memset(visited,0,sizeof(visited));
    queue<int>q;
    q.push(s);
    visited[s]=true;
    parent[s]=-1;

    while(!q.empty())
    {
        int u=q.front();
        q.pop();

        for(int v=1;v<=N;v++)
        {
            if(visited[v]==false && rGraph[u][v]>0)
            {
                if(v==t)
                {
                    parent[v]=u;
                    return true;

                }
                q.push(v);
                parent[v]=u;
                visited[v]=true;
            }
        }
    }
    return false;
}

void dfsMinCut(vector<vector<int>>& rGraph, int u, vector<bool>& visited) {
    visited[u] = true;

    for (int v = 1; v <= N; v++) {
        if (!visited[v] && rGraph[u][v] > 0) {
            dfsMinCut(rGraph, v, visited);
        }
    }
}

int fordFulkerson(vector<vector<int>>&graph,int s,int t,vector<int>&inoutflow,vector<vector<int>>&minCut)
{
    vector<vector<int>> rGraph(N+1,vector<int>(N + 1));

    for(int u=1;u<=N;u++)
    {
        for(int v=1;v<=N;v++)
        {
            rGraph[u][v]=graph[u][v];
        }
    }

    int parent[N];

    int max_flow=0;

    while(bfs(rGraph,s,t,parent))
    {
        int path_flow=INT_MAX;
        for(int v=t;v!=s;v=parent[v])
        {
            int u=parent[v];
            path_flow=min(path_flow,rGraph[u][v]);
        }

        for(int v=t;v!=s;v=parent[v])
        {
            int u=parent[v];
            inoutflow[u]+=path_flow;
            inoutflow[v]+=path_flow;
            rGraph[u][v]-=path_flow;
            rGraph[v][u]+=path_flow;
        }
        max_flow+=path_flow;
    }
    vector<bool> visited(N+1, false);
    dfsMinCut(rGraph, s, visited);

    for (int i = 1; i <= N; i++) {
        if (visited[i]) {
            minCut[0].push_back(i);
        } else {
            minCut[1].push_back(i);
        }
    }

    return max_flow;
}



int main()
{

    cin>>N>>M;
    vector<vector<int>> graph(N + 1, vector<int>(N + 1, 0));

    for(int  i=0;i<M;i++)
    {
        int u,v,c;
        cin>>u>>v>>c;
        graph[u][v]=c;
    }
    int s,t;
    cin>>s>>t;
    int n_max,f_max=INT_MIN;
    vector<int>inoutflow(N+1);
    vector<vector<int>> minCut(2);
    int maxflow=fordFulkerson(graph,s,t,inoutflow,minCut);
    for(int i=1;i<=N;i++)
    {
        if(f_max<inoutflow[i])
        {
            n_max=i;
            f_max=inoutflow[i];
        }
    }
    cout<<n_max<<" "<<f_max/2<<endl;
    cout << "[ {";
    for (int i : minCut[0]) {
        cout << i << " ";
    }
    cout << "}, {";
    for (int i : minCut[1]) {
        cout << i << " ";
    }
    cout << "} ]" << endl;
    cout<<maxflow<<endl;
    return 0;

}

