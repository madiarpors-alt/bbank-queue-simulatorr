// CSE6781: Медициналық кезек басқару жүйесі (Priority Queue + AVL Tree + Hashing)
// Толық жүйе (1-14 апта): тізім, стек, Quick sort, Hash (+rehash), BST, AVL, Heap, BFS, Dijkstra, DP, Greedy
// Компиляция: g++ -std=c++17 medical_queue_full.cpp -o medical && ./medical
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <stack>
#include <algorithm>
#include <queue>
#include <climits>
#include <utility>

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
    int count;
    std::vector<std::list<Patient*>> table;
    int hash(int key, int s) const { return key % s; }
public:
    HashTable(int s) : size(s), count(0), table(s) {}

    // 13-апта: rehashing (load factor > 0.75), O(n)
    void rehash() {
        int newSize = size * 2;
        std::vector<std::list<Patient*>> newTable(newSize);
        for (auto& bucket : table)
            for (auto* p : bucket) newTable[hash(p->id, newSize)].push_back(p);
        table = std::move(newTable);
        size = newSize;
        std::cout << "  [rehash] жаңа өлшем: " << size << "\n";
    }

    void insert(Patient* p) { // O(1) орташа
        table[hash(p->id, size)].push_back(p);
        count++;
        if ((double)count / size > 0.75) rehash();
    }
    Patient* search(int id) { // O(1) орташа
        for (auto* pat : table[hash(id, size)])
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

// ======================= 9-апта: Граф және BFS (аурухана бөлімдері) =======================
const char* DEPT[] = {"Қабылдау", "Кардиология", "Травматология", "Зертхана", "Рентген", "Дәріхана"};
const int D = 6;
std::vector<std::vector<int>> hospital(D); // көрші бөлімдер

void BFS(int start) { // O(V + E)
    std::vector<bool> visited(D, false);
    std::queue<int> q;
    q.push(start);
    visited[start] = true;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        std::cout << "  " << DEPT[u] << "\n";
        for (int v : hospital[u])
            if (!visited[v]) { visited[v] = true; q.push(v); }
    }
}

// ======================= 10-апта: Dijkstra (бөлімдер арасындағы жол, метр) =======================
typedef std::pair<int, int> pii; // <қашықтық, төбе>

void dijkstra(int src, std::vector<std::vector<pii>>& g, std::vector<int>& dist) { // O((V+E) log V)
    std::priority_queue<pii, std::vector<pii>, std::greater<pii>> pq;
    dist[src] = 0;
    pq.push({0, src});
    while (!pq.empty()) {
        pii top = pq.top(); pq.pop();
        int d = top.first, u = top.second;
        if (d > dist[u]) continue;
        for (auto& edge : g[u]) {
            int v = edge.first, w = edge.second;
            if (dist[v] > d + w) { dist[v] = d + w; pq.push({dist[v], v}); }
        }
    }
}

// ======================= 11-апта: 0-1 Knapsack (дәрігер уақытын оңтайландыру) =======================
// time[i] - емдеу ұзақтығы (минут), value[i] - маңыздылығы. O(nW)
int knapsack(int W, const std::vector<int>& time, const std::vector<int>& value) {
    int n = (int)time.size();
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(W + 1, 0));
    for (int i = 1; i <= n; i++)
        for (int w = 0; w <= W; w++) {
            dp[i][w] = dp[i - 1][w];
            if (time[i - 1] <= w)
                dp[i][w] = std::max(dp[i][w], value[i - 1] + dp[i - 1][w - time[i - 1]]);
        }
    return dp[n][W];
}

// ======================= 12-апта: Greedy (Activity Selection, дәрігер кестесі) =======================
// Қабылдаулар {басталуы, аяқталуы}. Ең ерте аяқталатынды таңдау, O(n log n)
std::vector<std::pair<int, int>> activitySelection(std::vector<std::pair<int, int>> acts) {
    std::sort(acts.begin(), acts.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
        return a.second < b.second;
    });
    std::vector<std::pair<int, int>> chosen;
    int lastEnd = INT_MIN;
    for (auto& a : acts)
        if (a.first >= lastEnd) { chosen.push_back(a); lastEnd = a.second; }
    return chosen;
}

