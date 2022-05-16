#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <functional>
#include "my_algorithms.h"

Huffman::Huffman(const std::string& word) {
    root = new Node();
    this->word = word;
    initFreqs();
    initTree();
    initCodes(root, "");
}

Huffman::Huffman() {
    root = new Node();
}

Huffman::~Huffman() {
    delete root;
}

void Huffman::addWord(char letter, const std::string& code) {
    auto cur = root;
    for(auto& bit : code) {
        if(bit == '0') {
            if(cur->left == nullptr) cur->left = new Node();
            cur = cur->left;
        } else if (bit == '1') {
            if(cur->right == nullptr) cur->right = new Node();
            cur = cur->right;
        }
    }
    cur->letter = letter;
}

std::string Huffman::encode() {
    std::string encoded = "";
    for(auto& letter : word){
        encoded += codes[letter];
    }
    return encoded;
}

std::string Huffman::decode(const std::string& encoded) {
    std::string originalText = "";
    auto cur = root;
    for(auto& bit : encoded){
        cur = bit == '0' ? cur->left : cur->right;
        if(cur->left == nullptr) {
            originalText += cur->letter;
            cur = root;
        }
    }
    return originalText;
}

unsigned char Huffman::binaryStringToByte(std::string& str) {
    unsigned char res = 0;
    while(!str.empty()) {
        res <<= 1;
        if(str.back() == '1') res |= 0x1;
        str.pop_back();
    }
    return res;
}


std::string Huffman::binaryStringToBytes(const std::string& str) {
    std::string bytes = "";
    std::string curByte = "";
    int bitCount = 0;
    for(auto& bit : str) {
        bitCount += 1;
        curByte += bit;
        if(bitCount == 8) {
            bitCount = 0;
            bytes += binaryStringToByte(curByte);
        }
    }
    if(!curByte.empty()) bytes += binaryStringToByte(curByte);
    return bytes;
}

std::string Huffman::getFormatedCodes() {
    std::string printedCodes = "";
    int maxLen = 0;
    for(char letter = firstLetter; charID(letter) < alpha; ++letter) {
        int id = charID(letter);
        if(freqs[id] > 0){
            printedCodes += std::string(1, letter);
            printedCodes += char(int(codes[letter].size()) + '0');
            //std::cout << letter << ": " << codes[letter] << '\n';
            maxLen = std::max(maxLen, (int)codes[letter].size());
            printedCodes += binaryStringToBytes(codes[letter]);
        }   
    }
    //std::cout << "max code len: " << maxLen << '\n';
    return printedCodes;
}
void Huffman::initFreqs() {
    freqs.assign(alpha, 0);
    for(auto& letter : word){
        freqs[charID(letter)] += 1;
    }
}

void Huffman::initTree() {
    auto cmp = [](const Node* a, const Node* b) { return a->freq > b->freq; };
    std::priority_queue<Node*, std::vector<Node*>, decltype(cmp)> pq(cmp);
    for(char letter = firstLetter; charID(letter) < alpha; ++letter) {
        int id = charID(letter);
        if(freqs[id] > 0){
            auto* node = new Node(letter, freqs[id]);
            pq.push(node);
        }
    }
    while(int(pq.size()) > 1) {
        auto left = pq.top();
        pq.pop();
        auto right = pq.top();
        pq.pop();

        auto* node = new Node();
        node->freq = left->freq + right->freq;
        node->left = left;
        node->right = right;
        pq.push(node);
    }
    root = pq.top();
    pq.pop();
}
void Huffman::initCodes(Node* curRoot, std::string curCode) {
    if(curRoot->left == nullptr) {
        codes[curRoot->letter] = curCode;
    } else {
        for(int i = 0; i < 2; ++i) {
            auto nxtRoot = i == 0 ? curRoot->left : curRoot->right;
            initCodes(nxtRoot, curCode += char(i + '0'));
            curCode.pop_back();
        }
    }
}