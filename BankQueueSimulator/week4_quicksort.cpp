// 4-апта: Сұрыптау: Merge sort, Quick sort. Пациенттерді приоритет бойынша сұрыптау
// Компиляция: g++ -std=c++17 week4_quicksort.cpp -o week4 && ./week4
#include <iostream>
#include <string>
#include <vector>

class Patient {
public:
    int id;
    std::string name;
    int priority; // 1 - өте ауыр, 10 - жеңіл
    std::string diagnosis;

    Patient(int i, std::string n, int p, std::string d)
        : id(i), name(n), priority(p), diagnosis(d) {}

    void display() const { // O(1)
        std::cout << "  ID: " << id << ", Аты: " << name << ", Приоритет: " << priority
                  << ", Диагноз: " << diagnosis << "\n";
    }
};

void swapPtr(Patient*& a, Patient*& b) {
    Patient* temp = a;
    a = b;
    b = temp;
}

int partition(Patient* arr[], int low, int high) {
    int pivot = arr[high]->priority;
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j]->priority < pivot) { // төмен сан = ауыр науқас, бірінші
            i++;
            swapPtr(arr[i], arr[j]);
        }
    }
    swapPtr(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(Patient* arr[], int low, int high) { // орташа O(n log n), ең нашар O(n^2)
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// Merge sort: барлық жағдайда O(n log n), тұрақты
void mergeParts(std::vector<Patient*>& a, int l, int m, int r) {
    std::vector<Patient*> tmp;
    int i = l, j = m + 1;
    while (i <= m && j <= r)
        tmp.push_back(a[i]->priority <= a[j]->priority ? a[i++] : a[j++]);
    while (i <= m) tmp.push_back(a[i++]);
    while (j <= r) tmp.push_back(a[j++]);
    for (int k = 0; k < (int)tmp.size(); k++) a[l + k] = tmp[k];
}
void mergeSort(std::vector<Patient*>& a, int l, int r) {
    if (l >= r) return;
    int m = (l + r) / 2;
    mergeSort(a, l, m);
    mergeSort(a, m + 1, r);
    mergeParts(a, l, m, r);
}

int main() {
    std::vector<Patient*> base = {
        new Patient(101, "Айдар", 4, "Тұмау"),
        new Patient(102, "Болат", 1, "Жүрек талмасы"),
        new Patient(103, "Гүлнар", 7, "Бас ауруы"),
        new Patient(104, "Дана", 2, "Сынық"),
        new Patient(105, "Ермек", 9, "Тексеру")
    };

    std::vector<Patient*> a = base;
    quickSort(a.data(), 0, (int)a.size() - 1);
    std::cout << "Quick sort (ауыры бірінші):\n";
    for (auto* p : a) p->display();

    std::vector<Patient*> b = base;
    mergeSort(b, 0, (int)b.size() - 1);
    std::cout << "\nMerge sort:\n";
    for (auto* p : b) p->display();

    for (auto* p : base) delete p;
    return 0;
}
