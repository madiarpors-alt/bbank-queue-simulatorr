// 3-апта: Стектер, кезектер, дектер. Стек арқылы undo
// Компиляция: g++ -std=c++17 week3_stack.cpp -o week3 && ./week3
#include <iostream>
#include <string>
#include <stack>
#include <queue>

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

std::stack<Patient*> patientStack;

void addToStack(Patient* p) { // O(1)
    patientStack.push(p);
}

Patient* undoAdd() { // O(1)
    if (!patientStack.empty()) {
        Patient* p = patientStack.top();
        patientStack.pop();
        return p;
    }
    return nullptr;
}

int main() {
    Patient p1(1, "Али", 2, "Жүрек");
    Patient p2(2, "Айгүл", 5, "Тұмау");
    Patient p3(3, "Берік", 1, "Сынық");

    addToStack(&p1);
    addToStack(&p2);
    addToStack(&p3);
    std::cout << "Стекте: " << patientStack.size() << " пациент\n";

    std::cout << "Undo (LIFO: соңғы қосылған бірінші шығады):\n";
    while (Patient* p = undoAdd()) p->display();

    std::cout << "Стек бос ма? " << (patientStack.empty() ? "иә" : "жоқ") << "\n";
    std::cout << "Бос стектен undo: " << (undoAdd() == nullptr ? "nullptr" : "қате") << "\n";

    // Салыстыру үшін кезек (FIFO)
    std::queue<Patient*> q;
    q.push(&p1); q.push(&p2); q.push(&p3);
    std::cout << "\nКезек (FIFO: бірінші келген бірінші шығады):\n";
    while (!q.empty()) { q.front()->display(); q.pop(); }
    return 0;
}
