#include <iostream>
#include <cmath>
#include <chrono>
#include <cstdlib>
#include <ctime>

using namespace std;

struct Point3D {
    double x, y, z;
    bool isActive = true;

    double dist() const {
        return sqrt(x * x + y * y + z * z);
    }

    bool operator<(const Point3D& other) const {
        double d1 = dist();
        double d2 = other.dist();
        if (abs(d1 - d2) > 1e-9) return d1 < d2;
        if (abs(x - other.x) > 1e-9) return x < other.x;
        if (abs(y - other.y) > 1e-9) return y < other.y;
        return z < other.z;
    }

    bool operator>(const Point3D& other) const {
        return other < *this;
    }

    bool operator==(const Point3D& other) const {
        return abs(x - other.x) < 1e-9 && abs(y - other.y) < 1e-9 && abs(z - other.z) < 1e-9;
    }
};

void printPoint(const Point3D& p) {
    cout << "(" << p.x << ", " << p.y << ", " << p.z << ") [dist=" << p.dist() << "] ";
}

class OrderedList {
public:
    virtual ~OrderedList() {}
    virtual void insert(Point3D p) = 0;
    virtual void remove(Point3D p) = 0;
    virtual bool search(Point3D p) = 0;
    virtual void searchRange(double minDist, double maxDist) = 0;
    virtual void print() = 0;
    virtual void clear() = 0;
};

class ArrayList : public OrderedList {
    Point3D* arr;
    int capacity;
    int size;

    void resize() {
        capacity = capacity == 0 ? 4 : capacity * 2;
        Point3D* newArr = new Point3D[capacity];
        for (int i = 0; i < size; i++) newArr[i] = arr[i];
        delete[] arr;
        arr = newArr;
    }

public:
    ArrayList() : arr(nullptr), capacity(0), size(0) {}
    ~ArrayList() { delete[] arr; }

    void insert(Point3D p) override {
        if (size == capacity) resize();
        int i = size - 1;
        while (i >= 0 && arr[i] > p) {
            arr[i + 1] = arr[i];
            i--;
        }
        arr[i + 1] = p;
        size++;
    }

    void remove(Point3D p) override {
        int pos = -1;
        for (int i = 0; i < size; i++) {
            if (arr[i] == p) { pos = i; break; }
        }
        if (pos != -1) {
            for (int i = pos; i < size - 1; i++) arr[i] = arr[i + 1];
            size--;
        }
    }

    bool search(Point3D p) override {
        int l = 0, r = size - 1;
        while (l <= r) {
            int m = l + (r - l) / 2;
            if (arr[m] == p) return true;
            if (arr[m] < p) l = m + 1;
            else r = m - 1;
        }
        return false;
    }

    void searchRange(double minDist, double maxDist) override {
        for (int i = 0; i < size; i++) {
            double d = arr[i].dist();
            if (d >= minDist && d <= maxDist) printPoint(arr[i]);
        }
        cout << "\n";
    }

    void print() override {
        for (int i = 0; i < size; i++) printPoint(arr[i]);
        cout << "\n";
    }

    void clear() override {
        delete[] arr;
        arr = nullptr;
        capacity = 0; size = 0;
    }
};

class LinkedList : public OrderedList {
    struct Node {
        Point3D data;
        Node* next;
        Node(Point3D d) : data(d), next(nullptr) {}
    };
    Node* head;

public:
    LinkedList() : head(nullptr) {}
    ~LinkedList() { clear(); }

    void insert(Point3D p) override {
        Node* newNode = new Node(p);
        if (!head || head->data > p) {
            newNode->next = head;
            head = newNode;
            return;
        }
        Node* curr = head;
        while (curr->next && curr->next->data < p) curr = curr->next;
        newNode->next = curr->next;
        curr->next = newNode;
    }

    void remove(Point3D p) override {
        if (!head) return;
        if (head->data == p) {
            Node* temp = head;
            head = head->next;
            delete temp;
            return;
        }
        Node* curr = head;
        while (curr->next && !(curr->next->data == p)) curr = curr->next;
        if (curr->next) {
            Node* temp = curr->next;
            curr->next = curr->next->next;
            delete temp;
        }
    }

    bool search(Point3D p) override {
        Node* curr = head;
        while (curr) {
            if (curr->data == p) return true;
            if (curr->data > p) return false;
            curr = curr->next;
        }
        return false;
    }

    void searchRange(double minDist, double maxDist) override {
        Node* curr = head;
        while (curr) {
            double d = curr->data.dist();
            if (d >= minDist && d <= maxDist) printPoint(curr->data);
            if (d > maxDist) break;
            curr = curr->next;
        }
        cout << "\n";
    }

    void print() override {
        Node* curr = head;
        while (curr) { printPoint(curr->data); curr = curr->next; }
        cout << "\n";
    }

