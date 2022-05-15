#include<string>
#include<vector>
#include<unordered_map>

static int charID(char c) { return int(c) - int(' '); }
const static int alpha = 95;

#ifndef HUFFMAN_H
#define HUFFMAN_H
struct Huffman {
public:
    struct Node {
        char letter;
        int freq;
        Node* left;
        Node* right;
        Node(char letter = '\0', int freq = 0, Node* left = nullptr, Node* right = nullptr):
            letter(letter), freq(freq), left(left), right(right){}
        ~Node() {
            delete left;
            delete right;
        }
        bool operator < (const Node& other) const {
            return freq < other.freq;
        }
    };

    Huffman(const std::string& word);
    ~Huffman();
    std::string encode();
    std::string decode(const std::string& encoded);
    std::string getFormatedCodes();
private:
    Node* root;
    std::string word;
    std::vector<int> freqs;
    std::unordered_map<char, std::string> codes;

    void initFreqs();
    void initTree();
    void initCodes(Node* curRoot, std::string curCode);
};
#endif // HUFFMAN_H

#ifndef SUFFIXARRAY_H
#define SUFFIXARRAY_H
struct SuffixArray {
public:
    SuffixArray(const std::string& text);
    SuffixArray(const std::vector<int>& sa, const std::vector<int>& freq);
    std::vector<int> matchPattern(const std::string& pattern);
    
    std::string getText();
    std::vector<int> getSuffixArray();
    int& operator[](int index);
private:
    std::string text;
    std::vector<int> sa;

    void countSort(const std::vector<int>& count);
    void buildSuffixArray();
    int lowerBound(const std::string& pattern);
    int upperBound(const std::string& pattern);
};
#endif // SUFFIXARRAY_H
#ifndef BINIO_H
#define BINIO_H
struct BinIO{
    public:
        BinIO();
        std::pair<std::string,std::string> read();
        void write(std::string table, std::string code);
    private:
        int currentBit;
        unsigned char bitBuffer;
        std::string convertReadFile(std::string filename, std::ifstream& file, int codeSize);
        void flushBits(std::ofstream& wf);
        void writeBit(std::ofstream& wf, int bit);
};

#endif // BINIO_H
