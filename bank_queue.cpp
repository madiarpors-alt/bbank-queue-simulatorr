// CSE6781: Банк кезегі симуляторы (Queue + Priority Queue + Stack)
// Компиляция: g++ -std=c++17 bank_queue.cpp -o bank_queue
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <deque>
#include <algorithm>
#include <climits>
#include <utility>

// ======================= 1-апта: Customer класы, Big-O =======================
class Customer {
public:
    int id;
    std::string name;
    int priority;      // 1 - VIP, 2 - жеңілдікті, 3 - қарапайым
    int arrivalTime;
    int serviceTime;   // минутпен

    Customer(int i, std::string n, int p, int a, int s)
        : id(i), name(n), priority(p), arrivalTime(a), serviceTime(s) {}

    void display() const { // O(1)
        std::cout << "  #" << id << " " << name << " | приоритет=" << priority
                  << " | келді=" << arrivalTime << " | қызмет=" << serviceTime << " мин\n";
    }
};

// Рекурсия: n клиенттің жалпы қызмет уақыты, O(n)
int totalTime(int n, int t) { return n <= 0 ? 0 : t + totalTime(n - 1, t); }

// ======================= 2-апта: Linked List кезегі =======================
struct Node {
    Customer* data;
    Node* next;
    Node(Customer* c) : data(c), next(nullptr) {}
};

class LinkedQueue {
    Node* head = nullptr;
    Node* tail = nullptr;
    int count = 0;
public:
    ~LinkedQueue() {
        while (head) { Node* n = head; head = head->next; delete n; }
    }
    void enqueue(Customer* c) { // O(1)
        Node* n = new Node(c);
        if (tail) tail->next = n; else head = n;
        tail = n;
        count++;
    }
    Customer* dequeue() { // O(1)
        if (!head) return nullptr;
        Node* n = head;
        Customer* c = n->data;
        head = head->next;
        if (!head) tail = nullptr;
        delete n;
        count--;
        return c;
    }
    bool empty() const { return count == 0; }
    int size() const { return count; }
};

// ======================= 3-апта: Stack (undo) және Deque =======================
std::stack<Customer*> undoStack;
std::deque<Customer*> returnLine; // қайта оралған клиенттер

void registerCustomer(LinkedQueue& q, Customer* c) {
    q.enqueue(c);
    undoStack.push(c); // O(1)
}

Customer* undoLast() { // O(1)
    if (undoStack.empty()) return nullptr;
    Customer* c = undoStack.top();
    undoStack.pop();
    return c;
}

void returnToFront(Customer* c) { returnLine.push_front(c); } // O(1)

// ======================= 4-апта: Merge sort =======================
void mergeArr(std::vector<Customer*>& a, int l, int m, int r) {
    std::vector<Customer*> tmp;
    int i = l, j = m + 1;
    while (i <= m && j <= r)
        tmp.push_back(a[i]->serviceTime <= a[j]->serviceTime ? a[i++] : a[j++]);
    while (i <= m) tmp.push_back(a[i++]);
    while (j <= r) tmp.push_back(a[j++]);
    for (int k = 0; k < (int)tmp.size(); k++) a[l + k] = tmp[k];
}

void mergeSort(std::vector<Customer*>& a, int l, int r) { // O(n log n)
    if (l >= r) return;
    int m = (l + r) / 2;
    mergeSort(a, l, m);
    mergeSort(a, m + 1, r);
    mergeArr(a, l, m, r);
}

// ======================= 5-апта және 13-апта: Hash Table + rehash =======================
class HashTable {
    int size;
    int count;
    std::vector<std::list<Customer*>> table;
    int hash(int key, int s) const { return key % s; }
public:
    HashTable(int s) : size(s), count(0), table(s) {}

