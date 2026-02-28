#include <iostream>
#include <string>
#include <stdexcept>
#include <chrono>

using namespace std;

class IPList {
public:
    virtual ~IPList() {}
    virtual void append(const string& ip) = 0;
    virtual void insert(int index, const string& ip) = 0;
    virtual void remove(int index) = 0;
    virtual string get(int index) const = 0;
    virtual void set(int index, const string& ip) = 0;
    virtual int length() const = 0;
};

class FixedArrayList : public IPList {
private:
    static const int CAPACITY = 20000;
    string arr[CAPACITY];
    int count;

public:
    FixedArrayList() : count(0) {}

    void append(const string& ip) override {
        if (count >= CAPACITY) throw overflow_error("Fixed array list is full.");
        arr[count++] = ip;
    }

    void insert(int index, const string& ip) override {
        if (index < 0 || index > count) throw out_of_range("Invalid index.");
        if (count >= CAPACITY) throw overflow_error("Fixed array list is full.");
        for (int i = count; i > index; --i) {
            arr[i] = arr[i - 1];
        }
        arr[index] = ip;
        count++;
    }

    void remove(int index) override {
        if (index < 0 || index >= count) throw out_of_range("Invalid index.");
        for (int i = index; i < count - 1; ++i) {
            arr[i] = arr[i + 1];
        }
        count--;
    }

    string get(int index) const override {
        if (index < 0 || index >= count) throw out_of_range("Invalid index.");
        return arr[index];
    }

    void set(int index, const string& ip) override {
        if (index < 0 || index >= count) throw out_of_range("Invalid index.");
        arr[index] = ip;
    }

    int length() const override {
        return count;
    }
};

class DynamicArrayList : public IPList {
private:
    string* arr;
    int capacity;
    int count;

    void resize(int newCapacity) {
        string* newArr = new string[newCapacity];
        for (int i = 0; i < count; ++i) {
            newArr[i] = arr[i];
        }
        delete[] arr;
        arr = newArr;
        capacity = newCapacity;
    }

public:
    DynamicArrayList() : capacity(2), count(0) {
        arr = new string[capacity];
    }

    ~DynamicArrayList() {
        delete[] arr;
    }

    void append(const string& ip) override {
        if (count == capacity) resize(capacity * 2);
        arr[count++] = ip;
    }

    void insert(int index, const string& ip) override {
        if (index < 0 || index > count) throw out_of_range("Invalid index.");
        if (count == capacity) resize(capacity * 2);
        for (int i = count; i > index; --i) {
            arr[i] = arr[i - 1];
        }
        arr[index] = ip;
        count++;
    }

    void remove(int index) override {
        if (index < 0 || index >= count) throw out_of_range("Invalid index.");
        for (int i = index; i < count - 1; ++i) {
            arr[i] = arr[i + 1];
        }
        count--;
    }

    string get(int index) const override {
        if (index < 0 || index >= count) throw out_of_range("Invalid index.");
        return arr[index];
    }

    void set(int index, const string& ip) override {
        if (index < 0 || index >= count) throw out_of_range("Invalid index.");
        arr[index] = ip;
    }

    int length() const override {
        return count;
    }
};

class LinkedList : public IPList {
private:
    struct Node {
        string ip;
        Node* next;
        Node(string val) : ip(val), next(nullptr) {}
    };
    Node* head;
    int count;

public:
    LinkedList() : head(nullptr), count(0) {}

    ~LinkedList() {
        Node* curr = head;
        while (curr) {
            Node* next = curr->next;
            delete curr;
            curr = next;
        }
    }

