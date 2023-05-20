#ifndef HUFFMAN_H_
#define HUFFMAN_H_

#include <array>
#include <cstddef>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <queue>
#include <cmath>
#include <cstdio>

#include "bstream.h"
#include "pqueue.h"




class HuffmanNode {
public:
    explicit HuffmanNode(char ch, size_t freq,
                         HuffmanNode *left = nullptr,
                         HuffmanNode *right = nullptr)
            : ch_(ch), freq_(freq), left_(left), right_(right) { }


    bool IsLeaf() {
        // Node is a leaf if it doesn't have any children
        return left_ == nullptr && right_ == nullptr;
    }

    bool operator < (const HuffmanNode &n) const {
        // In case of equality, make it deterministic based on character
        if (freq_ == n.freq_)
            return ch_ < n.ch_;
        // Otherwise compare frequencies
        return freq_ < n.freq_;
    }


    size_t freq() { return freq_; }
    size_t data() { return ch_; }
    HuffmanNode* left() { return left_; }
    HuffmanNode* right() { return right_; }

private:
    char ch_;
    size_t freq_;
    HuffmanNode *left_, *right_;
};

class Huffman {
public:
    static void Compress(std::ifstream &ifs, std::ofstream &ofs);

    static void Decompress(std::ifstream &ifs, std::ofstream &ofs);

private:
    // Helper methods...
    static void PreorderRecur(HuffmanNode *n, std::string str, BinaryOutputStream &bio);
    static void Des_PreorderRecur(HuffmanNode* n, std::string str, BinaryInputStream& bis);
    static std::map<char , std::string> code_table;
    static std::map<std::string, char> build_tree;
    static std::map<char,std::string> copy_build_tree;
};

// To be completed below


std::map<char, std::string>Huffman::code_table;
std::map<std::string, char>Huffman::build_tree;
std::map<char, std::string>Huffman::copy_build_tree;


void Huffman::PreorderRecur(HuffmanNode *n, std::string str, BinaryOutputStream &bio) {
    static std::string output_tree;
    if(n != NULL) {

        // write trees
        if(n->IsLeaf() != 1){
            bio.PutBit(0);
        } else {
            bio.PutBit(1);
            bio.PutChar(n->data());
        }

        // read in to codetable vector map.
        code_table.emplace(n->data(), str);

        PreorderRecur(n->left(), str+"0",bio);
        PreorderRecur(n->right(),str+"1",bio);
    }

}


void Huffman::Compress(std::ifstream &ifs, std::ofstream &ofs) {


    //reading
    std::string  read_file;
    std:: string copy_file;
    std::map<char , int> start_map;

    getline(ifs,read_file,'\n');
    copy_file =read_file;

    for (size_t i = 0; i < read_file.size(); i++) {

        auto it = start_map.find(read_file[i]);
        // 1 for the values
        if (it != start_map.end()) {  //strmap
            it->second = it->second + 1;
        } else {
            start_map.emplace(read_file[i], 1);
        }
    }

    struct cmp2{
        bool operator()(HuffmanNode* x,HuffmanNode* y)
        {
            return x->freq() < y->freq();
        }
    };

    PQueue<HuffmanNode*, cmp2> PQ;

    for (const auto& p : start_map) {
        PQ.Push(new HuffmanNode(p.first, p.second));
    }


    while(PQ.Size() != 1) {
        HuffmanNode *temp_left = PQ.Top();
        PQ.Pop();
        HuffmanNode *temp_right = PQ.Top();
        PQ.Pop();

        if(temp_left->freq() == temp_right->freq()){
            if(temp_left->data() > temp_right->data()) {
                PQ.Push(new HuffmanNode ('0', temp_left->freq() + temp_right->freq(), temp_right, temp_left));
            } else {
                PQ.Push(new HuffmanNode('0', temp_left->freq() + temp_right->freq(), temp_left, temp_right));
            }
        } else {
            PQ.Push(new HuffmanNode('0', temp_left->freq() + temp_right->freq(), temp_left, temp_right));
        }
    }



//     HuffmanNode *root =PQ.Top();
    std::string str;

    BinaryOutputStream bio(ofs);
    PreorderRecur(PQ.Top(), str, bio);



    // write number of encode character
    bio.PutInt(copy_file.size());




    std::string code_table_output;
    for (size_t i = 0; i < copy_file.size(); i++) {
        code_table_output = code_table_output + code_table.find(copy_file[i])->second;
    }

    for(size_t i = 0; i < code_table_output.size(); i++) {
        if(code_table_output[i] == '1') {
            bio.PutBit(1);
        } else {
            bio.PutBit(0);
        }
    }
}



