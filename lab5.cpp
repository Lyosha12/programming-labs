#include <iostream>
#include <string>
#include <algorithm>
#include <chrono>
#include <ctime>

using namespace std;

struct Message {
    int type;
    int priority;
    string text;
};

void printStringArray(string* arr, int n) {
    for (int i = 0; i < n; i++) cout << arr[i] << " ";
    cout << "\n";
}

string* cloneStringArray(string* arr, int n) {
    string* clone = new string[n];
    for (int i = 0; i < n; i++) clone[i] = arr[i];
    return clone;
}

void printMessageArray(Message* arr, int n) {
    for (int i = 0; i < n; i++) {
        cout << "[Type: " << arr[i].type
             << " | Prio: " << arr[i].priority
             << " | Text: " << arr[i].text << "]\n";
    }
}

Message* cloneMessageArray(Message* arr, int n) {
    Message* clone = new Message[n];
    for (int i = 0; i < n; i++) clone[i] = arr[i];
    return clone;
}


void selectionSort(string* arr, int n, bool trace) {
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx]) min_idx = j;
        }
        swap(arr[i], arr[min_idx]);
        if (trace) {
            cout << "Step " << i + 1 << ": ";
            printStringArray(arr, n);
        }
    }
}

int partitionStr(string* arr, int low, int high) {
    string pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return (i + 1);
}

void quickSortHelper(string* arr, int low, int high, int n, bool trace) {
    if (low < high) {
        int pi = partitionStr(arr, low, high);
        if (trace) {
            cout << "Pivot placed at " << pi << ": ";
            printStringArray(arr, n);
        }
        quickSortHelper(arr, low, pi - 1, n, trace);
        quickSortHelper(arr, pi + 1, high, n, trace);
    }
}

void quickSort(string* arr, int n, bool trace) {
    quickSortHelper(arr, 0, n - 1, n, trace);
}

void mergeStr(string* arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    string* L = new string[n1];
    string* R = new string[n2];

    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    delete[] L;
    delete[] R;
}

void mergeSortBU(string* arr, int n, bool trace) {
    for (int curr_size = 1; curr_size <= n - 1; curr_size = 2 * curr_size) {
        for (int left_start = 0; left_start < n - 1; left_start += 2 * curr_size) {
            int mid = min(left_start + curr_size - 1, n - 1);
            int right_end = min(left_start + 2 * curr_size - 1, n - 1);
            mergeStr(arr, left_start, mid, right_end);
        }
        if (trace) {
            cout << "Merged size " << curr_size * 2 << ": ";
            printStringArray(arr, n);
        }
    }
}

