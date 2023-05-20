#include <gtest/gtest.h>

#include <cstdio>
#include <fstream>

#include "bstream.h"

TEST(BStream, input) {
  std::string filename{"test_bstream_input"};

  const unsigned char val[] = {
    0x58, 0x90, 0xab, 0x08,
    0x00, 0x4e, 0xdb, 0x40,
  };
  // Equivalent in binary is:
  // 0101100010010000101010110000100000000000010011101101101101000000
  // ^5  ^8  ^9  ^0  ^a  ^b  ^0  ^8  ^0  ^0  ^4  ^e  ^d  ^b  ^4  ^0

  // Write this to a file
  std::ofstream ofs(filename, std::ios::out |
                    std::ios::trunc |
                    std::ios::binary);
  ofs.write(reinterpret_cast<const char *>(val), sizeof(val));
  ofs.close();

  // Read it back in binary format
  std::ifstream ifs(filename, std::ios::in |
                    std::ios::binary);
  BinaryInputStream bis(ifs);

  // Make sure that we reading the binary in the correct order
  EXPECT_EQ(bis.GetBit(), 0);
  EXPECT_EQ(bis.GetBit(), 1);
  EXPECT_EQ(bis.GetChar(), 0x62);  // 01100010
  EXPECT_EQ(bis.GetChar(), 0x42);  // 01000010
  EXPECT_EQ(bis.GetBit(), 1);
  EXPECT_EQ(bis.GetInt(), 0x58400276);  // 01011000010000000000001001110110

  ifs.close();

  std::remove(filename.c_str());
}



TEST(BStream_output, input) {
std::string filename{"test_bstream_output"};


// Equivalent in binary is:
// 0101100010010000101010110000100000000000010011101101101101000000
// ^5  ^8  ^9  ^0  ^a  ^b  ^0  ^8  ^0  ^0  ^4  ^e  ^d  ^b  ^4  ^0

// Write this to a file
std::ofstream ofs(filename, std::ios::out |
                            std::ios::trunc |
                            std::ios::binary);
BinaryOutputStream bio(ofs);
bio.PutBit(0);
bio.PutBit(1);
bio.PutBit(0);
bio.PutBit(1);
bio.PutBit(0);
bio.PutBit(1);
bio.PutBit(0);
bio.PutBit(1);
// because it should have at 8, 16, 24.... to fill the buffer
bio.PutChar(0x62);
bio.PutChar(0x42);
bio.PutInt(0x58400276);
ofs.close();

// Read it back in binary format
std::ifstream ifs(filename, std::ios::in |
                            std::ios::binary);
BinaryInputStream bis(ifs);

// Make sure that we reading the binary in the correct order
EXPECT_EQ(bis.GetBit(), 0);
EXPECT_EQ(bis.GetBit(), 1);
EXPECT_EQ(bis.GetBit(), 0);
EXPECT_EQ(bis.GetBit(), 1);
EXPECT_EQ(bis.GetBit(), 0);
EXPECT_EQ(bis.GetBit(), 1);
EXPECT_EQ(bis.GetBit(), 0);
EXPECT_EQ(bis.GetBit(), 1);
EXPECT_EQ(bis.GetChar(), 0x62);  // 01100010
EXPECT_EQ(bis.GetChar(), 0x42);  // 01000010
EXPECT_EQ(bis.GetInt(), 0x58400276);  // 01011000010000000000001001110110

ifs.close();

std::remove(filename.c_str());
}


int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
