#include <iostream>
#include <unordered_map>
#include <fstream>
#include <queue>
#include <vector>
#include <string>

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
        if (code.empty()) {
            code = "0";
        }
        codeMap[node->ch] = code;
    }
    if (node->left != nullptr) {
        genCode(node->left, code + "0", codeMap);
    }
    if (node->right != nullptr) {
        genCode(node->right, code + "1", codeMap);
    }
}

void writeTree(Node* node, std::ofstream& output) {
    if (node->left == nullptr && node->right == nullptr) {
        char marker = '1';
        output.write(&marker, 1);
        output.write(&node->ch, 1);
        return;
    }

    char marker = '0';
    output.write(&marker, 1);

    writeTree(node->left, output);
    writeTree(node->right, output);
}

Node* readTree(std::ifstream& input) {
    char marker;
    input.read(&marker, 1);

    if (marker == '1') {
        char ch;
        input.read(&ch, 1);

        return new Node(ch, 0);
    }
    Node* node = new Node('\0', 0);
    node->left = readTree(input);
    node->right = readTree(input);
    return node;
}

void decompress(std::ifstream& input, std::ofstream& output, Node* root, int ogSize) {
    if (root->left == nullptr && root->right == nullptr) {
        for (int i = 0; i < ogSize; i++) {
            output.put(root->ch);
        }

        return;
    }

    Node* current = root;
    unsigned char byte;
    int decodedCount = 0;

    while (decodedCount < ogSize && input.read(reinterpret_cast<char*>(&byte), 1)) {
        for (int i = 7; i >= 0; i--) {
            int bit = (byte >> i) & 1;
            if (bit == 0) {
                current = current->left;
            } else {
                current = current->right;
            }
            if (current->left == nullptr && current->right == nullptr) {
                output.put(current->ch);
                decodedCount++;
                if (decodedCount == ogSize) {
                    break;
                }

                current = root;
            }
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cout << "Usage: main <compress/decompress> <input-file> <output-file>" << std::endl;
        return 1;
    }

    std::string mode = argv[1];
    if (mode == "compress") {
        std::priority_queue<Node*, std::vector<Node*>, Compare> minHeap;
        std::ifstream input(argv[2], std::ios::binary);
        if (!input.is_open()) {
            std::cout << "Could not open input file." << std::endl;
            return 1;
        }
        std::unordered_map<char, int> freqMap;
        char c;
        while (input.get(c)) {
            freqMap[c]++;
        }
        if (freqMap.empty()) {
            std::cout << "File is empty." << std::endl;
            return 0;
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
        std::ofstream output(argv[3], std::ios::binary);

        if (!output.is_open()) {
            std::cout << "Could not create output file." << std::endl;
            return 1;
        }
        writeTree(root, output);
        int ogSize = root->freq;
        output.write(
            reinterpret_cast<char*>(&ogSize),
            sizeof(ogSize)
        );
        for (char bit : encodedTxt) {
            byte = byte << 1;
            if (bit == '1') {
                byte = byte | 1;
            }
            bitCount++;
            if (bitCount == 8) {
                output.write(
                    reinterpret_cast<char*>(&byte),
                    1
                );
                byte = 0;
                bitCount = 0;
            }
        }
        if (bitCount > 0) {
            byte = byte << (8 - bitCount);

            output.write(
                reinterpret_cast<char*>(&byte),
                1
            );
        }
        output.close();
        input.close();
    }
    else if (mode == "decompress") {
        std::ifstream input(argv[2], std::ios::binary);
        if (!input.is_open()) {
            std::cout << "Could not open input file." << std::endl;
            return 1;
        }
        std::ofstream output(argv[3], std::ios::binary);
        if (!output.is_open()) {
            std::cout << "Could not create output file." << std::endl;
            return 1;
        }
        Node* root = readTree(input);
        int ogSize;
        input.read(
            reinterpret_cast<char*>(&ogSize),
            sizeof(ogSize)
        );
        decompress(input, output, root, ogSize);
        output.close();
        input.close();
    }
    else {
        std::cout << "Invalid mode." << std::endl;
        return 1;
    }
    return 0;
}