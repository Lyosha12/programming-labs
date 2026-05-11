#include <iostream>
#include <chrono>
#include <ctime>

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
        front = 0; rear = -1; count = 0;
    }
    ~SimpleQueue() { delete[] arr; }
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
    bool isEmpty() { return count == 0; }
};

class DSU {
    int* parent;
    int* rank;
public:
    DSU(int n) {
        parent = new int[n];
        rank = new int[n];
        for (int i = 0; i < n; i++) {
            parent[i] = i;
            rank[i] = 0;
        }
    }
    ~DSU() {
        delete[] parent;
        delete[] rank;
    }
    int find(int i) {
        if (parent[i] == i) return i;
        return parent[i] = find(parent[i]);
    }
    void unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);
        if (root_i != root_j) {
            if (rank[root_i] < rank[root_j]) parent[root_i] = root_j;
            else if (rank[root_i] > rank[root_j]) parent[root_j] = root_i;
            else {
                parent[root_j] = root_i;
                rank[root_i]++;
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

    Graph(int V, bool useMatrix, bool directed = false) : V(V), isMatrix(useMatrix), isDirected(directed) {
        if (isMatrix) {
            matrix = new int*[V];
            for (int i = 0; i < V; i++) {
                matrix[i] = new int[V];
                for (int j = 0; j < V; j++) matrix[i][j] = 0;
            }
            adjList = nullptr;
        } else {
            adjList = new Node*[V];
            for (int i = 0; i < V; i++) adjList[i] = nullptr;
            matrix = nullptr;
        }
    }

    ~Graph() {
        if (isMatrix) {
            for (int i = 0; i < V; i++) delete[] matrix[i];
            delete[] matrix;
        } else {
            for (int i = 0; i < V; i++) {
                Node* curr = adjList[i];
                while (curr != nullptr) {
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
            if (!isDirected) matrix[v][u] = w;
        } else {
            Node* newNode = new Node{v, w, adjList[u]};
            adjList[u] = newNode;
            if (!isDirected) {
                Node* newNodeRev = new Node{u, w, adjList[v]};
                adjList[v] = newNodeRev;
            }
        }
    }

    int getWeight(int u, int v) {
        if (isMatrix) return matrix[u][v];
        Node* curr = adjList[u];
        while (curr) {
            if (curr->dest == v) return curr->weight;
            curr = curr->next;
        }
        return 0;
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
                for (int j = 0; j < V; j++) cout << matrix[i][j] << "\t";
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
        Graph* gMatrix = new Graph(V, true, isDirected);
        int* neighbors = new int[V];
        int* weights = new int[V];
        for (int i = 0; i < V; i++) {
            int count = getNeighbors(i, neighbors, weights);
            for (int j = 0; j < count; j++) {
                gMatrix->matrix[i][neighbors[j]] = weights[j];
            }
        }
        delete[] neighbors; delete[] weights;
        return gMatrix;
    }
};

void findConnectedComponents(Graph& g) {
    bool* visited = new bool[g.V];
    for (int i = 0; i < g.V; i++) visited[i] = false;

    int* neighbors = new int[g.V];
    int* weights = new int[g.V];
    int componentCount = 0;

    cout << "Connected Components:\n";
    for (int i = 0; i < g.V; i++) {
        if (!visited[i]) {
            componentCount++;
            cout << "Component " << componentCount << ": ";

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
    delete[] visited; delete[] neighbors; delete[] weights;
}

void BFS(Graph& g, int start, bool sortByWeight) {
    bool* visited = new bool[g.V];
    for (int i = 0; i < g.V; i++) visited[i] = false;

    SimpleQueue q(g.V);
    q.push(start);
    visited[start] = true;

    int* neighbors = new int[g.V];
    int* weights = new int[g.V];

    cout << "BFS Traversal (Sort by weight: " << (sortByWeight ? "Yes" : "No") << "): ";

    while (!q.isEmpty()) {
        int u = q.pop();
        cout << u << " ";

        int count = g.getNeighbors(u, neighbors, weights);

        if (sortByWeight) {
            for (int i = 0; i < count - 1; i++) {
                for (int j = 0; j < count - i - 1; j++) {
                    if (weights[j] > weights[j+1]) {
                        swap(weights[j], weights[j+1]);
                        swap(neighbors[j], neighbors[j+1]);
                    }
                }
            }
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
    delete[] visited; delete[] neighbors; delete[] weights;
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

    for (int countV = 0; countV < g.V - 1; countV++) {
        int min = INF, u = -1;
        for (int v = 0; v < g.V; v++) {
            if (!visited[v] && dist[v] <= min) {
                min = dist[v];
                u = v;
            }
        }
        if (u == -1) break;
        visited[u] = true;

        int countN = g.getNeighbors(u, neighbors, weights);
        for (int j = 0; j < countN; j++) {
            int v = neighbors[j];
            int w = weights[j];
            if (!visited[v] && dist[u] != INF && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
            }
        }
    }

    cout << "Dijkstra distances from vertex " << start << ":\n";
    for (int i = 0; i < g.V; i++) {
        cout << "To " << i << " -> " << (dist[i] == INF ? -1 : dist[i]) << "\n";
    }
    delete[] dist; delete[] visited; delete[] neighbors; delete[] weights;
}

void KahnTopologicalSort(Graph& g) {
    if (!g.isDirected) {
        cout << "Kahn's Algorithm requires a Directed Graph!\n";
        return;
    }
    int* inDegree = new int[g.V];
    for (int i = 0; i < g.V; i++) inDegree[i] = 0;

    int* neighbors = new int[g.V];
    int* weights = new int[g.V];

    for (int u = 0; u < g.V; u++) {
        int count = g.getNeighbors(u, neighbors, weights);
        for (int j = 0; j < count; j++) inDegree[neighbors[j]]++;
    }

    SimpleQueue q(g.V);
    for (int i = 0; i < g.V; i++) {
        if (inDegree[i] == 0) q.push(i);
    }

    int countVisited = 0;
    cout << "Topological Sort (Kahn): ";
    while (!q.isEmpty()) {
        int u = q.pop();
        cout << u << " ";
        countVisited++;

        int countN = g.getNeighbors(u, neighbors, weights);
        for (int j = 0; j < countN; j++) {
            int v = neighbors[j];
            inDegree[v]--;
            if (inDegree[v] == 0) q.push(v);
        }
    }
    if (countVisited != g.V) cout << "\nCycle detected! Not a DAG.";
    cout << "\n";

    delete[] inDegree; delete[] neighbors; delete[] weights;
}

void DFSSpanningTreeHelper(Graph& g, int u, bool* visited, int* neighbors, int* weights, int& totalWeight) {
    visited[u] = true;
    int count = g.getNeighbors(u, neighbors, weights);
    for (int i = 0; i < count; i++) {
        int v = neighbors[i];
        if (!visited[v]) {
            cout << "Edge (" << u << " - " << v << ") weight: " << weights[i] << "\n";
            totalWeight += weights[i];
            DFSSpanningTreeHelper(g, v, visited, neighbors, weights, totalWeight);
        }
    }
}

void DFSSpanningTree(Graph& g, int start) {
    bool* visited = new bool[g.V];
    for (int i = 0; i < g.V; i++) visited[i] = false;
    int* neighbors = new int[g.V];
    int* weights = new int[g.V];
    int totalWeight = 0;

    cout << "DFS Spanning Tree Edges:\n";
    DFSSpanningTreeHelper(g, start, visited, neighbors, weights, totalWeight);
    cout << "Total DFS Spanning Tree Weight: " << totalWeight << "\n";

    delete[] visited; delete[] neighbors; delete[] weights;
}

void KruskalMST(Graph& g) {
    int maxEdges = g.isDirected ? g.V * g.V : (g.V * (g.V - 1)) / 2;
    Edge* edges = new Edge[maxEdges];
    int edgeCount = 0;

    int* neighbors = new int[g.V];
    int* weights = new int[g.V];

    for (int u = 0; u < g.V; u++) {
        int count = g.getNeighbors(u, neighbors, weights);
        for (int i = 0; i < count; i++) {
            int v = neighbors[i];
            if (g.isDirected || u < v) {
                edges[edgeCount++] = {u, v, weights[i]};
            }
        }
    }

    for (int i = 0; i < edgeCount - 1; i++) {
        for (int j = 0; j < edgeCount - i - 1; j++) {
            if (edges[j].w > edges[j+1].w) {
                swap(edges[j], edges[j+1]);
            }
        }
    }

    DSU dsu(g.V);
    int mstWeight = 0;
    cout << "Kruskal MST Edges:\n";

    int edgesTaken = 0;
    for (int i = 0; i < edgeCount; i++) {
        int u = edges[i].u;
        int v = edges[i].v;
        if (dsu.find(u) != dsu.find(v)) {
            dsu.unite(u, v);
            mstWeight += edges[i].w;
            cout << "Edge (" << u << " - " << v << ") weight: " << edges[i].w << "\n";
            edgesTaken++;
            if (edgesTaken == g.V - 1) break;
        }
    }
    cout << "Total Kruskal MST Weight: " << mstWeight << "\n";

    delete[] edges; delete[] neighbors; delete[] weights;
}

void runDemo() {
    cout << "\nDEMO MODE\n";

    cout << "\n1. Undirected Graph (Dijkstra, BFS, Kruskal)\n";
    Graph g1(5, true, false);
    g1.addEdge(0, 1, 2);
    g1.addEdge(0, 3, 6);
    g1.addEdge(1, 2, 3);
    g1.addEdge(1, 3, 8);
    g1.addEdge(1, 4, 5);
    g1.addEdge(2, 4, 7);
    g1.addEdge(3, 4, 9);

    g1.print();
    findConnectedComponents(g1);
    BFS(g1, 0, false);
    BFS(g1, 0, true);
    Dijkstra(g1, 0);
    DFSSpanningTree(g1, 0);
    KruskalMST(g1);

    cout << "\n2. Directed DAG (For Kahn's Algorithm)\n";
    Graph g2(6, false, true);
    g2.addEdge(5, 2);
    g2.addEdge(5, 0);
    g2.addEdge(4, 0);
    g2.addEdge(4, 1);
    g2.addEdge(2, 3);
    g2.addEdge(3, 1);

    g2.print();
    KahnTopologicalSort(g2);
}

void runInteractive() {
    cout << "\nINTERACTIVE MODE\n";
    int V;
    cout << "Enter number of vertices: ";
    cin >> V;
    cout << "Use Matrix (1) or List (0)? ";
    int type; cin >> type;
    cout << "Directed (1) or Undirected (0)? ";
    int dir; cin >> dir;

    Graph g(V, type == 1, dir == 1);

    int edges;
    cout << "Enter number of edges to add: ";
    cin >> edges;
    for (int i = 0; i < edges; i++) {
        int u, v, w;
        cout << "Edge " << i + 1 << " (u v w): ";
        cin >> u >> v >> w;
        g.addEdge(u, v, w);
    }

    int choice;
    do {
        cout << "\n1. Print Graph\n2. BFS\n3. Dijkstra\n4. Kruskal\n5. Kahn (Topological)\n0. Back\nChoice: ";
        cin >> choice;
        if (choice == 1) g.print();
        else if (choice == 2) BFS(g, 0, false);
        else if (choice == 3) Dijkstra(g, 0);
        else if (choice == 4) KruskalMST(g);
        else if (choice == 5) KahnTopologicalSort(g);
    } while (choice != 0);
}

void runBenchmark() {
    cout << "\nBENCHMARK MODE\n";
    int sizes[] = {100, 500};

    for (int V : sizes) {
        cout << "\nBenchmarking Graph Size: " << V << " vertices\n";
        Graph gMat(V, true, false);
        Graph gList(V, false, false);

        int E = V * 5;
        for (int i = 0; i < E; i++) {
            int u = rand() % V;
            int v = rand() % V;
            int w = 1 + rand() % 20;
            if (u != v) {
                gMat.addEdge(u, v, w);
                gList.addEdge(u, v, w);
            }
        }

        auto start = chrono::high_resolution_clock::now();
        Dijkstra(gMat, 0);
        auto end = chrono::high_resolution_clock::now();
        cout << "Matrix Dijkstra Time: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " us\n";

        start = chrono::high_resolution_clock::now();
        Dijkstra(gList, 0);
        end = chrono::high_resolution_clock::now();
        cout << "List Dijkstra Time: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " us\n";
    }
}

int main() {
    srand(time(0));
    int mode;
    do {
        cout << "1. Interactive Mode\n";
        cout << "2. Demo Mode\n";
        cout << "3. Benchmark Mode\n";
        cout << "0. Exit\n";
        cout << "Your choice: ";
        cin >> mode;

        switch (mode) {
            case 1: runInteractive(); break;
            case 2: runDemo(); break;
            case 3: runBenchmark(); break;
            case 0: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice!\n"; break;
        }
    } while (mode != 0);

    return 0;
}