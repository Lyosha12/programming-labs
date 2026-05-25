#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

struct NaryNode {
    int data;
    NaryNode** children;
    int childCount;
    int childCapacity;

    NaryNode(int val) : data(val), children(nullptr), childCount(0), childCapacity(0) {}

    ~NaryNode() {
        for (int i = 0; i < childCount; ++i) {
            delete children[i];
        }
        delete[] children;
    }

    void addChild(NaryNode* child) {
        if (childCount == childCapacity) {
            childCapacity = (childCapacity == 0) ? 2 : childCapacity * 2;
            NaryNode** newChildren = new NaryNode*[childCapacity];
            for (int i = 0; i < childCount; ++i) {
                newChildren[i] = children[i];
            }
            delete[] children;
            children = newChildren;
        }
        children[childCount++] = child;
    }

    void removeChildAtIndex(int index) {
        if (index < 0 || index >= childCount) return;
        for (int i = index; i < childCount - 1; ++i) {
            children[i] = children[i + 1];
        }
        childCount--;
    }
};

class NaryTree {
public:
    NaryNode* root;

    NaryTree() : root(nullptr) {}
    ~NaryTree() { delete root; }

    void addProb(int value, double p) {
        if (!root) {
            root = new NaryNode(value);
            return;
        }
        addProbRecursive(root, value, p);
    }

    void print() {
        if (!root) {
            cout << "Tree is empty.\n";
            return;
        }
        printRecursive(root, 0);
    }

    NaryNode* deleteByValue(int value) {
        if (!root) return nullptr;
        if (root->data == value) {
            NaryNode* temp = root;
            root = nullptr;
            return temp;
        }
        return deleteRecursive(root, value);
    }

private:
    void addProbRecursive(NaryNode* current, int value, double p) {
        double r = (double)rand() / RAND_MAX;
        if (r <= p || current->childCount == 0) {
            current->addChild(new NaryNode(value));
        } else {
            int childIdx = rand() % current->childCount;
            addProbRecursive(current->children[childIdx], value, p);
        }
    }

    void printRecursive(NaryNode* node, int depth) {
        for (int i = 0; i < depth; ++i) cout << "    ";
        cout << "- " << node->data << "\n";
        for (int i = 0; i < node->childCount; ++i) {
            printRecursive(node->children[i], depth + 1);
        }
    }

    NaryNode* deleteRecursive(NaryNode* parent, int value) {
        for (int i = 0; i < parent->childCount; ++i) {
            if (parent->children[i]->data == value) {
                NaryNode* removedNode = parent->children[i];
                parent->removeChildAtIndex(i);
                return removedNode;
            }
            NaryNode* result = deleteRecursive(parent->children[i], value);
            if (result) return result;
        }
        return nullptr;
    }
};

struct BSTNode {
    int data;
    BSTNode* left;
    BSTNode* right;

    BSTNode(int val) : data(val), left(nullptr), right(nullptr) {}
    ~BSTNode() {
        delete left;
        delete right;
    }
};

class BST {
public:
    BSTNode* root;

    BST() : root(nullptr) {}
    ~BST() { delete root; }

    void insert(int value) {
        root = insertRecursive(root, value);
    }

    void print() {
        if (!root) {
            cout << "Tree is empty.\n";
            return;
        }
        printRecursive(root, 0);
    }

    int* toPreOrderArray(int& outSize) {
        outSize = countNodes(root);
        if (outSize == 0) return nullptr;

        int* arr = new int[outSize];
        int currentIndex = 0;
        preOrderRecursive(root, arr, currentIndex);
        return arr;
    }

private:
    BSTNode* insertRecursive(BSTNode* node, int value) {
        if (!node) return new BSTNode(value);
        if (value < node->data) {
            node->left = insertRecursive(node->left, value);
        } else {
            node->right = insertRecursive(node->right, value);
        }
        return node;
    }

    void printRecursive(BSTNode* node, int depth) {
        if (!node) return;
        printRecursive(node->right, depth + 1);
        for (int i = 0; i < depth; ++i) cout << "    ";
        cout << node->data << "\n";
        printRecursive(node->left, depth + 1);
    }

