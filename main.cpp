#include <iostream>
#include <unordered_map>
#include <fstream>
#include <queue>
#include <vector>

struct Node {
    char ch;
    int freq;
    Node* left;
    Node* right;

    Node(char c, int f) {
        ch = c;
        freq = f;
        left = nullptr;
        right = nullptr;
    }
};

struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq;
    }
};

int main() {
    std::priority_queue<
        Node*,
        std::vector<Node*>,
        Compare
    > minHeap;
    std::ifstream input("test.txt");
    std::unordered_map<char, int> freqMap;
    char c;
    while (input.get(c)) {
        freqMap[c]++;
    }
    for (const auto& [ch, f] : freqMap) {
        Node* node = new Node(ch, f);
        minHeap.push(node);
    }
    while (minHeap.size() > 1) {
        Node* left = minHeap.top();
        minHeap.pop();
        Node* right = minHeap.top();
        minHeap.pop();
        int totalFreq = left->freq + right->freq;
        Node* parent = new Node('\0', totalFreq);
        parent->left = left;
        parent->right = right;
        minHeap.push(parent);
    }
    Node* root = minHeap.top();
}
