#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
using namespace std;

vector<int> parent(100);
vector<int> size(100);

void make(int v)
{
    parent[v] = v;
    size[v] = 1;
}

int find_set(int v)
{
    if (v == parent[v])
        return v;
    return find_set(parent[v]);
}

void union_sets(int a, int b)
{
    a = find_set(a);
    b = find_set(b);
    if (a != b)
    {
        if (size[a] < size[b])
        {
            swap(a, b);
        }
        parent[b] = a;
        size[a] += size[b];
    }
}

bool detectCycle(vector<vector<int>>& edges, int n)
{
    vector<vector<int>> adj(n + 1);
    for (const auto& edge : edges)
    {
        int u = edge[1];
        int v = edge[2];
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    vector<bool> visited(n + 1, false);
    vector<int> parent(n + 1, -1);

    for (int i = 1; i <= n; ++i)
    {
        if (!visited[i])
        {
            stack<int> s;
            stack<int> parent_stack;

            s.push(i);
            parent_stack.push(-1);
            visited[i] = true;

            while (!s.empty())
            {
                int node = s.top();
                int par = parent_stack.top();
                s.pop();
                parent_stack.pop();

                for (int neighbor : adj[node])
                {
                    if (!visited[neighbor])
                    {
                        s.push(neighbor);
                        parent_stack.push(node);
                        visited[neighbor] = true;
                        parent[neighbor] = node;
                    }
                    else if (neighbor != par)
                    {

                        return true;
                    }
                }
            }
        }
    }

    return false;
}

int main()
{
    for (int i = 0; i < 100; i++)
    {
        make(i);
    }

    int n, m;
    cin >> n >> m;
    vector<vector<vector<int>>> mst(10);
    int count = 0;

    vector<vector<int>> edges;
    vector<vector<int>> edges2;

    for (int i = 0; i < m; i++)
    {
        int u, v, w;
        cin >> u >> v >> w;
        edges.push_back({w, u, v});
        edges2.push_back({w, u, v});
    }

    sort(edges.begin(), edges.end());

    int cost = 0;
    int wlast = 0;

    vector<string> edgeInclusion(m, "any");

    for (auto i : edges)
    {
        int w = i[0];
        int u = i[1];
        int v = i[2];
        int x = find_set(u);
        int y = find_set(v);

        if (x == y)
        {
            if (wlast == w)
            {
                count++;


                vector<vector<int>> msttemp;


                for (const auto& edge : mst[count - 1])
                {
                    msttemp.push_back(edge);
                }


                msttemp.pop_back();

                msttemp.push_back({w, u, v});
                if (detectCycle(msttemp, n))
                {
                    count--;
                    edgeInclusion[count] = "none";
                }
                else
                {
                    mst[count] = msttemp;
                }
            }
        }
        else
        {
            cost += w;
            union_sets(u, v);
            wlast = w;

            for (int i = 0; i <= count; i++)
            {
                mst[i].push_back({w, u, v});
            }
        }
    }

    int arr[m];
    for (int i = 0; i < m; ++i) {
    arr[i] = 0;
}

    int ac=0;
    for (int i = 0; i < m; i++)
    {
        int w = edges2[i][0];
        int u = edges2[i][1];
        int v = edges2[i][2];

        for (int j = 0; j <= count; j++)
        {


            for (const auto& edge : mst[j])
            {
                if ((edge[0] == w && ((edge[1] == u && edge[2] == v) || (edge[1] == v && edge[2] == u))) )
                {
                    arr[ac]+=1;


                    break;
                }
            }

        }
        ac++;

    }

    for(int i=0;i<m;i++)
    {
        if(arr[i]==0)
        {
            cout<<"none"<<endl;
        }
        else if(arr[i]==count+1)
        {
            cout<<"any"<<endl;
        }
        else
        {
            cout<<"at least one"<<endl;
        }
    }

    return 0;
}

