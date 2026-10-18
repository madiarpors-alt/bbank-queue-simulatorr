// 1-апта: Кіріспе және алгоритмдерді талдау (Big-O, Ω, Θ)
// Patient класы және рекурсия
// Компиляция: g++ -std=c++17 week1_patient.cpp -o week1 && ./week1
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

// Рекурсиялық мысал: пациент санын есептеу, O(n)
int countPatients(int n) {
    if (n <= 0) return 0;
    return 1 + countPatients(n - 1);
}

int main() {
    Patient p1(1, "Али", 2, "Жүрек");
    Patient p2(2, "Айгүл", 5, "Тұмау");
    p1.display(); // O(1)
    p2.display();
    std::cout << "Пациент саны (рекурсия): " << countPatients(5) << "\n";
    return 0;
}
