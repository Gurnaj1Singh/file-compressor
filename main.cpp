#include <iostream>
#include <fstream>
#include <unordered_map>
#include <queue>
#include<bitset>
#include<string>

using namespace std;


struct Node {
    char ch;
    int freq;
    Node *left, *right;

    Node(char c, int f) {
        ch = c;
        freq = f;
        left = right = nullptr;
    }
};

struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq;  // smaller freq gets higher priority
    }
};



unordered_map<char, int> buildFrequencyMap(const string& filename) {
    unordered_map<char, int> freq;
    ifstream infile(filename, ios::binary);

    if (!infile.is_open()) {
        cerr << "Failed to open file.\n";
        return freq;
    }

    char ch;
    while (infile.get(ch)) {
        freq[ch]++;
    }

    infile.close();
    return freq;
}

Node* buildHuffmanTree(unordered_map<char, int>& freqMap) {
    priority_queue<Node*, vector<Node*>, Compare> minHeap;

    // Step 1: Push all characters as leaf nodes
    for (auto& pair : freqMap) {
        minHeap.push(new Node(pair.first, pair.second));
    }

    // Step 2: Merge nodes until one tree remains
    while (minHeap.size() > 1) {
        Node* left = minHeap.top(); minHeap.pop();
        Node* right = minHeap.top(); minHeap.pop();

        Node* merged = new Node('\0', left->freq + right->freq);  // internal node
        merged->left = left;
        merged->right = right;

        minHeap.push(merged);
    }

    return minHeap.top();  // Root of Huffman Tree
}

void generateCodes(Node* root, string currentCode, unordered_map<char, string>& huffmanCodes) {
    if (!root) return;

    // If it's a leaf node, store the code
    if (!root->left && !root->right) {
        huffmanCodes[root->ch] = currentCode;
        return;
    }

    // Recurse left (add '0') and right (add '1')
    generateCodes(root->left, currentCode + "0", huffmanCodes);
    generateCodes(root->right, currentCode + "1", huffmanCodes);
}

void saveHuffmanCodesToFile(const string& outputFile, const unordered_map<char, string>& huffmanCodes) {
    ofstream outfile(outputFile);

    if (!outfile.is_open()) {
        cerr << "Could not open " << outputFile << " for writing.\n";
        return;
    }

    for (auto& pair : huffmanCodes) {
        // Format: char <tab> code
        if (pair.first == '\n') outfile << "\\n\t" << pair.second << "\n";
        else if (pair.first == '\t') outfile << "\\t\t" << pair.second << "\n";
        else outfile << pair.first << "\t" << pair.second << "\n";
    }

    outfile.close();
    cout << "Huffman codes saved to " << outputFile << "\n";
}

void compressFile(const string& inputFilename, const string& outputFilename, const unordered_map<char, string>& huffmanCodes) {
    ifstream infile(inputFilename, ios::binary);
    ofstream outfile(outputFilename, ios::binary);

    if (!infile.is_open() || !outfile.is_open()) {
        cerr << "Error opening input/output file for compression.\n";
        return;
    }

    string bitBuffer = "";  // Buffer to accumulate bits

    char ch;
    while (infile.get(ch)) {
        bitBuffer += huffmanCodes.at(ch);

        // Write bytes when buffer is >= 8 bits
        while (bitBuffer.size() >= 8) {
            bitset<8> byte(bitBuffer.substr(0, 8));
            outfile.put(static_cast<unsigned char>(byte.to_ulong()));
            bitBuffer.erase(0, 8);
        }
    }

    // Handle leftover bits (pad with zeros)
    if (!bitBuffer.empty()) {
        bitBuffer.append(8 - bitBuffer.size(), '0');  // pad to 8 bits
        bitset<8> byte(bitBuffer);
        outfile.put(static_cast<unsigned char>(byte.to_ulong()));
    }

    infile.close();
    outfile.close();
    cout << "File compressed successfully to " << outputFilename << "\n";
}

unordered_map<string, char> loadCodesFromFile(const string& codeFile) {
    unordered_map<string, char> reverseCodes;
    ifstream infile(codeFile);

    if (!infile.is_open()) {
        cerr << "Failed to open code file.\n";
        return reverseCodes;
    }

    string line;
    while (getline(infile, line)) {
        if (line.empty()) continue;

        string key = "", code = "";
        if (line[0] == '\\') {
            if (line.substr(0, 2) == "\\n") key = "\n";
            else if (line.substr(0, 2) == "\\t") key = "\t";
            code = line.substr(3);
        } else {
            key = line[0];
            code = line.substr(2);
        }

        reverseCodes[code] = key[0];
    }

    infile.close();
    return reverseCodes;
}

void decompressFile(const string& compressedFile, const string& outputFile, const unordered_map<string, char>& reverseCodes) {
    ifstream infile(compressedFile, ios::binary);
    ofstream outfile(outputFile);

    if (!infile.is_open() || !outfile.is_open()) {
        cerr << "Failed to open compressed/output file.\n";
        return;
    }

    string bitStream = "";
    char byte;
    while (infile.get(byte)) {
        bitset<8> bits(static_cast<unsigned char>(byte));
        bitStream += bits.to_string();
    }

    string current = "";
    for (char bit : bitStream) {
        current += bit;
        if (reverseCodes.count(current)) {
            outfile.put(reverseCodes.at(current));
            current = "";
        }
    }

    infile.close();
    outfile.close();
    cout << "File decompressed successfully to " << outputFile << "\n";
}


int main() {
    string filename = "input.txt";
    unordered_map<char, int> freq = buildFrequencyMap(filename);

    Node* root = buildHuffmanTree(freq);

    unordered_map<char, string> huffmanCodes;
    generateCodes(root, "", huffmanCodes);

    cout << "Huffman Codes:\n";
    for (auto& pair : huffmanCodes) {
        cout << "'" << pair.first << "' : " << pair.second << endl;
    }

    saveHuffmanCodesToFile("codes.txt", huffmanCodes);
    compressFile("input.txt", "compressed.bin", huffmanCodes);

    unordered_map<string, char> reverseCodes = loadCodesFromFile("codes.txt");
    decompressFile("compressed.bin", "decompressed.txt", reverseCodes);


    return 0;
}