    void clear() override {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
};

class BST : public OrderedList {
protected:
    struct Node {
        Point3D data;
        Node* left;
        Node* right;
        Node(Point3D d) : data(d), left(nullptr), right(nullptr) {}
    };
    Node* root;

    Node* insertRec(Node* node, Point3D p) {
        if (!node) return new Node(p);
        if (p < node->data) node->left = insertRec(node->left, p);
        else if (p > node->data) node->right = insertRec(node->right, p);
        return node;
    }

    Node* findMin(Node* node) {
        while (node && node->left != nullptr) node = node->left;
        return node;
    }

    Node* removeRec(Node* node, Point3D p) {
        if (!node) return node;
        if (p < node->data) node->left = removeRec(node->left, p);
        else if (p > node->data) node->right = removeRec(node->right, p);
        else {
            if (!node->left) { Node* temp = node->right; delete node; return temp; }
            else if (!node->right) { Node* temp = node->left; delete node; return temp; }
            Node* temp = findMin(node->right);
            node->data = temp->data;
            node->right = removeRec(node->right, temp->data);
        }
        return node;
    }

    bool searchRec(Node* node, Point3D p) {
        if (!node) return false;
        if (node->data == p) return true;
        if (p < node->data) return searchRec(node->left, p);
        return searchRec(node->right, p);
    }

    void rangeRec(Node* node, double minD, double maxD) {
        if (!node) return;
        double d = node->data.dist();
        if (d > minD) rangeRec(node->left, minD, maxD);
        if (d >= minD && d <= maxD) printPoint(node->data);
        if (d < maxD) rangeRec(node->right, minD, maxD);
    }

    void printRec(Node* node) {
        if (!node) return;
        printRec(node->left);
        printPoint(node->data);
        printRec(node->right);
    }

    void clearRec(Node* node) {
        if (!node) return;
        clearRec(node->left);
        clearRec(node->right);
        delete node;
    }

public:
    BST() : root(nullptr) {}
    ~BST() { clear(); }
    void insert(Point3D p) override { root = insertRec(root, p); }
    void remove(Point3D p) override { root = removeRec(root, p); }
    bool search(Point3D p) override { return searchRec(root, p); }
    void searchRange(double minD, double maxD) override { rangeRec(root, minD, maxD); cout << "\n"; }
    void print() override { printRec(root); cout << "\n"; }
    void clear() override { clearRec(root); root = nullptr; }
};

class AVLTree : public OrderedList {
    struct Node {
        Point3D data;
        Node* left;
        Node* right;
        int height;
        Node(Point3D d) : data(d), left(nullptr), right(nullptr), height(1) {}
    };
    Node* root;

    int height(Node* N) { return N ? N->height : 0; }
    int getBalance(Node* N) { return N ? height(N->left) - height(N->right) : 0; }

    Node* rightRotate(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;
        x->right = y;
        y->left = T2;
        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;
        return x;
    }

    Node* leftRotate(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;
        y->left = x;
        x->right = T2;
        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;
        return y;
    }

    Node* insertRec(Node* node, Point3D p) {
        if (!node) return new Node(p);
        if (p < node->data) node->left = insertRec(node->left, p);
        else if (p > node->data) node->right = insertRec(node->right, p);
        else return node;

        node->height = 1 + max(height(node->left), height(node->right));
        int balance = getBalance(node);

        if (balance > 1 && p < node->left->data) return rightRotate(node);
        if (balance < -1 && p > node->right->data) return leftRotate(node);
        if (balance > 1 && p > node->left->data) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        if (balance < -1 && p < node->right->data) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
        return node;
    }

    Node* findMin(Node* node) {
        while (node->left != nullptr) node = node->left;
        return node;
    }

    Node* removeRec(Node* root, Point3D p) {
        if (!root) return root;
        if (p < root->data) root->left = removeRec(root->left, p);
        else if (p > root->data) root->right = removeRec(root->right, p);
        else {
            if (!root->left || !root->right) {
                Node* temp = root->left ? root->left : root->right;
                if (!temp) { temp = root; root = nullptr; }
                else *root = *temp;
                delete temp;
            } else {
                Node* temp = findMin(root->right);
                root->data = temp->data;
                root->right = removeRec(root->right, temp->data);
            }
        }
        if (!root) return root;

        root->height = 1 + max(height(root->left), height(root->right));
        int balance = getBalance(root);

        if (balance > 1 && getBalance(root->left) >= 0) return rightRotate(root);
        if (balance > 1 && getBalance(root->left) < 0) {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }
        if (balance < -1 && getBalance(root->right) <= 0) return leftRotate(root);
        if (balance < -1 && getBalance(root->right) > 0) {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }
        return root;
    }

