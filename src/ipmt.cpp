#include <iostream>
#include <getopt.h>
#include <string.h>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include "my_algorithms.h"

using namespace std;

enum InterruptionTypes {
    HELP,
    NON_EXISTING_FILE,
    UNKNOWN_ARGUMENT
};
string InterruptionMessages[] = {
    "Usages:\n"
    "./bin/ipmt index textfile                         index textfile to a .idx file\n" 
    "./bin/ipmt search [options] pattern indexfile     search for pattern in indexfile\n"
    "options:\n"
    "   -c            :                                count occorrences of pattern(s) in indexfile\n"
    "   -p patternfile:                                search for multiple patterns in indexfile\n"
    "./bin/ipmt zip textfile                           compress textfile to textfile.myz with Huffman's algorithm\n"
    "./bin/ipmt unzip textfile.myz                     decompress textfile.myz to textfile with Huffman's algorithm",
    "File not found",
    "Unknown option"
};
void usage(InterruptionTypes status) {
    std::cerr << InterruptionMessages[status] << '\n'; 
    exit (status);
}

#define RED "\033[1;31m"
#define WHITE "\033[0m"
void printOcurrences(const string& textLine, int lineIndex, vector<int>& matchLengths) {
    if(textLine.empty()) return;
    if(*max_element(begin(matchLengths), end(matchLengths)) == 0) return;
    int textLen = (int)textLine.length();
    int redIndex = 0;
    //cout << "Line " << lineIndex << ": ";
    bool onRed = false;
    for (int i = 0; i < textLen; ++i){
        redIndex = max(redIndex, i + matchLengths[i]);

        if(!onRed && i < redIndex) { // at the start of a pattern match
            onRed = true;
            cout << RED;
        }

        if(onRed && i == redIndex) { // at the end of a pattern match
            onRed = false;
            cout << WHITE;
        }
        cout << textLine[i];
    }
    if(onRed && redIndex == textLen) cout << WHITE;
    cout << '\n';
}
vector<int> getFrequencies(const string& text) {
    vector<int> freq(alpha);
    for(auto& letter : text) {
        freq[charID(letter)] += 1;
    }
    return freq;
}
pair<vector<int>, vector<int>> getSuffixData(ifstream& indexfile) {
    int n;
    indexfile >> n;
    if(n == -1) return {{}, {}};
    vector<int> sa(n), freq(alpha);
    for(auto& x : sa) indexfile >> x;
    for(auto& x : freq) indexfile >> x;
    return {sa, freq};
}

void indexLine(const string& textLine, ofstream& indexfile){
    SuffixArray suffixArray(textLine);
    auto sa = suffixArray.getSuffixArray();
    auto freq = getFrequencies(textLine);
    indexfile << (int)sa.size() << ' ';
    for(auto& x : sa){
        indexfile << x << ' ';
    }
    indexfile << '\n';
    for(auto& x : freq) {
        indexfile << x << ' ';
    }
    indexfile << '\n';
}
void searchLine(const vector<string>& patterns, int& countOcorrences, bool countFlag, int& lineIndex, bool& endFlag, ifstream& indexfile) {
    auto [sa, freq] = getSuffixData(indexfile);
    if(sa.empty()) {
        endFlag = true;
        return;
    }
    SuffixArray suffixArray(sa, freq);
    auto textLine = suffixArray.getText();
    vector<int> matchLengths;
    if(!countFlag) {
        matchLengths.assign((int)textLine.size(), 0);
    }
    for(auto& pattern : patterns) {
        auto occorrences = suffixArray.matchPattern(pattern);
        if(countFlag) {
            countOcorrences += (int)occorrences.size();
        } else {
            for(int position : occorrences) {
                matchLengths[position] = max(matchLengths[position], (int)pattern.size());
            }
        }
    }
    if(!countFlag) {
        printOcurrences(textLine, lineIndex++, matchLengths);
    }
}

string byteTobinaryString(char codeChar, char codeLen) {
    string str = "";
    for(int i = 0; i < codeLen; ++i) {
        str += ((codeChar & 1) == 1) + '0';
        codeChar >>= 1;
    }
    return str;
}

string bytesTobinaryString(const vector<char>& codesChar, char codeLen) {
    string str = "";
    int len = (int)codesChar.size();
    char lastLen = codeLen % 8 > 0 ? codeLen % 8 : 8;
    for(int i = 0; i < len; ++i) {
        if(i + 1 < len) str += byteTobinaryString(codesChar[i], 8);
        else str += byteTobinaryString(codesChar[i], lastLen);
    }
    return str;
}

