#include <iostream>
#include <chrono>
#include <ctime>
#include <algorithm>

using namespace std;

const int INF = 1e9;

struct Edge {
    int u, v, w;
};

struct Node {
    int dest;
    int weight;
    Node* next;
};

class SimpleQueue {
    int* arr;
    int capacity;
    int front, rear, count;

public:
    SimpleQueue(int size) {
        capacity = size;
        arr = new int[capacity];
        front = 0;
        rear = -1;
        count = 0;
    }

    ~SimpleQueue() {
        delete[] arr;
    }

    void push(int val) {
        if (count < capacity) {
            rear = (rear + 1) % capacity;
            arr[rear] = val;
            count++;
        }
    }

    int pop() {
        int val = arr[front];
        front = (front + 1) % capacity;
        count--;
        return val;
    }

    bool isEmpty() {
        return count == 0;
    }
};

class DSU {
    int* parent;
    int* rankArr;

public:
    DSU(int n) {
        parent = new int[n];
        rankArr = new int[n];

        for (int i = 0; i < n; i++) {
            parent[i] = i;
            rankArr[i] = 0;
        }
    }

    ~DSU() {
        delete[] parent;
        delete[] rankArr;
    }

    int find(int v) {
        if (parent[v] == v) return v;
        return parent[v] = find(parent[v]);
    }

    void unite(int a, int b) {
        a = find(a);
        b = find(b);

        if (a != b) {
            if (rankArr[a] < rankArr[b]) {
                parent[a] = b;
            } else if (rankArr[a] > rankArr[b]) {
                parent[b] = a;
            } else {
                parent[b] = a;
                rankArr[a]++;
            }
        }
    }
};

class Graph {
public:
    int V;
    bool isMatrix;
    bool isDirected;

    int** matrix;
    Node** adjList;

    Graph(int vertices, bool matrixMode, bool directed = false) {
        V = vertices;
        isMatrix = matrixMode;
        isDirected = directed;

        if (isMatrix) {
            matrix = new int*[V];

            for (int i = 0; i < V; i++) {
                matrix[i] = new int[V];

                for (int j = 0; j < V; j++) {
                    matrix[i][j] = 0;
                }
            }

            adjList = nullptr;
        } else {
            adjList = new Node*[V];

            for (int i = 0; i < V; i++) {
                adjList[i] = nullptr;
            }

            matrix = nullptr;
        }
    }

    ~Graph() {
        if (isMatrix) {
            for (int i = 0; i < V; i++) {
                delete[] matrix[i];
            }

            delete[] matrix;
        } else {
            for (int i = 0; i < V; i++) {
                Node* curr = adjList[i];

                while (curr) {
                    Node* temp = curr;
                    curr = curr->next;
                    delete temp;
                }
            }

            delete[] adjList;
        }
    }

    void addEdge(int u, int v, int w = 1) {
        if (isMatrix) {
            matrix[u][v] = w;

            if (!isDirected) {
                matrix[v][u] = w;
            }
        } else {
            Node* newNode = new Node{v, w, nullptr};

            if (adjList[u] == nullptr) {
                adjList[u] = newNode;
            } else {
                Node* curr = adjList[u];

                while (curr->next) {
                    curr = curr->next;
                }

                curr->next = newNode;
            }

            if (!isDirected) {
                Node* revNode = new Node{u, w, nullptr};

                if (adjList[v] == nullptr) {
                    adjList[v] = revNode;
                } else {
                    Node* curr = adjList[v];

                    while (curr->next) {
                        curr = curr->next;
                    }

                    curr->next = revNode;
                }
            }
        }
    }

    int getNeighbors(int u, int* neighbors, int* weights) {
        int count = 0;

        if (isMatrix) {
            for (int v = 0; v < V; v++) {
                if (matrix[u][v] != 0) {
                    neighbors[count] = v;
                    weights[count] = matrix[u][v];
                    count++;
                }
            }
        } else {
            Node* curr = adjList[u];

            while (curr) {
                neighbors[count] = curr->dest;
                weights[count] = curr->weight;
                count++;
                curr = curr->next;
            }
        }

        return count;
    }

    void print() {
        if (isMatrix) {
            cout << "Adjacency Matrix:\n";

            for (int i = 0; i < V; i++) {
                for (int j = 0; j < V; j++) {
                    cout << matrix[i][j] << "\t";
                }

                cout << "\n";
            }
        } else {
            cout << "Adjacency List:\n";

            for (int i = 0; i < V; i++) {
                cout << "[" << i << "]: ";

                Node* curr = adjList[i];

                while (curr) {
                    cout << "->" << curr->dest << "(w:" << curr->weight << ") ";
                    curr = curr->next;
                }

                cout << "\n";
            }
        }
    }

