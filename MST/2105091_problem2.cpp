#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
using namespace std;

vector<int> parent(100);
vector<int> size(100);

void make(int v) {
    parent[v] = v;
    size[v] = 1;
}

int find_set(int v) {
    if (v == parent[v])
        return v;
    return find_set(parent[v]);
}

void union_sets(int a, int b) {
    a = find_set(a);
    b = find_set(b);
    if (a != b) {
        if (size[a] < size[b]) {
            swap(a, b);
        }
        parent[b] = a;
        size[a] += size[b];
    }
}

bool detectCycle(vector<vector<int>>& edges, int n) {
    vector<vector<int>> adj(n + 1);
    for (const auto& edge : edges) {
        int u = edge[1];
        int v = edge[2];
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    vector<bool> visited(n + 1, false);
    vector<int> parent(n + 1, -1);

    for (int i = 1; i <= n; ++i) {
        if (!visited[i]) {
            stack<int> s;
            stack<int> parent_stack;

            s.push(i);
            parent_stack.push(-1);
            visited[i] = true;

            while (!s.empty()) {
                int node = s.top();
                int par = parent_stack.top();
                s.pop();
                parent_stack.pop();

                for (int neighbor : adj[node]) {
                    if (!visited[neighbor]) {
                        s.push(neighbor);
                        parent_stack.push(node);
                        visited[neighbor] = true;
                        parent[neighbor] = node;
                    } else if (neighbor != par) {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

int main() {
    for (int i = 0; i < 100; i++) {
        make(i);
    }

    int n, m;
    cin >> n >> m;
    vector<vector<vector<int>>> mst(10);
    int count = 0;

    vector<vector<int>> edges;

    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        edges.push_back({w, u, v});
    }

    sort(edges.begin(), edges.end());

    int cost = 0;
    int wlast = 0;

    for (auto i : edges) {
        int w = i[0];
        int u = i[1];
        int v = i[2];
        int x = find_set(u);
        int y = find_set(v);

        if (x == y) {
            if (wlast == w) {
                count++;

                vector<vector<int>> msttemp;

                for (const auto& edge : mst[count - 1]) {
                    msttemp.push_back(edge);
                }


                msttemp.pop_back();

                msttemp.push_back({w, u, v});
                if (detectCycle(msttemp, n)) {
                    count--;
                } else {
                    mst[count] = msttemp;
                }
            }
        } else {
            cost += w;
            union_sets(u, v);
            wlast = w;

            for (int i = 0; i <= count; i++) {
                mst[i].push_back({w, u, v});
            }
        }
    }
    cout<<"MSTs: "<<endl;

    for (int i = 0; i <= count; i++) {
        cout << i + 1 << ": [";
        for (const auto& edge : mst[i]) {
            cout <<"["<< edge[1] << ", " << edge[2] << ", " << edge[0] <<" ]";

        }
        cout<<"]"<<endl;
    }



     return 0;
}
