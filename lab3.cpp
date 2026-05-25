#include <iostream>
#include <string>
#include <chrono>
#include <algorithm>

using namespace std;

struct Message {
    string text;
    int type;
    int priority;
};

void printStringArray(string* arr, int n) {
    for (int i = 0; i < n; i++) cout << arr[i] << " ";
    cout << "\n";
}

void printMessageArray(Message* arr, int n) {
    for (int i = 0; i < n; i++) {
        cout << "[Type: " << arr[i].type << ", Priority: " << arr[i].priority << ", Text: " << arr[i].text << "]\n";
    }
}

string* cloneStringArray(string* src, int n) {
    string* dest = new string[n];
    for (int i = 0; i < n; i++) dest[i] = src[i];
    return dest;
}

Message* cloneMessageArray(Message* src, int n) {
    Message* dest = new Message[n];
    for (int i = 0; i < n; i++) dest[i] = src[i];
    return dest;
}

void selectionSort(string* arr, int n, bool printSteps = false) {
    for (int i = 0; i < n - 1; i++) {
        int max_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] > arr[max_idx]) max_idx = j;
        }
        swap(arr[i], arr[max_idx]);
        if (printSteps) {
            cout << "Step " << i + 1 << ": ";
            printStringArray(arr, n);
        }
    }
}

int hoarePartition(string* arr, int low, int high) {
    string pivot = arr[low + (high - low) / 2];
    int i = low - 1;
    int j = high + 1;
    while (true) {
        do { i++; } while (arr[i] > pivot);
        do { j--; } while (arr[j] < pivot);
        if (i >= j) return j;
        swap(arr[i], arr[j]);
    }
}

void quickSortRecursive(string* arr, int low, int high) {
    if (low < high) {
        int pi = hoarePartition(arr, low, high);
        quickSortRecursive(arr, low, pi);
        quickSortRecursive(arr, pi + 1, high);
    }
}

void quickSort(string* arr, int n, bool printSteps = false) {
    if (printSteps) cout << "Initial for QuickSort: ";
    if (printSteps) printStringArray(arr, n);
    quickSortRecursive(arr, 0, n - 1);
    if (printSteps) cout << "Final QuickSort: ";
    if (printSteps) printStringArray(arr, n);
}

void merge(string* arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    string* L = new string[n1];
    string* R = new string[n2];
    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int i = 0; i < n2; i++) R[i] = arr[mid + 1 + i];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] >= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    delete[] L;
    delete[] R;
}

void mergeSortBU(string* arr, int n, bool printSteps = false) {
    int step = 1;
    for (int curr_size = 1; curr_size <= n - 1; curr_size = 2 * curr_size) {
        for (int left_start = 0; left_start < n - 1; left_start += 2 * curr_size) {
            int mid = min(left_start + curr_size - 1, n - 1);
            int right_end = min(left_start + 2 * curr_size - 1, n - 1);
            merge(arr, left_start, mid, right_end);
        }
        if (printSteps) {
            cout << "Merge step " << step++ << " (size " << curr_size * 2 << "): ";
            printStringArray(arr, n);
        }
    }
}

void selectionSortRange(string* arr, int low, int high) {
    for (int i = low; i < high; i++) {
        int max_idx = i;
        for (int j = i + 1; j <= high; j++) {
            if (arr[j] > arr[max_idx]) max_idx = j;
        }
        swap(arr[i], arr[max_idx]);
    }
}

void combinedSortRecursive(string* arr, int low, int high, int threshold) {
    if (high - low + 1 <= threshold) {
        selectionSortRange(arr, low, high);
    } else if (low < high) {
        int pi = hoarePartition(arr, low, high);
        combinedSortRecursive(arr, low, pi, threshold);
        combinedSortRecursive(arr, pi + 1, high, threshold);
    }
}

void combinedSort(string* arr, int n, int threshold, bool printSteps = false) {
    combinedSortRecursive(arr, 0, n - 1, threshold);
    if (printSteps) {
        cout << "Combined sort finished: ";
        printStringArray(arr, n);
    }
}

void stdLibrarySort(string* arr, int n) {
    sort(arr, arr + n, greater<string>());
}

void countingSortStable(Message* arr, int n) {
    int max_type = 0;
    for (int i = 0; i < n; i++) {
        if (arr[i].type > max_type) max_type = arr[i].type;
    }

    int* count = new int[max_type + 1]{0};
    Message* output = new Message[n];

    for (int i = 0; i < n; i++) count[arr[i].type]++;
    for (int i = 1; i <= max_type; i++) count[i] += count[i - 1];

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
    int max_val = 0;
    for (int i = 0; i < n; i++) {
        if (arr[i].priority > max_val) max_val = arr[i].priority;
    }

    for (int exp = 1; max_val / exp > 0; exp *= 10) {
        countingSortForRadix(arr, n, exp);
    }
}

bool messageComparator(const Message& m1, const Message& m2) {
    if (m1.type != m2.type) return m1.type < m2.type;
    return m1.priority < m2.priority;
}

void comparisonSortMessages(Message* arr, int n) {
    stable_sort(arr, arr + n, messageComparator);
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
        else if (algo == 5) stdLibrarySort(arr, n);

        if (algo == 5) {
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

string generateRandomString() {
    string s = "";
    int len = 3 + rand() % 5;
    for (int i = 0; i < len; i++) s += (char)('a' + rand() % 26);
    return s;
}

void runBenchmark() {
    cout << "\nBenchmark Mode\n";
    int sizes[] = {1000, 5000, 10000};

    for (int sIdx = 0; sIdx < 3; sIdx++) {
        int n = sizes[sIdx];
        cout << "\nArray Size: " << n << "\n";

        string* randomArr = new string[n];
        for (int i = 0; i < n; i++) randomArr[i] = generateRandomString();

        auto measure = [&](void (*sortFunc)(string*, int), string name) {
            string* copy = cloneStringArray(randomArr, n);
            auto start = chrono::high_resolution_clock::now();
            sortFunc(copy, n);
            auto end = chrono::high_resolution_clock::now();
            cout << name << ": " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " us\n";
            delete[] copy;
        };

        auto measureCombined = [&](int threshold) {
            string* copy = cloneStringArray(randomArr, n);
            auto start = chrono::high_resolution_clock::now();
            combinedSort(copy, n, threshold, false);
            auto end = chrono::high_resolution_clock::now();
            cout << "Combined Sort (Threshold " << threshold << "): " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " us\n";
            delete[] copy;
        };

        measure([](string* a, int s) { selectionSort(a, s, false); }, "Selection Sort");
        measure([](string* a, int s) { quickSort(a, s, false); }, "Quick Sort");
        measure([](string* a, int s) { mergeSortBU(a, s, false); }, "Merge Sort BU");
        measure([](string* a, int s) { stdLibrarySort(a, s); }, "Library Sort");
        measureCombined(10);
        measureCombined(50);
        measureCombined(100);

        delete[] randomArr;

        cout << "\nMessage Sorting Benchmark (Size " << n << ")\n";
        Message* msgArr = new Message[n];
        for (int i = 0; i < n; i++) {
            msgArr[i].text = generateRandomString();
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
            case 0: cout << "Program terminated.\n"; break;
            default: cout << "Invalid choice.\n"; break;
        }
    } while (mode != 0);

    return 0;
}