#include<iostream>
#include<vector>
using namespace std;

    vector<vector<int>>adj;
    vector<bool>visited;
    bool graphConnected(int n,int ignoreEdge)
    {
        visited.assign(n+1,false);
        int start=1;
        while(start==ignoreEdge || adj[start].empty() && start<=n)//suitable start
        {
            start++;
        }
        if(start>n)//empty/disconnected
        {
            return true;
        }
        vector <int>s;
        s.push_back(start);
        while(!s.empty())
        {
            int node = s.back();
            s.pop_back();
            if(!visited[node]){
                visited[node]=true;

                for(int neighbor:adj[node])
                {
                    if(neighbor!=ignoreEdge)
                    {
                        s.push_back(neighbor);
                    }
                }
            }
        }
        for(int i=0;i<=n;i++)
        {
            if(!visited[i] && !adj[i].empty() && i!=ignoreEdge){//any one of the nodes left visiting
                return false;
            }
        }
        return true;
    }

int main()
{
    int n,m;
    cin>>n>>m;
    adj.resize(n+1);
    for (int i=0;i<m;i++)
    {
        int u,v;
        cin>>u>>v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    for(int i=0;i<=n;i++)
    {
        for(int neighbor: adj[i])
        {
            if(i<neighbor){
                if(graphConnected(n,i))
                {
                    cout<<"YES"<<" "<<i<<" "<<neighbor;
                    return 0;
                }
            }
        }
    }
    cout<<"NO"<<endl;
    return 0;

}
