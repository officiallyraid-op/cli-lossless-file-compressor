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
    unsigned char byte = 0;
    int bitCount = 0;
    std::ofstream output("compressed", std::ios::binary);
    size_t uniqueChars = freqMap.size();
    output.write(reinterpret_cast<char*>(&uniqueChars), sizeof(uniqueChars)); //*
    for (const auto& [ch, freq] : freqMap) { //**
        output.write(&ch, sizeof(ch));
        output.write(
            reinterpret_cast<const char*>(&freq),
            sizeof(freq)
        );
    }
    int ogSize = root->freq;
    output.write(reinterpret_cast<char*>(&ogSize), sizeof(ogSize));

    for (char bit : encodedTxt) { //*
        byte = byte << 1;

        if (bit == '1') {
            byte = byte | 1;
        }

        bitCount++;

        if (bitCount == 8) {
            output.write(reinterpret_cast<char*>(&byte), 1);
            byte = 0;
            bitCount = 0;
        }
    }
    if (bitCount > 0) {
        byte = byte << (8 - bitCount);
        output.write(reinterpret_cast<char*>(&byte), 1);
    }
    output.close();
}
