#include <iostream>
#include <getopt.h>
#include <string.h>
#include <vector>
#include <algorithm>
#include <fstream>
#include "my_algorithms.h"

using namespace std;

enum InterruptionTypes {
    HELP,
    NON_EXISTING_FILE,
    UNKNOWN_ARGUMENT
};
string InterruptionMessages[] = {
    "Help",
    "File not found",
    "Unknown option"
};
void usage(InterruptionTypes status){
    std::cerr << InterruptionMessages[status] << '\n'; 
    exit (status);
}

int main(int argc, char **argv){
    
    int opt;
    bool count_flag = false;
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
            count_flag = true;
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
    /*
    vector<string> patterns;
    if(patternsFile.is_open()){
        string pattern;
        while(getline(patternsFile, pattern)){
            patterns.emplace_back(pattern);
        }
        patternsFile.close();
    }else{
        patterns.emplace_back(argv[optind]);
        ++optind;
    }
    //make sure all patterns are different
    sort(begin(patterns), end(patterns));
    patterns.erase(unique(begin(patterns),end(patterns)), end(patterns));
    */

    //cout << curMode << '\n';
    if(curMode == "index"){
        
    }else if(curMode == "search"){

    }else if (curMode == "zip"){
        fstream indexfile;
        string filename = argv[optind];
        string outputfilename = filename + ".myz";
        indexfile.open(filename);
        if (!indexfile) {
            usage(NON_EXISTING_FILE);
        }
        string input;
        getline(indexfile, input);
        auto huffman = Huffman(input);
        ofstream outputfile(outputfilename);
        outputfile << huffman.getFormatedCodes();
        outputfile << huffman.encode();
        //indexfile >> input;
        cout << input << '\n';
    }else{ // curMode == "unzip"

    }
    
    return 0;
}