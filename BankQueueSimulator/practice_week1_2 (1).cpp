// CSE6781: Практикалық есептер. Кіріспе тізім, 1-апта және 2-апта
// Компиляция: g++ -std=c++17 practice_week1_2.cpp -o practice && ./practice
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <climits>
#include <cassert>
using namespace std;

// ============================================================
//  КІРІСПЕ ТІЗІМ (рекурсия және базалық талдау)
// ============================================================

// 1. Рекурсиялық факториал, O(n)
long long factorial(int n) { return n <= 1 ? 1 : n * factorial(n - 1); }

// 2. Sum of Array (Codeforces 339A стиліндегі), O(n)
long long sumArray(const vector<int>& a) {
    long long s = 0;
    for (int x : a) s += x;
    return s;
}

// 3. Power Function: рекурсиялық дәреже, O(log y)
long long power(long long x, int y) {
    if (y == 0) return 1;
    long long half = power(x, y / 2);
    return (y % 2 == 0) ? half * half : half * half * x;
}

// 4. GCD (Евклид алгоритмі), O(log min(a,b))
int gcd(int a, int b) { return b == 0 ? a : gcd(b, a % b); }

// 5. Palindrome: рекурсиялық (жол) және LeetCode #9 (сан)
bool isPalRec(const string& s, int l, int r) {
    if (l >= r) return true;
    if (s[l] != s[r]) return false;
    return isPalRec(s, l + 1, r - 1); // O(n)
}
bool isPalindromeNumber(int x) { // LeetCode #9
    if (x < 0 || (x % 10 == 0 && x != 0)) return false;
    int rev = 0;
    while (x > rev) { rev = rev * 10 + x % 10; x /= 10; }
    return x == rev || x == rev / 10; // O(log10 x)
}

// 6. Binary Search Template, O(log n)
int binarySearch(const vector<int>& a, int target) {
    int l = 0, r = (int)a.size() - 1;
    while (l <= r) {
        int m = l + (r - l) / 2;
        if (a[m] == target) return m;
        if (a[m] < target) l = m + 1; else r = m - 1;
    }
    return -1;
}

// 7. Tower of Hanoi, O(2^n) жылжыту саны 2^n - 1
int hanoi(int n, char from, char to, char via, bool print = false) {
    if (n == 0) return 0;
    int c = hanoi(n - 1, from, via, to, print);
    if (print) cout << "  диск " << n << ": " << from << " -> " << to << "\n";
    return c + 1 + hanoi(n - 1, via, to, from, print);
}

// 8. Print Numbers Recursively: 1-ден n-ге дейін, O(n)
void printNumbers(int n, int cur = 1) {
    if (cur > n) return;
    cout << cur << (cur == n ? "\n" : " ");
    printNumbers(n, cur + 1);
}

// 9. Reverse String (LeetCode #344), рекурсия, O(n)
void reverseRec(string& s, int l, int r) {
    if (l >= r) return;
    swap(s[l], s[r]);
    reverseRec(s, l + 1, r - 1);
}

// 10. Sum of Digits (HackerRank), O(log10 n)
int sumDigits(int n) { return n == 0 ? 0 : n % 10 + sumDigits(n / 10); }

// ============================================================
//  1-АПТА
// ============================================================

// 1. Fibonacci Number (LeetCode #509)
int fibRec(int n) { return n <= 1 ? n : fibRec(n - 1) + fibRec(n - 2); } // O(2^n)
int fibIter(int n) { // O(n), O(1) жад
    if (n <= 1) return n;
    int a = 0, b = 1;
    for (int i = 2; i <= n; i++) { int c = a + b; a = b; b = c; }
    return b;
}

// 2. Climbing Stairs (LeetCode #70), O(n)
int climbStairs(int n) {
    if (n <= 2) return n;
    int a = 1, b = 2;
    for (int i = 3; i <= n; i++) { int c = a + b; a = b; b = c; }
    return b;
}

// 3. Factorial және рекурсиялық қосынды (HackerRank), O(n)
long long recursiveSum(int n) { return n <= 0 ? 0 : n + recursiveSum(n - 1); }