    void append(const string& ip) override {
        Node* newNode = new Node(ip);
        if (!head) {
            head = newNode;
        } else {
            Node* temp = head;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
        count++;
    }

    void insert(int index, const string& ip) override {
        if (index < 0 || index > count) throw out_of_range("Invalid index.");
        Node* newNode = new Node(ip);
        if (index == 0) {
            newNode->next = head;
            head = newNode;
        } else {
            Node* temp = head;
            for (int i = 0; i < index - 1; ++i) {
                temp = temp->next;
            }
            newNode->next = temp->next;
            temp->next = newNode;
        }
        count++;
    }

    void remove(int index) override {
        if (index < 0 || index >= count) throw out_of_range("Invalid index.");
        if (index == 0) {
            Node* temp = head;
            head = head->next;
            delete temp;
        } else {
            Node* temp = head;
            for (int i = 0; i < index - 1; ++i) {
                temp = temp->next;
            }
            Node* toDelete = temp->next;
            temp->next = toDelete->next;
            delete toDelete;
        }
        count--;
    }

    string get(int index) const override {
        if (index < 0 || index >= count) throw out_of_range("Invalid index.");
        Node* temp = head;
        for (int i = 0; i < index; ++i) {
            temp = temp->next;
        }
        return temp->ip;
    }

    void set(int index, const string& ip) override {
        if (index < 0 || index >= count) throw out_of_range("Invalid index.");
        Node* temp = head;
        for (int i = 0; i < index; ++i) {
            temp = temp->next;
        }
        temp->ip = ip;
    }

    int length() const override {
        return count;
    }
};

void printList(const IPList& list) {
    cout << "List (" << list.length() << " items): ";
    for (int i = 0; i < list.length(); ++i) {
        cout << list.get(i) << " ";
    }
    cout << "\n";
}

class PriorityQueue {
private:
    struct PQNode {
        string ip;
        int priority;
        PQNode* next;

        PQNode(string val, int p) : ip(val), priority(p), next(nullptr) {}
    };

    PQNode* head;
    int count;

public:
    PriorityQueue() : head(nullptr), count(0) {}

    ~PriorityQueue() {
        PQNode* curr = head;
        while (curr) {
            PQNode* next = curr->next;
            delete curr;
            curr = next;
        }
    }

    void enqueue(const string& ip, int priority) {
        PQNode* newNode = new PQNode(ip, priority);

        if (head == nullptr || priority < head->priority) {
            newNode->next = head;
            head = newNode;
        } else {
            PQNode* current = head;
            while (current->next != nullptr && current->next->priority <= priority) {
                current = current->next;
            }
            newNode->next = current->next;
            current->next = newNode;
        }
        count++;
    }

    string dequeue() {
        if (head == nullptr) {
            throw runtime_error("Error: Queue is empty.");
        }

        PQNode* temp = head;
        string highestPriorityIP = temp->ip;

        head = head->next;
        delete temp;
        count--;

        return highestPriorityIP;
    }

    bool isEmpty() const {
        return head == nullptr;
    }

