#include <fstream>
#include <algorithm>
#include <iostream>
#include "my_algorithms.h"

BinIO::BinIO(){
    currentBit = 0;
    bitBuffer = 0;
}

void BinIO::writeBit (std::ofstream& wf, int bit) {
    bitBuffer <<= 1;
    if (bit) bitBuffer |= 0x1;
    currentBit++;
    if (currentBit == 8) {
        wf.write((char *)&bitBuffer, 1);
        currentBit = 0;
        bitBuffer = 0;
    }
}

unsigned char BinIO::flushBits (std::ofstream& wf) {
    unsigned char cnt = 0;
    while (currentBit) {
        writeBit(wf, 0);
        cnt += 1;
    }
    return cnt;
}

std::string BinIO::convertReadFile(std::ifstream& file, int codeSize) {
    std::vector<unsigned char> fileData(codeSize);
    file.read((char*) &fileData[0], codeSize);
    std::string outputZeroOne = "";
    for(auto code : fileData){
        std::string zeroOne = "";
        for(int i = 0; i < 8; ++i){
            zeroOne += char((code & 1) + '0');
            code >>= 1;
        }
        reverse(begin(zeroOne), end(zeroOne));
        outputZeroOne += zeroOne;
    }
    return outputZeroOne;
}

void BinIO::write(const std::string& table, const std::string& code, std::ofstream& wf){
    int codeSize = code.length() / 8;
    if(code.length() % 8 != 0) {
        codeSize++;
    }
    //write table size to file
    int size = table.length();
    wf.write((char *)&size, sizeof(size));
    //write table to file
    wf.write(&table[0], size);
    //write code size to file
    wf.write((char *)&codeSize, sizeof(codeSize));
    //write code to file
    int len = (int)code.size();
    for(int i = 0; i < len; ++i){
        writeBit(wf, code[i] - '0');
    }
    auto cnt = flushBits(wf);
    wf.write((char *)&cnt, sizeof(cnt));
}

std::pair<std::string, std::string> BinIO::read(std::ifstream& rf){
    std::string table;
    std::pair<std::string, std::string> ret;
    //read table size
    int size;
    rf.read((char *)&size, sizeof(size));
    //read table
    table.resize(size);
    rf.read((char*)&table[0], size);
    ret.first = table;
    //read code size
    rf.read((char *)&size, sizeof(size));
    ret.second = convertReadFile(rf, size);
    
    unsigned char cnt;
    rf.read((char *)&cnt, sizeof(cnt));
    for(int i = 0; i < cnt; ++ i) ret.second.pop_back();
    return ret;
}