// 4. Maximum Depth of Binary Tree (LeetCode #104), O(n)
struct TreeNode {
    int val;
    TreeNode *left, *right;
    TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
};
int maxDepth(TreeNode* root) {
    if (!root) return 0;
    return 1 + max(maxDepth(root->left), maxDepth(root->right));
}

// 5. Count Vowels: рекурсиялық, O(n)
int countVowels(const string& s, int i = 0) {
    if (i == (int)s.size()) return 0;
    char c = tolower(s[i]);
    int v = (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') ? 1 : 0;
    return v + countVowels(s, i + 1);
}

// 6. Time Complexity Quiz: үлгі циклдер мен күрделіліктері
//   for (i=0;i<n;i++) x++;                        -> O(n)
//   for (i=0;i<n;i++) for (j=0;j<n;j++) x++;      -> O(n^2)
//   for (i=1;i<n;i*=2) x++;                       -> O(log n)
//   for (i=0;i<n;i++) for (j=0;j<i;j++) x++;      -> O(n^2)  (n(n-1)/2)
//   fib(n-1)+fib(n-2) рекурсиясы                  -> O(2^n)
//   Binary search                                 -> O(log n)

// 7. Rotate Array (LeetCode #189): reverse тәсілі, O(n) уақыт, O(1) жад
void rotateArray(vector<int>& nums, int k) {
    int n = nums.size();
    if (n == 0) return;
    k %= n;
    reverse(nums.begin(), nums.end());
    reverse(nums.begin(), nums.begin() + k);
    reverse(nums.begin() + k, nums.end());
}

// 8. Find the Duplicate Number (LeetCode #287): Floyd, O(n) уақыт, O(1) жад
int findDuplicate(vector<int>& nums) {
    int slow = nums[0], fast = nums[0];
    do { slow = nums[slow]; fast = nums[nums[fast]]; } while (slow != fast);
    slow = nums[0];
    while (slow != fast) { slow = nums[slow]; fast = nums[fast]; }
    return slow;
}

// ============================================================
//  2-АПТА
// ============================================================

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int v) : val(v), next(nullptr) {}
};
ListNode* makeList(const vector<int>& v) {
    ListNode dummy(0); ListNode* t = &dummy;
    for (int x : v) { t->next = new ListNode(x); t = t->next; }
    return dummy.next;
}
vector<int> toVec(ListNode* h) {
    vector<int> v;
    for (; h; h = h->next) v.push_back(h->val);
    return v;
}

// 1. Merge Two Sorted Lists (LeetCode #21), O(n+m)
ListNode* mergeTwoLists(ListNode* a, ListNode* b) {
    ListNode dummy(0); ListNode* t = &dummy;
    while (a && b) {
        if (a->val <= b->val) { t->next = a; a = a->next; }
        else { t->next = b; b = b->next; }
        t = t->next;
    }
    t->next = a ? a : b;
    return dummy.next;
}

// 2. Remove Duplicates from Sorted Array (LeetCode #26), O(n)
int removeDuplicates(vector<int>& nums) {
    if (nums.empty()) return 0;
    int k = 1;
    for (int i = 1; i < (int)nums.size(); i++)
        if (nums[i] != nums[k - 1]) nums[k++] = nums[i];
    return k;
}

// 3. Reverse Linked List (LeetCode #206), O(n)
ListNode* reverseList(ListNode* head) {
    ListNode* prev = nullptr;
    while (head) { ListNode* nx = head->next; head->next = prev; prev = head; head = nx; }
    return prev;
}

// 4. Middle of the Linked List (LeetCode #876), O(n)
ListNode* middleNode(ListNode* head) {
    ListNode *slow = head, *fast = head;
    while (fast && fast->next) { slow = slow->next; fast = fast->next->next; }
    return slow;
}

// 5. Linked List Cycle (LeetCode #141), O(n)
bool hasCycle(ListNode* head) {
    ListNode *slow = head, *fast = head;
    while (fast && fast->next) {
        slow = slow->next; fast = fast->next->next;
        if (slow == fast) return true;
    }
    return false;
}