    Graph* convertToMatrix() {
        if (isMatrix) return nullptr;

        Graph* newGraph = new Graph(V, true, isDirected);

        int* neighbors = new int[V];
        int* weights = new int[V];

        for (int i = 0; i < V; i++) {
            int count = getNeighbors(i, neighbors, weights);

            for (int j = 0; j < count; j++) {
                newGraph->matrix[i][neighbors[j]] = weights[j];
            }
        }

        delete[] neighbors;
        delete[] weights;

        return newGraph;
    }

    Graph* convertToList() {
        if (!isMatrix) return nullptr;

        Graph* newGraph = new Graph(V, false, isDirected);

        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                if (matrix[i][j] != 0) {
                    if (isDirected || i < j) {
                        newGraph->addEdge(i, j, matrix[i][j]);
                    }
                }
            }
        }

        return newGraph;
    }
};

void sortNeighbors(int* neighbors, int* weights, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (weights[j] > weights[j + 1]) {
                swap(weights[j], weights[j + 1]);
                swap(neighbors[j], neighbors[j + 1]);
            }
        }
    }
}

void findConnectedComponents(Graph& g) {
    bool* visited = new bool[g.V];

    for (int i = 0; i < g.V; i++) {
        visited[i] = false;
    }

    int* neighbors = new int[g.V];
    int* weights = new int[g.V];

    int component = 1;

    cout << "Connected Components:\n";

    for (int i = 0; i < g.V; i++) {
        if (!visited[i]) {
            cout << "Component " << component++ << ": ";

            SimpleQueue q(g.V);
            q.push(i);
            visited[i] = true;

            while (!q.isEmpty()) {
                int u = q.pop();

                cout << u << " ";

                int count = g.getNeighbors(u, neighbors, weights);

                for (int j = 0; j < count; j++) {
                    int v = neighbors[j];

                    if (!visited[v]) {
                        visited[v] = true;
                        q.push(v);
                    }
                }
            }

            cout << "\n";
        }
    }

    delete[] visited;
    delete[] neighbors;
    delete[] weights;
}

void BFS(Graph& g, int start, bool sortByWeight) {
    bool* visited = new bool[g.V];

    for (int i = 0; i < g.V; i++) {
        visited[i] = false;
    }

    int* neighbors = new int[g.V];
    int* weights = new int[g.V];

    SimpleQueue q(g.V);

    q.push(start);
    visited[start] = true;

    cout << "BFS Traversal (Sort by weight: "
         << (sortByWeight ? "Yes" : "No") << "): ";

    while (!q.isEmpty()) {
        int u = q.pop();

        cout << u << " ";

        int count = g.getNeighbors(u, neighbors, weights);

        if (sortByWeight) {
            sortNeighbors(neighbors, weights, count);
        }

        for (int i = 0; i < count; i++) {
            int v = neighbors[i];

            if (!visited[v]) {
                visited[v] = true;
                q.push(v);
            }
        }
    }

    cout << "\n";

    delete[] visited;
    delete[] neighbors;
    delete[] weights;
}

void Dijkstra(Graph& g, int start) {
    int* dist = new int[g.V];
    bool* visited = new bool[g.V];

    for (int i = 0; i < g.V; i++) {
        dist[i] = INF;
        visited[i] = false;
    }

    dist[start] = 0;

    int* neighbors = new int[g.V];
    int* weights = new int[g.V];

    for (int i = 0; i < g.V - 1; i++) {
        int minDist = INF;
        int u = -1;

        for (int j = 0; j < g.V; j++) {
            if (!visited[j] && dist[j] < minDist) {
                minDist = dist[j];
                u = j;
            }
        }

        if (u == -1) break;

        visited[u] = true;

        int count = g.getNeighbors(u, neighbors, weights);

        for (int j = 0; j < count; j++) {
            int v = neighbors[j];
            int w = weights[j];

            if (!visited[v] && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
            }
        }
    }

    cout << "Dijkstra distances from vertex " << start << ":\n";

    for (int i = 0; i < g.V; i++) {
        cout << "To " << i << " -> ";

        if (dist[i] == INF) cout << -1;
        else cout << dist[i];

        cout << "\n";
    }

    delete[] dist;
    delete[] visited;
    delete[] neighbors;
    delete[] weights;
}