void insertionSort(string* arr, int low, int high) {
    for (int i = low + 1; i <= high; i++) {
        string key = arr[i];
        int j = i - 1;
        while (j >= low && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void combinedSortHelper(string* arr, int low, int high, int threshold, int n, bool trace) {
    if (low < high) {
        if (high - low + 1 <= threshold) {
            insertionSort(arr, low, high);
        } else {
            int pi = partitionStr(arr, low, high);
            combinedSortHelper(arr, low, pi - 1, threshold, n, trace);
            combinedSortHelper(arr, pi + 1, high, threshold, n, trace);
        }
    }
}

void combinedSort(string* arr, int n, int threshold, bool trace) {
    combinedSortHelper(arr, 0, n - 1, threshold, n, trace);
    if (trace) {
        cout << "Combined Sort finished: ";
        printStringArray(arr, n);
    }
}

void stdLibrarySort(string* arr, int n) {
    sort(arr, arr + n);
}


void countingSortStable(Message* arr, int n) {
    if (n <= 0) return;
    int maxType = arr[0].type;
    for (int i = 1; i < n; i++) {
        if (arr[i].type > maxType) maxType = arr[i].type;
    }

    int* count = new int[maxType + 1];
    for (int i = 0; i <= maxType; i++) count[i] = 0;

    Message* output = new Message[n];

    for (int i = 0; i < n; i++) count[arr[i].type]++;
    for (int i = 1; i <= maxType; i++) count[i] += count[i - 1];

    for (int i = n - 1; i >= 0; i--) {
        output[count[arr[i].type] - 1] = arr[i];
        count[arr[i].type]--;
    }
    for (int i = 0; i < n; i++) arr[i] = output[i];

    delete[] count;
    delete[] output;
}

void countingSortForRadix(Message* arr, int n, int exp) {
    Message* output = new Message[n];
    int count[10] = {0};

    for (int i = 0; i < n; i++) count[(arr[i].priority / exp) % 10]++;
    for (int i = 1; i < 10; i++) count[i] += count[i - 1];

    for (int i = n - 1; i >= 0; i--) {
        output[count[(arr[i].priority / exp) % 10] - 1] = arr[i];
        count[(arr[i].priority / exp) % 10]--;
    }

    for (int i = 0; i < n; i++) arr[i] = output[i];
    delete[] output;
}

void radixSortStable(Message* arr, int n) {
    if (n <= 0) return;
    int m = arr[0].priority;
    for (int i = 1; i < n; i++) {
        if (arr[i].priority > m) m = arr[i].priority;
    }
    for (int exp = 1; m / exp > 0; exp *= 10) {
        countingSortForRadix(arr, n, exp);
    }
}

bool compareMessages(const Message& a, const Message& b) {
    if (a.type != b.type) return a.type < b.type;
    return a.priority < b.priority;
}

void comparisonSortMessages(Message* arr, int n) {
    stable_sort(arr, arr + n, compareMessages);
}


string generateRandomString() {
    string s = "";
    int len = 3 + rand() % 5;
    for (int i = 0; i < len; i++) s += (char)('a' + rand() % 26);
    return s;
}

void runInteractive() {
    cout << "\nInteractive Mode\n";
    cout << "1. String Array (Lab 3a)\n";
    cout << "2. Message Array (Lab 3b)\n";
    cout << "Choice: ";
    int choice;
    cin >> choice;

    if (choice == 1) {
        cout << "Enter array size: ";
        int n;
        cin >> n;
        string* arr = new string[n];
        cout << "Enter " << n << " strings:\n";
        for (int i = 0; i < n; i++) cin >> arr[i];

        cout << "1. Selection Sort\n2. Quicksort\n3. Merge Sort BU\n4. Combined Sort\n5. Library Sort\nAlgorithm: ";
        int algo;
        cin >> algo;

        if (algo == 1) selectionSort(arr, n, true);
        else if (algo == 2) quickSort(arr, n, true);
        else if (algo == 3) mergeSortBU(arr, n, true);
        else if (algo == 4) combinedSort(arr, n, 2, true);
        else if (algo == 5) {
            stdLibrarySort(arr, n);
            cout << "Sorted: ";
            printStringArray(arr, n);
        }
        delete[] arr;
    } else if (choice == 2) {
        cout << "Enter array size: ";
        int n;
        cin >> n;
        Message* arr = new Message[n];
        for (int i = 0; i < n; i++) {
            cout << "Message " << i + 1 << " (Type Priority Text): ";
            cin >> arr[i].type >> arr[i].priority >> arr[i].text;
        }

        cout << "1. Counting Sort (by Type)\n2. Radix Sort (by Priority)\n3. Comparison Sort (Type then Priority)\nAlgorithm: ";
        int algo;
        cin >> algo;

        if (algo == 1) countingSortStable(arr, n);
        else if (algo == 2) radixSortStable(arr, n);
        else if (algo == 3) comparisonSortMessages(arr, n);

        cout << "Sorted:\n";
        printMessageArray(arr, n);
        delete[] arr;
    }
}

void runDemo() {
    cout << "\nDemo Mode\n";
    int n = 4;
    string initial[] = {"Grape", "Plum", "Apple", "Banana"};

    string* arr = new string[n];
    for (int i = 0; i < n; i++) arr[i] = initial[i];
    cout << "Original Array: "; printStringArray(arr, n);
    cout << "Selection Sort:\n";
    selectionSort(arr, n, true);
    delete[] arr;

    arr = new string[n];
    for (int i = 0; i < n; i++) arr[i] = initial[i];
    cout << "\nOriginal Array: "; printStringArray(arr, n);
    cout << "Merge Sort BU:\n";
    mergeSortBU(arr, n, true);
    delete[] arr;

    arr = new string[n];
    for (int i = 0; i < n; i++) arr[i] = initial[i];
    cout << "\nOriginal Array: "; printStringArray(arr, n);
    cout << "Quick Sort:\n";
    quickSort(arr, n, true);
    delete[] arr;
}

void runBenchmark() {
    cout << "\nBenchmark Mode\n";
    int sizes[] = {1000, 5000, 10000};

    for (int sIdx = 0; sIdx < 3; sIdx++) {
        int n = sizes[sIdx];
        cout << "\n--- Array Size: " << n << " ---\n";

        string* randomArr = new string[n];
        for (int i = 0; i < n; i++) randomArr[i] = generateRandomString();

        auto measureStr = [&](void (*sortFunc)(string*, int, bool), string name) {
            string* copy = cloneStringArray(randomArr, n);
            auto start = chrono::high_resolution_clock::now();
            sortFunc(copy, n, false);
            auto end = chrono::high_resolution_clock::now();
            cout << name << ": " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " us\n";
            delete[] copy;
        };

        measureStr(selectionSort, "Selection Sort");
        measureStr(quickSort, "Quick Sort");
        measureStr(mergeSortBU, "Merge Sort BU");

        delete[] randomArr;

        cout << "\nMessage Benchmark (Size " << n << ")\n";
        Message* msgArr = new Message[n];
        for (int i = 0; i < n; i++) {
            msgArr[i].text = "msg";
            msgArr[i].type = rand() % 10;
            msgArr[i].priority = rand() % 1000;
        }

        auto measureMsg = [&](void (*sortFunc)(Message*, int), string name) {
            Message* copy = cloneMessageArray(msgArr, n);
            auto start = chrono::high_resolution_clock::now();
            sortFunc(copy, n);
            auto end = chrono::high_resolution_clock::now();
            cout << name << ": " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " us\n";
            delete[] copy;
        };

        measureMsg(countingSortStable, "Counting Sort (Type)");
        measureMsg(radixSortStable, "Radix Sort (Priority)");
        measureMsg(comparisonSortMessages, "Comparison Sort (Type -> Priority)");

        delete[] msgArr;
    }
}

int main() {
    srand(time(0));
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
            case 1: runInteractive(); break;
            case 2: runDemo(); break;
            case 3: runBenchmark(); break;
            case 0: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice!\n"; break;
        }
    } while (mode != 0);

    return 0;
}