    bool searchRec(Node* node, Point3D p) {
        if (!node) return false;
        if (node->data == p) return true;
        if (p < node->data) return searchRec(node->left, p);
        return searchRec(node->right, p);
    }

    void rangeRec(Node* node, double minD, double maxD) {
        if (!node) return;
        double d = node->data.dist();
        if (d > minD) rangeRec(node->left, minD, maxD);
        if (d >= minD && d <= maxD) printPoint(node->data);
        if (d < maxD) rangeRec(node->right, minD, maxD);
    }

    void printRec(Node* node) {
        if (!node) return;
        printRec(node->left);
        printPoint(node->data);
        printRec(node->right);
    }

    void clearRec(Node* node) {
        if (!node) return;
        clearRec(node->left);
        clearRec(node->right);
        delete node;
    }

public:
    AVLTree() : root(nullptr) {}
    ~AVLTree() { clear(); }
    void insert(Point3D p) override { root = insertRec(root, p); }
    void remove(Point3D p) override { root = removeRec(root, p); }
    bool search(Point3D p) override { return searchRec(root, p); }
    void searchRange(double minD, double maxD) override { rangeRec(root, minD, maxD); cout << "\n"; }
    void print() override { printRec(root); cout << "\n"; }
    void clear() override { clearRec(root); root = nullptr; }
};

class TwoThreeTree : public OrderedList {
    struct Node {
        int n;
        Point3D keys[3];
        Node* child[4];
        bool leaf;
        Node(bool leaf) : n(0), leaf(leaf) {
            for (int i = 0; i < 4; i++) child[i] = nullptr;
        }
    };
    Node* root;

    void splitChild(Node* parent, int i, Node* y) {
        Node* z = new Node(y->leaf);
        z->n = 1;
        z->keys[0] = y->keys[2];
        if (!y->leaf) {
            z->child[0] = y->child[2];
            z->child[1] = y->child[3];
        }
        y->n = 1;
        for (int j = parent->n; j >= i + 1; j--) parent->child[j + 1] = parent->child[j];
        parent->child[i + 1] = z;
        for (int j = parent->n - 1; j >= i; j--) parent->keys[j + 1] = parent->keys[j];
        parent->keys[i] = y->keys[1];
        parent->n++;
    }

    void insertNonFull(Node* x, Point3D k) {
        int i = x->n - 1;
        if (x->leaf) {
            while (i >= 0 && x->keys[i] > k) {
                x->keys[i + 1] = x->keys[i];
                i--;
            }
            x->keys[i + 1] = k;
            x->n++;
        } else {
            while (i >= 0 && x->keys[i] > k) i--;
            i++;
            if (x->child[i]->n == 2) {
                x->child[i]->keys[2] = k;
                splitChild(x, i, x->child[i]);
                if (x->keys[i] < k) i++;
            }
            insertNonFull(x->child[i], k);
        }
    }

    Node* searchRec(Node* x, Point3D k) {
        int i = 0;
        while (i < x->n && k > x->keys[i]) i++;
        if (i < x->n && k == x->keys[i] && x->keys[i].isActive) return x;
        if (x->leaf) return nullptr;
        return searchRec(x->child[i], k);
    }

    void printRec(Node* x) {
        if (!x) return;
        int i;
        for (i = 0; i < x->n; i++) {
            if (!x->leaf) printRec(x->child[i]);
            if (x->keys[i].isActive) printPoint(x->keys[i]);
        }
        if (!x->leaf) printRec(x->child[i]);
    }

    void rangeRec(Node* x, double minD, double maxD) {
        if (!x) return;
        int i;
        for (i = 0; i < x->n; i++) {
            if (!x->leaf) rangeRec(x->child[i], minD, maxD);
            double d = x->keys[i].dist();
            if (x->keys[i].isActive && d >= minD && d <= maxD) printPoint(x->keys[i]);
        }
        if (!x->leaf) rangeRec(x->child[i], minD, maxD);
    }

    void removeLazy(Node* x, Point3D k) {
        int i = 0;
        while (i < x->n && k > x->keys[i]) i++;
        if (i < x->n && k == x->keys[i]) { x->keys[i].isActive = false; return; }
        if (!x->leaf) removeLazy(x->child[i], k);
    }

    void clearRec(Node* x) {
        if (!x) return;
        if (!x->leaf) {
            for (int i = 0; i <= x->n; i++) clearRec(x->child[i]);
        }
        delete x;
    }

public:
    TwoThreeTree() : root(nullptr) {}
    ~TwoThreeTree() { clear(); }

    void insert(Point3D k) override {
        if (!root) {
            root = new Node(true);
            root->keys[0] = k;
            root->n = 1;
        } else {
            if (root->n == 2) {
                Node* s = new Node(false);
                s->child[0] = root;
                splitChild(s, 0, root);
                int i = 0;
                if (s->keys[0] < k) i++;
                insertNonFull(s->child[i], k);
                root = s;
            } else {
                insertNonFull(root, k);
            }
        }
    }

