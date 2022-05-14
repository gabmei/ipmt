#include<string>
#include<vector>
#include<unordered_map>

#ifndef HUFFMAN_H
#define HUFFMAN_H
struct Huffman{
public:
    struct Node{
        char letter;
        int freq;
        Node* left;
        Node* right;
        Node(char _letter = '\0', int _freq = 0, Node* _left = nullptr, Node* _right = nullptr):
            letter(_letter), freq(_freq), left(_left), right(_right){}
        ~Node(){
            delete left;
            delete right;
        }
        bool operator < (const Node& other) const {
            return freq < other.freq;
        }
    };
    Huffman(const std::string& _word);

    ~Huffman();
    std::string encode();
    std::string decode(const std::string& encoded);
    std::string getFormatedCodes();
private:
    static int charID(char c) { return int(c) - int(' '); }
    const static int alpha = 95;
    Node* root;
    std::string word;
    std::vector<int> freqs;
    std::unordered_map<char, std::string> codes;

    void initFreqs();
    void initTree();
    void initCodes(Node* curRoot, std::string curCode);
};

#endif // HUFFMAN_H