    // 13-апта: rehashing, O(n)
    void rehash() {
        int newSize = size * 2;
        std::vector<std::list<Customer*>> newTable(newSize);
        for (auto& bucket : table)
            for (auto* c : bucket)
                newTable[hash(c->id, newSize)].push_back(c);
        table = std::move(newTable);
        size = newSize;
        std::cout << "  [rehash] жаңа өлшем: " << size << "\n";
    }

    void insert(Customer* c) { // O(1) орташа
        table[hash(c->id, size)].push_back(c);
        count++;
        if ((double)count / size > 0.75) rehash(); // load factor
    }

    Customer* search(int id) { // O(1) орташа
        for (auto* c : table[hash(id, size)])
            if (c->id == id) return c;
        return nullptr;
    }
};

// Binary search (қызмет уақыты бойынша сұрыпталған массивте), O(log n)
int binarySearchByTime(std::vector<Customer*>& a, int t) {
    int l = 0, r = (int)a.size() - 1;
    while (l <= r) {
        int m = (l + r) / 2;
        if (a[m]->serviceTime == t) return m;
        if (a[m]->serviceTime < t) l = m + 1; else r = m - 1;
    }
    return -1;
}

// ======================= 6-апта: BST =======================
struct TreeNode {
    Customer* data;
    TreeNode* left;
    TreeNode* right;
    int height;
    TreeNode(Customer* c) : data(c), left(nullptr), right(nullptr), height(1) {}
};

TreeNode* insertBST(TreeNode* node, Customer* c) { // O(log n) орташа
    if (!node) return new TreeNode(c);
    if (c->id < node->data->id) node->left = insertBST(node->left, c);
    else if (c->id > node->data->id) node->right = insertBST(node->right, c);
    return node;
}

void inorder(TreeNode* n) { // ID өсу ретімен
    if (!n) return;
    inorder(n->left);
    n->data->display();
    inorder(n->right);
}

void freeTree(TreeNode* n) {
    if (!n) return;
    freeTree(n->left);
    freeTree(n->right);
    delete n;
}

// ======================= 7-апта: AVL ағашы =======================
int height(TreeNode* n) { return n ? n->height : 0; }
int getBalance(TreeNode* n) { return n ? height(n->left) - height(n->right) : 0; }

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

