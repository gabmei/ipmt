#include <string>
#include <vector>
#include <unordered_map>

const static int TEXT_LIM = 3e6;
const static int firstLetter = ' ';
const static int alpha = 95;
static int charID(char c) { return c == '\n' ? alpha : int(c) - int(firstLetter); }
static char getChar(int id) { return id == alpha ? '\n' : firstLetter + id; }

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
    };

    Huffman(const std::string& word);
    Huffman();
    ~Huffman();
    void addWord(char letter, const std::string& code);
    std::string encode();
    std::string decode(const std::string& encoded);
    unsigned char binaryStringToByte(std::string& str);
    std::string binaryStringToBytes(const std::string& str);
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
        std::pair<std::string,std::string> read(std::ifstream& rf);
        void write(const std::string& table, const std::string& code, std::ofstream& wf);
    private:
        int currentBit;
        unsigned char bitBuffer;
        std::string convertReadFile(std::ifstream& file, int codeSize);
        unsigned char flushBits(std::ofstream& wf);
        void writeBit(std::ofstream& wf, int bit);
};
#endif // BINIO_H
