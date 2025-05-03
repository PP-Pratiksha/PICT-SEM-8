#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <omp.h>
#include <cstdlib>
#include <ctime>

using namespace std;

class ParallelGraph
{
public:
    vector<int> parallelDFS(int V, vector<int> adj[], int startNode)
    {
        vector<int> result;
        vector<int> visited(V + 1, 0);
        stack<int> s;
        s.push(startNode);
        visited[startNode] = 1;

        #pragma omp parallel
        {
            #pragma omp single nowait
            {
                while (!s.empty())
                {
                    int node;
                    #pragma omp critical
                    {
                        node = s.top();
                        s.pop();
                    }
                    result.push_back(node);

                    #pragma omp parallel for
                    for (int i = 0; i < adj[node].size(); i++)
                    {
                        int nextNode = adj[node][i];
                        if (!visited[nextNode])
                        {
                            #pragma omp critical
                            {
                                visited[nextNode] = 1;
                                s.push(nextNode);
                            }
                        }
                    }
                }
            }
        }
        return result;
    }

    vector<int> SeqDFS(int V, vector<int> adj[], int startNode)
    {
        vector<int> result;
        vector<int> visited(V + 1, 0);
        stack<int> s;
        s.push(startNode);
        visited[startNode] = 1;

        while (!s.empty())
        {
            int node;
            node = s.top();
            s.pop();

            result.push_back(node);

            for (int i = 0; i < adj[node].size(); i++)
            {
                int nextNode = adj[node][i];
                if (!visited[nextNode])
                {
                    visited[nextNode] = 1;
                    s.push(nextNode);
                }
            }
        }

        return result;
    }

    vector<int> parallelBFS(int V, vector<int> adj[], int startNode)
    {
        vector<int> result;
        vector<int> visited(V + 1, 0);
        queue<int> q;
        q.push(startNode);
        visited[startNode] = 1;

#pragma omp parallel
        {
#pragma omp single nowait
            {
                while (!q.empty())
                {
                    int node;
#pragma omp critical
                    {
                        node = q.front();
                        q.pop();
                    }
                    result.push_back(node);

#pragma omp parallel for
                    for (int i = 0; i < adj[node].size(); i++)
                    {
                        int nextNode = adj[node][i];
                        if (!visited[nextNode])
                        {
#pragma omp critical
                            {
                                visited[nextNode] = 1;
                                q.push(nextNode);
                            }
                        }
                    }
                }
            }
        }
        return result;
    }

    vector<int> SeqBFS(int V, vector<int> adj[], int startNode)
    {
        vector<int> result;
        vector<int> visited(V + 1, 0);
        queue<int> q;
        q.push(startNode);
        visited[startNode] = 1;

        while (!q.empty())
        {
            int node;
            node = q.front();
            q.pop();
            result.push_back(node);

            for (int i = 0; i < adj[node].size(); i++)
            {
                int nextNode = adj[node][i];
                if (!visited[nextNode])
                {
                    visited[nextNode] = 1;
                    q.push(nextNode);
                }
            }
        }

        return result;
    }
};

void addEdge(vector<int> adj[], int u, int v)
{
    adj[u].push_back(v);
    adj[v].push_back(u);
}

void generateRandomGraph(vector<int> adj[], int nodes, int edges)
{
    srand(time(0));

    for (int i = 1; i < nodes; i++)
    {
        addEdge(adj, i, i + 1);
    }

    int extraEdges = edges - (nodes - 1);
    while (extraEdges > 0)
    {
        int a = rand() % nodes + 1;
        int b = rand() % nodes + 1;
        if (a != b)
        {
            addEdge(adj, a, b);
            extraEdges--;
        }
    }
}

int main()
{
    int nodes, edges;
    cout << "Enter the number of nodes: ";
    cin >> nodes;
    cout << "Enter the number of edges: ";
    cin >> edges;

    vector<int> adj[nodes + 1];
    generateRandomGraph(adj, nodes, edges);

    int startNode;
    cout << "Enter the starting node for traversals: ";
    cin >> startNode;

    int threads;
    cout << "Enter the number of threads: ";
    cin >> threads;
    omp_set_num_threads(threads);

    ParallelGraph graph;

    auto start_time = omp_get_wtime();
    vector<int> seqDFS = graph.SeqDFS(nodes, adj, startNode);
    auto end_time = omp_get_wtime();
    auto seqDFS_time = (end_time - start_time);

    start_time = omp_get_wtime();
    vector<int> parallelDFS = graph.parallelDFS(nodes, adj, startNode);
    end_time = omp_get_wtime();
    auto parallelDFS_time = (end_time - start_time);    

    start_time = omp_get_wtime();
    vector<int> seqBFS = graph.SeqBFS(nodes, adj, startNode);
    end_time = omp_get_wtime();
    auto seqBFS_time = (end_time - start_time);

    start_time = omp_get_wtime();
    vector<int> parallelBFS = graph.parallelBFS(nodes, adj, startNode);
    end_time = omp_get_wtime();
    auto parallelBFS_time = (end_time - start_time);

    // cout << "Sequential DFS Traversal: " << endl;
    // for (int node : seqDFS)
    //     cout << node << " ";
    cout << "\nSequential DFS Time: " << seqDFS_time << " seconds\n";

    // cout << "Parallel DFS Traversal: " << endl;
    // for (int node : parallelDFS)
    //     cout << node << " ";
    cout << "\nParallel DFS Time: " << parallelDFS_time << " seconds\n";

    // cout << "Sequential BFS Traversal: " << endl;
    // for (int node : seqBFS)
    //     cout << node << " ";
    cout << "\nSequential BFS Time: " << seqBFS_time << " seconds\n";

    // cout << "Parallel BFS Traversal: " << endl;
    // for (int node : parallelBFS)
    //     cout << node << " ";
    cout << "\nParallel BFS Time: " << parallelBFS_time << " seconds\n";

    return 0;
}
