// 2-апта: Сызықтық құрылымдар: массивтер, байланысты тізімдер
// Компиляция: g++ -std=c++17 week2_linked_list.cpp -o week2 && ./week2
#include <iostream>
#include <string>

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

struct Node {
    Patient* data;
    Node* next;
    Node(Patient* p) : data(p), next(nullptr) {}
};

class PatientList {
    Node* head;
    int count;
public:
    PatientList() : head(nullptr), count(0) {}
    ~PatientList() {
        while (head) { Node* n = head; head = head->next; delete n; }
    }
    void add(Patient* p) { // O(1), басына қосу
        Node* newNode = new Node(p);
        newNode->next = head;
        head = newNode;
        count++;
    }
    Patient* removeFirst() { // O(1)
        if (!head) return nullptr;
        Node* n = head;
        Patient* p = n->data;
        head = head->next;
        delete n;
        count--;
        return p;
    }
    int size() const { return count; }
    void display() const { // O(n)
        for (Node* n = head; n; n = n->next) n->data->display();
    }
};

int main() {
    Patient p1(1, "Али", 2, "Жүрек");
    Patient p2(2, "Айгүл", 5, "Тұмау");
    Patient p3(3, "Берік", 1, "Сынық");

    PatientList list;
    list.add(&p1);
    list.add(&p2);
    list.add(&p3);
    std::cout << "Тізім (соңғы қосылған бірінші), саны " << list.size() << ":\n";
    list.display();

    list.removeFirst();
    std::cout << "\nБірінші элемент алынғаннан кейін, саны " << list.size() << ":\n";
    list.display();
    return 0;
}