void KahnTopologicalSort(Graph& g) {
    if (!g.isDirected) {
        cout << "Kahn requires directed graph!\n";
        return;
    }

    int* inDegree = new int[g.V];

    for (int i = 0; i < g.V; i++) {
        inDegree[i] = 0;
    }

    int* neighbors = new int[g.V];
    int* weights = new int[g.V];

    for (int i = 0; i < g.V; i++) {
        int count = g.getNeighbors(i, neighbors, weights);

        for (int j = 0; j < count; j++) {
            inDegree[neighbors[j]]++;
        }
    }

    SimpleQueue q(g.V);

    for (int i = 0; i < g.V; i++) {
        if (inDegree[i] == 0) {
            q.push(i);
        }
    }

    int visitedCount = 0;

    cout << "Topological Sort (Kahn): ";

    while (!q.isEmpty()) {
        int u = q.pop();

        cout << u << " ";

        visitedCount++;

        int count = g.getNeighbors(u, neighbors, weights);

        for (int j = 0; j < count; j++) {
            int v = neighbors[j];

            inDegree[v]--;

            if (inDegree[v] == 0) {
                q.push(v);
            }
        }
    }

    if (visitedCount != g.V) {
        cout << "\nCycle detected!";
    }

    cout << "\n";

    delete[] inDegree;
    delete[] neighbors;
    delete[] weights;
}

void DFSHelper(Graph& g, int u, bool* visited,
               int* neighbors, int* weights,
               int& totalWeight) {
    visited[u] = true;

    int count = g.getNeighbors(u, neighbors, weights);

    for (int i = 0; i < count; i++) {
        int v = neighbors[i];

        if (!visited[v]) {
            cout << "Edge (" << u << " - "
                 << v << ") weight: "
                 << weights[i] << "\n";

            totalWeight += weights[i];

            DFSHelper(g, v, visited,
                      neighbors, weights,
                      totalWeight);
        }
    }
}

void DFSSpanningTree(Graph& g, int start) {
    bool* visited = new bool[g.V];

    for (int i = 0; i < g.V; i++) {
        visited[i] = false;
    }

    int* neighbors = new int[g.V];
    int* weights = new int[g.V];

    int totalWeight = 0;

    cout << "DFS Spanning Tree Edges:\n";

    DFSHelper(g, start, visited,
              neighbors, weights,
              totalWeight);

    cout << "Total DFS Spanning Tree Weight: "
         << totalWeight << "\n";

    delete[] visited;
    delete[] neighbors;
    delete[] weights;
}

void BFSSpanningTree(Graph& g, int start) {
    bool* visited = new bool[g.V];

    for (int i = 0; i < g.V; i++) {
        visited[i] = false;
    }

    int* neighbors = new int[g.V];
    int* weights = new int[g.V];

    SimpleQueue q(g.V);

    q.push(start);
    visited[start] = true;

    int totalWeight = 0;

    cout << "BFS Spanning Tree Edges:\n";

    while (!q.isEmpty()) {
        int u = q.pop();

        int count = g.getNeighbors(u, neighbors, weights);

        for (int i = 0; i < count; i++) {
            int v = neighbors[i];

            if (!visited[v]) {
                visited[v] = true;
                q.push(v);

                cout << "Edge (" << u << " - "
                     << v << ") weight: "
                     << weights[i] << "\n";

                totalWeight += weights[i];
            }
        }
    }

    cout << "Total BFS Spanning Tree Weight: "
         << totalWeight << "\n";

    delete[] visited;
    delete[] neighbors;
    delete[] weights;
}

void KruskalMST(Graph& g) {
    int maxEdges =
        g.isDirected ?
        g.V * g.V :
        (g.V * (g.V - 1)) / 2;

    Edge* edges = new Edge[maxEdges];

    int edgeCount = 0;

    int* neighbors = new int[g.V];
    int* weights = new int[g.V];

    for (int u = 0; u < g.V; u++) {
        int count = g.getNeighbors(u, neighbors, weights);

        for (int i = 0; i < count; i++) {
            int v = neighbors[i];

            if (g.isDirected || u < v) {
                edges[edgeCount++] =
                    {u, v, weights[i]};
            }
        }
    }

    for (int i = 0; i < edgeCount - 1; i++) {
        for (int j = 0; j < edgeCount - i - 1; j++) {
            if (edges[j].w > edges[j + 1].w) {
                swap(edges[j], edges[j + 1]);
            }
        }
    }

    DSU dsu(g.V);

    int totalWeight = 0;

    cout << "Kruskal MST Edges:\n";

    int used = 0;

    for (int i = 0; i < edgeCount; i++) {
        int u = edges[i].u;
        int v = edges[i].v;

        if (dsu.find(u) != dsu.find(v)) {
            dsu.unite(u, v);

            cout << "Edge (" << u << " - "
                 << v << ") weight: "
                 << edges[i].w << "\n";

            totalWeight += edges[i].w;
            used++;

            if (used == g.V - 1) break;
        }
    }

    cout << "Total Kruskal MST Weight: "
         << totalWeight << "\n";

    delete[] edges;
    delete[] neighbors;
    delete[] weights;
}