string solveHuffman(const string& table, const string& encoded) {
    Huffman huffman;
    istringstream ss(table);
    char letter, codeLen;
    string code;
    while(ss.get(letter)) {
        ss.get(codeLen);
        codeLen -= '0';
        vector<char> codesChar((codeLen + 7) / 8);
        for(auto& codeChar: codesChar) {
            ss.get(codeChar);
        }
        code = bytesTobinaryString(codesChar, codeLen);;
        huffman.addWord(letter, code);
    }
    auto decoded = huffman.decode(encoded);
    return decoded;
}

int main(int argc, char **argv) {
    int opt;
    bool countFlag = false;
    fstream patternsFile;

    static struct option long_options[] = {
            {"pattern", required_argument, 0, 'p'},
            {"count", no_argument, 0, 'c'},
            {"help", no_argument, 0, 'h'},
            {0,0,0,0}
    };

    int option_index = 0;

    //iterate trhoug option arguments with getopt
    while ((opt = getopt_long (argc, argv, "p:ch", long_options, &option_index)) != -1) {  
      switch (opt) {
        case 'c':
            countFlag = true;
            break;
        case 'p':
	        if (optarg == NULL && optind < argc && argv[optind][0] != '-') {
                optarg = argv[optind++];
            }
            patternsFile.open(optarg);
            if (!patternsFile) {
                usage(NON_EXISTING_FILE);
            }
            break;
        case 'h':
            usage(HELP);
            break;
        case '?':
            exit(UNKNOWN_ARGUMENT);
            break;
        case 0:
          break;
        default :
            abort();
        }
    }
    string curMode = argv[optind++];
    vector<string> modes = {"index", "search", "zip", "unzip"};

    if(find(begin(modes), end(modes), curMode) == end(modes)){
        usage(UNKNOWN_ARGUMENT);
    }

    // checar argc  
    //cout << curMode << '\n';
    if(curMode == "index") {
        string textfilename = argv[optind];
        string indexfilename = textfilename;
        size_t pos = textfilename.find_last_of(".");
        if(pos != string::npos) {
            indexfilename = textfilename.substr(0, pos);
        }
        indexfilename += ".idx";
        ifstream textfile(textfilename);
        ofstream indexfile(indexfilename);
        if(!textfile) {
            usage(NON_EXISTING_FILE);
        }
        vector<string> textLines;
        string textLine;
        while(getline(textfile, textLine)) {
            indexLine(textLine, indexfile);
        }
        indexfile << -1;
        textfile.close();
        indexfile.close();
    } else if (curMode == "search"){
        vector<string> patterns;
        if(patternsFile.is_open()){
            string pattern;
            while(getline(patternsFile, pattern)){
                patterns.emplace_back(pattern);
            }
            patternsFile.close();
        } else {
            patterns.emplace_back(argv[optind]);
            ++optind;
        }
        //make sure all patterns are different
        sort(begin(patterns), end(patterns));
        patterns.erase(unique(begin(patterns),end(patterns)), end(patterns));

        string indexfilename = argv[optind]; 
        ifstream indexfile(indexfilename);
        if(!indexfile) {
            usage(NON_EXISTING_FILE);
        }
        int countOcorrences = 0;
        int lineIndex = 1;
        bool endFlag = false;
        while(!endFlag) {
            searchLine(patterns, countOcorrences, countFlag, lineIndex, endFlag, indexfile);
        }
        if(countFlag) {
            cout << countOcorrences << '\n';
        }
    } else if (curMode == "zip"){
        ifstream textfile;
        string textfilename = argv[optind];
        string outputfilename = textfilename + ".myz";
        textfile.open(textfilename);
        if (!textfile) {
            usage(NON_EXISTING_FILE);
        }
        std::ofstream wf(outputfilename, std::ios::binary);
        BinIO outputfile;
        string textinput = "";
        for(char textletter; textfile.get(textletter); textinput += textletter) {
            if((int)textinput.size() == TEXT_LIM) {
                auto huffman = Huffman(textinput);
                outputfile.write(huffman.getFormatedCodes(), huffman.encode(), wf);
                textinput = "";
            }
        }
        if(!textinput.empty()) {
            auto huffman = Huffman(textinput);
            outputfile.write(huffman.getFormatedCodes(), huffman.encode(), wf);
        }
        wf.close();
    } else { // curMode == "unzip"
        string textfilename = argv[optind];
        ifstream rf(textfilename, std::ios::binary);
        string outputfilename = textfilename;
        size_t pos = textfilename.find_last_of(".");
        outputfilename = outputfilename.substr(0, pos);
        BinIO inputfile;
        ofstream outputfile(outputfilename);
        while(!rf.eof()){
            auto [table, encoded] = inputfile.read(rf);
            outputfile << solveHuffman(table, encoded);
        }
        rf.close();
        outputfile.close();
    }
    return 0;
}