TreeNode* insertAVL(TreeNode* node, Customer* c) { // O(log n)
    if (!node) return new TreeNode(c);
    if (c->id < node->data->id) node->left = insertAVL(node->left, c);
    else if (c->id > node->data->id) node->right = insertAVL(node->right, c);
    else return node;

    node->height = 1 + std::max(height(node->left), height(node->right));
    int bal = getBalance(node);

    if (bal > 1 && c->id < node->left->data->id) return rightRotate(node);          // LL
    if (bal < -1 && c->id > node->right->data->id) return leftRotate(node);         // RR
    if (bal > 1 && c->id > node->left->data->id) {                                  // LR
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (bal < -1 && c->id < node->right->data->id) {                                // RL
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

Customer* searchTree(TreeNode* n, int id) { // O(log n)
    while (n) {
        if (id == n->data->id) return n->data;
        n = (id < n->data->id) ? n->left : n->right;
    }
    return nullptr;
}

// ======================= 7-апта: Priority Queue (Min-Heap) =======================
class PriorityQueue {
    std::vector<Customer*> h;
    bool less(Customer* a, Customer* b) const { // приоритет, содан соң келу уақыты
        return a->priority != b->priority ? a->priority < b->priority
                                          : a->arrivalTime < b->arrivalTime;
    }
public:
    void push(Customer* c) { // O(log n)
        h.push_back(c);
        int i = (int)h.size() - 1;
        while (i > 0 && less(h[i], h[(i - 1) / 2])) {
            std::swap(h[i], h[(i - 1) / 2]);
            i = (i - 1) / 2;
        }
    }
    Customer* pop() { // O(log n)
        if (h.empty()) return nullptr;
        Customer* top = h[0];
        h[0] = h.back();
        h.pop_back();
        int i = 0, n = (int)h.size();
        while (true) {
            int l = 2 * i + 1, r = 2 * i + 2, m = i;
            if (l < n && less(h[l], h[m])) m = l;
            if (r < n && less(h[r], h[m])) m = r;
            if (m == i) break;
            std::swap(h[i], h[m]);
            i = m;
        }
        return top;
    }
    bool empty() const { return h.empty(); }
    int size() const { return (int)h.size(); }
};

// ======================= 9-апта: Граф және BFS =======================
std::vector<std::vector<int>> branchGraph(6);

void BFS(int start) { // O(V + E)
    std::vector<bool> visited(branchGraph.size(), false);
    std::queue<int> q;
    q.push(start);
    visited[start] = true;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        std::cout << "Филиал " << u << "  ";
        for (int v : branchGraph[u])
            if (!visited[v]) { visited[v] = true; q.push(v); }
    }
    std::cout << "\n";
}

// ======================= 10-апта: Dijkstra =======================
typedef std::pair<int, int> pii; // <қашықтық, төбе>

void dijkstra(int src, std::vector<std::vector<pii>>& g, std::vector<int>& dist) { // O((V+E) log V)
    std::priority_queue<pii, std::vector<pii>, std::greater<pii>> pq;
    dist[src] = 0;
    pq.push({0, src});
    while (!pq.empty()) {
        pii top = pq.top();
        pq.pop();
        int d = top.first, u = top.second;
        if (d > dist[u]) continue;
        for (auto& edge : g[u]) {
            int v = edge.first, w = edge.second;
            if (dist[v] > d + w) {
                dist[v] = d + w;
                pq.push({dist[v], v});
            }
        }
    }
}

// ======================= 11-апта: 0-1 Knapsack (DP) =======================
int knapsack(int W, std::vector<int>& time, std::vector<int>& value) { // O(nW)
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

// ======================= 12-апта: Greedy (Shortest Job First) =======================
double avgWaitSJF(std::vector<Customer*> list) { // O(n log n)
    std::sort(list.begin(), list.end(),
              [](Customer* a, Customer* b) { return a->serviceTime < b->serviceTime; });
    int time = 0;
    double totalWait = 0;
    for (auto* c : list) { totalWait += time; time += c->serviceTime; }
    return list.empty() ? 0 : totalWait / list.size();
}

double avgWaitFIFO(const std::vector<Customer*>& list) { // салыстыру үшін, O(n)
    int time = 0;
    double totalWait = 0;
    for (auto* c : list) { totalWait += time; time += c->serviceTime; }
    return list.empty() ? 0 : totalWait / list.size();
}

// ======================= 14-апта: Толық жүйе =======================
int main() {
    HashTable ht(5); // кішкентай өлшем: rehash жұмысын көрсету үшін
    PriorityQueue vipQ;
    LinkedQueue normalQ;
    TreeNode* history = nullptr;

    std::vector<Customer*> all = {
        new Customer(1, "Али",    3, 0, 5),
        new Customer(2, "Айгүл",  1, 1, 3),
        new Customer(3, "Берік",  3, 2, 8),
        new Customer(4, "Дана",   2, 3, 4),
        new Customer(5, "Ермек",  3, 4, 2),
        new Customer(6, "Жанар",  1, 5, 6)
    };

    std::cout << "=== 1. Клиенттерді тіркеу ===\n";
    for (auto* c : all) {
        ht.insert(c);
        if (c->priority < 3) vipQ.push(c);
        else registerCustomer(normalQ, c);
        c->display();
    }
    std::cout << "VIP/жеңілдікті кезек: " << vipQ.size()
              << ", қарапайым кезек: " << normalQ.size() << "\n";

    std::cout << "\n=== 2. Хэш-таблицадан іздеу (ID=4) ===\n";
    Customer* f = ht.search(4);
    if (f) f->display();

    std::cout << "\n=== 3. Undo стегі: соңғы тіркелген қарапайым клиент ===\n";
    Customer* u = undoLast();
    if (u) { std::cout << "Стектің жоғарғысы: "; u->display(); returnToFront(u); }
    std::cout << "Deque басында тұрған клиент: " << returnLine.front()->name << "\n";

    std::cout << "\n=== 4. Симуляция: бір кассир ===\n";
    int time = 0, served = 0;
    double totalWait = 0;
    while (!vipQ.empty() || !normalQ.empty()) {
        Customer* c = !vipQ.empty() ? vipQ.pop() : normalQ.dequeue();
        int start = std::max(time, c->arrivalTime);
        int wait = start - c->arrivalTime;
        totalWait += wait;
        time = start + c->serviceTime;
        history = insertAVL(history, c);
        served++;
        std::cout << "  Қызмет: " << c->name << " (P" << c->priority
                  << "), күтті " << wait << " мин, аяқтады t=" << time << "\n";
    }
    std::cout << "Қызмет көрсетілді: " << served
              << ", орташа күту: " << (served ? totalWait / served : 0) << " мин\n";

    std::cout << "\n=== 5. Тарих (AVL, ID бойынша сұрыпталған) ===\n";
    inorder(history);
    std::cout << "AVL биіктігі: " << height(history) << "\n";
    Customer* hc = searchTree(history, 3);
    if (hc) { std::cout << "AVL-дан табылды ID=3: "; hc->display(); }

    std::cout << "\n=== 6. Merge sort (қызмет уақыты бойынша) және Binary search ===\n";
    std::vector<Customer*> sorted = all;
    mergeSort(sorted, 0, (int)sorted.size() - 1);
    for (auto* c : sorted) c->display();
    int idx = binarySearchByTime(sorted, 8);
    std::cout << "Қызмет уақыты 8 мин клиент индексі: " << idx << "\n";

    std::cout << "\n=== 7. Филиалдар графы: BFS ===\n";
    branchGraph[0] = {1, 2};
    branchGraph[1] = {0, 3};
    branchGraph[2] = {0, 3, 4};
    branchGraph[3] = {1, 2, 5};
    branchGraph[4] = {2, 5};
    branchGraph[5] = {3, 4};
    BFS(0);

    std::cout << "\n=== 8. Dijkstra: 0-филиалдан қашықтықтар (км) ===\n";
    std::vector<std::vector<pii>> g(6);
    auto addEdge = [&](int a, int b, int w) { g[a].push_back({b, w}); g[b].push_back({a, w}); };
    addEdge(0, 1, 4); addEdge(0, 2, 2); addEdge(1, 3, 5);
    addEdge(2, 3, 8); addEdge(2, 4, 10); addEdge(3, 5, 2); addEdge(4, 5, 3);
    std::vector<int> dist(6, INT_MAX);
    dijkstra(0, g, dist);
    for (int i = 0; i < 6; i++) std::cout << "  0 -> " << i << ": " << dist[i] << "\n";

    std::cout << "\n=== 9. DP Knapsack: 15 минутта ең көп пайда ===\n";
    std::vector<int> times = {5, 3, 8, 4, 2, 6};
    std::vector<int> values = {10, 15, 12, 8, 5, 20};
    std::cout << "Ең көп пайда: " << knapsack(15, times, values) << "\n";

    std::cout << "\n=== 10. Greedy: SJF және FIFO салыстыру ===\n";
    std::cout << "FIFO орташа күту: " << avgWaitFIFO(all) << " мин\n";
    std::cout << "SJF орташа күту:  " << avgWaitSJF(all) << " мин\n";

    std::cout << "\n=== Big-O қысқаша ===\n";
    std::cout << "Queue: O(1) | Heap: O(log n) | Hash: O(1) орташа | AVL: O(log n)\n";
    std::cout << "Merge sort: O(n log n) | Dijkstra: O((V+E) log V) | Knapsack: O(nW)\n";

    freeTree(history);
    for (auto* c : all) delete c;
    return 0;
}
