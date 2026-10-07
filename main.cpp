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

void genCode(Node* node, std::string code, std::unordered_map<char, std::string>& codeMap) {
    if (node->left == nullptr && node->right == nullptr) {
        std::cout << node->ch << ": " << code << std::endl;
        codeMap[node->ch] = code;
    }

    if (node->left != nullptr) {
        genCode(node->left, code + "0", codeMap);
    }

    if (node->right != nullptr) {
        genCode(node->right, code + "1", codeMap);
    }
}

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
    std::unordered_map<char, std::string> codeMap;
    genCode(root, "", codeMap);
    input.clear();
    input.seekg(0);
    std::string encodedTxt;
    while (input.get(c)) {
        encodedTxt = encodedTxt + codeMap[c];
    }
    std::cout << "Encoded: " << encodedTxt << std::endl;
    
}
