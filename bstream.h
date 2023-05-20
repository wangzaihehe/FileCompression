#ifndef BSTREAM_H_
#define BSTREAM_H_

#include <cstddef>
#include <fstream>
#include <iostream>

class BinaryInputStream {
public:
    explicit BinaryInputStream(std::ifstream &ifs);

    bool GetBit();
    char GetChar();
    int GetInt();

private:
    std::ifstream &ifs;
    char buffer = 0;
    size_t avail = 0;

    // Helpers
    void RefillBuffer();
};

BinaryInputStream::BinaryInputStream(std::ifstream &ifs) : ifs(ifs) { }

void BinaryInputStream::RefillBuffer() {
    // Read the next byte from the input stream
    ifs.get(buffer);
    if (ifs.gcount() != 1)
        throw std::underflow_error("No more characters to read");
    avail = 8;
}

bool BinaryInputStream::GetBit() {
    bool bit;

    if (!avail)
        RefillBuffer();

    avail--;

    bit = ((buffer >> avail) & 1) == 1;
//  std::cout << bit << std::endl;

#if 0  // Switch to 1 for debug purposes
    if (bit)
    std::cout << '1' << std::endl;
  else
    std::cout << '0' << std::endl;
#endif

    return bit;
}

char BinaryInputStream::GetChar() {
    // To be completed
    char getchar = 0;


    for (size_t i = 0; i < sizeof(char) * 8; i++) {
        getchar = (getchar << 1) | GetBit();
    }


//    bool bit[8];
//    for(int i = 0; i < 8; i++) {
//        std::cout << ((getchar >> i) & 0x01);
//    }
//    std::cout << std::endl;

    return getchar;
}

int BinaryInputStream::GetInt() {
    // To be completed

    int getint = 0;


    for (size_t i = 0; i < sizeof(int) * 8; i++) {
        getint = (getint << 1) | GetBit();
    }


//    bool bit[8];
//    for(int i = 0; i < 32; i++) {
//        std::cout << ((getint >> i) & 0x01);
//    }
//    std::cout << std::endl;

    return getint;
}

class BinaryOutputStream {
public:
    explicit BinaryOutputStream(std::ofstream &ofs);
    ~BinaryOutputStream();

    void Close();

    void PutBit(bool bit);
    void PutChar(char byte);
    void PutInt(int word);

private:
    std::ofstream &ofs;
    char buffer = 0;
    size_t count = 0;

    // Helpers
    void FlushBuffer();
};

BinaryOutputStream::BinaryOutputStream(std::ofstream &ofs) : ofs(ofs) { }

BinaryOutputStream::~BinaryOutputStream() {
    Close();
}

void BinaryOutputStream::Close() {
    FlushBuffer();
}

void BinaryOutputStream::FlushBuffer() {
    // Nothing to flush
    if (!count)
        return;

    // If buffer isn't complete, pad with 0s before writing
    if (count > 0)
        buffer <<= (8 - count);
    // Write to output stream
    ofs.put(buffer);

    // Reset buffer
    buffer = 0;
    count = 0;
}

void BinaryOutputStream::PutBit(bool bit) {
    // Make some space and add bit to buffer
    buffer <<= 1;
    if (bit)
        buffer |= 1;

    // If buffer is full, write it
    if (++count == 8)
        FlushBuffer();
}

void BinaryOutputStream::PutChar(char byte) {
    // To be completed

    //01100010
    int i = 7;
    while (i != -1) {
        PutBit((byte >> i) & 1);
//        FlushBuffer();
        i--;
    }
}

void BinaryOutputStream::PutInt(int word) {
    // To be completed

    int i = 31;
    while (i != -1) {
        PutBit((word >> i) & 1);
        i--;
    }
}

#endif  // BSTREAM_H_