    int countNodes(BSTNode* node) {
        if (!node) return 0;
        return 1 + countNodes(node->left) + countNodes(node->right);
    }

    void preOrderRecursive(BSTNode* node, int* arr, int& index) {
        if (!node) return;
        arr[index++] = node->data;
        preOrderRecursive(node->left, arr, index);
        preOrderRecursive(node->right, arr, index);
    }
};

struct FSNode {
    string name;
    bool isFolder;
    int size;
    time_t timeModified;

    FSNode** children;
    int childCount;
    int childCapacity;

    FSNode(string n, bool folder, int s) : name(n), isFolder(folder), size(s),
                                           children(nullptr), childCount(0), childCapacity(0) {
        timeModified = time(0) - (rand() % 1000000);
    }

    ~FSNode() {
        for (int i = 0; i < childCount; ++i) {
            delete children[i];
        }
        delete[] children;
    }

    void addChild(FSNode* child) {
        if (childCount == childCapacity) {
            childCapacity = (childCapacity == 0) ? 2 : childCapacity * 2;
            FSNode** newChildren = new FSNode*[childCapacity];
            for (int i = 0; i < childCount; ++i) {
                newChildren[i] = children[i];
            }
            delete[] children;
            children = newChildren;
        }
        children[childCount++] = child;
    }
};

class FileSystem {
public:
    FSNode* root;

    FileSystem(string rootName) {
        root = new FSNode(rootName, true, 0);
    }
    ~FileSystem() { delete root; }

    void addNode(FSNode* parent, string name, bool isFolder, int size = 0) {
        parent->addChild(new FSNode(name, isFolder, size));
    }

    int getTotalSize(FSNode* node) {
        if (!node) return 0;
        int total = node->size;
        for (int i = 0; i < node->childCount; ++i) {
            total += getTotalSize(node->children[i]);
        }
        return total;
    }

    void countFilesFolders(FSNode* node, int& files, int& folders) {
        if (!node) return;
        if (node->isFolder) folders++;
        else files++;
        for (int i = 0; i < node->childCount; ++i) {
            countFilesFolders(node->children[i], files, folders);
        }
    }

    void print(FSNode* node, int depth = 0) {
        if (!node) return;
        for (int i = 0; i < depth; ++i) cout << "    ";
        cout << (node->isFolder ? "[D] " : "[F] ") << node->name
             << " (" << getTotalSize(node) << " B)\n";
        for (int i = 0; i < node->childCount; ++i) {
            print(node->children[i], depth + 1);
        }
    }

    FSNode* filterTree(FSNode* node, bool (*criterion)(FSNode*)) {
        if (!node) return nullptr;
        FSNode* copy = new FSNode(node->name, node->isFolder, node->size);
        copy->timeModified = node->timeModified;

        for (int i = 0; i < node->childCount; ++i) {
            FSNode* filteredChild = filterTree(node->children[i], criterion);
            if (filteredChild) {
                copy->addChild(filteredChild);
            }
        }

        if (criterion(node) || copy->childCount > 0) {
            return copy;
        } else {
            delete copy;
            return nullptr;
        }
    }
};

bool filterBySize(FSNode* node) {
    return node->size > 1000;
}