// ======================= 14-апта: Толық жүйе =======================
int main() {
    PatientList list;
    HashTable ht(4); // кішкентай өлшем: rehash жұмысын көрсету үшін
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

    std::cout << "=== 1. Пациенттерді тіркеу ===\n";
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
        list.removeFirst();
        std::cout << "Тізімдегі пациент саны: " << list.size() << "\n";
    }

    std::cout << "\n=== 4. Quick sort: приоритет бойынша ===\n";
    std::vector<Patient*> arr(patients.begin(), patients.end());
    quickSort(arr.data(), 0, (int)arr.size() - 1);
    for (auto* p : arr) p->display();

    std::cout << "\n=== 5. BST пен AVL биіктігі ===\n";
    std::cout << "BST биіктігі: " << bst.height() << " | AVL биіктігі: " << height(avl) << "\n";
    Patient* a = searchAVL(avl, "Дана");
    std::cout << "AVL бойынша іздеу (Дана): " << (a ? "табылды, ID=" + std::to_string(a->id) : "табылмады") << "\n";

    std::cout << "\n=== 6. Приоритет кезегі (heap): қабылдау реті ===\n";
    int order = 1;
    while (!pq.empty()) {
        Patient* p = pq.pop();
        std::cout << "  " << order++ << ". " << p->name << " (приоритет " << p->priority
                  << ", " << p->diagnosis << ")\n";
    }

    std::cout << "\n=== 7. Аурухана графы: BFS (Қабылдаудан) ===\n";
    hospital[0] = {1, 2, 3};
    hospital[1] = {0, 3};
    hospital[2] = {0, 4};
    hospital[3] = {0, 1, 5};
    hospital[4] = {2, 5};
    hospital[5] = {3, 4};
    BFS(0);

    std::cout << "\n=== 8. Dijkstra: Қабылдаудан бөлімдерге қашықтық (м) ===\n";
    std::vector<std::vector<pii>> g(D);
    auto addEdge = [&](int x, int y, int w) { g[x].push_back({y, w}); g[y].push_back({x, w}); };
    addEdge(0, 1, 40); addEdge(0, 2, 60); addEdge(0, 3, 25);
    addEdge(1, 3, 15); addEdge(2, 4, 30); addEdge(3, 5, 50); addEdge(4, 5, 20);
    std::vector<int> dist(D, INT_MAX);
    dijkstra(0, g, dist);
    for (int i = 0; i < D; i++) std::cout << "  Қабылдау -> " << DEPT[i] << ": " << dist[i] << " м\n";

    std::cout << "\n=== 9. DP Knapsack: дәрігердің 60 минутындағы ең маңызды емдеулер ===\n";
    std::vector<int> treatTime  = {20, 30, 15, 25, 10};
    std::vector<int> treatValue = {40, 50, 25, 45, 15};
    std::cout << "Ең жоғары маңыздылық: " << knapsack(60, treatTime, treatValue) << "\n";

    std::cout << "\n=== 10. Greedy: дәрігер кестесінен ең көп қабылдау ===\n";
    auto chosen = activitySelection({{9, 10}, {9, 11}, {10, 12}, {11, 13}, {12, 14}, {13, 15}});
    std::cout << "Таңдалған қабылдаулар саны: " << chosen.size() << "\n";
    for (auto& c : chosen) std::cout << "  " << c.first << ":00 - " << c.second << ":00\n";

    std::cout << "\n=== Big-O қысқаша ===\n";
    std::cout << "Тізім: O(1) | Стек: O(1) | Хэш: O(1) орташа | Quick sort: O(n log n)\n";
    std::cout << "BST: O(log n) орташа | AVL: O(log n) | Heap: O(log n)\n";
    std::cout << "BFS: O(V+E) | Dijkstra: O((V+E) log V) | Knapsack: O(nW) | Greedy: O(n log n)\n";

    freeAVL(avl);
    for (auto* p : patients) delete p;
    return 0;
}