    void printQueue() const {
        cout << "Priority Queue (" << count << " items):\n";
        PQNode* temp = head;
        while (temp != nullptr) {
            cout << "Priority " << temp->priority << ", IP: " << temp->ip << "\n";
            temp = temp->next;
        }
        if (count == 0) cout << "Empty\n";
    }
};

void runInteractive() {
    cout << "\nInteractive Mode\n";
    cout << "Choose a data structure:\n";
    cout << "1. Fixed-size Array\n";
    cout << "2. Dynamic Array\n";
    cout << "3. Linked List\n";
    cout << "4. Priority Queue\n";
    cout << "Choice: ";

    int choice;
    cin >> choice;

    if (choice >= 1 && choice <= 3) {
        IPList* list = nullptr;
        if (choice == 1) list = new FixedArrayList();
        else if (choice == 2) list = new DynamicArrayList();
        else if (choice == 3) list = new LinkedList();

        while (true) {
            cout << "\n1: append, 2: insert, 3: remove, 4: get, 5: set, 6: length, 7: print, 0: exit\nCommand: ";
            int op;
            cin >> op;

            if (op == 0) break;

            try {
                if (op == 1) {
                    string ip;
                    cout << "Enter IP: ";
                    cin >> ip;
                    list->append(ip);
                }
                else if (op == 2) {
                    int idx; string ip;
                    cout << "Index and IP: ";
                    cin >> idx >> ip;
                    list->insert(idx, ip);
                }
                else if (op == 3) {
                    int idx;
                    cout << "Index: ";
                    cin >> idx;
                    list->remove(idx);
                }
                else if (op == 4) {
                    int idx;
                    cout << "Index: ";
                    cin >> idx;
                    cout << "Value: " << list->get(idx) << "\n";
                }
                else if (op == 5) {
                    int idx; string ip;
                    cout << "Index and IP: ";
                    cin >> idx >> ip;
                    list->set(idx, ip);
                }
                else if (op == 6) {
                    cout << "Length: " << list->length() << "\n";
                }
                else if (op == 7) {
                    printList(*list);
                }
            } catch (const exception& e) {
                cout << "Error: " << e.what() << "\n";
            }
        }
        delete list;
    }
    else if (choice == 4) {
        PriorityQueue pq;
        while (true) {
            cout << "\n1: enqueue, 2: dequeue, 3: print queue, 0: exit\nCommand: ";
            int op;
            cin >> op;

            if (op == 0) break;

            try {
                if (op == 1) {
                    string ip; int p;
                    cout << "Enter IP and Priority (lower number = higher priority): ";
                    cin >> ip >> p;
                    pq.enqueue(ip, p);
                }
                else if (op == 2) {
                    cout << "Dequeued: " << pq.dequeue() << "\n";
                }
                else if (op == 3) {
                    pq.printQueue();
                }
            } catch (const exception& e) {
                cout << "Error: " << e.what() << "\n";
            }
        }
    } else {
        cout << "Invalid choice.\n";
    }
}

void runDemo() {
    cout << "\nDemo Mode\n";

    IPList* lists[] = {new FixedArrayList(), new DynamicArrayList(), new LinkedList()};
    string names[] = {"Fixed-size Array", "Dynamic Array", "Linked List"};

    for (int i = 0; i < 3; ++i) {
        cout << "\nDemo: " << names[i] << "\n";
        lists[i]->append("192.168.0.1");
        lists[i]->insert(0, "10.0.0.1");
        printList(*lists[i]);
        delete lists[i];
    }

    cout << "\nDemo: Priority Queue\n";
    PriorityQueue pq;
    cout << "1. Enqueue 192.168.0.1 (Priority 5)\n";
    pq.enqueue("192.168.0.1", 5);
    cout << "2. Enqueue 10.0.0.1 (Priority 1)\n";
    pq.enqueue("10.0.0.1", 1);
    cout << "3. Enqueue 172.16.0.1 (Priority 5)\n";
    pq.enqueue("172.16.0.1", 5);
    cout << "4. Enqueue 127.0.0.1 (Priority 3)\n";
    pq.enqueue("127.0.0.1", 3);

    pq.printQueue();

    cout << "\nDequeue 2 elements:\n";
    cout << "Dequeued: " << pq.dequeue() << "\n";
    cout << "Dequeued: " << pq.dequeue() << "\n";

    cout << "\nRemaining:\n";
    pq.printQueue();
}

void runBenchmark() {
    cout << "\nBenchmark Mode\n";
    cout << "This mode tests the performance of standard lists.\n";
    int testSize = 3000;

    auto runTest = [&](IPList* list, const string& name) {
        using namespace std::chrono;
        cout << "\nTest: " << name << " (" << testSize << " ops)\n";
        auto start = high_resolution_clock::now();
        for (int i = 0; i < testSize; ++i) list->append("192.168.0.1");
        auto end = high_resolution_clock::now();
        cout << "Append: " << duration_cast<microseconds>(end - start).count() << " us\n";
    };

    FixedArrayList fixedList;
    runTest(&fixedList, "Fixed-size Array");

    DynamicArrayList dynList;
    runTest(&dynList, "Dynamic Array");

    LinkedList lnkList;
    runTest(&lnkList, "Linked List");
}

int main() {
    int mode;
    do {
        cout << "\nChoose a mode:\n";
        cout << "1. Interactive Mode\n";
        cout << "2. Demo Mode\n";
        cout << "3. Benchmark Mode\n";
        cout << "0. Exit\n";
        cout << "Your choice: ";
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
                cout << "Program terminated.\n";
                break;
            default:
                cout << "Invalid choice.\n";
                break;
        }
    } while (mode != 0);

    return 0;
}