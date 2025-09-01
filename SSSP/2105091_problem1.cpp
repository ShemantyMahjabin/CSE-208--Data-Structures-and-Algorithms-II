#include<iostream>
#include<bits/stdc++.h>
using namespace std;


class Edge
{
public:
    int v,w;
    Edge(int v,int w):v(v),w(w) {}
};

class State
{
public:
    int city,gas;
    long long cost;
    State(int city,int gas,long long cost):city(city),gas(gas),cost(cost) {}
    bool operator<(const State &a)const
    {
        return a.cost<cost;
    }
};

class Graph
{
public:
    int N,M,C;
    vector<vector<Edge>>adj;
    vector<int>gasPrices;
    Graph(int N,int M, int C):N(N),M(M),C(C),adj(N+1),gasPrices(N+1)
    {
        for(int i=1; i<=N; i++)
        {
            adj[i]=vector<Edge>();
        }
    }

    void addEdge(int u,int v,int w)
    {
        adj[u].emplace_back(v,w);
        adj[v].emplace_back(u,w);
    }

    void setGasPrice()
    {
        for(int i=1; i<=N; ++i)
        {
            cin>>gasPrices[i];
        }
    }
    long long minCost(int start,int destination,int tankCapacity)
    {
        vector<vector<long long>>dp(N+1,vector<long long>(tankCapacity+1,LLONG_MAX));
        priority_queue<State>pq;
        pq.push(State(start,0,0));
        while(!pq.empty())
        {
            State current=pq.top();
            pq.pop();
            cout<<"state Pop: "<<current.city<<","<<current.gas<<","<<current.cost<<endl;
            int u=current.city;
            int gas=current.gas;
            long long cost=current.cost;

            if(dp[u][gas]<cost)
            {
                continue;
            }
            dp[u][gas]=cost;
            if(u==destination)
            {
                cout << "DP Table:" << endl;
                for (int i = 1; i <= N; ++i)
                {
                    for (int j = 0; j <= C; ++j)
                    {
                        if (dp[i][j] == LLONG_MAX)
                        {
                            cout << "INF ";
                        }
                        else
                        {
                            cout << dp[i][j] << " ";
                        }
                    }
                    cout << endl;
                }
                return cost;
            }

            if(gas<tankCapacity)
            {
                int nextGas=gas+1;
                long long nextCost=cost+gasPrices[u];
                pq.push(State(u,nextGas,nextCost));
                cout<<"Push when gas<tankcapacity  "<<u<<","<<nextGas<<","<<nextCost<<endl;

            }

            for(const Edge& edge:adj[u])
            {
                int v=edge.v;
                int w=edge.w;
                if(gas>=w && dp[v][gas-w]>cost)
                {
                    dp[v][gas-w]=cost;
                    pq.push(State(v,gas-w,cost));
                    cout<<"Push adj "<<v<<","<<gas-w<<","<<cost<<endl;
                }
            }
        }

        return -1;
    }

};
int main()
{
    int N,M,C;
    cin>>N>>M>>C;
    Graph graph(N,M,C);

    graph.setGasPrice();

    for(int i=1; i<=M; ++i)
    {
        int u,v,d;
        cin>>u>>v>>d;
        graph.addEdge(u,v,d);
    }

    int S,D;
    cin>>S>>D;

    long long result=graph.minCost(S,D,C);
    if(result!=-1)
    {
        cout<<result<<endl;
    }
    else
    {
        cout<<"impossible"<<endl;
    }


    return 0;
}

