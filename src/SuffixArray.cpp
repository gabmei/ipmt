#include<string>
#include<vector>
#include <numeric>
#include <algorithm>
#include "my_algorithms.h"

SuffixArray::SuffixArray(const std::string& text) {
    this->text = text + char(31);
    buildSuffixArray();
}

SuffixArray::SuffixArray(const std::vector<int>& sa, const std::vector<int>& freq) {
    this->sa = sa;
    text.assign((int)sa.size(), char(31));
    int position = 1;
    for(char letter = firstLetter; charID(letter) < alpha; ++letter) {
        for(int count = freq[charID(letter)]; count > 0; --count) {
            text[sa[position++]] = letter;
        }
    }
}

std::vector<int> SuffixArray::matchPattern(const std::string& pattern) {
    auto lo = lowerBound(pattern), hi = upperBound(pattern);
    std::vector<int> occorrences;
    for(int i = lo; i < hi; ++i) {
        occorrences.emplace_back(sa[i]);
    }
    return occorrences;
}

std::string SuffixArray::getText() {
    auto txt = this->text;
    txt.pop_back();
    return txt;
}
std::vector<int> SuffixArray::getSuffixArray() { return sa; }

int& SuffixArray::operator[](int index) { return sa[index]; }

void SuffixArray::countSort(const std::vector<int>& count) {
    int len = (int)sa.size();
    std::vector<int> cnt(len + 1), newSa(len);
    for(int x : count) {
        ++cnt[x + 1];
    }
    for(int i = 1; i < len; ++i) {
        cnt[i] += cnt[i - 1];
    }
    for(int x : sa) {
        newSa[ cnt[count[x]]++ ] = x;
    }
    sa.swap(newSa);
}

void SuffixArray::buildSuffixArray() {
    int len = (int)text.size();
    auto mod = [&](int x) {
        if(x < 0) x += len;
        else if(x >= len) x -= len;
        return x;
    };
    this->sa.assign(len, 0);
    std::vector<int> count(len, 0); 
    iota(begin(sa), end(sa), 0); //sa[i] = i
    sort(begin(sa), end(sa), [&](int a, int b) { return text[a] < text[b]; });
    int m = 0;
    count[sa[0]] = m++;
    for(int i = 1; i < len; ++i) {
        count[sa[i]] = text[sa[i]] != text[sa[i - 1]] ? m++ : m-1;
    }
    for(int h = 1; h < len && m < len; h <<= 1) {
        for(int& x : sa) {
            x = mod(x - h);
        }
        countSort(count);
        std::vector<int> newCount(len);
        m = 0;
        newCount[sa[0]] = m++;
        for(int i = 1; i < len; ++i){
            std::pair<int,int> prev = {count[sa[i - 1]], count[mod(sa[i - 1] + h)]};
            std::pair<int,int> cur = {count[sa[i]], count[mod(sa[i] + h)]};
            newCount[sa[i]] = prev != cur ? m++ : m-1;
        }
    count.swap(newCount);
    }
}

int SuffixArray::lowerBound(const std::string& pattern) {
    int lo = 0, hi = (int)text.size();
    int len = (int)pattern.size();
    while(hi - lo > 1) {
        int m = (lo + hi)/2;
        if(text.substr(sa[m], len) < pattern) lo = m;
        else hi = m; 
    }
    return lo + 1;
}

int SuffixArray::upperBound(const std::string& pattern) {
    int lo = 0, hi = (int)text.size();
    int len = (int)pattern.size();
    while(hi - lo > 1) {
        int m = (lo + hi)/2;
        if(text.substr(sa[m], len) > pattern) hi = m;
        else lo = m; 
    }
    return hi;
}
