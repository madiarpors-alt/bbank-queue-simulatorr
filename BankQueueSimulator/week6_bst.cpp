// 6-апта: Ағаштар: Binary trees, BST. Пациенттерді есім бойынша сақтау
// Компиляция: g++ -std=c++17 week6_bst.cpp -o week6 && ./week6
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

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

struct TreeNode {
    Patient* data;
    TreeNode* left;
    TreeNode* right;
    TreeNode(Patient* p) : data(p), left(nullptr), right(nullptr) {}
};

class BST {
    TreeNode* root;
    TreeNode* insertRec(TreeNode* node, Patient* p) { // орташа O(log n), ең нашар O(n)
        if (node == nullptr) return new TreeNode(p);
        if (p->name < node->data->name) node->left = insertRec(node->left, p);
        else if (p->name > node->data->name) node->right = insertRec(node->right, p);
        return node;
    }
    Patient* searchRec(TreeNode* n, const std::string& name) const {
        if (!n) return nullptr;
        if (name == n->data->name) return n->data;
        return name < n->data->name ? searchRec(n->left, name) : searchRec(n->right, name);
    }
    int heightRec(TreeNode* n) const { return n ? 1 + std::max(heightRec(n->left), heightRec(n->right)) : 0; }
    void inorderRec(TreeNode* n) const {
        if (!n) return;
        inorderRec(n->left); n->data->display(); inorderRec(n->right);
    }
    void freeRec(TreeNode* n) { if (n) { freeRec(n->left); freeRec(n->right); delete n; } }
public:
    BST() : root(nullptr) {}
    ~BST() { freeRec(root); }
    void insert(Patient* p) { root = insertRec(root, p); }
    Patient* search(const std::string& name) const { return searchRec(root, name); }
    int height() const { return heightRec(root); }
    void inorder() const { inorderRec(root); }
};

int main() {
    std::vector<Patient*> patients = {
        new Patient(101, "Дана", 2, "Сынық"),
        new Patient(102, "Болат", 1, "Жүрек талмасы"),
        new Patient(103, "Жанар", 3, "Аллергия"),
        new Patient(104, "Айдар", 4, "Тұмау"),
        new Patient(105, "Ермек", 9, "Тексеру")
    };

    BST bst;
    for (auto* p : patients) bst.insert(p);

    std::cout << "Inorder (есім әліпбиі бойынша):\n";
    bst.inorder();

    Patient* f = bst.search("Жанар");
    std::cout << "\nІздеу 'Жанар': " << (f ? "табылды, ID=" + std::to_string(f->id) : "табылмады") << "\n";
    std::cout << "BST биіктігі: " << bst.height() << "\n";

    for (auto* p : patients) delete p;
    return 0;
}