void Huffman::Des_PreorderRecur(HuffmanNode* n, std::string str_out, BinaryInputStream& bis) {
    int left_or_right = bis.GetBit();
    if (left_or_right == 0) {

        HuffmanNode* n = new HuffmanNode('0', 0);
        Des_PreorderRecur(n->left(), str_out + "0", bis);
        Des_PreorderRecur(n->right(), str_out + "1", bis);
    }
    else {
        char c = bis.GetChar();
        build_tree[str_out] = c;
        return;
    }
}



void Huffman::Decompress(std::ifstream &ifs, std::ofstream &ofs) {

    std::string zap_file;
    BinaryInputStream bis(ifs);


//    HuffmanNode * n = ;
    HuffmanNode *n = new HuffmanNode('0',0 );

    std::string str_out;

    Des_PreorderRecur(n, str_out, bis);

    for (const auto& p : build_tree) {
        copy_build_tree.emplace(p.second,p.first);
    }

    // write totol number


    struct cmp2{
        bool operator()(HuffmanNode* x,HuffmanNode* y)
        {
            return x->freq() > y->freq();
        }
    };

    PQueue<HuffmanNode*, cmp2> PQ;

    for (const auto& p : build_tree) {


        // convert bin to dec
        int base = 1;
        int temp = stoi(p.first);
        int dec_value = 0;
        while (temp) {
            int last_digit = temp % 10;
            temp = temp / 10;

            dec_value += last_digit * base;

            base = base * 2;
        }
        PQ.Push(new HuffmanNode(p.second, pow (2, p.first.size()) + dec_value));
    }

    // write totol number
    int total_number = bis.GetInt();



    for (size_t i = 0; i < build_tree.size() - 1; i++) {
        HuffmanNode *temp_right = PQ.Top();
        PQ.Pop();
        HuffmanNode *temp_left = PQ.Top();
        PQ.Pop();

        if(temp_right->IsLeaf() && temp_left->IsLeaf()) {
            PQ.Push(new HuffmanNode('0', temp_left->freq() + temp_right->freq(), temp_left, temp_right));
        } else {
            int size;
            int dirction;

            if(temp_left->IsLeaf()){
                size = copy_build_tree.find(temp_left->data())->second.size();
                dirction = copy_build_tree.find(temp_left->data())->second[size - 1];

            } else {
                size = copy_build_tree.find(temp_right->data())->second.size();
                dirction = copy_build_tree.find(temp_right->data())->second[size - 1];
            }

            if (dirction == 48) {
                PQ.Push(new HuffmanNode('0', temp_left->freq() + temp_right->freq(), temp_left, temp_right));
            } else {
                PQ.Push(new HuffmanNode('0', temp_left->freq() + temp_right->freq(), temp_right, temp_left));
            }
        }
    }


    std::string output_text;
    for(int i = 0; i < total_number; i++) {
        HuffmanNode * point = PQ.Top();

        while(point->IsLeaf() != 1){
            int direction = bis.GetBit();
            if (direction == 0) {
                point = point->left();
            } else {
                point = point->right();
            }
        }
        output_text = output_text + char(point->data());
    }

    //final answear save at output_text;
    //std::cout << output_text<<std::endl;



}

#endif  // HUFFMAN_H_