void runDemo() {
    cout << "\nDEMO MODE\n";

    cout << "\n[Block 0-3: N-ary Tree]\n";
    NaryTree nTree;
    cout << "Adding root (10) and 5 random elements with p=0.6...\n";
    nTree.addProb(10, 1.0);
    for (int i = 1; i <= 5; ++i) nTree.addProb(rand() % 50, 0.6);
    nTree.print();

    if (nTree.root && nTree.root->childCount > 0) {
        int valToDelete = nTree.root->children[0]->data;
        cout << "\nAttempting to delete node with value: " << valToDelete << "\n";
        NaryNode* deleted = nTree.deleteByValue(valToDelete);
        if (deleted) {
            cout << "Successfully deleted. Tree after deletion:\n";
            nTree.print();
            delete deleted;
        }
    }

    cout << "\n[Block 4-5: Binary Search Tree]\n";
    BST bst;
    int bstVals[] = {15, 10, 20, 8, 12, 17, 25};
    cout << "Adding elements: 15, 10, 20, 8, 12, 17, 25\n";
    for(int i = 0; i < 7; ++i) bst.insert(bstVals[i]);
    bst.print();

    cout << "Pre-order array: ";
    int arrSize = 0;
    int* preOrderArr = bst.toPreOrderArray(arrSize);
    if (preOrderArr) {
        for (int i = 0; i < arrSize; ++i) cout << preOrderArr[i] << " ";
        cout << "\n";
        delete[] preOrderArr;
    }

    cout << "\n[Block 6: File System]\n";
    FileSystem fs("Root");
    fs.addNode(fs.root, "Docs", true);
    fs.addNode(fs.root, "Photos", true);
    fs.addNode(fs.root->children[0], "report.pdf", false, 1500);
    fs.addNode(fs.root->children[0], "notes.txt", false, 200);
    fs.addNode(fs.root->children[1], "img1.png", false, 3500);

    fs.print(fs.root);

    int files = 0, folders = 0;
    fs.countFilesFolders(fs.root, files, folders);
    cout << "Total: " << files << " files, " << folders << " folders.\n";

    cout << "\nFiltering: only files/folders with size > 1000 B:\n";

    FSNode* filtered = fs.filterTree(fs.root, filterBySize);
    fs.print(filtered);
    delete filtered;
}

void runInteractive() {
    cout << "\nINTERACTIVE MODE\n";
    int choice;
    do {
        cout << "\n1. N-ary Tree (Blocks 0-3)\n";
        cout << "2. BST (Blocks 4-5)\n";
        cout << "3. File System (Block 6)\n";
        cout << "0. Return to main menu\n";
        cout << "Your choice: ";
        cin >> choice;

        if (choice == 1) {
            NaryTree nTree;
            int nChoice;
            do {
                cout << "\n[N-ary Tree] 1-Add, 2-Print, 3-Delete, 0-Back: ";
                cin >> nChoice;
                if (nChoice == 1) {
                    int val; double p;
                    cout << "Value: "; cin >> val;
                    cout << "Probability p (0.0 - 1.0): "; cin >> p;
                    nTree.addProb(val, p);
                } else if (nChoice == 2) {
                    nTree.print();
                } else if (nChoice == 3) {
                    int val; cout << "Delete value: "; cin >> val;
                    NaryNode* del = nTree.deleteByValue(val);
                    if (del) { cout << "Deleted.\n"; delete del; }
                    else { cout << "Not found.\n"; }
                }
            } while (nChoice != 0);
        } else if (choice == 2) {
            BST bst;
            int bChoice;
            do {
                cout << "\n[BST] 1-Add, 2-Print, 3-Pre-order array, 0-Back: ";
                cin >> bChoice;
                if (bChoice == 1) {
                    int val; cout << "Value: "; cin >> val;
                    bst.insert(val);
                } else if (bChoice == 2) {
                    bst.print();
                } else if (bChoice == 3) {
                    int arrSize = 0;
                    int* arr = bst.toPreOrderArray(arrSize);
                    cout << "Array (" << arrSize << " elements): ";
                    for(int i = 0; i < arrSize; ++i) cout << arr[i] << " ";
                    cout << "\n";
                    delete[] arr;
                }
            } while (bChoice != 0);
        } else if (choice == 3) {
            cout << "File system has a default structure for testing.\n";
            FileSystem fs("MyPC");
            fs.addNode(fs.root, "Games", true);
            fs.addNode(fs.root->children[0], "doom.exe", false, 50000);
            fs.print(fs.root);
            cout << "For detailed tests, see Demo Mode.\n";
        }
    } while (choice != 0);
}

int main() {
    srand(time(0));
    int mode;
    do {
        cout << "\nChoose an option:\n";
        cout << "1. Interactive Mode\n";
        cout << "2. Demo Mode\n";
        cout << "0. Exit\n";
        cout << "Your choice: ";
        cin >> mode;

        switch (mode) {
            case 1: runInteractive(); break;
            case 2: runDemo(); break;
            case 0: cout << "Program terminated.\n"; break;
            default: cout << "Invalid choice.\n"; break;
        }
    } while (mode != 0);

    return 0;
}