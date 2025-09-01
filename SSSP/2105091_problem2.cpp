#include <iostream>
#include <vector>
#include <climits>

using namespace std;

struct Edge
{
    int from, to, weight;
};

const int INF = 1e9;

vector<int> bellmanFord(int N, const vector<Edge>& edges, int source)
{
    vector<int> distance(N+1, INF);
    distance[source] = 0;

    for (int i = 0; i < N - 1; ++i)
    {
        for (const Edge& edge : edges)
        {
            if (distance[edge.from] != INF && distance[edge.to] > distance[edge.from] + edge.weight)
            {
                distance[edge.to] = min(distance[edge.to],distance[edge.from] + edge.weight);
            }
        }
    }


    return distance;
}

bool hasNegativeCycle(int N, const vector<Edge>& edges, int source)
{
    vector<int> distance(N+1, INF);
    distance[source] = 0;

    for (int i = 0; i < N-1; ++i)
    {
        for (const Edge& edge : edges)
        {
            if (distance[edge.from] != INF && distance[edge.to] > distance[edge.from] + edge.weight)
            {
                distance[edge.to] = distance[edge.from] + edge.weight;
            }
        }
    }


    for (const Edge& edge : edges)
    {
        if (distance[edge.from] != INF && distance[edge.to] > distance[edge.from] + edge.weight)
        {
            return true;
        }
    }

    return false;
}

int main()
{
    int N, M;
    cin >> N >> M;

    vector<Edge> edges(M);
    for (int i = 0; i < M; ++i)
    {
        cin >> edges[i].from >> edges[i].to >> edges[i].weight;
    }


    int A, B;
    cin >> A >> B;

    int L, H;
    cin >> L >> H;

    int S, D;
    cin >> S >> D;

    vector<int> originalDistances = bellmanFord(N, edges, S);

    int minDistance = originalDistances[D];


    if (originalDistances[A] == INF || originalDistances[B] == INF)
    {
        cout << "impossible" << endl;
    }
    else
    {
        int assignedweight=L;

        for (int weight = L; weight <= H; ++weight)
        {

            vector<Edge> modifiedEdges = edges;
            modifiedEdges.push_back({A, B, weight});
            if(!hasNegativeCycle(N, modifiedEdges, S))
            {
                vector<int> newDistances = bellmanFord(N, modifiedEdges, S);
                if(newDistances[D]<minDistance)
                {
                    assignedweight=weight;
                }
                minDistance = min(minDistance, newDistances[D]);

                break;
            }

        }

        if (minDistance == originalDistances[D])
        {
            cout << "impossible" << endl;
        }
        else
        {
            cout << assignedweight << " " << minDistance << endl;
        }
    }

    return 0;
}
