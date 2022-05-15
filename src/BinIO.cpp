#include<bits/stdc++.h>
#include "my_algorithms.h"

using namespace std;


BinIO::BinIO(){
    currentBit = 0;
    bitBuffer = 0;
}

void BinIO::writeBit (ofstream& wf, int bit) {
    bitBuffer <<= 1;
    if (bit) bitBuffer |= 0x1;
    currentBit++;
    if (currentBit == 8) {
        wf.write(reinterpret_cast<char*>(&bitBuffer), 1);
        currentBit = 0;
        bitBuffer = 0;
    }
}

void BinIO::flushBits (ofstream& wf) {
    while (currentBit) {
        writeBit(wf, 0);
    }
}

string BinIO::convertReadFile(string filename, ifstream& file, int codeSize) {
    std::vector<unsigned char> fileData(codeSize);
    file.read((char*) &fileData[0], codeSize);
    string outputZeroOne = "";
    for(auto code : fileData){
        string zeroOne = "";
        for(int i = 0; i < 8; ++i){
            zeroOne += char((code & 1) + '0');
            code >>= 1;
        }
        reverse(begin(zeroOne), end(zeroOne));
        outputZeroOne += zeroOne;
    }
    return outputZeroOne;
}

void BinIO::write(string table, string code){
    ofstream wf("test.bin", ios::binary);
    int codeSize = code.length()/8;
    if(code.length() % 8 != 0){
        codeSize++;
    }
    //write table size to file
    int size = table.length();
    char x = (char)size;
    wf.write((char *)&size, sizeof(size));
    //write table to file
    wf.write(&table[0],size);
    //write code size to file
    wf.write((char *)&codeSize, sizeof(codeSize));
    //write code to file
    int len = (int)code.size();
    for(int i = 0; i < len; ++i){
        writeBit(wf, code[i] - '0');
    }
    flushBits(wf);
    wf.close();
}

pair<string, string> BinIO::read(){
    ifstream rf("test.bin", ios::binary);
    string table;
    pair<string, string> ret;
    //read table size
    int size;
    char aux;
    rf.read(&aux, 4);
    size = (int)aux;
    //read table
    table.resize(size);
    rf.read(&table[0], size);
    ret.first = table;
    //read code size
    rf.read((char *)&aux, 4);
    size = (int)aux;
    ret.second = convertReadFile("test.bin", rf, size);
    rf.close();
    return ret;
}