void runDemo() {
    cout << "\nDEMO MODE\n";

    Graph g1(5, true, false);

    g1.addEdge(0, 1, 9);
    g1.addEdge(0, 2, 1);
    g1.addEdge(0, 3, 7);
    g1.addEdge(1, 4, 2);
    g1.addEdge(2, 4, 3);
    g1.addEdge(3, 4, 8);

    g1.print();

    findConnectedComponents(g1);

    BFS(g1, 0, false);
    BFS(g1, 0, true);

    Dijkstra(g1, 0);

    DFSSpanningTree(g1, 0);
    BFSSpanningTree(g1, 0);

    KruskalMST(g1);

    Graph* listGraph = g1.convertToList();

    cout << "\nConverted To List:\n";
    listGraph->print();

    Graph g2(6, false, true);

    g2.addEdge(5, 2);
    g2.addEdge(5, 0);
    g2.addEdge(4, 0);
    g2.addEdge(4, 1);
    g2.addEdge(2, 3);
    g2.addEdge(3, 1);

    cout << "\nDirected Graph:\n";

    g2.print();

    KahnTopologicalSort(g2);

    delete listGraph;
}

void runInteractive() {
    cout << "\nINTERACTIVE MODE\n";

    int V;

    cout << "Enter number of vertices: ";
    cin >> V;

    int type;

    cout << "Matrix (1) or List (0): ";
    cin >> type;

    int directed;

    cout << "Directed (1) or Undirected (0): ";
    cin >> directed;

    Graph g(V, type == 1, directed == 1);

    int E;

    cout << "Enter number of edges: ";
    cin >> E;

    for (int i = 0; i < E; i++) {
        int u, v, w;

        cout << "Edge " << i + 1 << " (u v w): ";

        cin >> u >> v >> w;

        g.addEdge(u, v, w);
    }

    int choice;

    do {
        cout << "\n1.Print Graph\n";
        cout << "2.BFS\n";
        cout << "3.Dijkstra\n";
        cout << "4.Kruskal\n";
        cout << "5.Kahn\n";
        cout << "6.DFS Spanning Tree\n";
        cout << "7.BFS Spanning Tree\n";
        cout << "0.Back\n";
        cout << "Choice: ";

        cin >> choice;

        if (choice == 1) {
            g.print();
        }

        else if (choice == 2) {
            int start;

            cout << "Start vertex: ";
            cin >> start;

            BFS(g, start, false);
        }

        else if (choice == 3) {
            int start;

            cout << "Start vertex: ";
            cin >> start;

            Dijkstra(g, start);
        }

        else if (choice == 4) {
            KruskalMST(g);
        }

        else if (choice == 5) {
            KahnTopologicalSort(g);
        }

        else if (choice == 6) {
            int start;

            cout << "Start vertex: ";
            cin >> start;

            DFSSpanningTree(g, start);
        }

        else if (choice == 7) {
            int start;

            cout << "Start vertex: ";
            cin >> start;

            BFSSpanningTree(g, start);
        }

    } while (choice != 0);
}

void benchmarkDijkstra(Graph& g, const string& name) {
    auto start =
        chrono::high_resolution_clock::now();

    Dijkstra(g, 0);

    auto end =
        chrono::high_resolution_clock::now();

    auto duration =
        chrono::duration_cast
        <chrono::microseconds>
        (end - start).count();

    cout << name << " Time: "
         << duration << " us\n";
}

void runBenchmark() {
    cout << "\nBENCHMARK MODE\n";

    int sizes[] = {100, 500};

    for (int s = 0; s < 2; s++) {
        int V = sizes[s];

        cout << "\nVertices: "
             << V << "\n";

        Graph gMatrix(V, true, false);
        Graph gList(V, false, false);

        int E = V * 5;

        for (int i = 0; i < E; i++) {
            int u = rand() % V;
            int v = rand() % V;
            int w = 1 + rand() % 20;

            if (u != v) {
                gMatrix.addEdge(u, v, w);
                gList.addEdge(u, v, w);
            }
        }

        int matrixMemory =
            V * V * sizeof(int);

        int listMemory =
            E * sizeof(Node) * 2;

        cout << "Approx Matrix Memory: "
             << matrixMemory
             << " bytes\n";

        cout << "Approx List Memory: "
             << listMemory
             << " bytes\n";

        benchmarkDijkstra(gMatrix,
                          "Matrix Dijkstra");

        benchmarkDijkstra(gList,
                          "List Dijkstra");
    }
}

int main() {
    srand(time(0));

    int mode;

    do {
        cout << "\n1. Interactive Mode\n";
        cout << "2. Demo Mode\n";
        cout << "3. Benchmark Mode\n";
        cout << "0. Exit\n";
        cout << "Choice: ";

        cin >> mode;

        switch (mode) {
            case 1:
                runInteractive();
                break;

            case 2:
                runDemo();
                break;

            case 3:
                runBenchmark();
                break;

            case 0:
                cout << "Exiting...\n";
                break;

            default:
                cout << "Invalid choice!\n";
        }

    } while (mode != 0);

    return 0;
}