// 6. Rotate Array: жоғарыдағы rotateArray (1-апта, №7)

// 7. Plus One (LeetCode #66), O(n)
vector<int> plusOne(vector<int> d) {
    for (int i = (int)d.size() - 1; i >= 0; i--) {
        if (d[i] < 9) { d[i]++; return d; }
        d[i] = 0;
    }
    d.insert(d.begin(), 1);
    return d;
}

// 8. Move Zeroes (LeetCode #283), O(n)
void moveZeroes(vector<int>& nums) {
    int k = 0;
    for (int i = 0; i < (int)nums.size(); i++)
        if (nums[i] != 0) swap(nums[k++], nums[i]);
}

// 9. Intersection of Two Arrays (LeetCode #349), O(n+m)
vector<int> intersection(vector<int>& a, vector<int>& b) {
    unordered_set<int> s(a.begin(), a.end()), res;
    for (int x : b) if (s.count(x)) res.insert(x);
    return vector<int>(res.begin(), res.end());
}

// 10. Valid Sudoku (LeetCode #36), O(81)
bool isValidSudoku(vector<vector<char>>& b) {
    bool row[9][9] = {}, col[9][9] = {}, box[9][9] = {};
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++) {
            if (b[i][j] == '.') continue;
            int d = b[i][j] - '1', k = (i / 3) * 3 + j / 3;
            if (row[i][d] || col[j][d] || box[k][d]) return false;
            row[i][d] = col[j][d] = box[k][d] = true;
        }
    return true;
}

// 11. Remove Element (LeetCode #27), O(n)
int removeElement(vector<int>& nums, int val) {
    int k = 0;
    for (int x : nums) if (x != val) nums[k++] = x;
    return k;
}

// 12. Find Pivot Index (LeetCode #724), O(n)
int pivotIndex(vector<int>& nums) {
    int total = 0, left = 0;
    for (int x : nums) total += x;
    for (int i = 0; i < (int)nums.size(); i++) {
        if (left == total - left - nums[i]) return i;
        left += nums[i];
    }
    return -1;
}

// 13. Majority Element (LeetCode #169): Boyer-Moore, O(n), O(1) жад
int majorityElement(vector<int>& nums) {
    int cand = 0, cnt = 0;
    for (int x : nums) {
        if (cnt == 0) cand = x;
        cnt += (x == cand) ? 1 : -1;
    }
    return cand;
}

// 14. Third Maximum Number (LeetCode #414), O(n)
int thirdMax(vector<int>& nums) {
    set<int> s;
    for (int x : nums) {
        s.insert(x);
        if (s.size() > 3) s.erase(s.begin());
    }
    return s.size() == 3 ? *s.begin() : *s.rbegin();
}

// 15. Product of Array Except Self (LeetCode #238), O(n), бөлусіз
vector<int> productExceptSelf(vector<int>& nums) {
    int n = nums.size();
    vector<int> res(n, 1);
    for (int i = 1; i < n; i++) res[i] = res[i - 1] * nums[i - 1];
    int right = 1;
    for (int i = n - 1; i >= 0; i--) { res[i] *= right; right *= nums[i]; }
    return res;
}

// 16. Best Time to Buy and Sell Stock (LeetCode #121), O(n)
int maxProfit(vector<int>& prices) {
    int minP = INT_MAX, best = 0;
    for (int p : prices) { minP = min(minP, p); best = max(best, p - minP); }
    return best;
}

// 17. Two Sum (LeetCode #1), O(n)
vector<int> twoSum(vector<int>& nums, int target) {
    unordered_map<int, int> mp;
    for (int i = 0; i < (int)nums.size(); i++) {
        auto it = mp.find(target - nums[i]);
        if (it != mp.end()) return {it->second, i};
        mp[nums[i]] = i;
    }
    return {};
}