    void remove(Point3D p) override {
        if (root) removeLazy(root, p);
    }

    bool search(Point3D p) override {
        return searchRec(root, p) != nullptr;
    }

    void searchRange(double minD, double maxD) override {
        rangeRec(root, minD, maxD); cout << "\n";
    }

    void print() override {
        printRec(root); cout << "\n";
    }

    void clear() override {
        clearRec(root); root = nullptr;
    }
};

OrderedList* createStructure(int choice) {
    switch (choice) {
        case 1: return new ArrayList();
        case 2: return new LinkedList();
        case 3: return new BST();
        case 4: return new AVLTree();
        case 5: return new TwoThreeTree();
        default: return nullptr;
    }
}

void runInteractive() {
    cout << "\nInteractive Mode\n";
    cout << "Choose data structure:\n1. Array List\n2. Linked List\n3. BST\n4. AVL Tree\n5. 2-3 Tree\nChoice: ";
    int structChoice; cin >> structChoice;

    OrderedList* list = createStructure(structChoice);
    if (!list) return;

    int op;
    do {
        cout << "\nOperations:\n1. Add point\n2. Remove point\n3. Find point\n4. Search in range (by distance)\n5. Print all\n0. Exit\nChoice: ";
        cin >> op;
        if (op >= 1 && op <= 3) {
            Point3D p;
            cout << "Enter coordinates x y z: ";
            cin >> p.x >> p.y >> p.z;
            if (op == 1) list->insert(p);
            else if (op == 2) list->remove(p);
            else cout << (list->search(p) ? "Found!\n" : "Not found!\n");
        } else if (op == 4) {
            double minD, maxD;
            cout << "Enter min and max distance: ";
            cin >> minD >> maxD;
            list->searchRange(minD, maxD);
        } else if (op == 5) {
            list->print();
        }
    } while (op != 0);
    delete list;
}

void runDemo() {
    cout << "\nDemo Mode (AVL Tree)\n";
    OrderedList* list = new AVLTree();

    Point3D points[] = {
        {1, 1, 1}, {10, 0, 0}, {3, 4, 0}, {0, 0, 0}, {2, 2, 2}
    };

    for (int i = 0; i < 5; i++) {
        cout << "Adding: "; printPoint(points[i]); cout << "\n";
        list->insert(points[i]);
    }

    cout << "\nAll points (sorted by distance):\n";
    list->print();

    cout << "\nRange search (distance from 2 to 6):\n";
    list->searchRange(2.0, 6.0);

    cout << "\nRemoving point (3, 4, 0) [dist=5]\n";
    list->remove({3, 4, 0});
    list->print();

    delete list;
}

void runBenchmark() {
    cout << "\nBenchmark Mode\n";
    const int N = 10000;
    Point3D* randomPoints = new Point3D[N];
    Point3D* sortedPoints = new Point3D[N];
    Point3D* reversePoints = new Point3D[N];

    for (int i = 0; i < N; i++) {
        randomPoints[i] = {(double)(rand() % 100), (double)(rand() % 100), (double)(rand() % 100)};
        sortedPoints[i] = {(double)i, 0, 0};
        reversePoints[i] = {(double)(N - i), 0, 0};
    }

    string names[] = {"ArrayList", "LinkedList", "BST", "AVL Tree", "2-3 Tree"};

    for (int s = 1; s <= 5; s++) {
        cout << "\nTesting " << names[s-1] << " (N = " << N << ")...\n";

        auto measure = [&](Point3D* arr, string label) {
            OrderedList* list = createStructure(s);
            auto start = chrono::high_resolution_clock::now();
            for (int i = 0; i < N; i++) list->insert(arr[i]);
            auto end = chrono::high_resolution_clock::now();
            cout << label << ": " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms\n";
            delete list;
        };

        measure(randomPoints, "  Random data");
        measure(sortedPoints, "  Nearly sorted (ascending)");
        measure(reversePoints, "  Nearly sorted (descending)");
    }

    delete[] randomPoints;
    delete[] sortedPoints;
    delete[] reversePoints;
}

int main() {
    srand(time(0));
    int mode;
    do {
        cout << "1. Interactive Mode\n";
        cout << "2. Demo Mode\n";
        cout << "3. Benchmark Mode\n";
        cout << "0. Exit\n";
        cout << "Choice: ";
        cin >> mode;

        switch (mode) {
            case 1: runInteractive(); break;
            case 2: runDemo(); break;
            case 3: runBenchmark(); break;
            case 0: cout << "Exiting.\n"; break;
            default: cout << "Invalid choice!\n"; break;
        }
    } while (mode != 0);

    return 0;
}