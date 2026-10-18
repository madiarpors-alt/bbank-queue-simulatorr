// 7-апта: Heap және Heap sort. AVL ағаштары
// Компиляция: g++ -std=c++17 week7_heap_avl.cpp -o week7 && ./week7
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

// ---------- Priority Queue (Min-Heap): приоритет 1 бірінші ----------
class PriorityQueue {
    std::vector<Patient*> h;
    bool before(Patient* a, Patient* b) const { // приоритет, содан соң ID
        return a->priority != b->priority ? a->priority < b->priority : a->id < b->id;
    }
public:
    void push(Patient* p) { // O(log n)
        h.push_back(p);
        int i = (int)h.size() - 1;
        while (i > 0 && before(h[i], h[(i - 1) / 2])) {
            std::swap(h[i], h[(i - 1) / 2]);
            i = (i - 1) / 2;
        }
    }
    Patient* pop() { // O(log n)
        if (h.empty()) return nullptr;
        Patient* top = h[0];
        h[0] = h.back();
        h.pop_back();
        int i = 0, n = (int)h.size();
        while (true) {
            int l = 2 * i + 1, r = 2 * i + 2, m = i;
            if (l < n && before(h[l], h[m])) m = l;
            if (r < n && before(h[r], h[m])) m = r;
            if (m == i) break;
            std::swap(h[i], h[m]);
            i = m;
        }
        return top;
    }
    bool empty() const { return h.empty(); }
};

// ---------- AVL ағашы (есім бойынша) ----------
struct TreeNode {
    Patient* data;
    TreeNode* left;
    TreeNode* right;
    int height;
    TreeNode(Patient* p) : data(p), left(nullptr), right(nullptr), height(1) {}
};

int height(TreeNode* node) { return node ? node->height : 0; }

int getBalance(TreeNode* node) { return node ? height(node->left) - height(node->right) : 0; }

TreeNode* rightRotate(TreeNode* y) { // O(1)
    TreeNode* x = y->left;
    TreeNode* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = std::max(height(y->left), height(y->right)) + 1;
    x->height = std::max(height(x->left), height(x->right)) + 1;
    return x;
}

TreeNode* leftRotate(TreeNode* x) { // O(1)
    TreeNode* y = x->right;
    TreeNode* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = std::max(height(x->left), height(x->right)) + 1;
    y->height = std::max(height(y->left), height(y->right)) + 1;
    return y;
}

TreeNode* insertAVL(TreeNode* node, Patient* p) { // O(log n)
    if (!node) return new TreeNode(p);
    if (p->name < node->data->name) node->left = insertAVL(node->left, p);
    else if (p->name > node->data->name) node->right = insertAVL(node->right, p);
    else return node;

    node->height = 1 + std::max(height(node->left), height(node->right));
    int bal = getBalance(node);

    if (bal > 1 && p->name < node->left->data->name) return rightRotate(node);          // LL
    if (bal < -1 && p->name > node->right->data->name) return leftRotate(node);         // RR
    if (bal > 1 && p->name > node->left->data->name) {                                  // LR
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (bal < -1 && p->name < node->right->data->name) {                                // RL
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

void inorderAVL(TreeNode* n) {
    if (!n) return;
    inorderAVL(n->left); n->data->display(); inorderAVL(n->right);
}

void freeAVL(TreeNode* n) {
    if (!n) return;
    freeAVL(n->left); freeAVL(n->right); delete n;
}

int main() {
    std::vector<Patient*> patients = {
        new Patient(101, "Айдар", 4, "Тұмау"),
        new Patient(102, "Болат", 1, "Жүрек талмасы"),
        new Patient(103, "Гүлнар", 7, "Бас ауруы"),
        new Patient(104, "Дана", 2, "Сынық"),
        new Patient(105, "Ермек", 9, "Тексеру"),
        new Patient(106, "Жанар", 3, "Аллергия"),
        new Patient(107, "Нұрлан", 1, "Қан кету")
    };

    PriorityQueue pq;
    TreeNode* avl = nullptr;
    for (auto* p : patients) { pq.push(p); avl = insertAVL(avl, p); }

    std::cout << "Приоритет кезегі: қабылдау реті\n";
    int k = 1;
    while (!pq.empty()) {
        Patient* p = pq.pop();
        std::cout << "  " << k++ << ". " << p->name << " (приоритет " << p->priority << ")\n";
    }

    std::cout << "\nAVL inorder (есім әліпбиі):\n";
    inorderAVL(avl);
    std::cout << "AVL биіктігі: " << height(avl) << " (7 түйін, әліпби ретімен қосылса да теңгерімді)\n";

    freeAVL(avl);
    for (auto* p : patients) delete p;
    return 0;
}