// ============================================================
//  ТЕСТТЕР
// ============================================================
int main() {
    // Кіріспе
    assert(factorial(5) == 120);
    assert(sumArray({1, 2, 3, 4}) == 10);
    assert(power(2, 10) == 1024);
    assert(gcd(48, 18) == 6);
    assert(isPalRec("level", 0, 4) && !isPalRec("hello", 0, 4));
    assert(isPalindromeNumber(121) && !isPalindromeNumber(-121) && !isPalindromeNumber(10));
    assert(binarySearch({1, 3, 5, 7, 9}, 7) == 3 && binarySearch({1, 3, 5}, 4) == -1);
    assert(hanoi(3, 'A', 'C', 'B') == 7);
    string s = "hello"; reverseRec(s, 0, 4); assert(s == "olleh");
    assert(sumDigits(1234) == 10);
    cout << "Hanoi (n=2):\n"; hanoi(2, 'A', 'C', 'B', true);
    cout << "printNumbers(5): "; printNumbers(5);

    // 1-апта
    assert(fibRec(10) == 55 && fibIter(10) == 55);
    assert(climbStairs(5) == 8);
    assert(recursiveSum(10) == 55);
    TreeNode r(1); r.left = new TreeNode(2); r.right = new TreeNode(3); r.left->left = new TreeNode(4);
    assert(maxDepth(&r) == 3);
    assert(countVowels("Algorithms") == 3);
    vector<int> rot = {1, 2, 3, 4, 5, 6, 7}; rotateArray(rot, 3);
    assert((rot == vector<int>{5, 6, 7, 1, 2, 3, 4}));
    vector<int> dup = {1, 3, 4, 2, 2}; assert(findDuplicate(dup) == 2);

    // 2-апта
    assert((toVec(mergeTwoLists(makeList({1, 2, 4}), makeList({1, 3, 4}))) == vector<int>{1, 1, 2, 3, 4, 4}));
    vector<int> d1 = {0, 0, 1, 1, 1, 2, 2, 3}; assert(removeDuplicates(d1) == 4);
    assert((toVec(reverseList(makeList({1, 2, 3}))) == vector<int>{3, 2, 1}));
    assert(middleNode(makeList({1, 2, 3, 4, 5}))->val == 3);
    ListNode* c = makeList({1, 2, 3}); c->next->next->next = c->next;
    assert(hasCycle(c) && !hasCycle(makeList({1, 2, 3})));
    assert((plusOne({1, 9, 9}) == vector<int>{2, 0, 0}) && (plusOne({9}) == vector<int>{1, 0}));
    vector<int> z = {0, 1, 0, 3, 12}; moveZeroes(z); assert((z == vector<int>{1, 3, 12, 0, 0}));
    vector<int> a1 = {1, 2, 2, 1}, a2 = {2, 2}; auto in = intersection(a1, a2); assert(in.size() == 1 && in[0] == 2);
    vector<vector<char>> sd(9, vector<char>(9, '.')); sd[0][0] = '5'; sd[0][1] = '5'; assert(!isValidSudoku(sd));
    sd[0][1] = '3'; assert(isValidSudoku(sd));
    vector<int> re = {3, 2, 2, 3}; assert(removeElement(re, 3) == 2);
    vector<int> pv = {1, 7, 3, 6, 5, 6}; assert(pivotIndex(pv) == 3);
    vector<int> mj = {2, 2, 1, 1, 1, 2, 2}; assert(majorityElement(mj) == 2);
    vector<int> t1 = {2, 2, 3, 1}, t2 = {1, 2}; assert(thirdMax(t1) == 1 && thirdMax(t2) == 2);
    vector<int> pe = {1, 2, 3, 4}; assert((productExceptSelf(pe) == vector<int>{24, 12, 8, 6}));
    vector<int> pr = {7, 1, 5, 3, 6, 4}; assert(maxProfit(pr) == 5);
    vector<int> ts = {2, 7, 11, 15}; assert((twoSum(ts, 9) == vector<int>{0, 1}));

    cout << "Барлық тесттер сәтті өтті.\n";
    return 0;
}
