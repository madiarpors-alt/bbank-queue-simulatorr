// 5-апта: Іздеу алгоритмдері. Хэш-таблицалар. Пациентті ID бойынша іздеу
// Компиляция: g++ -std=c++17 week5_hashtable.cpp -o week5 && ./week5
#include <iostream>
#include <string>
#include <vector>
#include <list>

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

class HashTable {
    int size;
    std::vector<std::list<Patient*>> table;
    int hash(int key) const { return key % size; }
public:
    HashTable(int s) : size(s), table(s) {}
    void insert(Patient* p) { // O(1) орташа
        table[hash(p->id)].push_back(p);
    }
    Patient* search(int id) { // O(1) орташа, O(n) ең нашар
        for (auto* pat : table[hash(id)])
            if (pat->id == id) return pat;
        return nullptr;
    }
    void print() const {
        for (int i = 0; i < size; i++) {
            std::cout << "  [" << i << "]";
            for (auto* p : table[i]) std::cout << " -> " << p->id;
            std::cout << "\n";
        }
    }
};

// Binary search: ID бойынша сұрыпталған массивте (ID өсу ретімен), O(log n)
int binarySearchById(const std::vector<Patient*>& a, int id) {
    int l = 0, r = (int)a.size() - 1;
    while (l <= r) {
        int m = (l + r) / 2;
        if (a[m]->id == id) return m;
        if (a[m]->id < id) l = m + 1; else r = m - 1;
    }
    return -1;
}

int main() {
    std::vector<Patient*> patients = {
        new Patient(101, "Айдар", 4, "Тұмау"),
        new Patient(102, "Болат", 1, "Жүрек талмасы"),
        new Patient(104, "Дана", 2, "Сынық"),
        new Patient(108, "Гүлнар", 7, "Бас ауруы") // 108 % 7 = 3 және 101 % 7 = 3: коллизия
    };

    HashTable ht(7);
    for (auto* p : patients) ht.insert(p);
    std::cout << "Хэш-таблица құрылымы:\n";
    ht.print();

    Patient* f = ht.search(108);
    std::cout << "\nІздеу ID=108:\n";
    if (f) f->display();
    std::cout << "Іздеу ID=999: " << (ht.search(999) ? "табылды" : "табылмады") << "\n";

    std::cout << "Binary search ID=104 индексі: " << binarySearchById(patients, 104) << "\n";

    for (auto* p : patients) delete p;
    return 0;
}
