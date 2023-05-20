#include <iostream>
#include "huffman.h"



int main(int argc, char* argv[]) {

    //frederick_douglass.txt frederick_douglass.txt.zap
    //zap all_ABC.txt all_ABC.txt.zap
    // read file_name
    //all_ABC.txt.zap all_ABC.txt.unzap
    if (argc != 3 ) {
        std::cerr << "Usage: " << argv[0]
                  << " <inputfile> <zapfile>"
                  << std::endl;
        exit(1);
    }
    std::string zip = argv[1];
    std::string ifs_name = argv[2];
    std::string ofs_name = argv[3];


    // Write this to a file
    std::ofstream ofs(ofs_name, std::ios::out |
    std::ios::trunc |
    std::ios::binary);

    std::ifstream ifs(ifs_name, std::ios::in |
    std::ios::binary);


        Huffman dp;
        dp.Compress(ifs, ofs);

    return 0;
}
