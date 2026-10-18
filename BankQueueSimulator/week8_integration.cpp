// CSE6781: Медициналық кезек басқару жүйесі (Priority Queue + AVL Tree + Hashing)
// 8-апта: бірінші аралық аттестация. Барлық компоненттерді біріктіру және тестілеу
// Компиляция: g++ -std=c++17 week8_integration.cpp -o week8 && ./medical
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <stack>
#include <algorithm>

// ======================= 1-апта: Patient класы, Big-O =======================
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

// ======================= 2-апта: Байланысты тізім =======================
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

// ======================= 3-апта: Стек (undo) =======================
std::stack<Patient*> patientStack;

void addToStack(Patient* p) { patientStack.push(p); } // O(1)

Patient* undoAdd() { // O(1)
    if (!patientStack.empty()) {
        Patient* p = patientStack.top();
        patientStack.pop();
        return p;
    }
    return nullptr;
}

// ======================= 4-апта: Quick sort приоритет бойынша =======================
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

void quickSort(Patient* arr[], int low, int high) { // орташа O(n log n)
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// ======================= 5-апта: Hash Table (chaining) =======================
class HashTable {
    int size;
    std::vector<std::list<Patient*>> table;
    int hash(int key) const { return key % size; }
public:
    HashTable(int s) : size(s), table(s) {}
    void insert(Patient* p) { // O(1) орташа
        table[hash(p->id)].push_back(p);
    }
    Patient* search(int id) { // O(1) орташа
        for (auto* pat : table[hash(id)])
            if (pat->id == id) return pat;
        return nullptr;
    }
};

// ======================= 6-апта: BST (есім бойынша) =======================
struct TreeNode {
    Patient* data;
    TreeNode* left;
    TreeNode* right;
    int height; // AVL үшін (7-апта)
    TreeNode(Patient* p) : data(p), left(nullptr), right(nullptr), height(1) {}
};

class BST {
    TreeNode* root;
    TreeNode* insertRec(TreeNode* node, Patient* p) { // орташа O(log n), ең нашар O(n)
        if (node == nullptr) return new TreeNode(p);
        if (p->name < node->data->name) node->left = insertRec(node->left, p);
        else if (p->name > node->data->name) node->right = insertRec(node->right, p);
        return node;
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
    int height() const { return heightRec(root); }
    void inorder() const { inorderRec(root); }
};

// ======================= 7-апта: AVL ағашы (есім бойынша) =======================
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

Patient* searchAVL(TreeNode* n, const std::string& name) { // O(log n)
    while (n) {
        if (name == n->data->name) return n->data;
        n = (name < n->data->name) ? n->left : n->right;
    }
    return nullptr;
}

void inorderAVL(TreeNode* n) {
    if (!n) return;
    inorderAVL(n->left); n->data->display(); inorderAVL(n->right);
}

void freeAVL(TreeNode* n) {
    if (!n) return;
    freeAVL(n->left); freeAVL(n->right); delete n;
}

// ======================= 7-апта: Priority Queue (Min-Heap) =======================
class PriorityQueue {
    std::vector<Patient*> h;
    bool before(Patient* a, Patient* b) const { // приоритет, содан соң ID (келу реті)
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
    int size() const { return (int)h.size(); }
};

// ======================= 8-апта: Компоненттерді біріктіру және тестілеу =======================
int main() {
    PatientList list;
    HashTable ht(7);
    BST bst;
    TreeNode* avl = nullptr;
    PriorityQueue pq;

    // Есімдер әдейі әліпби ретімен: BST мен AVL биіктігін салыстыру үшін
    std::vector<Patient*> patients = {
        new Patient(101, "Айдар",  4, "Тұмау"),
        new Patient(102, "Болат",  1, "Жүрек талмасы"),
        new Patient(103, "Гүлнар", 7, "Бас ауруы"),
        new Patient(104, "Дана",   2, "Сынық"),
        new Patient(105, "Ермек",  9, "Тексеру"),
        new Patient(106, "Жанар",  3, "Аллергия"),
        new Patient(107, "Нұрлан", 1, "Қан кету")
    };

    std::cout << "=== 1. Пациенттерді тіркеу (тізім, стек, хэш, BST, AVL, heap) ===\n";
    for (auto* p : patients) {
        list.add(p);
        addToStack(p);
        ht.insert(p);
        bst.insert(p);
        avl = insertAVL(avl, p);
        pq.push(p);
    }
    std::cout << "Тізімдегі пациент саны: " << list.size()
              << " (рекурсия бойынша: " << countPatients(list.size()) << ")\n";
    list.display();

    std::cout << "\n=== 2. Хэш-таблица: ID бойынша іздеу ===\n";
    Patient* f = ht.search(104);
    if (f) { std::cout << "Табылды ID=104:\n"; f->display(); }
    std::cout << "ID=999: " << (ht.search(999) ? "табылды" : "табылмады") << "\n";

    std::cout << "\n=== 3. Стек: undo (соңғы тіркелген пациентті тізімнен алу) ===\n";
    Patient* u = undoAdd();
    if (u) {
        std::cout << "Болдырылмады:\n"; u->display();
        list.removeFirst(); // тізімнің басына соңғы қосылған тұр, сондықтан O(1)
        std::cout << "Тізімдегі пациент саны: " << list.size() << "\n";
    }

    std::cout << "\n=== 4. Quick sort: приоритет бойынша (ауыры бірінші) ===\n";
    std::vector<Patient*> arr(patients.begin(), patients.end());
    quickSort(arr.data(), 0, (int)arr.size() - 1);
    for (auto* p : arr) p->display();

    std::cout << "\n=== 5. Ағаштар: BST пен AVL биіктігін салыстыру ===\n";
    std::cout << "BST биіктігі: " << bst.height() << " (есімдер әліпби ретімен келгендіктен тізбекке айналды)\n";
    std::cout << "AVL биіктігі: " << height(avl) << " (өзін-өзі теңгереді)\n";
    std::cout << "AVL бойынша есім іздеу (Дана): ";
    Patient* a = searchAVL(avl, "Дана");
    std::cout << (a ? "табылды, ID=" + std::to_string(a->id) : "табылмады") << "\n";
    std::cout << "AVL inorder (есім әліпбиі):\n";
    inorderAVL(avl);

    std::cout << "\n=== 6. Приоритет кезегі (heap): қабылдау реті ===\n";
    int order = 1;
    while (!pq.empty()) {
        Patient* p = pq.pop();
        std::cout << "  " << order++ << ". " << p->name << " (приоритет " << p->priority
                  << ", " << p->diagnosis << ")\n";
    }

    std::cout << "\n=== Big-O қысқаша ===\n";
    std::cout << "Тізімге қосу: O(1) | Стек push/pop: O(1) | Хэш іздеу: O(1) орташа\n";
    std::cout << "Quick sort: O(n log n) орташа | BST: O(log n) орташа, O(n) ең нашар\n";
    std::cout << "AVL: O(log n) | Heap push/pop: O(log n)\n";

    freeAVL(avl);
    for (auto* p : patients) delete p;
    return 0;
}
